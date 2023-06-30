#ifndef PULSAR_VECTOR_ALL_INCLUDED
#define PULSAR_VECTOR_ALL_INCLUDED

#include "setup.hpp"
#include "Math/Vector1.inl"
#include "Math/Vector2.inl"
#include "Math/Vector3.inl"
#include "Math/Vector4.inl"
#include "Math/Functions.hpp"

using vec1 = Pulsar::vec<1, Pulsar::Decimal>;
using vec2 = Pulsar::vec<2, Pulsar::Decimal>;
using vec3 = Pulsar::vec<3, Pulsar::Decimal>;
using vec4 = Pulsar::vec<4, Pulsar::Decimal>;
using fvec1 = Pulsar::vec<1, float>;
using fvec2 = Pulsar::vec<2, float>;
using fvec3 = Pulsar::vec<3, float>;
using fvec4 = Pulsar::vec<4, float>;
using dvec1 = Pulsar::vec<1, double>;
using dvec2 = Pulsar::vec<2, double>;
using dvec3 = Pulsar::vec<3, double>;
using dvec4 = Pulsar::vec<4, double>;
using bvec1 = Pulsar::vec<1, bool>;
using bvec2 = Pulsar::vec<2, bool>;
using bvec3 = Pulsar::vec<3, bool>;
using bvec4 = Pulsar::vec<4, bool>;
using svec1 = Pulsar::vec<1, int16_t>;
using svec2 = Pulsar::vec<2, int16_t>;
using svec3 = Pulsar::vec<3, int16_t>;
using svec4 = Pulsar::vec<4, int16_t>;
using ivec1 = Pulsar::vec<1, int32_t>;
using ivec2 = Pulsar::vec<2, int32_t>;
using ivec3 = Pulsar::vec<3, int32_t>;
using ivec4 = Pulsar::vec<4, int32_t>;
using llvec1 = Pulsar::vec<1, int64_t>;
using llvec2 = Pulsar::vec<2, int64_t>;
using llvec3 = Pulsar::vec<3, int64_t>;
using llvec4 = Pulsar::vec<4, int64_t>;
using ullvec1 = Pulsar::vec<1, uint64_t>;
using ullvec2 = Pulsar::vec<2, uint64_t>;
using ullvec3 = Pulsar::vec<3, uint64_t>;
using ullvec4 = Pulsar::vec<4, uint64_t>;
#endif