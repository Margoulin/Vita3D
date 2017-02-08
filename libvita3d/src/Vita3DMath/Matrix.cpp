#include "Vita3DMath/Matrix.hpp"

#include <cstring>

#include "Vita3DMath/Math.hpp"

const Matrix4x4F Matrix4x4F::identity = Matrix4x4F(
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f);

const Matrix4x4F Matrix4x4F::zero = Matrix4x4F();

Matrix4x4F::Matrix4x4F()
{
	for (unsigned int i = 0; i < 16; i++)
		_values[i] = 0.f;
}


Matrix4x4F::Matrix4x4F(float n1, float n2, float n3, float n4, float n5, float n6, float n7, float n8, float n9, float n10, float n11, float n12, float n13, float n14, float n15, float n16)
{
	_values[0] = n1;
	_values[1] = n2;
	_values[2] = n3;
	_values[3] = n4;
	_values[4] = n5;
	_values[5] = n6;
	_values[6] = n7;
	_values[7] = n8;
	_values[8] = n9;
	_values[9] = n10;
	_values[10] = n11;
	_values[11] = n12;
	_values[12] = n13;
	_values[13] = n14;
	_values[14] = n15;
	_values[15] = n16;
}


Matrix4x4F::Matrix4x4F(const Matrix4x4F& mat)
{
	for (unsigned int pos = 0u; pos < 16; pos++)
		_values[pos] = mat[pos];
}


auto	Matrix4x4F::Mult(const Matrix4x4F& mat1, const Matrix4x4F& mat2) -> Matrix4x4F
{
	Matrix4x4F res = Matrix4x4F();

	for (unsigned int j = 0; j < 4; j++)
		for (unsigned int i = 0; i < 4; i++)
			res[j * 4 + i] = mat1[i] * mat2[j * 4] + mat1[i + 4] * mat2[j * 4 + 1] + mat1[i + 8] * mat2[j * 4 + 2] + mat1[i + 12] * mat2[j * 4 + 3];

	return res;
}


auto	Matrix4x4F::Mult(const Matrix4x4F& mat, const Vector4F& vect) -> Vector4F
{
	//VECTOR AS COLUMN VECTOR
	float x = mat[0] * vect.x + mat[4] * vect.y + mat[8] * vect.z + mat[12] * vect.w;
	float y = mat[1] * vect.x + mat[5] * vect.y + mat[9] * vect.z + mat[13] * vect.w;
	float z = mat[2] * vect.x + mat[6] * vect.y + mat[10] * vect.z + mat[14] * vect.w;
	float w = mat[3] * vect.x + mat[7] * vect.y + mat[11] * vect.z + mat[15] * vect.w;

	return Vector4F(x, y, z, w);
}


auto	Matrix4x4F::Translate(const Matrix4x4F& mat, const Vector3F& value) -> Matrix4x4F
{
	Matrix4x4F	Result(mat);
	Result[12] += value.x;
	Result[13] += value.y;
	Result[14] += value.z;
	return Result;
}


