#include "pch.hpp"

//Tests based on glm library
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp >

#include <Maths.hpp>


#define CLASS_NAME Nan
#define TOLERANCE 3e-7f
#define ANGLE_TOLERANCE 6e-5f

using namespace Maths;

#define RAND_FLOAT static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
//if you have a test with Nan as class you fucked. do like this :
// 
//#pragma region ClassName
//#define CLASS_NAME ClassName
//
//TEST(CLASS_NAME, Debug_Test)
//{
//	EXPECT_TRUE(true);
//}
//
//#pragma endregion


//EXPECT_EQ(1, 1); -> OK
//EXPECT_TRUE(true); -> OK
//EXPECT_NEAR(1.05, 1.0, 0.1); -> OK

template<typename T, typename U>
T DivideAlot(T _a, U _b, unsigned int _iterations = 1000)
{
	for (unsigned int i = 0; i < _iterations - 1; i++)
		_a / _b;
	return _a / _b;
}
template<typename T, typename U>
T MultiplyAlot(T _a, U _b, unsigned int _iterations = 1000)
{
	for (unsigned int i = 0; i < _iterations - 1; i++)
		_a* _b;
	return _a * _b;
}
//--VEC-2-------------------------------------------
#pragma region Vec2
#define CLASS_NAME Vec2
	//--Methods----------------------------------
TEST(CLASS_NAME, MagnitudeSquared)
{
	srand(static_cast <unsigned> (time(0)));

	float x = RAND_FLOAT;
	float y = RAND_FLOAT;

	EXPECT_NEAR(glm::dot(glm::vec2(x, y), glm::vec2(x, y)), Vec2(x, y).GetMagnitudeSquared(), TOLERANCE);
}
TEST(CLASS_NAME, Magnitude) // POP POP
{
	float x = RAND_FLOAT;
	float y = RAND_FLOAT;
	EXPECT_NEAR(glm::length(glm::vec2(x, y)), Vec2(x, y).GetMagnitude(), TOLERANCE);
}
TEST(CLASS_NAME, Normalized)
{
	float x = RAND_FLOAT;
	float y = RAND_FLOAT;
	glm::vec2 glmResult = glm::normalize(glm::vec2(x, y));
	Vec2 ourResult = Vec2(x, y).GetNormalized();
	Vec2 ourResult2 = ourResult;
	Normalize(ourResult2);

	EXPECT_NEAR(glmResult.x, ourResult.x, TOLERANCE);
	EXPECT_NEAR(glmResult.y, ourResult.y, TOLERANCE);
	EXPECT_NEAR(ourResult2.x, ourResult.x, TOLERANCE);
	EXPECT_NEAR(ourResult2.y, ourResult.y, TOLERANCE);

}
//--Operators----------------------------------
TEST(CLASS_NAME, Dot)
{
	float x = RAND_FLOAT;
	float y = RAND_FLOAT;
	float x2 = RAND_FLOAT;
	float y2 = RAND_FLOAT;
	EXPECT_NEAR(glm::dot(glm::vec2(x, y), glm::vec2(x2, y2)), DotProduct(Vec2(x, y), Vec2(x2, y2)), TOLERANCE);
}
TEST(CLASS_NAME, CrossProduct)
{
	float x = RAND_FLOAT;
	float y = RAND_FLOAT;
	float x2 = RAND_FLOAT;
	float y2 = RAND_FLOAT;
	EXPECT_NEAR(glm::cross(glm::vec3(x, y, 0.f), glm::vec3(x2, y2, 0.f)).z, CrossProduct(Vec2(x, y), Vec2(x2, y2)), TOLERANCE);
}
TEST(CLASS_NAME, Distance)
{
	float x = RAND_FLOAT;
	float y = RAND_FLOAT;
	float x2 = RAND_FLOAT;
	float y2 = RAND_FLOAT;
	EXPECT_NEAR(glm::distance(glm::vec2(x, y), glm::vec2(x2, y2)), DistanceBetween(Vec2(x, y), Vec2(x2, y2)), TOLERANCE);
}
TEST(CLASS_NAME, Angle_EXPERIMENTAL)
{
	float x = RAND_FLOAT;
	float y = RAND_FLOAT;
	float x2 = RAND_FLOAT;
	float y2 = RAND_FLOAT;
	float glmResult = glm::orientedAngle(glm::normalize(glm::vec2(x, y)), glm::normalize(glm::vec2(x2, y2)));
	float ourResult = AngleBetween(Vec2(x, y), Vec2(x2, y2));

	EXPECT_NEAR(glmResult, ourResult, ANGLE_TOLERANCE);
}


