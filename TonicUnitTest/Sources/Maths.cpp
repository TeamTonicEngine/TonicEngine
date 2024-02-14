#include "pch.hpp"
#include "Maths.hpp"
#include <cmath>

using namespace Maths;

float Maths::ToRadians(float _angleDegrees)
{
    return _angleDegrees * (Constants::PI / 180);
}
double Maths::ToRadians(double _angleDegrees)
{
    return _angleDegrees * (Constants::PI_PRECISE / 180);
}

float Maths::ToDegrees(float _angleRadians)
{
    return _angleRadians * (180 / Constants::PI);
}
double Maths::ToDegrees(double _angleRadians)
{
    return _angleRadians * (180 / Constants::PI_PRECISE);
}



Vec2::Vec2(void) : x(0.f), y(0.f) {}
Vec2::Vec2(float _xy) : x(_xy), y(_xy) {}
Vec2::Vec2(float _x, float _y) : x(_x), y(_y) {}
Vec2::Vec2(const Vec3& _vec3) : x(_vec3.x), y(_vec3.y) {}
Vec2::Vec2(const Vec4& _vec4) : x(_vec4.x), y(_vec4.y) {}

Vec2::~Vec2(void) {}

Vec2 Vec2::GetPerpendicular() const
{
    return { y, -x };
}
float Vec2::GetMagnitude() const
{
    return sqrt(GetMagnitudeSquared());
}
float Vec2::GetMagnitudeSquared() const
{
    return DotProduct(*this, *this);
}
Vec2 Vec2::GetNormalized() const
{
    float i = GetMagnitude();
    if (i != 0)
        return operator*(1/i);
    return *this;
}

Vec2 Vec2::operator = (const Vec3& _vec)
{
    x = _vec.x;
    y = _vec.y;
    return *this;
}
Vec2 Vec2::operator = (const Vec4& _vec)
{
    x = _vec.x;
    y = _vec.y;
    return *this;
}
Vec2 Vec2::operator = (float _sca)
{
    x = _sca;
    y = _sca;
    return *this;
}

Vec2 Vec2::operator - (void) const
{
    return { -x, -y };
}
bool Vec2::operator == (const Vec2& _vec) const
{
    return (x == _vec.x && y == _vec.y);
}
bool Vec2::operator != (const Vec2& _vec) const
{
    return (x != _vec.x || y != _vec.y);
}

float Vec2::operator [] (int _index) const
{
    return xy[_index];
}

Vec2 Vec2::operator + (const Vec2& _vec) const
{
    return
    {
        x + _vec.x,
        y + _vec.y
    };
}
Vec2 Vec2::operator - (const Vec2& _vec) const
{
    return
    {
        x - _vec.x,
        y - _vec.y
    };
}

Vec2 Vec2::operator += (const Vec2& _vec)
{
    x += _vec.x;
    y += _vec.y;
    return *this;
}
Vec2 Vec2::operator -= (const Vec2& _vec)
{
    x -= _vec.x;
    y -= _vec.y;
    return *this;
}

Vec2 Vec2::operator + (float _sca) const
{
    return
    {
        x + _sca,
        y + _sca
    };
}
Vec2 Vec2::operator - (float _sca) const
{
    return
    {
        x - _sca,
        y - _sca 
    };
}
Vec2 Vec2::operator * (float _sca) const
{
    return 
    {
        x * _sca,
        y * _sca 
    };
}
Vec2 Vec2::operator / (float _sca) const
{
    return
    {
        x / _sca,
        y / _sca 
    };
}

Vec2 Vec2::operator += (float _sca)
{
    x += _sca;
    y += _sca;
    return *this;
}
Vec2 Vec2::operator -= (float _sca)
{
    x -= _sca;
    y -= _sca;
    return *this;
}
Vec2 Vec2::operator *= (float _sca)
{
    x *= _sca;
    y *= _sca;
    return *this;
}
Vec2 Vec2::operator /= (float _sca)
{
    x /= _sca;
    y /= _sca;
    return *this;
}



Vec3::Vec3(void) : x(0.f), y(0.f), z(0.f) {}
Vec3::Vec3(float _xyz) : x(_xyz), y(_xyz), z(_xyz) {}
Vec3::Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
Vec3::Vec3(const class Vec2& _vec2, float _z) : x(_vec2.x), y(_vec2.y), z(_z) {}
Vec3::Vec3(const class Vec4& _vec4) : x(_vec4.x), y(_vec4.y), z(_vec4.z) {}

Vec3::~Vec3(void) {}

float Vec3::GetMagnitude() const
{
    return sqrt(GetMagnitudeSquared());
}
float Vec3::GetMagnitudeSquared() const
{
    return DotProduct(*this, *this);
}
Vec3 Vec3::GetNormalized() const
{
    float i = GetMagnitude();
    if (i != 0)
        return operator*(1/i);
    return *this;
}

Vec3 Vec3::operator = (const Vec2& _vec)
{
    x = _vec.x;
    y = _vec.y;
    z = 0.f;
    return *this;
}
Vec3 Vec3::operator = (const Vec4& _vec)
{
    x = _vec.x;
    y = _vec.y;
    z = _vec.z;
    return *this;
}
Vec3 Vec3::operator = (float _sca)
{
    x = _sca;
    y = _sca;
    z = _sca;
    return *this;
}

Vec3 Vec3::operator - (void) const
{
    return 
    {
        -x,
        -y,
        -z 
    };
}
bool Vec3::operator == (const Vec3& _vec) const
{
    return (x == _vec.x && y == _vec.y && z == _vec.z);
}
bool Vec3::operator != (const Vec3& _vec) const
{
    return (x != _vec.x || y != _vec.y || z != _vec.z);
}

float Vec3::operator [] (int _index) const
{
    return xyz[_index];
}

Vec3 Vec3::operator + (const Vec3& _vec) const
{
    return
    {
        x + _vec.x,
        y + _vec.y,
        z + _vec.z 
    };
}
Vec3 Vec3::operator - (const Vec3& _vec) const
{
    return
    {
        x - _vec.x,
        y - _vec.y,
        z - _vec.z 
    };
}

Vec3 Vec3::operator += (const Vec3& _vec)
{
    x += _vec.x;
    y += _vec.y;
    z += _vec.z;
    return *this;
}
Vec3 Vec3::operator -= (const Vec3& _vec)
{
    x -= _vec.x;
    y -= _vec.y;
    z -= _vec.z;
    return *this;
}