/*
auto	Matrix4x4F::Rotate(const Matrix4x4F& mat, const float angle, const Vector3F& axis) -> Matrix4x4F
{
	float	degangle = angle * radToDeg;
	float	cosAngle = Cos(degangle);
	float	sinAngle = Sin(degangle);

	Vector3F	NormAxis = axis.Normalized();
	Vector3F	Temp = (NormAxis * (1 - cosAngle));

	Matrix4x4F	rotate = identity;
	rotate[15] = 0.0f;

	rotate[0] = cosAngle + Temp.x * NormAxis.x;
	rotate[1] = Temp.x * NormAxis.y + sinAngle * NormAxis.z;
	rotate[2] = Temp.x * NormAxis.z - sinAngle * NormAxis.y;
	
	rotate[4] = Temp.y * NormAxis.x - sinAngle * NormAxis.z;
	rotate[5] = cosAngle + Temp.y * NormAxis.y;
	rotate[6] = Temp.y * NormAxis.z + sinAngle * NormAxis.x;

	rotate[8] = Temp.z * NormAxis.x + sinAngle * NormAxis.y;
	rotate[9] = Temp.z * NormAxis.y - sinAngle * NormAxis.x;
	rotate[10] = cosAngle + Temp.z * NormAxis.z;

	Vector4F	mat0(mat[0], mat[1], mat[2], mat[3]);
	Vector4F	mat1(mat[4], mat[5], mat[6], mat[7]);
	Vector4F	mat2(mat[8], mat[9], mat[10], mat[11]);
	Vector4F	firstColumn = mat0 * rotate[0] + mat1 * rotate[1] + mat2 * rotate[2];
	Vector4F	secondColumn = mat0 * rotate[4] + mat1 * rotate[5] + mat2 * rotate[6];
	Vector4F	thirdColumn = mat0 * rotate[8] + mat1 * rotate[9] + mat2 * rotate[10];
	
	Matrix4x4F Result(	mat0.x, mat0.y, mat0.z, mat0.w,
						mat1.x, mat1.y, mat1.z, mat1.w,
						mat2.x, mat2.y, mat2.z, mat2.w,
						mat[12], mat[13], mat[14], mat[15]	);
	return Result;
}


auto	Matrix4x4F::Rotate(const Matrix4x4F& mat, const Quaternion& value) -> Matrix4x4F
{
	auto angle = value.GetAngle();
	auto axis = value.GetAxis();
	return Rotate(mat, angle, axis);
}
*/


auto	Matrix4x4F::Scale(const Matrix4x4F& mat, const Vector3F& value) -> Matrix4x4F
{
	Matrix4x4F temp = identity;
	
	temp[0] = value.x;
	temp[5] = value.y;
	temp[10] = value.z;

	return Mult(mat, temp);
}


auto	Matrix4x4F::Perspective(float const Fov, float const apectRatio, float const zNear, float const zFar) -> Matrix4x4F
{
	float	fovRad = degToRad * Fov;
	float	tanHalfFov = Tan(fovRad * 0.5f);
	Matrix4x4F	res = Matrix4x4F::identity;

	res[0] = 1.f / (apectRatio * tanHalfFov);
	res[5] = 1.f / tanHalfFov;
	res[10] = -(zFar + zNear) / (zFar - zNear);
	res[11] = -1.f;
	res[14] = -(2.f * zFar * zNear) / (zFar - zNear);
	res[15] = 0.f;

	return res;
}


auto	Matrix4x4F::Orthographic(const float right, const float top, const float zNear, const float zFar) -> Matrix4x4F
{
	float	fMn = zFar - zNear;

	Matrix4x4F	res = Matrix4x4F::identity;

	res[0] = 1.0f / right;
	res[5] = 1.0f / top;
	res[10] = -2.0f / (fMn);
	res[14] = -(zFar + zNear) / fMn;
	return res;
}