#pragma endregion
//--VEC-3-------------------------------------------
#pragma region Vec3
#define CLASS_NAME Vec3
	//--Methods----------------------------------
TEST(CLASS_NAME, MagnitudeSquared)
{
	float x = RAND_FLOAT;
	float y = RAND_FLOAT;
	float z = RAND_FLOAT;

	EXPECT_NEAR(glm::dot(glm::vec3(x, y, z), glm::vec3(x, y, z)), Vec3(x, y, z).GetMagnitudeSquared(), TOLERANCE);
}
TEST(CLASS_NAME, Magnitude) // POP POP
{
	float x = RAND_FLOAT;
	float y = RAND_FLOAT;
	float z = RAND_FLOAT;
	EXPECT_NEAR(glm::length(glm::vec3(x, y, z)), Vec3(x, y, z).GetMagnitude(), TOLERANCE);
}
TEST(CLASS_NAME, Normalized)
{
	float x = RAND_FLOAT;
	float y = RAND_FLOAT;
	float z = RAND_FLOAT;
	glm::vec3 glmResult = glm::vec3(x, y, z) / glm::length(glm::vec3(x, y, z));
	Vec3 ourResult = Vec3(x, y, z).GetNormalized();
	Vec3 ourResult2 = ourResult;
	Normalize(ourResult2);
	for (int i = 0; i < 3; i++)
	{
		EXPECT_NEAR(glmResult[i], ourResult[i], TOLERANCE);
		EXPECT_NEAR(ourResult2[i], ourResult[i], TOLERANCE);
	}
}
//--Operators----------------------------------
TEST(CLASS_NAME, Dot)
{
	glm::vec3 glmResult, glmResult2;
	Vec3 ourResult, ourResult2;
	for (int i = 0; i < 3; i++)
	{
		glmResult[i] = ourResult.xyz[i] = RAND_FLOAT;
		glmResult2[i] = ourResult2.xyz[i] = RAND_FLOAT;
	}
	EXPECT_NEAR(glm::dot(glmResult, glmResult2), DotProduct(ourResult, ourResult2), TOLERANCE);
}
TEST(CLASS_NAME, CrossProduct)
{
	glm::vec3 glmResult, glmResult2;
	Vec3 ourResult, ourResult2;
	for (int i = 0; i < 3; i++)
	{
		glmResult[i] = ourResult.xyz[i] = RAND_FLOAT;
		glmResult2[i] = ourResult2.xyz[i] = RAND_FLOAT;
	}
	glmResult = glm::cross(glmResult, glmResult2);
	ourResult = CrossProduct(ourResult, ourResult2);
	for (int i = 0; i < 3; i++)
		EXPECT_NEAR(ourResult[i], glmResult[i], TOLERANCE);
}
TEST(CLASS_NAME, Distance)
{
	glm::vec3 glmResult, glmResult2;
	Vec3 ourResult, ourResult2;
	for (int i = 0; i < 3; i++)
	{
		glmResult[i] = ourResult.xyz[i] = RAND_FLOAT;
		glmResult2[i] = ourResult2.xyz[i] = RAND_FLOAT;
	}
	EXPECT_NEAR(glm::distance(glmResult, glmResult2), DistanceBetween(ourResult, ourResult2), TOLERANCE);
}
TEST(CLASS_NAME, Angle_EXPERIMENTAL)
{
	glm::vec3 glmResult, glmResult2;
	Vec3 ourResult, ourResult2;
	for (int i = 0; i < 3; i++)
	{
		glmResult[i] = ourResult.xyz[i] = RAND_FLOAT;
		glmResult2[i] = ourResult2.xyz[i] = RAND_FLOAT;
	}
	//Normalize for glm::angle to work
	glmResult = glm::normalize(glmResult);
	glmResult2 = glm::normalize(glmResult2);
	EXPECT_NEAR(glm::angle(glmResult, glmResult2), AngleBetween(ourResult, ourResult2), ANGLE_TOLERANCE);
}
#pragma endregion
//--VEC-4-------------------------------------------
#pragma region Vec4
#define CLASS_NAME Vec4

	//--Methods-------------------------------------