Vec3 Vec3::operator + (float _sca) const
{
    return
    {
        x + _sca,
        y + _sca,
        z + _sca 
    };
}
Vec3 Vec3::operator - (float _sca) const
{
    return 
    {
        x - _sca,
        y - _sca,
        z - _sca 
    };
}
Vec3 Vec3::operator * (float _sca) const
{
    return 
    {
        x * _sca,
        y * _sca,
        z * _sca
    };
}
Vec3 Vec3::operator / (float _sca) const
{
    return 
    {
        x / _sca,
        y / _sca,
        z / _sca
    };
}

Vec3 Vec3::operator += (float _sca)
{
    x += _sca;
    y += _sca;
    z += _sca;
    return *this;
}
Vec3 Vec3::operator -= (float _sca)
{
    x -= _sca;
    y -= _sca;
    z -= _sca;
    return *this;
}
Vec3 Vec3::operator *= (float _sca)
{
    x *= _sca;
    y *= _sca;
    z *= _sca;
    return *this;
}
Vec3 Vec3::operator /= (float _sca)
{
    x /= _sca;
    y /= _sca;
    z /= _sca;
    return *this;
}



Vec4::Vec4(void) : x(0.f), y(0.f), z(0.f), w(0.f) {}
Vec4::Vec4(float _xyzw) : x(_xyzw), y(_xyzw), z(_xyzw), w(_xyzw) {}
Vec4::Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
Vec4::Vec4(const class Vec2& _vec2, float _z, float _w) : x(_vec2.x), y(_vec2.y), z(_z), w(_w) {}
Vec4::Vec4(const class Vec3& _vec3, float _w) : x(_vec3.x), y(_vec3.y), z(_vec3.z), w(_w) {}

Vec4::~Vec4(void) {}

float Vec4::GetMagnitude() const
{
    return sqrt(GetMagnitudeSquared());
}
float Vec4::GetMagnitudeSquared() const
{
    return DotProduct(*this, *this);
}
Vec4 Vec4::GetNormalized() const
{
    float i = GetMagnitude();
    if (i != 0)
        return operator*(1/i);
    return *this;
}

Vec4 Vec4::operator = (const Vec2& _vec)
{
    x = _vec.x;
    y = _vec.y;
    z = 0.f;
    w = 1.f;
    return *this;
}
Vec4 Vec4::operator = (const Vec3& _vec)
{
    x = _vec.x;
    y = _vec.y;
    z = _vec.z;
    w = 1.f;
    return *this;
}
Vec4 Vec4::operator = (float _sca)
{
    x = _sca;
    y = _sca;
    z = _sca;
    w = _sca;
    return *this;
}

Vec2 Vec4::operator - (void) const
{
    return 
    {
        -x,
        -y 
    };
}
bool Vec4::operator == (const Vec4& _vec) const
{
    return (x == _vec.x && y == _vec.y && z == _vec.z && w == _vec.w);
}
bool Vec4::operator != (const Vec4& _vec) const
{
    return (x != _vec.x || y != _vec.y || z != _vec.z || w != _vec.w);
}

float Vec4::operator [] (int _index) const
{
    return xyzw[_index];
}

Vec4 Vec4::operator + (const Vec4& _vec) const
{
    return
    {
        x + _vec.x,
        y + _vec.y,
        z + _vec.z,
        w + _vec.w
    };
}
Vec4 Vec4::operator - (const Vec4& _vec) const
{
    return
    {
        x - _vec.x,
        y - _vec.y,
        z - _vec.z,
        w - _vec.w
    };
}

Vec4 Vec4::operator += (const Vec4& _vec)
{
    x += _vec.x;
    y += _vec.y;
    z += _vec.z;
    w += _vec.w;
    return *this;
}
Vec4 Vec4::operator -= (const Vec4& _vec)
{
    x -= _vec.x;
    y -= _vec.y;
    z -= _vec.z;
    w -= _vec.w;
    return *this;
}

Vec4 Vec4::operator + (float _sca) const
{
    return
    {
        x + _sca,
        y + _sca,
        z + _sca,
        w + _sca 
    };
}
Vec4 Vec4::operator - (float _sca) const
{
    return
    {
        x - _sca,
        y - _sca,
        z - _sca,
        w - _sca 
    };
}
Vec4 Vec4::operator * (float _sca) const
{
    return
    {
        x * _sca,
        y * _sca,
        z * _sca,
        w * _sca
    };
}
Vec4 Vec4::operator / (float _sca) const
{
    return
    {
        x / _sca,
        y / _sca,
        z / _sca,
        w / _sca
    };
}

Vec4 Vec4::operator += (float _sca)
{
    x += _sca;
    y += _sca;
    z += _sca;
    w += _sca;
    return *this;
}
Vec4 Vec4::operator -= (float _sca)
{
    x -= _sca;
    y -= _sca;
    z -= _sca;
    w -= _sca;
    return *this;
}
Vec4 Vec4::operator *= (float _sca)
{
    x *= _sca;
    y *= _sca;
    z *= _sca;
    w *= _sca;
    return *this;
}
Vec4 Vec4::operator /= (float _sca)
{
    x /= _sca;
    y /= _sca;
    z /= _sca;
    w /= _sca;
    return *this;
}



void Maths::Normalize(Vec2& _vec2)
{
    float m = _vec2.GetMagnitude();
    if (m != 0.f)
    {
        _vec2.operator*=(1/m);
    }
}
void Maths::Normalize(Vec3& _vec3)
{
    float m = _vec3.GetMagnitude();
    if (m != 0.f)
    {
        _vec3.operator*=(1/m);
    }
}
void Maths::Normalize(Vec4& _vec4)
{
    float m = _vec4.GetMagnitude();
    if (m != 0.f)
    {
        _vec4.operator*=(1/m);
    }
}

float Maths::DotProduct(const Vec2& _vecA, const Vec2& _vecB)
{
    return (_vecA.x * _vecB.x + _vecA.y * _vecB.y);
}
float Maths::DotProduct(const Vec3& _vecA, const Vec3& _vecB)
{
    return (_vecA.x * _vecB.x + _vecA.y * _vecB.y + _vecA.z * _vecB.z);
}
float Maths::DotProduct(const Vec4& _vecA, const Vec4& _vecB)
{
    return (_vecA.x * _vecB.x + _vecA.y * _vecB.y + _vecA.z * _vecB.z + _vecA.w * _vecB.w);
}

