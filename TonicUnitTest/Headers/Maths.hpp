#pragma once

#include "DLL_API.hpp"

namespace Maths
{
	namespace Constants
    {
        constexpr float PI = 3.1415927f;
        constexpr double PI_PRECISE = 3.141592653589793;
        constexpr float TOLERANCE = 3e-7f;
    }

 	//multiply your angle in degrees by this value to convert it to radians.
	float ToRadians(float _angleDegrees);
    TONIC_ENGINE_API double ToRadians(double _angleDegrees);

    //multiply your angle in radians by this value to convert it to degrees.
    TONIC_ENGINE_API float ToDegrees(float _angleRadians);
    TONIC_ENGINE_API double ToDegrees(double _angleRadians);

#pragma region Vector

    TONIC_ENGINE_API class Vec2
    {
    public:
        //MEMBERS :
        union
        {
            struct
            {
                float x, y;
            };
            float xy[2];
        };

        //CONSTRUCTORS :
        Vec2(void);
        Vec2(float _xy);
        Vec2(float _x, float _y);
        Vec2(const class Vec3& _vec3);
        Vec2(const class Vec4& _vec4);

        //DESTRUCTOR :
        ~Vec2(void);

        //UTILS :
        float GetMagnitude() const;
        float GetMagnitudeSquared() const;
        Vec2 GetNormalized() const;
        Vec2 GetPerpendicular() const;

        //ASSINGMENT AND EQUALITY OPERATIONS :
        //Vec2 operator = (const Vec2& _vec); //TODO
        Vec2 operator = (const Vec3& _vec);
        Vec2 operator = (const Vec4& _vec);
        Vec2 operator = (float _sca);

        Vec2 operator - (void) const;
        bool operator == (const Vec2& _vec) const;
        bool operator != (const Vec2& _vec) const;

        float operator [] (int _index) const;

        //Vec2 TO Vec2 OPERATIONS :

        Vec2 operator + (const Vec2& _vec) const;
        Vec2 operator - (const Vec2& _vec) const;

        //Vec2 TO THIS OPERATIONS :

        Vec2 operator += (const Vec2& _vec);
        Vec2 operator -= (const Vec2& _vec);

        //SCALER TO Vec2 OPERATIONS :

        Vec2 operator + (float _sca) const;
        Vec2 operator - (float _sca) const;
        Vec2 operator * (float _sca) const;
        Vec2 operator / (float _sca) const;

        //SCALER TO THIS OPERATIONS :

        Vec2 operator += (float _sca);
        Vec2 operator -= (float _sca);
        Vec2 operator *= (float _sca);
        Vec2 operator /= (float _sca);

    };

    TONIC_ENGINE_API class Vec3
    {
    public:
        //MEMBERS :
        union
        {
            struct
            {
                float x, y, z;
            };
            float xyz[3];
        };

        //CONSTRUCTORS :
        Vec3(void);
        Vec3(float _xyz);
        Vec3(float _x, float _y, float _z);
        Vec3(const class Vec2& _vec2, float _z = 0.f);
        Vec3(const class Vec4& _vec4);

        //DESTRUCTOR :
        ~Vec3(void);

        //UTILS :
        float GetMagnitude() const;
        float GetMagnitudeSquared() const;
        Vec3 GetNormalized() const;

        //ASSINGMENT AND EQUALITY OPERATIONS :
        //Vec3 operator = (const Vec3& _vec); //TODO
        Vec3 operator = (const Vec2& _vec);
        Vec3 operator = (const Vec4& _vec);
        Vec3 operator = (float _sca);

        Vec3 operator - (void) const;
        bool operator == (const Vec3& _vec) const;
        bool operator != (const Vec3& _vec) const;

        float operator [] (int _index) const;

        //Vec3 TO Vec3 OPERATIONS :

        Vec3 operator + (const Vec3& _vec) const;
        Vec3 operator - (const Vec3& _vec) const;

        //Vec3 TO THIS OPERATIONS :

        Vec3 operator += (const Vec3& _vec);
        Vec3 operator -= (const Vec3& _vec);

        //SCALER TO Vec3 OPERATIONS :

        Vec3 operator + (float _sca) const;
        Vec3 operator - (float _sca) const;
        Vec3 operator * (float _sca) const;
        Vec3 operator / (float _sca) const;

        //SCALER TO THIS OPERATIONS :

        Vec3 operator += (float _sca);
        Vec3 operator -= (float _sca);
        Vec3 operator *= (float _sca);
        Vec3 operator /= (float _sca);
    };