auto	Matrix4x4F::Inverse(const Matrix4x4F& value) -> Matrix4x4F
{
	float det;
	Matrix4x4F	ret;

	ret[0] = value[5] * value[10] * value[15] -
		value[5] * value[14] * value[11] -
		value[6] * value[9] * value[15] +
		value[6] * value[13] * value[11] +
		value[7] * value[9] * value[14] -
		value[7] * value[13] * value[10];

	ret[1] = -value[1] * value[10] * value[15] +
		value[1] * value[14] * value[11] +
		value[2] * value[9] * value[15] -
		value[2] * value[13] * value[11] -
		value[3] * value[9] * value[14] +
		value[3] * value[13] * value[10];

	ret[2] = value[1] * value[6] * value[15] -
		value[1] * value[14] * value[7] -
		value[2] * value[5] * value[15] +
		value[2] * value[13] * value[7] +
		value[3] * value[5] * value[14] -
		value[3] * value[13] * value[6];

	ret[3] = -value[1] * value[6] * value[11] +
		value[1] * value[10] * value[7] +
		value[2] * value[5] * value[11] -
		value[2] * value[9] * value[7] -
		value[3] * value[5] * value[10] +
		value[3] * value[9] * value[6];

	ret[4] = -value[4] * value[10] * value[15] +
		value[4] * value[14] * value[11] +
		value[6] * value[8] * value[15] -
		value[6] * value[12] * value[11] -
		value[7] * value[8] * value[14] +
		value[7] * value[12] * value[10];

	ret[5] = value[0] * value[10] * value[15] -
		value[0] * value[14] * value[11] -
		value[2] * value[8] * value[15] +
		value[2] * value[12] * value[11] +
		value[3] * value[8] * value[14] -
		value[3] * value[12] * value[10];

	ret[6] = -value[0] * value[6] * value[15] +
		value[0] * value[14] * value[7] +
		value[2] * value[4] * value[15] -
		value[2] * value[12] * value[7] -
		value[3] * value[4] * value[14] +
		value[3] * value[12] * value[6];

	ret[7] = value[0] * value[6] * value[11] -
		value[0] * value[10] * value[7] -
		value[2] * value[4] * value[11] +
		value[2] * value[8] * value[7] +
		value[3] * value[4] * value[10] -
		value[3] * value[8] * value[6];

	ret[8] = value[4] * value[9] * value[15] -
		value[4] * value[13] * value[11] -
		value[5] * value[8] * value[15] +
		value[5] * value[12] * value[11] +
		value[7] * value[8] * value[13] -
		value[7] * value[12] * value[9];

	ret[9] = -value[0] * value[9] * value[15] +
		value[0] * value[13] * value[11] +
		value[1] * value[8] * value[15] -
		value[1] * value[12] * value[11] -
		value[3] * value[8] * value[13] +
		value[3] * value[12] * value[9];

	ret[10] = value[0] * value[5] * value[15] -
		value[0] * value[13] * value[7] -
		value[1] * value[4] * value[15] +
		value[1] * value[12] * value[7] +
		value[3] * value[4] * value[13] -
		value[3] * value[12] * value[5];

	ret[11] = -value[0] * value[5] * value[11] +
		value[0] * value[9] * value[7] +
		value[1] * value[4] * value[11] -
		value[1] * value[8] * value[7] -
		value[3] * value[4] * value[9] +
		value[3] * value[8] * value[5];

	ret[12] = -value[4] * value[9] * value[14] +
		value[4] * value[13] * value[10] +
		value[5] * value[8] * value[14] -
		value[5] * value[12] * value[10] -
		value[6] * value[8] * value[13] +
		value[6] * value[12] * value[9];

	ret[13] = value[0] * value[9] * value[14] -
		value[0] * value[13] * value[10] -
		value[1] * value[8] * value[14] +
		value[1] * value[12] * value[10] +
		value[2] * value[8] * value[13] -
		value[2] * value[12] * value[9];

	ret[14] = -value[0] * value[5] * value[14] +
		value[0] * value[13] * value[6] +
		value[1] * value[4] * value[14] -
		value[1] * value[12] * value[6] -
		value[2] * value[4] * value[13] +
		value[2] * value[12] * value[5];

	ret[15] = value[0] * value[5] * value[10] -
		value[0] * value[9] * value[6] -
		value[1] * value[4] * value[10] +
		value[1] * value[8] * value[6] +
		value[2] * value[4] * value[9] -
		value[2] * value[8] * value[5];

	det = value[0] * ret[0] + value[4] * ret[1] + value[8] * ret[2] + value[12] * ret[3];

	if (det == 0) return Matrix4x4F::identity;

	det = 1.0f / det;

	for (unsigned int i = 0; i < 16; i++)
		ret[i] = ret[i] * det;

	return ret;
}