float Maths::DistanceBetween(const Vec2& _vecA, const Vec2& _vecB)
{
    return sqrt((_vecA.x - _vecB.x) * (_vecA.x - _vecB.x) + (_vecA.y - _vecB.y) * (_vecA.y - _vecB.y));
}
float Maths::DistanceBetween(const Vec3& _vecA, const Vec3& _vecB)
{
    return sqrt((_vecA.x - _vecB.x) * (_vecA.x - _vecB.x) + (_vecA.y - _vecB.y) * (_vecA.y - _vecB.y) + (_vecA.z - _vecB.z) * (_vecA.z - _vecB.z));
}
float Maths::DistanceBetween(const Vec4& _vecA, const Vec4& _vecB)
{
    return sqrt((_vecA.x - _vecB.x) * (_vecA.x - _vecB.x) + (_vecA.y - _vecB.y) * (_vecA.y - _vecB.y) + (_vecA.z - _vecB.z) * (_vecA.z - _vecB.z) + (_vecA.w - _vecB.w) * (_vecA.w - _vecB.w));
}

float Maths::AngleBetween(const Vec2& _vecA, const Vec2& _vecB)
{
    float angle = acos(DotProduct(_vecA, _vecB) / (_vecA.GetMagnitude() * _vecB.GetMagnitude()));
    if (CrossProduct(_vecA, _vecB) >= 0)
        return angle;
    return -angle;
}
float Maths::AngleBetween(const Vec3& _vecA, const Vec3& _vecB)
{
    return acos(DotProduct(_vecA, _vecB) / (_vecA.GetMagnitude() * _vecB.GetMagnitude()));
}
float Maths::AngleBetween(const Vec4& _vecA, const Vec4& _vecB)
{
    return acos(DotProduct(_vecA, _vecB) / (_vecA.GetMagnitude() * _vecB.GetMagnitude()));
}

float Maths::CrossProduct(const Vec2& _vecA, const Vec2& _vecB)
{
    return _vecA.x * _vecB.y - _vecA.y * _vecB.x;
}

Vec3 Maths::CrossProduct(const Vec3& _vecA, const Vec3& _vecB)
{
    return
    {
        _vecA.y * _vecB.z - _vecA.z * _vecB.y,
        _vecA.z * _vecB.x - _vecA.x * _vecB.z,
        _vecA.x * _vecB.y - _vecA.y * _vecB.x 
    };
}



Mat3::Mat3(void)
{
    data[0] = 0.f;
    data[1] = 0.f;
    data[2] = 0.f;

    data[3] = 0.f;
    data[4] = 0.f;
    data[5] = 0.f;

    data[6] = 0.f;
    data[7] = 0.f;
    data[8] = 0.f;
}
Mat3::Mat3(float _data[9])
{
    data[0] = _data[0];
    data[1] = _data[1];
    data[2] = _data[2];

    data[3] = _data[3];
    data[4] = _data[4];
    data[5] = _data[5];

    data[6] = _data[6];
    data[7] = _data[7];
    data[8] = _data[8];
}
Mat3::Mat3(float _indentityValue)
{
    data[0] = _indentityValue;
    data[1] = 0.f;
    data[2] = 0.f;

    data[3] = 0.f;
    data[4] = _indentityValue;
    data[5] = 0.f;

    data[6] = 0.f;
    data[7] = 0.f;
    data[8] = _indentityValue;
}
Mat3::Mat3(float _0, float _1, float _2,
    float _3, float _4, float _5,
    float _6, float _7, float _8)
{
    data[0] = _0;
    data[1] = _1;
    data[2] = _2;
                   
    data[3] = _3;
    data[4] = _4;
    data[5] = _5;
                   
    data[6] = _6;
    data[7] = _7;
    data[8] = _8;
}

Mat3::~Mat3(void) {}

Mat3 Mat3::Transform(const Vec2& _translation, float _angleInRad, const Vec2& _scaling)
{
    float ca = 1.f;
    float sa = 0.f;
    if (_angleInRad)
    {
        ca = std::cos(_angleInRad);
        sa = std::sin(_angleInRad);
        //Pas obligatoire
        if (abs(ca) <= Constants::TOLERANCE)
            ca = 0.f;
        if (abs(sa) <= Constants::TOLERANCE)
            sa = 0.f;
    }
    return
    {
        _scaling[0] *  ca, _scaling[0] * sa, 0.f,
        _scaling[1] * -sa, _scaling[1] * ca, 0.f,
        _translation[0]  , _translation[1] , 1.f
    };
}
Mat3 Mat3::Rotate2D(float angleinrad)
{
    float ca = 0.f;
    float sa = 1.f;
    if (angleinrad)
    {
        ca = std::cos(angleinrad);
        sa = std::sin(angleinrad);
        //Pas obligatoire
        if (abs(ca) <= Constants::TOLERANCE)
            ca = 0;
        if (abs(sa) <= Constants::TOLERANCE)
            sa = 0;
    }
    return
    {
        ca, sa, 0,
        -sa, ca, 0,
        0,  0,  1,
    };
}
Mat3 Mat3::Translate2D(const Vec2& _translation)
{
	return
	{
		1.f,			 0.f,			0.f,
		0.f,			 1.f,			0.f,
		_translation.x, _translation.y, 1.f
	};
}
Mat3 Mat3::Scale2D(const Vec2& _scaling)
{
    return
    {
        _scaling.x, 0.f,        0.f,
        0.f,        _scaling.y, 0.f,
        0.f,        0.f,        1.f
    };
}
float Mat3::Determinant() const
{
    return data[0] * (data[4] * data[8] - data[7] * data[5])
        - data[1] * (data[3] * data[8] - data[6] * data[5])
        + data[2] * (data[3] * data[7] - data[6] * data[4]);
}
float Mat3::Trace() const
{
    return data[0] + data[4] + data[8];
}
Mat3 Mat3::Inverse()
{
    float det = Determinant();
    if (det != 0.f)
    {
        det = 1.f / det;

        return *this =
        {

            (data_3_3[1][1] * data_3_3[2][2] - data_3_3[2][1] * data_3_3[1][2]) * det,
            (data_3_3[0][2] * data_3_3[2][1] - data_3_3[0][1] * data_3_3[2][2]) * det,
            (data_3_3[0][1] * data_3_3[1][2] - data_3_3[0][2] * data_3_3[1][1]) * det,
            (data_3_3[1][2] * data_3_3[2][0] - data_3_3[1][0] * data_3_3[2][2]) * det,
            (data_3_3[0][0] * data_3_3[2][2] - data_3_3[0][2] * data_3_3[2][0]) * det,
            (data_3_3[1][0] * data_3_3[0][2] - data_3_3[0][0] * data_3_3[1][2]) * det,
            (data_3_3[1][0] * data_3_3[2][1] - data_3_3[2][0] * data_3_3[1][1]) * det,
            (data_3_3[2][0] * data_3_3[0][1] - data_3_3[0][0] * data_3_3[2][1]) * det,
            (data_3_3[0][0] * data_3_3[1][1] - data_3_3[1][0] * data_3_3[0][1]) * det

        };
    }
    return *this;
}
Mat3 Mat3::Transpose()
{
    return *this =
    {
        data[0],
        data[3],
        data[6],

        data[1],
        data[4],
        data[7],

        data[2],
        data[5],
        data[8]
    };
}

