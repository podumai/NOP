#ifndef __NOP_CSV_PARSER_HPP__   /* Begin parser header file */
#define __NOP_CSV_PARSER_HPP__ 1

#include <iostream>
#include <tuple>
#include <sstream>
#include <fstream>
#include <memory>
#include <type_traits>
#include <boost/type_index.hpp>
#include "exception.hpp"

namespace nop /* Begin namespace nop */
{

  namespace csv /* Begin namespace csv */
  {

    template<size_t curr, size_t t_size, typename Ch, typename Tr, typename... Types>
    void print_tuple(std::basic_ostream<Ch, Tr>& out, const std::tuple<Types...>& t) noexcept
    {
      if constexpr (curr < t_size)
      {
        out << '<' << std::get<curr>(t) << '>';
        print_tuple<curr + 1, t_size, Ch, Tr,Types...>(out, t);
      }
    }

  } /* End namespace csv */

} /* End namespace nop */

template<typename Ch, typename Tr, typename... Types>
decltype(auto) operator<<(std::basic_ostream<Ch, Tr>& out, const std::tuple<Types...>& t) noexcept
{
  nop::csv::print_tuple<0 , sizeof... (Types), Ch, Tr, Types...>(out, t);
  return out;
}

namespace nop /* Begin namespace nop */
{

  namespace csv /* Begin namespace csv */
  {

    struct DefaultCfg
    {
    public:
      enum Symbol : char
      {
        Column = ',',
        Row = '\n',
        Escape = '\"'
      };
    };

    template<class Cfg, typename... Types>
    class Parser
    {
    private:
      class ControlBlock
      {
      private:
        std::ifstream* input;
        size_t m_row;
        size_t m_column;
        std::unique_ptr<std::tuple<Types...>> m_storage;

      public:
        ControlBlock(std::ifstream* in, size_t skip_lines)
          : input{in}
          , m_row{skip_lines}
          , m_column{1}
          , m_storage{std::make_unique<std::tuple<Types...>>(std::tuple<Types...>{})}
        {}

        ControlBlock(const ControlBlock&) = delete;
        ControlBlock(ControlBlock&&) = delete;
        ~ControlBlock() = default;

        void incColumn() noexcept
        {
          ++m_column;
        }

        void updatePosition() noexcept
        {
          ++m_row;
          m_column = 1;
        }

        [[nodiscard]] std::ifstream* getStream() noexcept
        {
          return input;
        }

        void freeStream() noexcept
        {
          input = nullptr;
        }

        [[nodiscard]] size_t getRow() const noexcept
        {
          return m_row;
        }

        [[nodiscard]] size_t getColumn() const noexcept
        {
          return m_column;
        }

        [[nodiscard]] std::tuple<Types...>& getStorage() noexcept
        {
          return *m_storage;
        }

        ControlBlock& operator=(const ControlBlock&) = delete;
        ControlBlock& operator=(ControlBlock&&) = delete;
      };

    public:
      class Iterator
      {
      private:
        std::unique_ptr<std::stringstream> buffer;
        std::shared_ptr<ControlBlock> m_block;

