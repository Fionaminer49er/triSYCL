/* RUN: %{execute}%s

  Test the behavior of vector math functions.

*/

#include <cmath>
#include <iostream>
#include <numeric>

#include <sycl/sycl.hpp>
#include <boost/test/minimal.hpp>

template <typename T, int Dim>
void do_vec_unary_math(sycl::vec<T, Dim> v) {
  auto clamp = sycl::clamp(v, 2.0f, 4.0f);
  auto floor = sycl::floor(v);
  auto length = sycl::length(v);
  auto normalize = sycl::normalize(v);

  T len {0};
  for (int i = 0; i < Dim; ++i) {
    len += std::pow(v[i], 2);
  }

  len = std::sqrt(len);

  BOOST_CHECK(length == len);

  for (int i = 0; i < Dim; ++i) {
    BOOST_CHECK(clamp[i] == std::clamp(v[i], 2.0f, 4.0f));
    BOOST_CHECK(floor[i] == std::floor(v[i]));
    BOOST_CHECK(normalize[i] == v[i] / len);
  }
}

// only intended for vecs of equal size, similar to the implemented math
// functions
template <typename T, int Dim>
void do_vec_binary_math(sycl::vec<T, Dim> v, sycl::vec<T, Dim> v2) {
  auto fmin = sycl::fmin(v, v2);
  auto fmax = sycl::fmax(v, v2);
  auto min = sycl::min(v, v2);
  auto max = sycl::max(v, v2);

  for (int i = 0; i < Dim; ++i) {
    BOOST_CHECK(fmin[i] == std::fmin(v2[i], v[i]));
    BOOST_CHECK(fmax[i] == std::fmax(v[i], v2[i]));
    BOOST_CHECK(min[i] == std::min(v2[i], v[i]));
    BOOST_CHECK(max[i] == std::max(v[i], v2[i]));
  }
}

int test_main(int argc, char *argv[]) {
  sycl::float2 f2 {2.3f, 4.1f};
  sycl::float3 f3_a {1,2,3};
  sycl::float3 f3_b {1,5,7};
  sycl::float3 f3_c {3.0,1.0,2.0};
  sycl::float4 f4_a {1,2,3,2};
  sycl::float4 f4_b {1,5,7,4};
  sycl::float8 f8 {-5.0f,-5.1f,-5.9f,-6.9f,-6.1f,5.0f,5.1f,5.9f};
  sycl::float16 f16_a {10.0f};
  sycl::float16 f16_b {15};

  do_vec_unary_math(f2);
  do_vec_unary_math(f3_a);
  do_vec_unary_math(f3_b);
  do_vec_unary_math(f3_c);
  do_vec_unary_math(f4_a);
  do_vec_unary_math(f4_b);
  do_vec_unary_math(f8);
  do_vec_unary_math(f16_a);
  do_vec_unary_math(f16_b);

  do_vec_binary_math(f3_a, f3_b);
  // do_vec_binary_math(f3_a, f3_c);
  // do_vec_binary_math(f4_a, f4_b);
  // do_vec_binary_math(f16_a, f16_b);

  // not as trivially testable generically as the above
  auto cross3_ab = sycl::cross(f3_a, f3_b);
  auto cross4_ab = sycl::cross(f4_a, f4_b);

  BOOST_CHECK(cross3_ab.x() == -1);
  BOOST_CHECK(cross3_ab.y() == -4);
  BOOST_CHECK(cross3_ab.z() == 3);

  BOOST_CHECK(cross4_ab.x() == -1);
  BOOST_CHECK(cross4_ab.y() == -4);
  BOOST_CHECK(cross4_ab.z() == 3);
  BOOST_CHECK(cross4_ab.w() == 0);

  auto dot3_ab = sycl::dot(f3_a, f3_b);
  auto dot4_ab = sycl::dot(f4_a, f4_b);;
  auto dot16_ab = sycl::dot(f16_a, f16_b);;

  BOOST_CHECK(dot3_ab == 32);
  BOOST_CHECK(dot4_ab == 40);
  BOOST_CHECK(dot16_ab == 2400);

  return 0;
}