Mat3 Mat3::operator = (float _data[9])
{
	return{
	_data[0],
	_data[1],
	_data[2],

	_data[3],
	_data[4],
	_data[5],

	_data[6],
	_data[7],
	_data[8]
	};
}

Mat3 Mat3::operator - (void) const
{
    return
    {
        -data[0],
        -data[1],
        -data[2],

        -data[3],
        -data[4],
        -data[5],

        -data[6],
        -data[7],
        -data[8]
    };
}
bool Mat3::operator == (const Mat3& _mat) const
{
    for (int i = 0; i < 9; i++)
    {
        if (data[i] != _mat.data[i]) return false;
    }
    return true;
}
bool Mat3::operator != (const Mat3& _mat) const
{
    for (int i = 0; i < 9; i++)
    {
        if (data[i] == _mat.data[i]) return false;
    }
    return true;
}

Vec3 Mat3::operator [] (int _index) const
{
    return data3V[_index];
}

Mat3 Mat3::operator + (const Mat3& _mat) const
{
    return
    {
        data[0] + _mat.data[0],
        data[1] + _mat.data[1],
        data[2] + _mat.data[2],

        data[3] + _mat.data[3],
        data[4] + _mat.data[4],
        data[5] + _mat.data[5],

        data[6] + _mat.data[6],
        data[7] + _mat.data[7],
        data[8] + _mat.data[8]
    };
}
Mat3 Mat3::operator - (const Mat3& _mat) const
{
    return
    {
        data[0] - _mat.data[0],
        data[1] - _mat.data[1],
        data[2] - _mat.data[2],

        data[3] - _mat.data[3],
        data[4] - _mat.data[4],
        data[5] - _mat.data[5],

        data[6] - _mat.data[6],
        data[7] - _mat.data[7],
        data[8] - _mat.data[8]
    };
}
Mat3 Mat3::operator * (const Mat3& _mat) const
{
	return
	{

        data[0] * _mat.data[0] + data[3] * _mat.data[1] + data[6] * _mat.data[2],
        data[1] * _mat.data[0] + data[4] * _mat.data[1] + data[7] * _mat.data[2],
        data[2] * _mat.data[0] + data[5] * _mat.data[1] + data[8] * _mat.data[2],

        data[0] * _mat.data[3] + data[3] * _mat.data[4] + data[6] * _mat.data[5],
        data[1] * _mat.data[3] + data[4] * _mat.data[4] + data[7] * _mat.data[5],
        data[2] * _mat.data[3] + data[5] * _mat.data[4] + data[8] * _mat.data[5],

        data[0] * _mat.data[6] + data[3] * _mat.data[7] + data[6] * _mat.data[8],
        data[1] * _mat.data[6] + data[4] * _mat.data[7] + data[7] * _mat.data[8],
        data[2] * _mat.data[6] + data[5] * _mat.data[7] + data[8] * _mat.data[8]

    };
}

Mat3 Mat3::operator += (const Mat3& _mat)
{
    data[0] += _mat.data[0];
    data[1] += _mat.data[1];
    data[2] += _mat.data[2];

    data[3] += _mat.data[3];
    data[4] += _mat.data[4];
    data[5] += _mat.data[5];

    data[6] += _mat.data[6];
    data[7] += _mat.data[7];
    data[8] += _mat.data[8];
    return *this;
}
Mat3 Mat3::operator -= (const Mat3& _mat)
{
    data[0] -= _mat.data[0];
    data[1] -= _mat.data[1];
    data[2] -= _mat.data[2];

    data[3] -= _mat.data[3];
    data[4] -= _mat.data[4];
    data[5] -= _mat.data[5];

    data[6] -= _mat.data[6];
    data[7] -= _mat.data[7];
    data[8] -= _mat.data[8];
    return *this;
}
Mat3 Mat3::operator *= (const Mat3& _mat)
{
    return *this =
    {
        data[0] * _mat.data[0] + data[3] * _mat.data[1] + data[6] * _mat.data[2],
        data[1] * _mat.data[0] + data[4] * _mat.data[1] + data[7] * _mat.data[2],
        data[2] * _mat.data[0] + data[5] * _mat.data[1] + data[8] * _mat.data[2],

        data[0] * _mat.data[3] + data[3] * _mat.data[4] + data[6] * _mat.data[5],
        data[1] * _mat.data[3] + data[4] * _mat.data[4] + data[7] * _mat.data[5],
        data[2] * _mat.data[3] + data[5] * _mat.data[4] + data[8] * _mat.data[5],

        data[0] * _mat.data[6] + data[3] * _mat.data[7] + data[6] * _mat.data[8],
        data[1] * _mat.data[6] + data[4] * _mat.data[7] + data[7] * _mat.data[8],
        data[2] * _mat.data[6] + data[5] * _mat.data[7] + data[8] * _mat.data[8]
    };
}

