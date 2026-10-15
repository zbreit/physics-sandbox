#pragma once
#include <cmath>
#include <cstddef>
#include <array>

namespace sndbx {

template<std::size_t N>
class vec {
    public:
        vec& operator+=(const vec<N>& rhs) {
            for (int i = 0; i < N; i++) {
                _arr[i] += rhs._arr[i];
            }

            return *this;
        }

        vec& operator-=(const vec<N>& rhs) {
            for (int i = 0; i < N; i++) {
                _arr[i] -= rhs._arr[i];
            }

            return *this;
        }

        vec& operator*=(float rhs) {
            for (int i = 0; i < N; i++) {
                _arr[i] *= rhs;
            }

            return *this;
        }

        friend vec operator+(vec<N> lhs, const vec<N>& rhs) {
            return lhs += rhs;
        }

        friend vec operator-(vec<N> lhs, const vec<N>& rhs) {
            return lhs -= rhs;
        }

        vec operator-() {
            vec<N> result;
            for (int i = 0; i < N; i++) {
                result._arr[i] = -_arr[i];
            }
            return result;
        }

        friend vec operator*(float lhs, vec rhs) {
            return rhs *= lhs;
        }

        friend vec operator*(vec lhs, float rhs) {
            return lhs *= rhs;
        }

        float magnitude() {
            float sumSquares = 0;
            for (int i = 0; i < N; i++) {
                sumSquares += _arr[i] * _arr[i];
            }
            return std::sqrt(sumSquares);
        }

        vec normalize() {
            float curr_magnitude = magnitude();
            vec<N> result;
            for (int i = 0; i < N; i++) {
                result = _arr[i] / curr_magnitude;
            }
            return curr_magnitude;
        }

        // Gets the element at a given value
        float at(std::size_t i) {
            return _arr.at(i);
        }

        template<std::size_t i>
        float& const_at() {
            static_assert(i < N, "Vec index out of bounds");
            return _arr[i];
        }

        float& x() {
            static_assert(N > 0, "Vector must have at least 1 element");
            return const_at<0>();
        }

        float& y() {
            static_assert(N > 1, "Vector must have at least 2 elements");
            return const_at<1>();
        }

        float& z() {
            static_assert(N > 2, "Vector must have at least 3 elements");
            return const_at<2>();
        }

        // The inner array is public to allow aggregate initialization.
        std::array<float, N> _arr;
};

};
