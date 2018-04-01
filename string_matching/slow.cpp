#include <iostream>
#include <string>

namespace dagomez {

size_t find(const std::string& text, const std::string& pattern) {
  for (size_t i = 0; i + pattern.size() <= text.size(); i++) {
    size_t j;
    for (j = 0; j < pattern.size(); j++) {
      if (text[i + j] != pattern[j]) {
        break;
      }
    }

    if (j == pattern.size()) {
      return i;
    }
  }

  return std::string::npos;
}

}  // dagomez

using namespace std;
int main() {
  cout << dagomez::find("this is my text", "fail") << endl;
  cout << dagomez::find("this is my text", "text") << endl;
  cout << dagomez::find("this is my text", "this") << endl;
  cout << dagomez::find("this is my text", "my") << endl;
  return 0;
}
