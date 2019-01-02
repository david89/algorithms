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

  const complex_vector y_even = fft(std::move(even));
  const complex_vector y_odd = fft(std::move(odd));

  const complex_d wn = std::polar(1.0, 2.0 * M_PI / double(n));
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
  v = fft(std::move(v));
  for (auto& e : v) e = conj(e);
  for (auto& e : v) e /= double(v.size());
  return v;
}
}

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
