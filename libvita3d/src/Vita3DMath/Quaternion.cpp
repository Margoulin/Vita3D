#include "Vita3DMath/Quaternion.hpp"
#include "Vita3DMath/Math.hpp"

const Quaternion Quaternion::identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion::Quaternion(float x, float y, float z, float w)
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

auto	Quaternion::Euler(float x, float y, float z) -> Quaternion
{
	auto	qY = Quaternion::AngleAxis(y, Vector3F::up);
	auto	qZ = Quaternion::AngleAxis(z, Vector3F::forward);
	auto	qX = Quaternion::AngleAxis(x, Vector3F::right);
	auto	temp = 	qY * qX * qZ;
	return temp;
}

auto	Quaternion::Euler(Vector3F const& value) -> Quaternion
{
	return Quaternion::Euler(value.x, value.y, value.z);
}

auto	Quaternion::Inverse(const Quaternion& value) -> Quaternion
{
	if (value.IsNormalized())
		return value.GetConjugate();
	else
		return value.Normalized().GetConjugate();
}

auto	Quaternion::AngleAxis(float angle, Vector3F const& axis) -> Quaternion
{
	auto	nAxis = axis.Normalized();
	float	sin_angle = Sin(angle / 2.0f);
	auto	temp = Quaternion(nAxis.x * sin_angle, nAxis.y * sin_angle,
		nAxis.z * sin_angle, Cos(angle / 2.0f));
	temp.Normalize();

	return temp;
}

auto	Quaternion::Dot(Quaternion const& first, Quaternion const& second) -> float
{
	return first.GetW() * second.GetW() + first.GetX() * second.GetX()
		+ first.GetY() * second.GetY() + first.GetZ() * second.GetZ();
}

auto	Quaternion::MatrixToQuaternion(Matrix4x4F const& mat) -> Quaternion
{
	float t = mat[0] + mat[5] + mat[10] + 1.0f;

	if (t > 0.0f)
	{
		float s = 0.5f / Sqrt(t);

		float x = (mat[6] - mat[9]) * s;
		float y = (mat[8] - mat[2]) * s;
		float z = (mat[1] - mat[4]) *s;
		float w = 0.25f / s;
		return Quaternion(x, y, z, w);
	}
	else
	{
		int idx = 0;
		float temp = mat[0];
		if (mat[5] > temp)
		{
			temp = mat[5];
			idx = 5;
		}
		if (mat[10] > temp)
		{
			temp = mat[10];
			idx = 10;
		}
		switch (idx)
		{
		default:
		case 0:
		{
			float s = Sqrt(1 + mat[0] - mat[5] - mat[10]) * 2;

			float x = 0.25f * s;
			float y = (mat[4] + mat[1]) / s;
			float z = (mat[8] + mat[2]) / s;
			float w = (mat[9] - mat[6]) / s;
			return Quaternion(x, y, z, w);
		}
		case 5:
		{
			float s = Sqrt(1 - mat[0] + mat[5] - mat[10]) * 2;

			float x = (mat[4] + mat[1]) / s;
			float y = 0.25f * s;
			float z = (mat[9] + mat[6]) / s;
			float w = (mat[8] - mat[2]) / s;
			return Quaternion(x, y, z, w);
		}
		case 10:
		{
			float s = Sqrt(1 - mat[0] - mat[5] + mat[10]) * 2;

			float x = (mat[8] + mat[2]) / s;
			float y = (mat[9] + mat[6]) / s;
			float z = 0.25f * s;
			float w = (mat[4] - mat[1]) / s;
			return Quaternion(x, y, z, w);
		}
		}
	}
}

auto	Quaternion::QuaternionToMatrix(Quaternion const& value) -> Matrix4x4F
{
	//		(1-2YY-2ZZ	2XY-2ZW		2XZ+2YW		)
	//	M = (2XY+2ZW	1-2XX-2ZZ	2YZ-2XW		)
	//		(2XZ-2YW	2YZ+2XW		1-2XX-2YY	)

	Matrix4x4F	ret = Matrix4x4F();

	ret[15] = 1.0f;

	float xx = value.GetX() * value.GetX();
	float yy = value.GetY() * value.GetY();
	float zz = value.GetZ() * value.GetZ();
	float xy = value.GetX() * value.GetY();
	float zw = value.GetZ() * value.GetW();
	float xz = value.GetX() * value.GetZ();
	float yw = value.GetY() * value.GetW();
	float yz = value.GetY() * value.GetZ();
	float xw = value.GetX() * value.GetW();

	ret[0] = (1.0f - 2.0f * yy - 2.0f * zz);
	ret[1] = (2.0f * xy + 2.0f * zw);
	ret[2] = (2.0f * xz - 2.0f * yw);
	
	ret[4] = (2.0f * xy - 2.0f * zw);
	ret[5] = (1.0f - 2.0f * xx - 2.0f * zz);
	ret[6] = (2.0f * yz + 2.0f * xw);
	
	ret[8] = (2.0f * xz + 2.0f * yw);
	ret[9] = (2.0f * yz - 2.0f * xw);
	ret[10] = (1.0f - 2.0f * xx - 2.0f * yy);

	return ret;
}

