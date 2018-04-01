// Check http://codeforces.com/blog/entry/43499 for a very good tutorial on FFT.
// This particular implementation corresponds to
// http://go/wiki/Cooley%E2%80%93Tukey_FFT_algorithm.

#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <numeric>
#include <vector>
 
using namespace std;
 
using d_type = double;
using complex_d = complex<d_type>;
using complex_vector = vector<complex_d>;
 
complex_vector fft(complex_vector v) {
  const size_t n = v.size();
  if (n <= 1) return v;
 
  complex_vector even(n / 2);
  complex_vector odd(n / 2);
  for (size_t i = 0; i < n; i++) {
    if (i & 1) {
      odd[i / 2] = v[i];
    } else {
      even[i / 2] = v[i];
    }
  }
 
  const complex_vector y_even = fft(move(even));
  const complex_vector y_odd = fft(move(odd));
 
  complex_d wn = polar(d_type(1.0), 2.0 * M_PI / d_type(n));
  complex_d w = 1;
 
  complex_vector y(n);
  for (size_t i = 0; i < n / 2; i++) {
    y[i] = y_even[i] + w * y_odd[i];
    y[i + n / 2] = y_even[i] - w * y_odd[i];
    w *= wn;
  }
 
  return y;
}
 
complex_vector inverse_fft(complex_vector v) {
  for (auto& e : v) e = conj(e);
  v = fft(move(v));
  for (auto& e : v) e = conj(e);
  for (auto& e : v) e /= double(v.size());
  return v;
}
 
complex_vector string_to_vector(string s) {
  reverse(begin(s), end(s));
  complex_vector ans;
  ans.reserve(s.size());
  for (char c : s) {
    ans.push_back({d_type(c - '0')});
  }
 
  return ans;
}
 
string vector_to_number(complex_vector v) {
  size_t last_non_zero = 0;
  int carry_over = 0;
  string s(v.size(), '0');
  for (size_t i = 0; i < v.size(); i++) {
    int num = carry_over + int(round(v[i].real()));
    carry_over = num / 10;
    s[i] = (num % 10) + '0';
    if (num % 10) last_non_zero = i;
  }
 
  reverse(begin(s), begin(s) + last_non_zero + 1);
  return s.substr(0, last_non_zero + 1);
}
 
int next_power_of_two(int n) {
  int p = 1;
  while (p < n) {
    p <<= 1;
  }
  return p;
}
 
string multiply_two_numbers(string a, string b) {
  complex_vector va = string_to_vector(move(a));
  complex_vector vb = string_to_vector(move(b));
 
  int n = next_power_of_two(va.size() + vb.size());
  va.resize(n, 0);
  vb.resize(n, 0);
 
  va = fft(move(va));
  vb = fft(move(vb));
  complex_vector vc(n);
  for (int i = 0; i < n; i++) {
    vc[i] = va[i] * vb[i];
  }
  vc = inverse_fft(move(vc));
 
  return vector_to_number(vc);
}
 
int main() {
  cin.sync_with_stdio(false);

  // Format is:
  // First line contains an integer n.
  // The next n lines contain two integers a and b.
  int n;
  cin >> n;
  while (n--) {
    string a, b;
    cin >> a >> b;
    cout << multiply_two_numbers(move(a), move(b)) << endl;
  }
 
  return 0;
}