Mat3 Mat3::operator * (float _sca) const
{
    return
    {

        data[0] * _sca,
        data[1] * _sca,
        data[2] * _sca,

        data[3] * _sca,
        data[4] * _sca,
        data[5] * _sca,

        data[6] * _sca,
        data[7] * _sca,
        data[8] * _sca

    };
}
Mat3 Mat3::operator / (float _sca) const
{
    return
    {

        data[0] / _sca,
        data[1] / _sca,
        data[2] / _sca,

        data[3] / _sca,
        data[4] / _sca,
        data[5] / _sca,

        data[6] / _sca,
        data[7] / _sca,
        data[8] / _sca

    };
}

Mat3 Mat3::operator *= (float _sca)
{
    data[0] *= _sca;
    data[1] *= _sca;
    data[2] *= _sca;

    data[3] *= _sca;
    data[4] *= _sca;
    data[5] *= _sca;

    data[6] *= _sca;
    data[7] *= _sca;
    data[8] *= _sca;

    return *this;
}
Mat3 Mat3::operator /= (float _sca)
{
    data[0] /= _sca;
    data[1] /= _sca;
    data[2] /= _sca;

    data[3] /= _sca;
    data[4] /= _sca;
    data[5] /= _sca;

    data[6] /= _sca;
    data[7] /= _sca;
    data[8] /= _sca;

    return *this;
}

Vec2 Mat3::operator * (const Vec2& _vec) const
{
    return
    {

        data[0] * _vec.x + data[1] * _vec.y + data[2],

        data[3] * _vec.x + data[4] * _vec.y + data[5]

    };
}

Vec3 Mat3::operator * (const Vec3& _vec) const
{
    return
    {

        data[0] * _vec.x + data[1] * _vec.y + data[2] * _vec.z,

        data[3] * _vec.x + data[4] * _vec.y + data[5] * _vec.z,

        data[6] * _vec.x + data[7] * _vec.y + data[8] * _vec.z

    };
}


Mat4::Mat4(void)
{
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;

    data[4] = 0;
    data[5] = 0;
    data[6] = 0;
    data[7] = 0;

    data[8] = 0;
    data[9] = 0;
    data[10] = 0;
    data[11] = 0;

    data[12] = 0;
    data[13] = 0;
    data[14] = 0;
    data[15] = 0;
}
Mat4::Mat4(float _data[16])
{
    data[0] = _data[0];
    data[1] = _data[1];
    data[2] = _data[2];
    data[3] = _data[3];

    data[4] = _data[4];
    data[5] = _data[5];
    data[6] = _data[6];
    data[7] = _data[7];

    data[8] = _data[8];
    data[9] = _data[9];
    data[10] = _data[10];
    data[11] = _data[11];

    data[12] = _data[12];
    data[13] = _data[13];
    data[14] = _data[14];
    data[15] = _data[15];
}
Mat4::Mat4(float _indentityValue)
{
    data[0] = _indentityValue;
    data[1] = 0.f;
    data[2] = 0.f;
    data[3] = 0.f;

    data[4] = 0.f;
    data[5] = _indentityValue;
    data[6] = 0.f;
    data[7] = 0.f;

    data[8] = 0.f;
    data[9] = 0.f;
    data[10] = _indentityValue;
    data[11] = 0.f;

    data[12] = 0.f;
    data[13] = 0.f;
    data[14] = 0.f;
    data[15] = _indentityValue;

}
Mat4::Mat4(float _0, float _1, float _2, float _3,
    float _4, float _5, float _6, float _7,
    float _8, float _9, float _10, float _11,
    float _12, float _13, float _14, float _15)
{
    data[0] = _0;
    data[1] = _1;
    data[2] = _2;
    data[3] = _3;

    data[4] = _4;
    data[5] = _5;
    data[6] = _6;
    data[7] = _7;

    data[8] = _8;
    data[9] = _9;
    data[10] = _10;
    data[11] = _11;

    data[12] = _12;
    data[13] = _13;
    data[14] = _14;
    data[15] = _15;
}

Mat4::~Mat4(void) {}