TEST(CLASS_NAME, MagnitudeSquared)
{
	float x = RAND_FLOAT;
	float y = RAND_FLOAT;
	float z = RAND_FLOAT;
	float w = RAND_FLOAT;
	EXPECT_NEAR(glm::dot(glm::vec4(x, y, z, w), glm::vec4(x, y, z, w)), Vec4(x, y, z, w).GetMagnitudeSquared(), TOLERANCE);
}
TEST(CLASS_NAME, Magnitude) // POP POP
{
	float x = RAND_FLOAT;
	float y = RAND_FLOAT;
	float z = RAND_FLOAT;
	float w = RAND_FLOAT;
	EXPECT_NEAR(glm::length(glm::vec4(x, y, z, w)), Vec4(x, y, z, w).GetMagnitude(), TOLERANCE);
}
TEST(CLASS_NAME, Normalized)
{
	glm::vec4 glmResult;
	Vec4 ourResult;
	for (int i = 0; i < 4; i++)
		glmResult[i] = ourResult.xyzw[i] = RAND_FLOAT;
	glmResult /= glm::length(glmResult);
	ourResult = ourResult.GetNormalized();
	Vec4 ourResult2 = ourResult;
	Normalize(ourResult2);
	for (int i = 0; i < 4; i++)
	{
		EXPECT_NEAR(glmResult[i], ourResult.xyzw[i], TOLERANCE);
		EXPECT_NEAR(ourResult2[i], ourResult[i], TOLERANCE);
	}
}
//--Operators----------------------------------
TEST(CLASS_NAME, Dot)
{
	glm::vec4 glmResult;
	Vec4 ourResult;
	for (int i = 0; i < 4; i++)
	{
		glmResult[i] = ourResult.xyzw[i] = RAND_FLOAT;
	}
	EXPECT_NEAR(glm::dot(glmResult, glmResult), DotProduct(ourResult, ourResult), TOLERANCE);
}

TEST(CLASS_NAME, Distance)
{
	glm::vec4 glmResult, glmResult2;
	Vec4 ourResult, ourResult2;
	for (int i = 0; i < 4; i++)
	{
		glmResult[i] = ourResult.xyzw[i] = RAND_FLOAT;
		glmResult2[i] = ourResult2.xyzw[i] = RAND_FLOAT;
	}
	EXPECT_NEAR(glm::distance(glmResult, glmResult2), DistanceBetween(ourResult, ourResult2), TOLERANCE);
}
TEST(CLASS_NAME, Angle_EXPERIMENTAL)
{
	glm::vec4 glmResult, glmResult2;
	Vec4 ourResult, ourResult2;
	for (int i = 0; i < 4; i++)
	{
		glmResult[i] = ourResult.xyzw[i] = RAND_FLOAT;
		glmResult2[i] = ourResult2.xyzw[i] = RAND_FLOAT;
	}
	//Normalize for glm::angle to work
	glmResult = glm::normalize(glmResult);
	glmResult2 = glm::normalize(glmResult2);
	EXPECT_NEAR(glm::angle(glmResult, glmResult2), AngleBetween(ourResult, ourResult2), ANGLE_TOLERANCE);
}

#pragma endregion
//--MAT-3-------------------------------------------
#pragma region Mat3
#define CLASS_NAME Mat3
   //Helper for uncanny comparision
