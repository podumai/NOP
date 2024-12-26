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

    /**
     * @brief Helper function to print tulpe elements
     *
     * @tparam current The actual index
     * @tparam tupleSize The tuple size
     * @tparam Ch Char type for output stream
     * @tparam Tr Char traits for output stream
     * @tparam Types... Variadic number of types
     *
     * @return None
     */
    template<size_t current, size_t tupleSize, typename Ch, typename Tr, typename... Types>
    void print_tuple(std::basic_ostream<Ch, Tr>& out, const std::tuple<Types...>& t) noexcept
    {
      if constexpr (current < tupleSize)
      {
        out << '<' << std::get<current>(t) << '>';
        print_tuple<current + 1, tupleSize, Ch, Tr, Types...>(out, t);
      }
    }

  } /* End namespace csv */

} /* End namespace nop */

/**
 * @brief Operator overload for tuple printing
 *
 * @tparam Ch Char type for output stream
 * @tparam Tr Char traits for output stream
 * @tparam Types... Variadic number of types
 *
 * @param [in] out Output stream
 * @param [in] t The tuple class
 *
 * @return Output stream reference type
 */
template<typename Ch, typename Tr, typename... Types>
decltype(auto) operator<<(std::basic_ostream<Ch, Tr>& out, const std::tuple<Types...>& t) noexcept
{
  nop::csv::print_tuple<0 , sizeof...(Types), Ch, Tr, Types...>(out, t);
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

    /**
     * @brief Parser class for parsing csv files
     *
     * @class Parser
     *
     * @tparam Cfg Configuration class that consists of char enum providing symbols
     * @tparam Types... Variadic number of types
     */
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
        std::tuple<Types...> m_storage;

      public:
        ControlBlock(std::ifstream* in, size_t skipLines)
          : input{in}
          , m_row{skipLines}
          , m_column{1}
          , m_storage{}
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
          return m_storage;
        }

        ControlBlock& operator=(const ControlBlock&) = delete;
        ControlBlock& operator=(ControlBlock&&) = delete;
      };

    public:
      /**
       * @brief Input iterator class for parsing and traversing file
       *
       * @class Iterator
       */
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
                  throw err::FormatError{std::format(
                        "\033[1;35m[ERROR]\033[0m Invalid column size.\n"
                        "\033[1;35m[MESSAGE]\033[0m Parse error position : <Row:{};Column:{}-{}>"
                        , m_block->getRow()
                        , startColumn
                        , m_block->getColumn())};
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
                  throw err::FormatError{std::format(
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
              throw err::FormatError{std::format(
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
          parse<0, sizeof...(Types)>();
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
      /**
       * @brief Parser constructor recieving two parameters
       *
       * @param [in] in Input file stream representing file to parse
       * @param [in] skipLines The number of lines to skip
       *
       * @throws invalid_argument
       */
      Parser(std::ifstream& in, size_t skipLines)
        : mainBlock{std::make_shared<ControlBlock>(&in, skipLines)}
      {
        if (in.is_open() == false)
          throw err::InvalidArgument{"\033[1;35m[ERROR]\033[0m Invalid file stream.\n"
                                     "\033[1;35m[MESSAGE]\033[0m Cannot parse the file."};

        while (skipLines > 0 && mainBlock->getStream()->eof() == false)
          if (mainBlock->getStream()->get() == Cfg::Symbol::Row)
            --skipLines;
      }

      Parser(const Parser&) = delete;
      Parser(Parser&&) = delete;

      ~Parser()
      {
        mainBlock->freeStream();
      }

      /**
       * @brief Begin method for creating Input Iterator
       *
       * @throws format_error
       */
      [[nodiscard]] Iterator begin()
      {
        return Iterator(mainBlock);
      }

      /**
       * @brief End method for creating Input Iterator
       */
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