Mat4 Mat4::Transform(Vec3 _translation, Vec3 _anglesInRad, Vec3 _scaling)
{
    //rotation
    Mat4 temp = Rotate(_anglesInRad);

    //scaling
    temp.data[0] *= _scaling.x; temp.data[1] *= _scaling.x; temp.data[2] *= _scaling.x;
    temp.data[4] *= _scaling.y; temp.data[5] *= _scaling.y; temp.data[6] *= _scaling.y;
    temp.data[8] *= _scaling.z; temp.data[9] *= _scaling.z; temp.data[10] *= _scaling.z;

    //translation
    temp.data[12] = _translation.x;
    temp.data[13] = _translation.y;
    temp.data[14] = _translation.z;

    return temp;
}
Mat4 Mat4::Rotate(Vec3 _anglesInRad)
{
    float cosA = cos(_anglesInRad.x);
    float cosB = cos(_anglesInRad.y);
    float cosC = cos(_anglesInRad.z);

    float sinA = sin(_anglesInRad.x);
    float sinB = sin(_anglesInRad.y);
    float sinC = sin(_anglesInRad.z);

    return
    {
        cosB * cosC, sinA* sinB* cosC + cosA * sinC,  -cosA * sinB * cosC + sinA * sinC, 0,
        -cosB * sinC, -sinA * sinB * sinC + cosA * cosC, cosA* sinB* sinC + sinA * cosC, 0,
        sinB, -sinA * cosB, cosA * cosB, 0,
        0,0,0,1
    };
}
Mat4 Mat4::RotateX(float _angleInRad)
{
    float cosX = cos(_angleInRad);
    float sinX = sin(_angleInRad);

    return
    {

        1.f,
        0.f,
        0.f,
        0.f,

        0.f,
        cosX,
        sinX,
        0.f,
    
        0.f,
        -sinX,
        cosX,
        0.f,

        0.f,
        0.f,
        0.f,
        1.f

    };
}
Mat4 Mat4::RotateY(float _angleInRad)
{
    float cosY = cos(_angleInRad);
    float sinY = sin(_angleInRad);

    return
    {

        cosY,
        0.f,
        -sinY,
        0.f,

        0.f,
        1.f,
        0.f,
        0.f,

        sinY,
        0.f,
        cosY,
        0.f,

        0.f,
        0.f,
        0.f,
        1.f

    };
}
Mat4 Mat4::RotateZ(float _angleInRad)
{
    float cosZ = cos(_angleInRad);
    float sinZ = sin(_angleInRad);

    return
    {

        cosZ,
        sinZ,
        0.f,
        0.f,

        -sinZ,
        cosZ,
        0.f,
        0.f,

        0.f,
        0.f,
        1.f,
        0.f,

        0.f,
        0.f,
        0.f,
        1.f

    };
}
Mat4 Mat4::Translate(Vec3 _translation)
{
    return
    {

         1.f,
         0.f,
         0.f,
         0.f,

         0.f,
         1.f,
         0.f,
         0.f,

         0.f,
         0.f,
         1.f,
         0.f,

         _translation.x,
         _translation.y,
         _translation.z,
         1.f

    };
}
Mat4 Mat4::Scale(Vec3 _scaling)
{
    return
    {

        _scaling.x,
        0.f,
        0.f,
        0.f,

        0.f,
        _scaling.y,
        0.f,
        0.f,

        0.f,
        0.f,
        _scaling.z,
        0.f,

        0.f,
        0.f,
        0.f,
        1.f

    };;
}
float Mat4::Determinant() const
{
    return data_4_4[0][0] * (data_4_4[1][1] * (data_4_4[2][2] * data_4_4[3][3] - data_4_4[2][3] * data_4_4[3][2])
        - data_4_4[1][2] * (data_4_4[2][1] * data_4_4[3][3] - data_4_4[2][3] * data_4_4[3][1])
        + data_4_4[1][3] * (data_4_4[2][1] * data_4_4[3][2] - data_4_4[2][2] * data_4_4[3][1]))
        - data_4_4[0][1] * (data_4_4[1][0] * (data_4_4[2][2] * data_4_4[3][3] - data_4_4[2][3] * data_4_4[3][2])
            - data_4_4[1][2] * (data_4_4[2][0] * data_4_4[3][3] - data_4_4[2][3] * data_4_4[3][0])
            + data_4_4[1][3] * (data_4_4[2][0] * data_4_4[3][2] - data_4_4[2][2] * data_4_4[3][0]))
        + data_4_4[0][2] * (data_4_4[1][0] * (data_4_4[2][1] * data_4_4[3][3] - data_4_4[2][3] * data_4_4[3][1])
            - data_4_4[1][1] * (data_4_4[2][0] * data_4_4[3][3] - data_4_4[2][3] * data_4_4[3][0])
            + data_4_4[1][3] * (data_4_4[2][0] * data_4_4[3][1] - data_4_4[2][1] * data_4_4[3][0]))
        - data_4_4[0][3] * (data_4_4[1][0] * (data_4_4[2][1] * data_4_4[3][2] - data_4_4[2][2] * data_4_4[3][1])
            - data_4_4[1][1] * (data_4_4[2][0] * data_4_4[3][2] - data_4_4[2][2] * data_4_4[3][0])
            + data_4_4[1][2] * (data_4_4[2][0] * data_4_4[3][1] - data_4_4[2][1] * data_4_4[3][0]));
}
float Mat4::Trace() const
{
    return data[0] + data[5] + data[10] + data[15];
}
Mat4 Mat4::Inverse()
{
    float det = Determinant();
    if (det != 0.f)
    {
        det = 1.f / det;
        return *this =
        {

            (data[5] * (data[10] * data[15] - data[11] * data[14])
            - data[9] * (data[6] * data[15] - data[7] * data[14])
            + data[13] * (data[6] * data[11] - data[7] * data[10])) * det,

            -(data[1] * (data[10] * data[15] - data[11] * data[14])
            - data[9] * (data[2] * data[15] - data[3] * data[14])
            + data[13] * (data[2] * data[11] - data[3] * data[10])) * det,

            (data[1] * (data[6] * data[15] - data[7] * data[14])
            - data[5] * (data[2] * data[15] - data[3] * data[14])
            + data[13] * (data[2] * data[7] - data[3] * data[6])) * det,

            -(data[1] * (data[6] * data[11] - data[7] * data[10])
            - data[5] * (data[2] * data[11] - data[3] * data[10])
            + data[9] * (data[2] * data[7] - data[3] * data[6])) * det,



            -(data[4] * (data[10] * data[15] - data[11] * data[14])
            - data[8] * (data[6] * data[15] - data[7] * data[14])
            + data[12] * (data[6] * data[11] - data[7] * data[10])) * det,

            (data[0] * (data[10] * data[15] - data[11] * data[14])
            - data[8] * (data[2] * data[15] - data[3] * data[14])
            + data[12] * (data[2] * data[11] - data[3] * data[10])) * det,

            -(data[0] * (data[6] * data[15] - data[7] * data[14])
            - data[4] * (data[2] * data[15] - data[3] * data[14])
            + data[12] * (data[2] * data[7] - data[3] * data[6])) * det,

            (data[0] * (data[6] * data[11] - data[7] * data[10])
            - data[4] * (data[2] * data[11] - data[3] * data[10])
            + data[8] * (data[2] * data[7] - data[3] * data[6])) * det,



            (data[4] * (data[9] * data[15] - data[11] * data[13])
            - data[8] * (data[5] * data[15] - data[7] * data[13])
            + data[12] * (data[5] * data[11] - data[7] * data[9])) * det,

            -(data[0] * (data[9] * data[15] - data[11] * data[13])
            - data[8] * (data[1] * data[15] - data[3] * data[13])
            + data[12] * (data[1] * data[11] - data[3] * data[9])) * det,

            (data[0] * (data[5] * data[15] - data[7] * data[13])
            - data[4] * (data[1] * data[15] - data[3] * data[13])
            + data[12] * (data[1] * data[7] - data[3] * data[5])) * det,

            -(data[0] * (data[5] * data[11] - data[7] * data[9])
            - data[4] * (data[1] * data[11] - data[3] * data[9])
            + data[8] * (data[1] * data[7] - data[3] * data[5])) * det,



            -(data[4] * (data[9] * data[14] - data[10] * data[13])
            - data[8] * (data[5] * data[14] - data[6] * data[13])
            + data[12] * (data[5] * data[10] - data[6] * data[9])) * det,

            (data[0] * (data[9] * data[14] - data[10] * data[13])
            - data[8] * (data[1] * data[14] - data[2] * data[13])
            + data[12] * (data[1] * data[10] - data[2] * data[9])) * det,

            -(data[0] * (data[5] * data[14] - data[6] * data[13])
            - data[4] * (data[1] * data[14] - data[2] * data[13])
            + data[12] * (data[1] * data[6] - data[2] * data[5])) * det,

            (data[0] * (data[5] * data[10] - data[6] * data[9])
            - data[4] * (data[1] * data[10] - data[2] * data[9])
            + data[8] * (data[1] * data[6] - data[2] * data[5])) * det
        };
    }
    return *this;
}
Mat4 Mat4::Transpose()
{
    return *this =
    {
        data[0],
        data[4],
        data[8],
        data[12],

        data[1],
        data[5],
        data[9],
        data[13],

        data[2],
        data[6],
        data[10],
        data[14],

        data[3],
        data[7],
        data[11],
        data[15]
    };
}

