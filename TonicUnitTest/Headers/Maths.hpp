#pragma once

#include "DLL_API.hpp"


namespace Maths
{
#pragma region Scalar
	namespace Constants
	{
		constexpr float PI = 3.1415927f;
		constexpr float TAU = PI * 2.f;//TAU = 2PI

		constexpr float PI_2 = PI / 2.f;//PI/2
		constexpr float PI_3 = PI / 3.f;//PI/3
		constexpr float PI_4 = PI / 4.f;//PI/4
		constexpr float PI_6 = PI / 6.f;//PI/6
		constexpr float I_PI = 1.f / PI;//1/PI
		constexpr float I_TAU = 1.f / TAU;//1/2PI

		constexpr double PI_PRECISE = 3.141592653589793;

		constexpr float TOLERANCE = 3e-7f; //Use this rather  than 0 comparison

		constexpr float DEG2RAD = PI / 180.f;
		constexpr double DEG2RAD_PRECISE = PI_PRECISE / 180.;

		constexpr float RAD2DEG = 180.f / PI;
		constexpr double RAD2DEG_PRECISE = 180. / PI_PRECISE;
	}

	//multiply your angle in degrees by this value to convert it to radians.
	float TONIC_ENGINE_API ToRadians(float _angleDegrees);
	double TONIC_ENGINE_API ToRadians(double _angleDegrees);

	//multiply your angle in radians by this value to convert it to degrees.
	float TONIC_ENGINE_API ToDegrees(float _angleRadians);
	double TONIC_ENGINE_API ToDegrees(double _angleRadians);
#pragma endregion

#pragma region Vector

	class TONIC_ENGINE_API Vec2
	{

#pragma region Members
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
#pragma endregion

#pragma region Constructors
		//CONSTRUCTORS :
		Vec2(void);
		Vec2(float _xy);
		Vec2(float _x, float _y);
		Vec2(const class Vec3& _vec3);
		Vec2(const class Vec4& _vec4);

		//DESTRUCTOR :
		~Vec2(void);
#pragma endregion

#pragma region Utils
		//UTILS :
		float GetMagnitude() const;
		float GetMagnitudeSquared() const;
		Vec2 GetNormalized() const;
		Vec2 GetPerpendicular() const;
#pragma endregion

#pragma region Operators
		//ASSINGMENT AND EQUALITY OPERATIONS :
		Vec2 operator = (const Vec2& _vec);
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
#pragma endregion
	};

	class Vec3
	{
	public:
#pragma region Members
		//MEMBERS :
		union
		{
			struct
			{
				float x, y, z;
			};
			float xyz[3];
		};
		//Static members :
		static const Vec3 UP;
		static const Vec3 DOWN;

		static const Vec3 LEFT;
		static const Vec3 RIGHT;

		static const Vec3 BACKWARD;
		static const Vec3 FORWARD;
#pragma endregion

#pragma region Constructors
		//CONSTRUCTORS :
		TONIC_ENGINE_API Vec3(void);
		TONIC_ENGINE_API Vec3(float _xyz);
		TONIC_ENGINE_API Vec3(float _x, float _y, float _z);
		TONIC_ENGINE_API Vec3(const class Vec2& _vec2, float _z = 0.f);
		TONIC_ENGINE_API Vec3(const class Vec4& _vec4);

		//DESTRUCTOR :
		TONIC_ENGINE_API ~Vec3(void);
#pragma endregion

#pragma region Utils
		//UTILS :
		float TONIC_ENGINE_API GetMagnitude() const;
		float TONIC_ENGINE_API GetMagnitudeSquared() const;
		Vec3  TONIC_ENGINE_API GetNormalized() const;
#pragma endregion

#pragma region Operators
		//ASSINGMENT AND EQUALITY OPERATIONS :
		Vec3 TONIC_ENGINE_API operator = (const Vec2& _vec);
		Vec3 TONIC_ENGINE_API operator = (const Vec4& _vec);
		Vec3 TONIC_ENGINE_API operator = (float _sca);

		Vec3 TONIC_ENGINE_API operator - (void) const;
		bool TONIC_ENGINE_API operator == (const Vec3& _vec) const;
		bool TONIC_ENGINE_API operator != (const Vec3& _vec) const;

		float TONIC_ENGINE_API operator[](int _index) const;

		//Vec3 TO Vec3 OPERATIONS :

		Vec3 TONIC_ENGINE_API operator + (const Vec3& _vec) const;
		Vec3 TONIC_ENGINE_API operator - (const Vec3& _vec) const;

		//Vec3 TO THIS OPERATIONS :

		Vec3 TONIC_ENGINE_API operator += (const Vec3& _vec);
		Vec3 TONIC_ENGINE_API operator -= (const Vec3& _vec);

		//SCALER TO Vec3 OPERATIONS :

