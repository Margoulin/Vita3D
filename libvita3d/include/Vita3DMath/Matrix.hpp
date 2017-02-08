#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include "Vita3DMath/Vector.hpp"
#include <string>

//class Quaternion;

//read by Column
class Matrix4x4F
{
public:
	Matrix4x4F();
	Matrix4x4F(float n1, float n2, float n3, float n4, float n5, float n6, float n7, float n8, float n9, float n10, float n11, float n12, float n13, float n14, float n15, float n16);
	Matrix4x4F(const Matrix4x4F&);

	static auto Mult(const Matrix4x4F& mat1, const Matrix4x4F& mat2) -> Matrix4x4F;
	static auto Mult(const Matrix4x4F& mat, const Vector4F& vect) -> Vector4F;
	static auto Mult(const Vector4F& vect, const Matrix4x4F& mat) -> Vector4F;

	static auto	Translate(const Matrix4x4F& mat, const Vector3F& value) -> Matrix4x4F;
	/*
	/// Rotate function wich return a matrix and take an axis and a radiant angle as parameters
	static auto	Rotate(const Matrix4x4F& mat, const float angle, const Vector3F& axis) -> Matrix4x4F;
	/// Rotate function wich return a matrix and take a quaternion
	static auto	Rotate(const Matrix4x4F& mat, const Quaternion& value) -> Matrix4x4F;
	*/
	static auto	Scale(const Matrix4x4F& mat, const Vector3F& value) -> Matrix4x4F;
	
	static auto	Perspective(const float Fov, const float aspectRatio, const float zNear, const float zFar) -> Matrix4x4F;
	static auto	Orthographic(const float right, const float top, const float zNear, const float zFar) -> Matrix4x4F;
	static auto	Transpose(const Matrix4x4F&) -> Matrix4x4F;
	static auto	Inverse(const Matrix4x4F&) -> Matrix4x4F;
	static auto	FastInverse(const Matrix4x4F&) -> Matrix4x4F;
	static auto	LookAt(Vector3F center, Vector3F up, Vector3F target) -> Matrix4x4F;
	
	static auto	GetUpFromView(const Matrix4x4F&) -> Vector3F;
	static auto	GetRightFromView(const Matrix4x4F&) -> Vector3F;
	static auto	GetForwardFromView(const Matrix4x4F&) -> Vector3F;
	static auto	GetPositionFromView(const Matrix4x4F&) -> Vector3F;
	static auto	GetPositionFromModelMatrix(const Matrix4x4F& value)->Vector3F;
	
	auto	ToString() const -> std::string;
	
	auto	SetRow(unsigned int const rowIdx, const Vector2F& value) -> void;
	auto	SetRow(unsigned int const rowIdx, const Vector3F& value) -> void;
	auto	SetRow(unsigned int const rowIdx, const Vector4F& value) -> void;

	auto	SetColumn(unsigned int const rowIdx, const Vector2F& value) -> void;
	auto	SetColumn(unsigned int const rowIdx, const Vector3F& value) -> void;
	auto	SetColumn(unsigned int const rowIdx, const Vector4F& value) -> void;

	auto	GetArray() const -> const float * const{ return _values; }

	auto	operator[] (int index) -> float& { return _values[index]; }
	auto	operator[] (int index) const -> float { return _values[index]; }
	auto	operator==(const Matrix4x4F& m2) const -> bool;
	auto	operator!=(const Matrix4x4F& m2) const -> bool;
	auto	operator=(const Matrix4x4F& m2) -> Matrix4x4F&;

	static const Matrix4x4F identity;
	static const Matrix4x4F zero;

private:

	float	_values[16];
};

#endif /*__MATRIX_HPP__*/