    TONIC_ENGINE_API class Vec4
    {
    public:
        //MEMBERS :
        union
        {
            struct
            {
                float x, y, z, w;
            };
            float xyzw[4];
        };

        //CONSTRUCTORS :
        Vec4(void);
        Vec4(float _xyzw);
        Vec4(float _x, float _y, float _z, float _w);
        Vec4(const class Vec2& _vec2, float _z = 0.f, float _w = 1.f);
        Vec4(const class Vec3& _vec3, float _w = 1.f);

        //DESTRUCTOR :
        ~Vec4(void);

        //UTILS :
        float GetMagnitude() const;
        float GetMagnitudeSquared() const;
        Vec4 GetNormalized() const;

        //ASSINGMENT AND EQUALITY OPERATIONS :
        //Vec4 operator = (const Vec4& _vec); //TODO
        Vec4 operator = (const Vec2& _vec);
        Vec4 operator = (const Vec3& _vec);
        Vec4 operator = (float _sca);

        Vec2 operator - (void) const;
        bool operator == (const Vec4& _vec) const;
        bool operator != (const Vec4& _vec) const;

        float operator [] (int _index) const;

        //Vec4 TO Vec4 OPERATIONS :

        Vec4 operator + (const Vec4& _vec) const;
        Vec4 operator - (const Vec4& _vec) const;

        //Vec4 TO THIS OPERATIONS :

        Vec4 operator += (const Vec4& _vec);
        Vec4 operator -= (const Vec4& _vec);

        //SCALER TO Vec4 OPERATIONS :

        Vec4 operator + (float _sca) const;
        Vec4 operator - (float _sca) const;
        Vec4 operator * (float _sca) const;
        Vec4 operator / (float _sca) const;

        //SCALER TO THIS OPERATIONS :

        Vec4 operator += (float _sca);
        Vec4 operator -= (float _sca);
        Vec4 operator *= (float _sca);
        Vec4 operator /= (float _sca);
    };

//------------------------------------------------------------------
//------------------------------------------------------------------
	TONIC_ENGINE_API void Normalize(Vec2& _vec2);
	TONIC_ENGINE_API void Normalize(Vec3& _vec3);
	TONIC_ENGINE_API void Normalize(Vec4& _vec4);
	
	TONIC_ENGINE_API float DotProduct(const Vec2& _vecA, const Vec2& _vecB);
	TONIC_ENGINE_API float DotProduct(const Vec3& _vecA, const Vec3& _vecB);
	TONIC_ENGINE_API float DotProduct(const Vec4& _vecA, const Vec4& _vecB);

	TONIC_ENGINE_API float DistanceBetween(const Vec2& _vecA, const Vec2& _vecB);
	TONIC_ENGINE_API float DistanceBetween(const Vec3& _vecA, const Vec3& _vecB);
	TONIC_ENGINE_API float DistanceBetween(const Vec4& _vecA, const Vec4& _vecB);

	TONIC_ENGINE_API float AngleBetween(const Vec2& _vecA, const Vec2& _vecB);
	TONIC_ENGINE_API float AngleBetween(const Vec3& _vecA, const Vec3& _vecB);
	TONIC_ENGINE_API float AngleBetween(const Vec4& _vecA, const Vec4& _vecB);

	//return the z value
	TONIC_ENGINE_API float CrossProduct(const Vec2& _vecA, const Vec2& _vecB);