auto	Matrix4x4F::FastInverse(const Matrix4x4F& value) -> Matrix4x4F
{
	Vector3F	X = GetRightFromView(value);
	Vector3F	Y = GetUpFromView(value);
	Vector3F	Z = GetForwardFromView(value);

	if (Vector3F::Dot(X, Y) != 0.0f)
		return identity;

	if (Vector3F::Dot(Y, Z) != 0.0f)
		return identity;

	if (Vector3F::Dot(X, Z) != 0.0f)
		return identity;

	return Matrix4x4F(
		X.x, X.y, X.z, -value[3],
		Y.x, Y.y, Y.z, -value[7],
		Z.x, Z.y, Z.z, -value[11],
		value[12], value[13], value[14], value[15]
	);
}


auto	Matrix4x4F::LookAt(Vector3F center, Vector3F up, Vector3F target) -> Matrix4x4F
{
	Vector3F	zaxis = (target - center).Normalized();
	Vector3F	xaxis = Vector3F::Cross(zaxis, up).Normalized();
	Vector3F	yaxis = Vector3F::Cross(xaxis, zaxis);

	Matrix4x4F	ret(
		xaxis.x, yaxis.x, -zaxis.x, 0.0f,
		xaxis.y, yaxis.y, -zaxis.y, 0.0f,
		xaxis.z, yaxis.z, -zaxis.z, 0.0f,
		-Vector3F::Dot(xaxis, center), -Vector3F::Dot(yaxis, center), Vector3F::Dot(zaxis, center), 1.0f);

	return ret;
}


auto	Matrix4x4F::GetUpFromView(const Matrix4x4F& value) -> Vector3F
{
	return Vector3F(value[1], value[5], value[9]);
}


auto	Matrix4x4F::GetRightFromView(const Matrix4x4F& value) -> Vector3F
{
	return Vector3F(value[0], value[4], value[8]);
}


auto	Matrix4x4F::GetForwardFromView(const Matrix4x4F& value) -> Vector3F
{
	return Vector3F(value[2], value[6], value[10]);
}


auto	Matrix4x4F::GetPositionFromView(const Matrix4x4F& value) -> Vector3F
{
	auto temp = Inverse(value);
	return Vector3F(temp[3], temp[7], temp[11]);
}


auto	Matrix4x4F::GetPositionFromModelMatrix(const Matrix4x4F& value) -> Vector3F
{
	return Vector3F(value[3], value[7], value[11]);
}


auto	Matrix4x4F::ToString() const -> std::string
{
	std::string res = "Matrix4x4F | \n\t";

	unsigned int	benchmark = 0u;

	for (unsigned int i = 0; i < 16; ++i)
	{

		res += " " + std::to_string(_values[i]) + " ";
		++benchmark;

		if (benchmark == 4)
		{
			res += "|\n\t   |";
			benchmark = 0u;
		}
	}

	return res;
}


auto	Matrix4x4F::SetRow(unsigned int const rowIdx, const Vector2F& value) -> void
{
	if (rowIdx > 3)
		return;
	switch (rowIdx)
	{
		case 0:
		{
			_values[0] = value.x;
			_values[4] = value.y;
			break;
		}
		case 1:
		{
			_values[1] = value.x;
			_values[5] = value.y;
			break;
		}
		case 2:
		{
			_values[2] = value.x;
			_values[6] = value.y;
			break;
		}
		case 3:
		{
			_values[3] = value.x;
			_values[7] = value.y;
			break;
		}
	}
}


auto	Matrix4x4F::SetRow(unsigned int const rowIdx, const Vector3F& value) -> void
{
	if (rowIdx > 3)
		return;
	switch (rowIdx)
	{
		case 0:
		{
			_values[0] = value.x;
			_values[4] = value.y;
			_values[8] = value.z;
			break;
		}
		case 1:
		{
			_values[1] = value.x;
			_values[5] = value.y;
			_values[9] = value.z;
			break;
		}
		case 2:
		{
			_values[2] = value.x;
			_values[6] = value.y;
			_values[10] = value.z;
			break;
		}
		case 3:
		{
			_values[3] = value.x;
			_values[7] = value.y;
			_values[11] = value.z;
			break;
		}
	}
}


