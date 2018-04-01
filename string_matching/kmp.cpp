#include <string>
#include <vector>

namespace dagomez {

/**
 * Calculates the length of the biggest proper prefix that is also a suffix of
 * each substring of pattern. For example:
 *
 * ABCABA:
 * "" -> -1 (we use -1 as a sentinel for the empty string case)
 * "A" -> 0
 * "AB" -> 0
 * "ABC" -> 0
 * "ABCA" -> 1
 * "ABCAB" -> 2
 * "ABCABA" -> 1
 *
 * Note that the prefix and suffix may overlap. For example:
 * 
 * ABABAC
 * "" -> -1
 * "A" -> 0
 * "AB" -> 0
 * "ABA" -> 1
 * "ABAB" -> 2
 * "ABABA" -> 3
 * "ABABAC" -> 0
 */
std::vector<int> failure_function(const std::string& pattern) {
  std::vector<int> v(pattern.size() + 1, 0);
  v[0] = -1;
  for (size_t i = 2; i < v.size(); i++) {
    int best_prefix = v[i - 1];
    while (best_prefix >= 0 && pattern[i - 1] != pattern[best_prefix]) {
      best_prefix = v[best_prefix];
    }
    // If we end up with no proper prefix (an empty string), best_prefix should
    // correspond to v[0], which is -1. So, incrementing by 1, gets us to 0
    // (empty string).
    v[i] = best_prefix + 1;
  }

  return v;
}

size_t find(const std::string& text, const std::string& pattern) {
  const std::vector<int> failure = failure_function(pattern);
  const int m = pattern.size();
  int j = 0;
  size_t i;
  for (i = 0; i < text.size() && j < m; i++) {
    while (j >= 0 && text[i] != pattern[j]) {
      j = failure[j];
    }
    // If we found a match, then it makes sense to increment j. If no prefix
    // worked, then j is equivalent to failure[0], which is -1. So, incrementing
    // by 1, just resets j to 0.
    j++;
  }

  // Check if we found a match.
  if (j == m) {
    return i - m;
  }

  return std::string::npos;
}

}  // namespace dagomez

#include <iostream>
using namespace std;

int main() {
  cout << dagomez::find("this is my text", "fail") << endl;
  cout << dagomez::find("this is my text", "text") << endl;
  cout << dagomez::find("this is my text", "this") << endl;
  cout << dagomez::find("this is my text", "my") << endl;
  return 0;
}
