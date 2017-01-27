#include <cmath>

#include "Vita3DMath/Math.hpp"

auto	Sqrt(float a) -> float
{
	//if (a < 0)
		//"Negative number always return -nan"
	return sqrt(a);
}


auto	InvSqrt(float a) -> float 
{
	//if (a < 0)
		//"Negative number always return -nan";
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = a * 0.5F;
	y = a;
	i = *(long *)&y; // evil floating point bit level hacking
	i = 0x5f375a86 - (i >> 1); // what the fuck?
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y)); // 1st iteration
	// y = y * (threehalfs - (x2 * y * y)); // 2nd iteration, this can be removed
	return y;
}


auto	Abs(float a) -> float
{
	if (a >= 0.f)
		return a;
	return -a;
}


auto	Abs(int a) -> int
{
	if (a >= 0)
		return a;
	return -a;
}


auto	AreSame(float a, float b, float epsilon) -> bool
{
	float difference = a - b;
	return difference < epsilon && -difference < epsilon;
}


auto	Cos(float a) -> float
{
	return cos(a);
}


auto	Sin(float a) -> float
{
	return sin(a);
}


auto	Tan(float a) -> float
{
	return tan(a);
}
	

auto	ArcCos(float a) -> float
{
	return acos(a);
}


auto	ArcSin(float a) -> float
{
	return asin(a);
}
	

auto	ArcTan(float a) -> float
{
	return atan(a);
}


auto	Ceil(float a) -> float
{
	return ceil(a);
}


auto	Floor(float a) -> float
{
	return floor(a);
}


auto	Round(float a) -> float
{
	return round(a);
}


auto	Ceilint(float a) -> int
{
	return (int)Ceil(a);
}


auto	Floorint(float a) -> int
{
	return (int)Floor(a);
}


auto	Roundint(float a) -> int
{
	return (int)Round(a);
}


auto	Clamp(float a, float min, float max) -> float
{
	if (a < min)
		return min;
	if (a > max)
		return max;
	return a;
}


auto	Clamp01(float a) -> float
{
	if (a < 0.f)
		return 0.f;
	if (a > 1.f)
		return 1.f;
	return a;
}


auto	Lerp(float a, float b, float t) -> float
{
	t = Clamp01(t);
	return a * (1.f - t) + b * t;
}


auto	Max(float a, float b) -> float
{
	if (a > b)
		return a;
	return b;
}


auto	Min(float a, float b) -> float
{
	if (a < b)
		return a;
	return b;
}


auto	Power(float a, float p) -> float
{
	//if (a < 0.f && p < 0.f && ((Round(p) - p) < FLT_EPSILON && -(Round(p) - p) < FLT_EPSILON))
		//"Negative a and negative p with decimal composant not equal to 0 will always return -nan";
	return powf(a, p);
}