auto	Matrix4x4F::SetRow(unsigned int const rowIdx, const Vector4F& value) -> void
{
	if (rowIdx > 3)
		return;
	switch (rowIdx)
	{
		case 0:
		{
			_values[0] = value.x;
			_values[4] = value.y;
			_values[8] = value.z;
			_values[12] = value.w;
			break;
		}
		case 1:
		{
			_values[1] = value.x;
			_values[5] = value.y;
			_values[9] = value.z;
			_values[13] = value.w;
			break;
		}
		case 2:
		{
			_values[2] = value.x;
			_values[6] = value.y;
			_values[10] = value.z;
			_values[14] = value.w;
			break;
		}
		case 3:
		{
			_values[3] = value.x;
			_values[7] = value.y;
			_values[11] = value.z;
			_values[15] = value.w;
			break;
		}
	}
}


auto	Matrix4x4F::SetColumn(unsigned int const columnIdx, const Vector2F& value) -> void
{
	if (columnIdx > 3)
		return;
	switch (columnIdx)
	{
		case 0:
		{
			_values[0] = value.x;
			_values[1] = value.y;
			break;
		}
		case 1:
		{
			_values[4] = value.x;
			_values[5] = value.y;
			break;
		}
		case 2:
		{
			_values[8] = value.x;
			_values[9] = value.y;
			break;
		}
		case 3:
		{
			_values[12] = value.x;
			_values[13] = value.y;
			break;
		}
	}
}


auto	Matrix4x4F::SetColumn(unsigned int const columnIdx, const Vector3F& value) -> void
{
	if (columnIdx > 3)
		return;
	switch (columnIdx)
	{
		case 0:
		{
			_values[0] = value.x;
			_values[1] = value.y;
			_values[2] = value.z;
			break;
		}
		case 1:
		{
			_values[4] = value.x;
			_values[5] = value.y;
			_values[6] = value.z;
			break;
		}
		case 2:
		{
			_values[8] = value.x;
			_values[9] = value.y;
			_values[10] = value.z;
			break;
		}
		case 3:
		{
			_values[12] = value.x;
			_values[13] = value.y;
			_values[14] = value.z;
			break;
		}
	}
}


auto	Matrix4x4F::SetColumn(unsigned int const columnIdx, const Vector4F& value) -> void
{
	if (columnIdx > 3)
		return;
	switch (columnIdx)
	{
		case 0:
		{
			_values[0] = value.x;
			_values[1] = value.y;
			_values[2] = value.z;
			_values[3] = value.w;
			break;
		}
		case 1:
		{
			_values[4] = value.x;
			_values[5] = value.y;
			_values[6] = value.z;
			_values[7] = value.w;
			break;
		}
		case 2:
		{
			_values[8] = value.x;
			_values[9] = value.y;
			_values[10] = value.z;
			_values[11] = value.w;
			break;
		}
		case 3:
		{
			_values[12] = value.x;
			_values[13] = value.y;
			_values[14] = value.z;
			_values[15] = value.w;
			break;
		}
	}
}


auto	Matrix4x4F::operator==(const Matrix4x4F& m2) const -> bool
{
	for (unsigned int pos = 0u; pos < 16; pos++)
	{
		if (this->_values[pos] != m2[pos])
			return false;
	}
	return true;
}


auto	Matrix4x4F::operator!=(const Matrix4x4F& m2) const -> bool
{
	for (unsigned int pos = 0u; pos < 16; pos++)
	{
		if (this->_values[pos] != m2[pos])
			return true;
	}
	return false;
}


auto	Matrix4x4F::operator=(const Matrix4x4F& m2) -> Matrix4x4F&
{
	memcpy(_values, m2._values, sizeof(float) * 16);
	return *this;
}


auto	Matrix4x4F::Transpose(const Matrix4x4F& value) -> Matrix4x4F
{
	return Matrix4x4F(value[0], value[4], value[8], value[12],
		value[1], value[5], value[9], value[13],
		value[2], value[6], value[10], value[14],
		value[3], value[7], value[11], value[15]);
}