	TONIC_ENGINE_API Vec3 CrossProduct(const Vec3& _vecA, const Vec3& _vecB);
//------------------------------------------------------------------
//------------------------------------------------------------------

#pragma endregion
#pragma region Matrix
    TONIC_ENGINE_API class Mat3
    {
    public:
        //MEMBERS :
        union
        {
            float data[9];
            Vec3 data3V[3];
            float data_3_3[3][3];
        };

        //CONSTRUCTORS :
        Mat3(void);
        Mat3(float _data[9]);
        Mat3(float _indentityValue);
        Mat3(float _0, float _1, float _2,
            float _3, float _4, float _5,
            float _6, float _7, float _8);

        //DESTRUCTOR :
        ~Mat3(void);

        //UTILS :
        static Mat3 Transform(const Vec2& _translation, float _angleInRad, const Vec2& _scaling);
        static Mat3 Rotate2D(float angleinrad);
        static Mat3 Translate2D(const Vec2& _translation);
        static Mat3 Scale2D(const Vec2& _scaling);
        float Determinant() const;
        float Trace() const;
        Mat3 Inverse();
        Mat3 Transpose();

        //ASSINGMENT AND EQUALITY OPERATIONS :
        Mat3 operator = (float _data[9]);

        Mat3 operator - (void) const;
        bool operator == (const Mat3& _mat) const;
        bool operator != (const Mat3& _mat) const;

        Vec3 operator [] (int _index) const;

        //Mat3 TO Mat3 OPERATIONS :
        Mat3 operator + (const Mat3& _mat) const;
        Mat3 operator - (const Mat3& _mat) const;
        Mat3 operator * (const Mat3& _mat) const;

        //Mat3 TO This OPERATIONS :
        Mat3 operator += (const Mat3& _mat);
        Mat3 operator -= (const Mat3& _mat);
        Mat3 operator *= (const Mat3& _mat);

        //Sca TO Mat3 OPERATIONS :
        Mat3 operator * (float _sca) const;
        Mat3 operator / (float _sca) const;

        //Sca TO This OPERATIONS :
        Mat3 operator *= (float _sca);
        Mat3 operator /= (float _sca);

        //Mat3 TO Vec2 OPERATIONS :
        Vec2 operator * (const Vec2& _vec) const;

        //Mat3 TO Vec3 OPERATIONS :
        Vec3 operator * (const Vec3& _vec) const;

    };

    TONIC_ENGINE_API class Mat4
    {
    public:
        //MEMBERS :
        union
        {
            float data[16];
            Vec4 data4V[4];
            float data_4_4[4][4];
        };

        //CONSTRUCTORS :
        Mat4(void);
        Mat4(float _data[16]);
        Mat4(float _indentityValue);
        Mat4(float _0, float _1, float _2, float _3,
            float _4, float _5, float _6, float _7,
            float _8, float _9, float _10, float _11,
            float _12, float _13, float _14, float _15);

        //DESTRUCTOR :
        ~Mat4(void);

        //UTILS
        static Mat4 Transform(Vec3 _translation, Vec3 _anglesInRad, Vec3 _scaling);
        static Mat4 Rotate(Vec3 _anglesInRad);
        static Mat4 RotateX(float _angleInRad);
        static Mat4 RotateY(float _angleInRad);
        static Mat4 RotateZ(float _angleInRad);
        static Mat4 Translate(Vec3 _translation);
        static Mat4 Scale(Vec3 _scaling);
        float Determinant() const;
        float Trace() const;
        Mat4 Inverse();
        Mat4 Transpose();

        //ASSINGMENT AND EQUALITY OPERATIONS :
        Mat4 operator = (float _data[16]);

        Mat4 operator - (void) const;
        bool operator == (const Mat4& _mat) const;
        bool operator != (const Mat4& _mat) const;

        Vec4 operator [] (int _index) const;

        //Mat4 TO Mat4 OPERATIONS :
        Mat4 operator + (const Mat4& _mat) const;
        Mat4 operator - (const Mat4& _mat) const;
        Mat4 operator * (const Mat4& _mat) const;

        //Mat4 TO This OPERATIONS :
        Mat4 operator += (const Mat4& _mat);
        Mat4 operator -= (const Mat4& _mat);
        Mat4 operator *= (const Mat4& _mat);

        //Sca TO Mat4 OPERATIONS :
        Mat4 operator * (float _sca) const;
        Mat4 operator / (float _sca) const;

        //Sca TO This OPERATIONS :
        Mat4 operator *= (float _sca);
        Mat4 operator /= (float _sca);

        //Mat3 TO Vec3 OPERATIONS :
        Vec3 operator * (const Vec3& _vec) const;

        //Mat3 TO Vec4 OPERATIONS :
        Vec4 operator * (const Vec4& _vec) const;
    };
#pragma endregion
}