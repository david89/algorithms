// Check http://codeforces.com/blog/entry/43499 for a very good tutorial on FFT.
// This particular implementation corresponds to
// http://go/wiki/Cooley%E2%80%93Tukey_FFT_algorithm.

#include <cassert>
#include <cmath>
#include <complex>
#include <iostream>
#include <numeric>
#include <vector>

namespace dagomez {
using complex_d = std::complex<double>;
using complex_vector = std::vector<complex_d>;

int reverse(int x, int num_bits) {
  int ans = 0;
  for (int i = 0; i < num_bits; i++) {
    ans = (ans << 1) | (x & 1);
    x >>= 1;
  }

  return ans;
}

int calculate_num_bits(int x) {
  int ans = 0;
  while (x) {
    ans++;
    x >>= 1;
  }

  return ans - 1;
}

complex_vector fft(complex_vector v, bool inverse = false) {
  const size_t n = v.size();
  const int num_bits = calculate_num_bits(n);
  complex_vector y(n);
  for (size_t i = 0; i < n; i++) {
    y[reverse(i, num_bits)] = v[i];
  }

  for (size_t p = 1; p < n; p <<= 1) {
    const double sign = inverse ? -1.0 : 1.0;
    const complex_d wn = std::polar(1.0, sign * 2.0 * M_PI / double(2 * p));

    for (size_t i = 0; i < n; i += 2 * p) {
      complex_d w = 1;
      for (size_t j = 0; j < p; j++) {
        complex_d even = y[i + j];
        complex_d odd = y[i + j + p];
        y[i + j] = even + w * odd;
        y[i + j + p] = even - w * odd;

        w *= wn;
      }
    }
  }

  return y;
}

complex_vector inverse_fft(complex_vector v) {
  v = fft(std::move(v), /*inverse=*/true);
  for (auto &e : v)
    e /= double(v.size());
  return v;
}
} // namespace dagomez

int next_power_of_two(int n) {
  int p = 1;
  while (p < n) {
    p <<= 1;
  }

  return p;
}

int main() {
  // a: 3 + 5x + 2x^2
  dagomez::complex_vector a = {3.0, 5.0, 2.0};
  // b: 5 + 3x^3
  dagomez::complex_vector b = {5.0, 0.0, 0.0, 3.0};
  const int size = next_power_of_two(a.size() + b.size() - 1);
  a.resize(size);
  b.resize(size);
  a = dagomez::fft(std::move(a));
  b = dagomez::fft(std::move(b));

  // Calculate a * b
  dagomez::complex_vector c(size);
  for (size_t i = 0; i < c.size(); i++) {
    c[i] = a[i] * b[i];
  }
  c = dagomez::inverse_fft(std::move(c));

  // c: 15 + 25x + 10x^2 + 9x^3 + 15x^4 + 6x^5
  const std::vector<int> expected_coefficients = {15, 25, 10, 9, 15, 6};
  for (size_t i = 0; i < expected_coefficients.size(); i++) {
    assert(int(round(c[i].real())) == expected_coefficients[i]);
  }

  return 0;
}