void CompareMat3WithGlmMat4(Mat3 _mat3, glm::mat4 _mat4)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			EXPECT_NEAR(_mat3.data_3_3[i][j], _mat4[i == 2 ? 3 : i][j == 2 ? 3 : j], TOLERANCE);
}
//--Constructors--------------------------------
TEST(CLASS_NAME, Translate2D)
{
	float rd1 = RAND_FLOAT;
	float rd2 = RAND_FLOAT;

	Mat3 ourResult = Mat3::Translate2D(Vec2(rd1, rd2));
	glm::mat4 glmResult = glm::translate(glm::vec3(rd1, rd2, 0.f));
	CompareMat3WithGlmMat4(ourResult, glmResult);
}
TEST(CLASS_NAME, Scale2D)
{
	float rd1 = RAND_FLOAT;
	float rd2 = RAND_FLOAT;

	Mat3 ourResult = Mat3::Scale2D(Vec2(rd1, rd2));
	glm::mat4 glmResult = glm::scale(glm::vec3(rd1, rd2, 0.f));
	CompareMat3WithGlmMat4(ourResult, glmResult);
}
TEST(CLASS_NAME, Rotation2D)
{
	float rdAngle = RAND_FLOAT;

	Mat3 ourResult = Mat3::Rotate2D(rdAngle);
	glm::mat4 glmResult = glm::rotate(rdAngle, glm::vec3(0.f, 0.f, 1.f)); //Rotate around z for 2D
	CompareMat3WithGlmMat4(ourResult, glmResult);
}
TEST(CLASS_NAME, Transform2D)
{
	float trs[5];
	for (int i = 0; i < 5; i++)
		trs[i] = RAND_FLOAT;

	Mat3 ourResult = Mat3::Transform(Vec2(trs[0], trs[1]), trs[2], Vec2(trs[3], trs[4]));
	glm::mat4 glmResult = glm::translate(glm::vec3(trs[0], trs[1], 0.f)) * glm::rotate(trs[2], glm::vec3(0.f, 0.f, 1.f)) * glm::scale(glm::vec3(trs[3], trs[4], 1.f));
	CompareMat3WithGlmMat4(ourResult, glmResult);
}
//--Operators----------------------------------
TEST(CLASS_NAME, Add2Mat3)
{
	Mat3 ourResult, ourResult2;
	glm::mat3 glmResult, glmResult2;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			ourResult.data_3_3[i][j] = glmResult[i][j] = RAND_FLOAT;
			ourResult2.data_3_3[i][j] = glmResult2[i][j] = RAND_FLOAT;
		};
	ourResult = ourResult + ourResult2;
	glmResult = glmResult + glmResult2;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			EXPECT_NEAR(ourResult.data_3_3[i][j], glmResult[i][j], TOLERANCE);

	//Try the += as Well
	ourResult += ourResult2;
	glmResult += glmResult2;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			EXPECT_NEAR(ourResult.data_3_3[i][j], glmResult[i][j], TOLERANCE);
}
TEST(CLASS_NAME, Substract2Mat3)
{
	Mat3 ourResult, ourResult2;
	glm::mat3 glmResult, glmResult2;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			ourResult.data_3_3[i][j] = glmResult[i][j] = RAND_FLOAT;
			ourResult2.data_3_3[i][j] = glmResult2[i][j] = RAND_FLOAT;
		};
	ourResult = ourResult - ourResult2;
	glmResult = glmResult - glmResult2;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			EXPECT_NEAR(ourResult.data_3_3[i][j], glmResult[i][j], TOLERANCE);

	//Try the -= as Well
	ourResult -= ourResult2;
	glmResult -= glmResult2;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			EXPECT_NEAR(ourResult.data_3_3[i][j], glmResult[i][j], TOLERANCE);
}
TEST(CLASS_NAME, Multiply2Mat3)
{
	Mat3 ourResult, ourResult2;
	glm::mat3 glmResult, glmResult2;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			ourResult.data_3_3[i][j] = glmResult[i][j] = RAND_FLOAT;
			ourResult2.data_3_3[i][j] = glmResult2[i][j] = RAND_FLOAT;
		};
	ourResult = ourResult * ourResult2;
	glmResult = glmResult * glmResult2;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			EXPECT_NEAR(ourResult.data_3_3[i][j], glmResult[i][j], TOLERANCE);

	//Try the *= as Well
	ourResult *= ourResult2;
	glmResult *= glmResult2;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			EXPECT_NEAR(ourResult.data_3_3[i][j], glmResult[i][j], TOLERANCE);
}
TEST(CLASS_NAME, MultiplyMat3byScalar)
{
	float rdFloat = RAND_FLOAT;
	Mat3 ourResult;
	glm::mat3 glmResult;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			ourResult.data_3_3[i][j] = glmResult[i][j] = RAND_FLOAT;
	//TODO: FIX IMPLICIT CAST
     //Implicit conversion issues
	//ourResult = ourResult * rdFloat;
//	glmResult = glmResult * rdFloat;

//	for (int i = 0; i < 3; i++)
//		for (int j = 0; j < 3; j++)
//			EXPECT_NEAR(ourResult.data_3_3[i][j], glmResult[i][j], TOLERANCE);

	//Try the *= as Well
	ourResult *= rdFloat;
	glmResult *= rdFloat;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			EXPECT_NEAR(ourResult.data_3_3[i][j], glmResult[i][j], TOLERANCE);
}
TEST(CLASS_NAME, DivideMat3byScalar)
{
	float rdFloat;
	do{
		rdFloat = RAND_FLOAT;
	} while (!rdFloat); //never divide by 0	

	Mat3 ourResult;
	glm::mat3 glmResult;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			ourResult.data_3_3[i][j] = glmResult[i][j] = RAND_FLOAT;
	ourResult = ourResult / rdFloat;
	glmResult = glmResult / rdFloat;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			EXPECT_NEAR(ourResult.data_3_3[i][j], glmResult[i][j], TOLERANCE);

	//Try the /= as Well
	ourResult /= rdFloat;
	glmResult /= rdFloat;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			EXPECT_NEAR(ourResult.data_3_3[i][j], glmResult[i][j], TOLERANCE);
}

#pragma endregion
//--MAT-4-------------------------------------------
#pragma region Mat4
#define CLASS_NAME Mat4

TEST(CLASS_NAME, Debug_Test)
{
	EXPECT_TRUE(true);
}

#pragma endregion
//--QUAT--------------------------------------------
#pragma region Quat
#define CLASS_NAME Quat

TEST(CLASS_NAME, Debug_Test)
{
	EXPECT_TRUE(true);
}

#pragma endregion
//--------------------------------------------------