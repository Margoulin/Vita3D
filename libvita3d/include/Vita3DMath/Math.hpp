#ifndef __MATH_HPP__
#define __MATH_HPP__

#include <float.h>

static const float PI = 3.141592653589793238462643383279502884f;

static const float degToRad = PI / 180.f;
static const float radToDeg = 180.f / PI;

auto	Sqrt(float a) -> float;

auto	InvSqrt(float a) -> float;

auto	Abs(float a) -> float;
auto	Abs(int a) -> int;

auto	AreSame(float a, float b, float epsilon = FLT_EPSILON) -> bool;

auto	Cos(float a) -> float;
auto	Sin(float a) -> float;
auto	Tan(float a) -> float;

auto	ArcCos(float a) -> float;
auto	ArcSin(float a) -> float;
auto	ArcTan(float a) -> float;

auto	Ceil(float a) -> float;
auto	Floor(float a) -> float;
auto	Round(float a) -> float;

auto	Ceilint(float a) -> int;
auto	Floorint(float a) -> int;
auto	Roundint(float a) -> int;

auto	Clamp(float a, float min, float max) -> float;
auto	Clamp01(float a) -> float;

auto	Lerp(float a, float b, float t) -> float;

auto	Max(float a, float b) -> float;
auto	Min(float a, float b) -> float;

auto	Power(float a, float p) -> float;

#endif /*__MATH_HPP__*/
