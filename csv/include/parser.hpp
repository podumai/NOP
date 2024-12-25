#ifndef __CSV_PARSER_HPP__   /* Begin parser header file */
#define __CSV_PARSER_HPP__ 1

#include <iostream>
#include <tuple>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <format>
#include <locale>
#include <memory>
#include <type_traits>
#include <boost/type_index.hpp>
#include "parser.hpp"
#include "exception.hpp"

/*template<typename Ch, typename Tr, class Tuple, size_t... I>
void print_tuple(std::basic_ostream<Ch, Tr>& out, const Tuple& t, std::index_sequence<I...>) noexcept
{
  ((out << '<' << std::get<I>(t) << '>'), ...);
}

template<typename Ch, typename Tr, typename... Types>
decltype(auto) operator<<(std::basic_ostream<Ch, Tr>& out, const std::tuple<Types...>& t) noexcept
{
  print_tuple(out, t, std::index_sequence_for<Types...>());
  return out;
}*/

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

template<typename Ch, typename Tr, typename... Types>
decltype(auto) operator<<(std::basic_ostream<Ch, Tr>& out, const std::tuple<Types...>& t) noexcept
{
  csv::print_tuple<0 , sizeof... (Types), Ch, Tr, Types...>(out, t);
  return out;
}

namespace csv /* Begin namespace csv */
{


  template<typename... Types>
  class parser
  {
  public:
    struct configuration
    {
      char del_column;
      char del_row;
      char escape_sym;

      configuration()
        : del_column{','}
        , del_row{'\n'}
        , escape_sym{'\"'}
      {}

      configuration(char d_c, char d_r, char s_sym)
        : del_column{d_c}
        , del_row{d_r}
        , escape_sym{s_sym}
      {}

      configuration(const configuration&) = default;
      configuration(configuration&&) = default;
      ~configuration() = default;

      configuration& operator=(const configuration&) = default;
      configuration& operator=(configuration&&) = default;
    };

  private:
    class control_block
    {
    private:
      std::ifstream* input;
      size_t m_row;
      size_t m_column;
      std::unique_ptr<std::tuple<Types...>> m_storage;

    public:
      control_block(std::ifstream* in, size_t skip_lines)
        : input{in}
        , m_row{skip_lines}
        , m_column{1}
        , m_storage{std::make_unique<std::tuple<Types...>>(std::tuple<Types...>{})}
      {}

      control_block(const control_block&) = delete;
      control_block(control_block&&) = delete;
      ~control_block() = default;

      void inc_column() noexcept
      {
        ++m_column;
      }

      void update_position() noexcept
      {
        ++m_row;
        m_column = 1;
      }

      [[nodiscard]] std::ifstream* get_stream() noexcept
      {
        return input;
      }

      void free_stream() noexcept
      {
        input = nullptr;
      }

      [[nodiscard]] size_t get_row() const noexcept
      {
        return m_row;
      }

      [[nodiscard]] size_t get_column() const noexcept
      {
        return m_column;
      }

      [[nodiscard]] std::tuple<Types...>& get_storage() noexcept
      {
        return *m_storage;
      }

      control_block& operator=(const control_block&) = delete;
      control_block& operator=(control_block&&) = delete;
    };

  public:
    class iterator
    {
    private:
      configuration m_cfg;
      std::unique_ptr<std::stringstream> buffer;
      std::shared_ptr<control_block> m_block;