		Vec3 TONIC_ENGINE_API operator + (float _sca) const;
		Vec3 TONIC_ENGINE_API operator - (float _sca) const;
		Vec3 TONIC_ENGINE_API operator* (float _sca) const;
		Vec3 TONIC_ENGINE_API operator / (float _sca) const;

		//SCALER TO THIS OPERATIONS :

		Vec3 TONIC_ENGINE_API operator += (float _sca);
		Vec3 TONIC_ENGINE_API operator -= (float _sca);
		Vec3 TONIC_ENGINE_API operator *= (float _sca);
		Vec3 TONIC_ENGINE_API operator /= (float _sca);
#pragma endregion
	};

	class TONIC_ENGINE_API Vec4
	{
	public:
#pragma region Members
		//MEMBERS :
		union
		{
			struct
			{
				float x, y, z, w;
			};
			float xyzw[4];
		};
#pragma endregion

#pragma region Constructors

		//CONSTRUCTORS :
		Vec4(void);
		Vec4(float _xyzw);
		Vec4(float _x, float _y, float _z, float _w);
		Vec4(const class Vec2& _vec2, float _z = 0.f, float _w = 1.f);
		Vec4(const class Vec3& _vec3, float _w = 1.f);

		//DESTRUCTOR :
		~Vec4(void);
#pragma endregion

#pragma region Utils
		//UTILS :
		float GetMagnitude() const;
		float GetMagnitudeSquared() const;
		Vec4 GetNormalized() const;
#pragma endregion

#pragma region Operators
		//ASSINGMENT AND EQUALITY OPERATIONS :
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

		//SCALAR TO Vec4 OPERATIONS :
		Vec4 operator + (float _sca) const;
		Vec4 operator - (float _sca) const;
		Vec4 operator * (float _sca) const;
		Vec4 operator / (float _sca) const;

		//SCALAR TO THIS OPERATIONS :

		Vec4 operator += (float _sca);
		Vec4 operator -= (float _sca);
		Vec4 operator *= (float _sca);
		Vec4 operator /= (float _sca);
#pragma endregion
	};

	namespace Vectors
	{
	void TONIC_ENGINE_API Normalize(Vec2& _vec2);
	void TONIC_ENGINE_API Normalize(Vec3& _vec3);
	void TONIC_ENGINE_API Normalize(Vec4& _vec4);

	float TONIC_ENGINE_API DotProduct(const Vec2& _vecA, const Vec2& _vecB);
	float TONIC_ENGINE_API DotProduct(const Vec3& _vecA, const Vec3& _vecB);
	float TONIC_ENGINE_API DotProduct(const Vec4& _vecA, const Vec4& _vecB);

	float TONIC_ENGINE_API DistanceBetween(const Vec2& _vecA, const Vec2& _vecB);
	float TONIC_ENGINE_API DistanceBetween(const Vec3& _vecA, const Vec3& _vecB);
	float TONIC_ENGINE_API DistanceBetween(const Vec4& _vecA, const Vec4& _vecB);

	float TONIC_ENGINE_API AngleBetween(const Vec2& _vecA, const Vec2& _vecB);
	float TONIC_ENGINE_API AngleBetween(const Vec3& _vecA, const Vec3& _vecB);
	float TONIC_ENGINE_API AngleBetween(const Vec4& _vecA, const Vec4& _vecB);

	//return the z value
	float TONIC_ENGINE_API CrossProduct(const Vec2& _vecA, const Vec2& _vecB);

	Vec3 TONIC_ENGINE_API CrossProduct(const Vec3& _vecA, const Vec3& _vecB);
	Vec3 TONIC_ENGINE_API Rotate(const Vec3& _vec, float _angle, const Vec3& _axis);
	}

	class Mat4;
#pragma endregion

#pragma region Matrix
	class TONIC_ENGINE_API Mat3
	{
	public:
#pragma region Members
		//MEMBERS :
		union
		{
			float data[9];
			Vec3 data3V[3];
			float data_3_3[3][3];
		};
#pragma endregion

#pragma region Constructors
		//CONSTRUCTORS :
		Mat3(void);
		Mat3(float _data[9]);
		Mat3(float _indentityValue);
		Mat3(float _0, float _1, float _2,
			float _3, float _4, float _5,
			float _6, float _7, float _8);
		Mat3(const Mat4& _mat4);
		//DESTRUCTOR :
		~Mat3(void);
#pragma endregion

#pragma region Utils
		//UTILS :
		static Mat3 Transform(const Vec2& _translation, float _angleInRad, const Vec2& _scaling);
		static Mat3 Rotate2D(float angleinrad);
		static Mat3 Translate2D(const Vec2& _translation);
		static Mat3 Scale2D(const Vec2& _scaling);
		float Determinant() const;
		float Trace() const;
		Mat3 Inverse();
		Mat3 Transpose();
#pragma endregion

#pragma region Operators

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
#pragma endregion

	};

