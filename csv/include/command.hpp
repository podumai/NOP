#ifndef __CSV_PARSER_COMMAND_HPP__ /* Begin command header file */
#define __CSV_PARSER_COMMAND_HPP__ 1

#include <utility>
#include <string_view>
#include <cinttypes>

namespace csv /* Begin namespace csv */
{

  namespace cmd /* Begin namespace cmd */
  {

    class DataHandler
    {
    private:
      std::pair<std::string_view, size_t> m_data;

    public:
      DataHandler(int32_t, char**);
      DataHandler(const DataHandler&) = default;
      DataHandler(DataHandler&&) = default;
      ~DataHandler() = default;

      [[nodiscard]] size_t getSkipLines() const noexcept;
      [[nodiscard]] const char* getFileName() const noexcept;

      DataHandler& operator=(const DataHandler&) = default;
      DataHandler& operator=(DataHandler&&) = default;
    };

  } /* End namespace cmd */

} /* End namespace csv */

#endif /* End command header file */