      private:
        template<size_t current, size_t totalSize>
        void parse()
        {
          if constexpr (current < totalSize)
          {
            size_t startColumn{m_block->getColumn()};
            buffer->str("");
            buffer->clear();

            while (m_block->getStream()->eof() == false)
            {
              char symbol{static_cast<char>(m_block->getStream()->get())};

              if (symbol == Cfg::Symbol::Column || symbol == Cfg::Symbol::Row)
              {
                if (buffer->rdbuf()->in_avail() == 0 ||
                  (current + 1 < totalSize && symbol == Cfg::Symbol::Row))
                  throw ::csv::err::format_error{std::format(
                         "\033[1;35m[ERROR]\033[0m Invalid column size.\n"
                         "\033[1;35m[MESSAGE]\033[0m Parse error position : <Row:{};Column:{}-{}>",
                         m_block->getRow(), startColumn, m_block->getColumn())};
                else
                  break;
              }
              else if (symbol == Cfg::Symbol::Escape)
              {
                std::string tmpBuffer;
                std::getline(*(m_block->getStream()), tmpBuffer, static_cast<char>(Cfg::Symbol::Escape));
                buffer->write(tmpBuffer.c_str(), tmpBuffer.length());
                m_block->getStream()->unget();
                symbol = m_block->getStream()->get();

                if (symbol != Cfg::Symbol::Escape)
                  throw ::csv::err::format_error{std::format(
                        "\033[1;35m[ERROR]\033[0m Unpaired escape character.\n"
                        "\033[1;35m[MESSAGE]\033[0m The escape string should be : {}str{}\n"
                        "\033[1;35m[MESSAGE]\033[0m Parse error position : <Row:{};Column:{}-{}>"
                        , static_cast<char>(Cfg::Symbol::Escape)
                        , static_cast<char>(Cfg::Symbol::Escape)
                        , m_block->getRow()
                        , startColumn
                        , m_block->getColumn())};
              }
              else
              {
                buffer->put(symbol);
                m_block->incColumn();
              }
            }

            m_block->incColumn();

            if constexpr (std::is_same_v<std::remove_reference_t<decltype(std::get<current>(m_block->getStorage()))>, std::string> == true)
              std::getline(*buffer, std::get<current>(m_block->getStorage()));
            else
              (*buffer) >> std::get<current>(m_block->getStorage());

            if (m_block->getStream()->eof() == false && buffer->fail() == true)
              throw ::csv::err::format_error{std::format(
                    "\033[1;35m[ERROR]\033[0m Invalid data type.\n"
                    "\033[1;35m[MESSAGE]\033[0m The expected type was : {}\n"
                    "\033[1;35m[MESSAGE]\033[0m Parse error position : <Row:{};Column:{}-{}>"
                    , boost::typeindex::type_id<decltype(std::get<current>(m_block->getStorage()))>().pretty_name()
                    , m_block->getRow()
                    , startColumn
                    , m_block->getColumn())};

            parse<current + 1, totalSize>();
          }
        }

      public:
        Iterator(std::shared_ptr<ControlBlock>& mainBlock)
          : buffer{std::make_unique<std::stringstream>(std::stringstream())}
          , m_block{mainBlock}
        {
          parse<0, sizeof... (Types)>();
          m_block->updatePosition();
        }

        Iterator(std::shared_ptr<ControlBlock>& mainBlock, [[maybe_unused]] void* ptr)
          : m_block{mainBlock}
        {}

        Iterator(const Iterator&) = delete;
        Iterator(Iterator&& other) = delete;

        ~Iterator() = default;

        [[nodiscard]] std::tuple<Types...>& operator*() noexcept
        {
          return m_block->getStorage();
        }

        Iterator& operator++()
        {
          if (m_block->getStream()->eof() == false)
          {
            parse<0, sizeof...(Types)>();
            m_block->updatePosition();
          }

          return *this;
        }

        Iterator& operator=(const Iterator&) = delete;
        Iterator& operator=(Iterator&& other) = delete;

        [[nodiscard]] bool operator==(const Iterator& other) const noexcept
        {
          return (m_block->getStream() == other.m_block->getStream())
                  && (m_block->getStream()->eof() == true);
        }

        [[nodiscard]] bool operator!=(const Iterator& other) const noexcept
        {
          return !(*this == other);
        }
      };

    private:
      std::shared_ptr<ControlBlock> mainBlock;

    public:
      Parser(std::ifstream& in, size_t skip_lines)
        : mainBlock{std::make_shared<ControlBlock>(&in, skip_lines)}
      {
        if (in.is_open() == false)
          throw ::csv::err::invalid_argument{"\033[1;35m[ERROR]\033[0m Invalid file stream.\n"
                                             "\033[1;35m[MESSAGE]\033[0m Cannot parse the file."};

        while (skip_lines > 0 && mainBlock->getStream()->eof() == false)
          if (mainBlock->getStream()->get() == Cfg::Symbol::Row)
            --skip_lines;
      }

      Parser(const Parser&) = delete;
      Parser(Parser&&) = delete;

      ~Parser()
      {
        mainBlock->freeStream();
      }

      [[nodiscard]] Iterator begin()
      {
        return Iterator(mainBlock);
      }

      [[nodiscard]] Iterator end()
      {
        return Iterator(mainBlock, nullptr);
      }

      Parser& operator=(const Parser&) = delete;
      Parser& operator=(Parser&&) = delete;
    };

  } /* End namespace csv */

} /* End namespace nop */

#endif /* End parser header file */