	class TONIC_ENGINE_API Mat4
	{
	public:
#pragma region Members
		//MEMBERS :
		union
		{
			float data[16];
			Vec4 data4V[4];
			float data_4_4[4][4];
		};
#pragma endregion

#pragma region Constructors
		//CONSTRUCTORS :
		Mat4(void);
		Mat4(float _data[16]);
		Mat4(float _indentityValue);
		Mat4(float _0, float _1, float _2, float _3,
			float _4, float _5, float _6, float _7,
			float _8, float _9, float _10, float _11,
			float _12, float _13, float _14, float _15);
		Mat4(const Mat3& _mat3);

		//DESTRUCTOR :
		~Mat4(void);
#pragma endregion

#pragma region Utils
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
#pragma endregion

#pragma region Operators
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
#pragma endregion
	};

	//------------------------------------------------------------------
	//------------------------------------------------------------------
	namespace Matrices
	{
	Mat4 TONIC_ENGINE_API Frustum(float _left, float _right, float _bottom, float _top, float _nearVal, float _farVal);
	Mat4 TONIC_ENGINE_API Ortho(float _left, float _right, float _bottom, float _top, float _nearVal, float _farVal);
	Mat4 TONIC_ENGINE_API LookAt(float _eyeX, float _eyeY, float _eyeZ, float _centerX, float _centerY, float _centerZ, float _upX, float _upY, float _upZ);
	Mat4 TONIC_ENGINE_API LookAt(Vec3 _eyeZ, Vec3 _centerZ, Vec3 _upZ);
	Mat4 TONIC_ENGINE_API LookAt(Vec3 _forward, Vec3 _localUp);
	Mat4 TONIC_ENGINE_API LookAtNormalized(Vec3 _forward, Vec3 _localUp);
	Mat4 TONIC_ENGINE_API LookAtNormalized(Vec3 _eye, Vec3 _center, Vec3 _up);
	Mat4 TONIC_ENGINE_API Perspective(float _fovY, float _aspect, float _near, float _far);
	}
	//------------------------------------------------------------------
	//------------------------------------------------------------------

#pragma endregion

#pragma region Quaternion
#pragma once

	class TONIC_ENGINE_API Quat
	{
	public:

#pragma region Members
		//MEMBERS :
		union
		{
			struct
			{
				Vec3 imaginary;
				float real;
			};
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
			float xyzw[4];
			Vec4 vecFour;

		};
#pragma endregion

#pragma region Constructors
		//CONSTRUCTORS :
		Quat();
		Quat(const Vec4& _vector);
		Quat(float _w, float _x, float _y, float _z);
		Quat(float _w, const Vec3& _imaginary);
		// From Vector angle i
		Quat(const Vec3& _vector);

		static Quat Identity();

		//DESTRUCTOR :
		~Quat(void);
#pragma endregion

#pragma region Utils
		//UTILS
		float GetMagnitudeSquared() const;
		float GetMagnitude() const;
		Quat GetNormalized() const;
		Quat GetConjugate() const;
		//CARE: can cause approximation errors if too many successive operation, use GetInversePrecise, if needed
		Quat GetInverse() const;
		//More precise, but a bit slower(4%), prefer GetInverse if you don't have to invert over and over the same quaternion
		Quat GetInversePrecise() const;
		static Quat Nlerp(Quat _start, Quat _end, float _ratio);
		static Quat Slerp(const Quat& _q1, const Quat& _q2, float _t);
#pragma endregion

#pragma region Conversion
		//CONVERSION
		Vec3 RotateVector(const Vec3& _vec) const;
		Mat3 ToRotationMatrix();
			//Credits to Timboc on https://forum.unity.com/threads/is-there-a-conversion-method-from-quaternion-to-euler.624007/
		Vec3 ToEulerAngles() const;
#pragma endregion

#pragma region Operator
		//OPERATOR

		Quat operator*(const Quat& _quat2) const;
		void operator*=(const Quat& _quat2);
		Quat operator*(float value) const;
		Quat operator+(const Quat& q) const;
		float operator[](int i) const;
		float& operator[](int i);

#pragma endregion
	};
	namespace Quaternions
	{
		float DotProduct(const Quat& _q1, const Quat& _q2);
		// in radians
		Quat TONIC_ENGINE_API FromAngleAxis(float _angle, const Vec3& _axis);
		Quat TONIC_ENGINE_API LookAt(const Vec3 _from, const Vec3 _to);
		Quat TONIC_ENGINE_API FromEulerAngles(const Vec3& _xyz);
		Quat TONIC_ENGINE_API FromMatrix(const Mat3& _matrix);
		Quat TONIC_ENGINE_API FromMatrix(const Mat4& _matrix);
	}
#pragma endregion
}