Mat4 Mat4::operator = (float _data[16])
{
    data[0] = _data[0];
    data[1] = _data[1];
    data[2] = _data[2];
    data[3] = _data[3];

    data[4] = _data[4];
    data[5] = _data[5];
    data[6] = _data[6];
    data[7] = _data[7];

    data[8] = _data[8];
    data[9] = _data[9];
    data[10] = _data[10];
    data[11] = _data[11];

    data[12] = _data[12];
    data[13] = _data[13];
    data[14] = _data[14];
    data[15] = _data[15];

    return *this;
}

Mat4 Mat4::operator - (void) const
{
    return
    {

        -data[0],
        -data[1],
        -data[2],
        -data[3],

        -data[4],
        -data[5],
        -data[6],
        -data[7],

        -data[8],
        -data[9],
        -data[10],
        -data[11],

        -data[12],
        -data[13],
        -data[14],
        -data[15]

    };
}
bool Mat4::operator == (const Mat4& _mat) const
{
    for (int i = 0; i < 16; i++)
    {
        if (data[i] != _mat.data[i]) return false;
    }
    return true;
}
bool Mat4::operator != (const Mat4& _mat) const
{
    for (int i = 0; i < 16; i++)
    {
        if (data[i] != _mat.data[i]) return true;
    }
    return false;
}

Vec4 Mat4::operator [] (int _index) const
{
    return data4V[_index];
}

Mat4 Mat4::operator + (const Mat4& _mat) const
{
    return
    {
        data[0] + _mat.data[0],
        data[1] + _mat.data[1],
        data[2] + _mat.data[2],
        data[3] + _mat.data[3],

        data[4] + _mat.data[4],
        data[5] + _mat.data[5],
        data[6] + _mat.data[6],
        data[7] + _mat.data[7],

        data[8] + _mat.data[8],
        data[9] + _mat.data[9],
        data[10] + _mat.data[10],
        data[11] + _mat.data[11],

        data[12] + _mat.data[12],
        data[13] + _mat.data[13],
        data[14] + _mat.data[14],
        data[15] + _mat.data[15]
    };
}
Mat4 Mat4::operator - (const Mat4& _mat) const
{
    return
    {
        data[0] - _mat.data[0],
        data[1] - _mat.data[1],
        data[2] - _mat.data[2],
        data[3] - _mat.data[3],

        data[4] - _mat.data[4],
        data[5] - _mat.data[5],
        data[6] - _mat.data[6],
        data[7] - _mat.data[7],

        data[8] - _mat.data[8],
        data[9] - _mat.data[9],
        data[10] - _mat.data[10],
        data[11] - _mat.data[11],

        data[12] - _mat.data[12],
        data[13] - _mat.data[13],
        data[14] - _mat.data[14],
        data[15] - _mat.data[15]
    };
}
Mat4 Mat4::operator * (const Mat4& _mat) const
{
    return
    {

        data[0] * _mat.data[0] + data[4] * _mat.data[1] + data[8] * _mat.data[2] + data[12] * _mat.data[3],
        data[1] * _mat.data[0] + data[5] * _mat.data[1] + data[9] * _mat.data[2] + data[13] * _mat.data[3],
        data[2] * _mat.data[0] + data[6] * _mat.data[1] + data[10] * _mat.data[2] + data[14] * _mat.data[3],
        data[3] * _mat.data[0] + data[7] * _mat.data[1] + data[11] * _mat.data[2] + data[15] * _mat.data[3],

        data[0] * _mat.data[4] + data[4] * _mat.data[5] + data[8] * _mat.data[6] + data[12] * _mat.data[7],
        data[1] * _mat.data[4] + data[5] * _mat.data[5] + data[9] * _mat.data[6] + data[13] * _mat.data[7],
        data[2] * _mat.data[4] + data[6] * _mat.data[5] + data[10] * _mat.data[6] + data[14] * _mat.data[7],
        data[3] * _mat.data[4] + data[7] * _mat.data[5] + data[11] * _mat.data[6] + data[15] * _mat.data[7],

        data[0] * _mat.data[8] + data[4] * _mat.data[9] + data[8] * _mat.data[10] + data[12] * _mat.data[11],
        data[1] * _mat.data[8] + data[5] * _mat.data[9] + data[9] * _mat.data[10] + data[13] * _mat.data[11],
        data[2] * _mat.data[8] + data[6] * _mat.data[9] + data[10] * _mat.data[10] + data[14] * _mat.data[11],
        data[3] * _mat.data[8] + data[7] * _mat.data[9] + data[11] * _mat.data[10] + data[15] * _mat.data[11],

        data[0] * _mat.data[12] + data[4] * _mat.data[13] + data[8] * _mat.data[14] + data[12] * _mat.data[15],
        data[1] * _mat.data[12] + data[5] * _mat.data[13] + data[9] * _mat.data[14] + data[13] * _mat.data[15],
        data[2] * _mat.data[12] + data[6] * _mat.data[13] + data[10] * _mat.data[14] + data[14] * _mat.data[15],
        data[3] * _mat.data[12] + data[7] * _mat.data[13] + data[11] * _mat.data[14] + data[15] * _mat.data[15]
    };
}

