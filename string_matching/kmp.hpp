#ifndef STRING_MATCHING_KMP_HPP
#define STRING_MATCHING_KMP_HPP

#include <iostream>

namespace dagomez {

// Searches the string `text` for the first occurrence of `pattern`, and returns
// the corresponding index. If no occurrence is found, it returns
// std::string::npos.
// TODO: add support for string_view.
size_t find(const std::string& text, const std::string& pattern);

}  // namespace dagomez
#endif  // ifndef STRING_MATCHING_KMP_HPP