    private:
      template<size_t current, size_t total_size>
      void parse()
      {
        if constexpr (current < total_size)
        {
          size_t start_column{m_block->get_column()};
          buffer->str("");
          buffer->clear();

          while (m_block->get_stream()->eof() == false)
          {
            char symbol{static_cast<char>(m_block->get_stream()->get())};

            if (symbol == m_cfg.del_row || symbol == m_cfg.del_column)
            {
              if (buffer->rdbuf()->in_avail() == 0 ||
                 (current + 1 < total_size && symbol == m_cfg.del_row))
              {
                std::string error_message{
                std::format(
                "\033[1;35m[ERROR]\033[0m Invalid column size.\n"
                "\033[1;35m[MESSAGE]\033[0m Parse error position : <Row:{};Column:{}-{}>",
                m_block->get_row(), start_column, m_block->get_column())};

                throw csv::err::format_error{error_message};
              }
              else
                break;
            }
            else if (symbol == m_cfg.escape_sym)
            {
              std::string tmp_buffer;
              std::getline(*(m_block->get_stream()), tmp_buffer, m_cfg.escape_sym);
              buffer->write(tmp_buffer.c_str(), tmp_buffer.length());
              m_block->get_stream()->unget();
              symbol = m_block->get_stream()->get();

              if (symbol != m_cfg.escape_sym)
              {
                std::string error_message{
                std::format(
                "\033[1;35m[ERROR]\033[0m Unpaired escape character.\n"
                "\033[1;35m[MESSAGE]\033[0m The escape string should be : {}str{}\n"
                "\033[1;35m[MESSAGE]\033[0m Parse error position : <Row:{};Column:{}-{}>",
                m_cfg.escape_sym, m_cfg.escape_sym, m_block->get_row(), start_column, m_block->get_column())};

                throw csv::err::format_error{error_message};
              }
            }
            else
            {
              buffer->put(symbol);
              m_block->inc_column();
            }
          }

          m_block->inc_column();

          if constexpr (std::is_same_v<std::remove_reference_t<decltype(std::get<current>(m_block->get_storage()))>, std::string> == true)
            std::getline(*buffer, std::get<current>(m_block->get_storage()));
          else
            (*buffer) >> std::get<current>(m_block->get_storage());

          if (m_block->get_stream()->eof() == false && buffer->fail() == true)
          {
            std::string error_message{
            std::format(
            "\033[1;35m[ERROR]\033[0m Invalid data type.\n"
            "\033[1;35m[MESSAGE]\033[0m The expected type was : {}\n"
            "\033[1;35m[MESSAGE]\033[0m Parse error position : <Row:{};Column:{}-{}>",
            boost::typeindex::type_id<decltype(std::get<current>(m_block->get_storage()))>().pretty_name(),
            m_block->get_row(), start_column, m_block->get_column())};

            throw csv::err::format_error{error_message};
          }

          parse<current + 1, total_size>();
        }
      }

    public:
      iterator(std::shared_ptr<control_block>& main_block, configuration new_cfg = {',', '\n', '\"'})
        : m_cfg{new_cfg}
        , buffer{std::make_unique<std::stringstream>(std::stringstream())}
        , m_block{main_block}
      {
        parse<0, sizeof... (Types)>();
        m_block->update_position();
      }

      iterator(std::shared_ptr<control_block>& main_block, [[maybe_unused]] void* ptr)
        : m_block{main_block}
      {}

      iterator(const iterator&) = delete;
      iterator(iterator&& other) = delete;

      ~iterator() = default;

      [[nodiscard]] std::tuple<Types...>& operator*() noexcept
      {
        return m_block->get_storage();
      }

      iterator& operator++()
      {
        if (m_block->get_stream()->eof() == false)
        {
          parse<0, sizeof...(Types)>();
          m_block->update_position();
        }

        return *this;
      }

      iterator& operator=(const iterator&) = delete;
      iterator& operator=(iterator&& other) = delete;

      [[nodiscard]] bool operator==(const iterator& other) const noexcept
      {
        return (m_block->get_stream() == other.m_block->get_stream())
                && (m_block->get_stream()->eof() == true);
      }

      [[nodiscard]] bool operator!=(const iterator& other) const noexcept
      {
        return !(*this == other);
      }
    };

  private:
    configuration m_cfg;
    std::shared_ptr<control_block> main_block;

  private:
    void skip(size_t skip_lines) noexcept
    {
      while (skip_lines > 0 && main_block->get_stream()->eof() == false)
        if (main_block->get_stream()->get() == m_cfg.del_row)
          --skip_lines;
    }

  public:
    parser(std::ifstream& in, size_t skip_lines, configuration new_cfg = {',', '\n', '\"'})
      : m_cfg{new_cfg}
      , main_block{std::make_shared<control_block>(&in, skip_lines)}
    {
      if (in.is_open() == false)
      {
        std::string error_message{"\033[1;35m[ERROR]\033[0m Invalid file stream.\n"
                                  "\033[1;35m[MESSAGE]\033[0m Cannot parse the file."};

        throw err::invalid_argument{error_message};
      }

      skip(skip_lines);
    }

    parser(const parser&) = delete;
    parser(parser&&) = delete;

    ~parser()
    {
      main_block->free_stream();
    }

    [[nodiscard]] iterator begin()
    {
      return iterator(main_block, m_cfg);
    }

    [[nodiscard]] iterator end()
    {
      return iterator(main_block, nullptr);
    }

    parser& operator=(const parser&) = delete;
    parser& operator=(parser&&) = delete;
  };

} /* End namespace csv */

#endif /* End parser header file */