Mat4 Mat4::operator += (const Mat4& _mat)
{
    data[0] += _mat.data[0];
    data[1] += _mat.data[1];
    data[2] += _mat.data[2];
    data[3] += _mat.data[3];

    data[4] += _mat.data[4];
    data[5] += _mat.data[5];
    data[6] += _mat.data[6];
    data[7] += _mat.data[7];

    data[8] += _mat.data[8];
    data[9] += _mat.data[9];
    data[10] += _mat.data[10];
    data[11] += _mat.data[11];

    data[12] += _mat.data[12];
    data[13] += _mat.data[13];
    data[14] += _mat.data[14];
    data[15] += _mat.data[15];
    return *this;
}
Mat4 Mat4::operator -= (const Mat4& _mat)
{
    data[0] -= _mat.data[0];
    data[1] -= _mat.data[1];
    data[2] -= _mat.data[2];
    data[3] -= _mat.data[3];

    data[4] -= _mat.data[4];
    data[5] -= _mat.data[5];
    data[6] -= _mat.data[6];
    data[7] -= _mat.data[7];

    data[8] -= _mat.data[8];
    data[9] -= _mat.data[9];
    data[10] -= _mat.data[10];
    data[11] -= _mat.data[11];

    data[12] -= _mat.data[12];
    data[13] -= _mat.data[13];
    data[14] -= _mat.data[14];
    data[15] -= _mat.data[15];
    return *this;
}
Mat4 Mat4::operator *= (const Mat4& _mat)
{
    return *this =
    {
        data[0] * _mat.data[0] + data[4] * _mat.data[1] + data[8] * _mat.data[2] + data[12] * _mat.data[3],
        data[1] * _mat.data[0] + data[5] * _mat.data[1] + data[9] * _mat.data[2] + data[13] * _mat.data[3],
        data[2] * _mat.data[0] + data[6] * _mat.data[1] + data[10] * _mat.data[2] + data[14] * _mat.data[3],
        data[3] * _mat.data[0] + data[7] * _mat.data[1] + data[11] * _mat.data[2] + data[15] * _mat.data[3],

        data[0] * _mat.data[4] + data[4] * _mat.data[5] + data[8] * _mat.data[6] + data[12] * _mat.data[7],
        data[1] * _mat.data[4] + data[5] * _mat.data[5] + data[9] * _mat.data[6] + data[13] * _mat.data[7],
        data[2] * _mat.data[4] + data[6] * _mat.data[5] + data[10] * _mat.data[6] + data[14] * _mat.data[7],
        data[3] * _mat.data[4] + data[7] * _mat.data[5] + data[11] * _mat.data[6] + data[15] * _mat.data[7],

        data[0] * _mat.data[8] + data[4] * _mat.data[9] + data[8] * _mat.data[10] + data[12] * _mat.data[11],
        data[1] * _mat.data[8] + data[5] * _mat.data[9] + data[9] * _mat.data[10] + data[13] * _mat.data[11],
        data[2] * _mat.data[8] + data[6] * _mat.data[9] + data[10] * _mat.data[10] + data[14] * _mat.data[11],
        data[3] * _mat.data[8] + data[7] * _mat.data[9] + data[11] * _mat.data[10] + data[15] * _mat.data[11],

        data[0] * _mat.data[12] + data[4] * _mat.data[13] + data[8] * _mat.data[14] + data[12] * _mat.data[15],
        data[1] * _mat.data[12] + data[5] * _mat.data[13] + data[9] * _mat.data[14] + data[13] * _mat.data[15],
        data[2] * _mat.data[12] + data[6] * _mat.data[13] + data[10] * _mat.data[14] + data[14] * _mat.data[15],
        data[3] * _mat.data[12] + data[7] * _mat.data[13] + data[11] * _mat.data[14] + data[15] * _mat.data[15]
    };
}

Mat4 Mat4::operator * (float _sca) const
{
    return
    {
        data[0] * _sca,
        data[1] * _sca,
        data[2] * _sca,
        data[3] * _sca,

        data[4] * _sca,
        data[5] * _sca,
        data[6] * _sca,
        data[7] * _sca,

        data[8] * _sca,
        data[9] * _sca,
        data[10] * _sca,
        data[11] * _sca,

        data[12] * _sca,
        data[13] * _sca,
        data[14] * _sca,
        data[15] * _sca
    };
}
Mat4 Mat4::operator / (float _sca) const
{
    return
    {
        data[0] / _sca,
        data[1] / _sca,
        data[2] / _sca,
        data[3] / _sca,

        data[4] / _sca,
        data[5] / _sca,
        data[6] / _sca,
        data[7] / _sca,

        data[8] / _sca,
        data[9] / _sca,
        data[10] / _sca,
        data[11] / _sca,

        data[12] / _sca,
        data[13] / _sca,
        data[14] / _sca,
        data[15] / _sca
    };
}

Mat4 Mat4::operator *= (float _sca)
{
    data[0] *= _sca;
    data[1] *= _sca;
    data[2] *= _sca;
    data[3] *= _sca;

    data[4] *= _sca;
    data[5] *= _sca;
    data[6] *= _sca;
    data[7] *= _sca;

    data[8] *= _sca;
    data[9] *= _sca;
    data[10] *= _sca;
    data[11] *= _sca;

    data[12] *= _sca;
    data[13] *= _sca;
    data[14] *= _sca;
    data[15] *= _sca;
    return *this;
}
Mat4 Mat4::operator /= (float _sca)
{
    data[0] /= _sca;
    data[1] /= _sca;
    data[2] /= _sca;
    data[3] /= _sca;

    data[4] /= _sca;
    data[5] /= _sca;
    data[6] /= _sca;
    data[7] /= _sca;

    data[8] /= _sca;
    data[9] /= _sca;
    data[10] /= _sca;
    data[11] /= _sca;

    data[12] /= _sca;
    data[13] /= _sca;
    data[14] /= _sca;
    data[15] /= _sca;
    return *this;
}

Vec3 Mat4::operator * (const Vec3& _vec) const
{
    return
    {
        data[0] * _vec.x + data[1] * _vec.y + data[2] * _vec.z + data[3],

        data[4] * _vec.x + data[5] * _vec.y + data[6] * _vec.z + data[7],

        data[8] * _vec.x + data[9] * _vec.y + data[10] * _vec.z + data[11]
    };
}

Vec4 Mat4::operator * (const Vec4& _vec) const
{
    return
    {
        data[0] * _vec.x + data[1] * _vec.y + data[2] * _vec.z + data[3] * _vec.w,

        data[4] * _vec.x + data[5] * _vec.y + data[6] * _vec.z + data[7] * _vec.w,

        data[8] * _vec.x + data[9] * _vec.y + data[10] * _vec.z + data[11] * _vec.w,

        data[12] * _vec.x + data[13] * _vec.y + data[14] * _vec.z + data[15] * _vec.w
    };
}