#pragma once
#include <array>
#include <cmath>
#include <cstddef>

namespace sndbx {

template <std::size_t N> class vec {
public:
  vec &operator+=(const vec<N> &rhs) {
    for (int i = 0; i < N; i++) {
      arr[i] += rhs.arr[i];
    }

    return *this;
  }

  vec &operator-=(const vec<N> &rhs) {
    for (int i = 0; i < N; i++) {
      arr[i] -= rhs.arr[i];
    }

    return *this;
  }

  vec &operator*=(float rhs) {
    for (int i = 0; i < N; i++) {
      arr[i] *= rhs;
    }

    return *this;
  }

  friend vec operator+(vec<N> lhs, const vec<N> &rhs) { return lhs += rhs; }

  friend vec operator-(vec<N> lhs, const vec<N> &rhs) { return lhs -= rhs; }

  vec operator-() {
    vec<N> result;
    for (int i = 0; i < N; i++) {
      result.arr[i] = -arr[i];
    }
    return result;
  }

  friend vec operator*(float lhs, vec rhs) { return rhs *= lhs; }

  friend vec operator*(vec lhs, float rhs) { return lhs *= rhs; }

  float square_mag() {
    float sum_squares = 0;
    for (int i = 0; i < N; i++) {
      sum_squares += arr[i] * arr[i];
    }
    return sum_squares;
  }

  float mag() { return std::sqrt(square_mag()); }

  vec normalize() {
    float curr_magnitude = mag();
    vec<N> result;
    for (int i = 0; i < N; i++) {
      result = arr[i] / curr_magnitude;
    }
    return curr_magnitude;
  }

  // Gets the element at a given value
  float at(std::size_t i) { return arr.at(i); }

  template <std::size_t I> float &const_at() {
    static_assert(I < N, "Vec index out of bounds");
    return arr[I];
  }

  float &x() {
    static_assert(N > 0, "Vector must have at least 1 element");
    return const_at<0>();
  }

  float &y() {
    static_assert(N > 1, "Vector must have at least 2 elements");
    return const_at<1>();
  }

  float &z() {
    static_assert(N > 2, "Vector must have at least 3 elements");
    return const_at<2>();
  }

  // The inner array is public to allow aggregate initialization.
  std::array<float, N> arr;
};

}; // namespace sndbx