auto	Quaternion::Rotate(Vector3F const& value) -> void
{
	Set(*this * Euler(value));
}

auto	Quaternion::Rotate(float const x, float const y, float const z) -> void
{
	Rotate(Vector3F(x, y, z));
}

auto	Quaternion::Normalize() -> void
{
	float magnitude = getMagnitude();
	_x /= magnitude;
	_y /= magnitude;
	_z /= magnitude;
	_w /= magnitude;
}

auto	Quaternion::Normalized() const -> Quaternion
{
	float magnitude = getMagnitude();
	return Quaternion(_x / magnitude, _y / magnitude, _z / magnitude, _w / magnitude);
}

auto	Quaternion::IsNormalized() const -> bool
{
	return AreSame(1.0f, getMagnitude(), 0.00001f);
}

auto	Quaternion::ToString() const -> std::string
{
	return "Quaternion {x: " + std::to_string(_x) + ", y: " + std::to_string(_y) + ", z: " + std::to_string(_z) + ", w: " + std::to_string(_w) + "}";
}

auto	Quaternion::GetEulerAngles() const -> Vector3F
{
	float xx = _x * _x;
	float yy = _y * _y;
	float zz = _z * _z;
	float ww = _w * _w;

	float x = Atan2F((2.0f * (_y * _z + _w * _x)), (ww - xx - yy + zz));
	float y = ArcSin(-2.0f * (_x * _z - _w * _y));
	float z = Atan2F(2.0f * (_x * _y + _w * _z), (ww + xx - yy - zz));

	return Vector3F(x, y, z);
}

auto	Quaternion::GetEulerAngles(Quaternion const& q) -> Vector3F
{
	float xx = q._x *q. _x;
	float yy = q._y *q. _y;
	float zz = q._z * q._z;
	float ww = q._w * q._w;

	float x = Atan2F((2.0f * (q._y * q._z + q._w * q._x)), (ww - xx - yy + zz));
	float y = ArcSin(-2.0f * (q._x * q._z - q._w * q._y));
	float z = Atan2F(2.0f * (q._x * q._y + q._w * q._z), (ww + xx - yy - zz));

	return Vector3F(x, y, z);
}

auto	Quaternion::GetVectorPart() const -> Vector3F
{
	return Vector3F(_x, _y, _z);
}

auto	Quaternion::GetConjugate() const -> Quaternion
{
	return Quaternion(-_x, -_y, -_z, _w);
}

/*
auto	Quaternion::GetAngle() const -> float
{
	return ArcCos(_w) * 2.0f;
}

auto	Quaternion::GetAxis() const -> Vector3F
{
	float temp = 1.0f - _w * _w;
	if (temp <= 0.0f)
		return Vector3F(0.0f, 0.0f, 1.0f);
	float temp2 = 1.0f / Sqrt(temp);
	return Vector3F(_x * temp2, _y * temp2, _z * temp2);
}
*/

auto	Quaternion::Set(Quaternion const& value) -> void
{
	_x = value.GetX();
	_y = value.GetY();
	_z = value.GetZ();
	_w = value.GetW();
}

auto Quaternion::Set(float x, float y, float z, float w) -> void
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

auto	Quaternion::operator*(const Quaternion& value) const -> Quaternion
{
	float	tempW = this->GetW() * value.GetW() - this->GetX() * value.GetX()
		- this->GetY() * value.GetY() - this->GetZ() * value.GetZ();
	float	tempX = value.GetW() * this->GetX() + value.GetX() * this->GetW()
		- value.GetY() * this->GetZ() + value.GetZ() * this->GetY();
	float	tempY = value.GetW() * this->GetY() + value.GetX() * this->GetZ()
		+ value.GetY() * this->GetW() - value.GetZ() * this->GetX();
	float tempZ = value.GetW() * this->GetZ() - value.GetX() * this->GetY()
		+ value.GetY() * this->GetX() + value.GetZ() * this->GetW();

	return Quaternion(tempX, tempY, tempZ, tempW);
}

auto	Quaternion::operator*(const Vector3F& value) const -> Vector3F
{
	auto temp = Vector3F::Cross(this->GetVectorPart(), value) * 2.0f;
	Vector3F ret = value + temp * _w + Vector3F::Cross(this->GetVectorPart(), temp);
	return ret;
}

auto	Quaternion::operator[](int value) const -> float
{
	switch (value)
	{
	case 0:
		return _x;
	case 1:
		return _y;
	case 2:
		return _z;
	case 3:
		return _w;
	default:
		return 0.f;
	}
}

auto	Quaternion::operator!=(Quaternion const& value) -> bool
{
	return (1.0f != Dot(*this, value));
}

auto	Quaternion::operator==(Quaternion const& value) -> bool
{
	return (1.0f == Dot(*this, value));
}

auto	Quaternion::getMagnitude() const -> float
{
	return Sqrt(_w * _w + _x * _x + _y * _y + _z * _z);
}

auto	Quaternion::operator=(const Quaternion& value) -> void
{
	_x = value.GetX();
	_y = value.GetY();
	_z = value.GetZ();
	_w = value.GetW();
}