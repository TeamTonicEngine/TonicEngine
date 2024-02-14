#include "pch.hpp"

//Tests based on glm library
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp >

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

	ourResult = ourResult * rdFloat;
	glmResult = glmResult * rdFloat;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			EXPECT_NEAR(ourResult.data_3_3[i][j], glmResult[i][j], TOLERANCE);

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

TEST(CLASS_NAME, DefaultConstructor)
{
    Mat4 matM; glm::mat4 matG(0);

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, FloatArrayConstructor)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    float values[16] = { _1, _2, _3, _4,
                         _5, _6, _7, _8,
                         _9, _10,_11,_12,
                         _13,_14,_15,_16 };

    Mat4 matM(values);  glm::mat4 matG(_1, _2, _3, _4,
                                    _5, _6, _7, _8,
                                    _9, _10, _11, _12,
                                    _13, _14, _15, _16);
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, IdentityValueConstructor)
{
    Mat4 matM(1);  glm::mat4 matG(1);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, 16FloatConstructor)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matM(_1, _2, _3, _4,
              _5, _6, _7, _8,
              _9, _10, _11, _12,
              _13, _14, _15, _16);

    glm::mat4 matG(_1, _2, _3, _4,
                   _5, _6, _7, _8,
                   _9, _10, _11, _12,
                   _13, _14, _15, _16);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, EqualityBetweenUnionMember)//isnt usefull but is testing is the union is working
{
    Mat4 matM(RAND_FLOAT, RAND_FLOAT, RAND_FLOAT, RAND_FLOAT,
              RAND_FLOAT, RAND_FLOAT, RAND_FLOAT, RAND_FLOAT,
              RAND_FLOAT, RAND_FLOAT, RAND_FLOAT, RAND_FLOAT,
              RAND_FLOAT, RAND_FLOAT, RAND_FLOAT, RAND_FLOAT);

    for (int i = 0; i < 4; i++)
    {

        for (int j = 0; j < 4; j++)
        {

            EXPECT_EQ(matM.data_4_4[i][j], matM.data4V[i].xyzw[j]);
            EXPECT_EQ(matM.data[i * 4 + j], matM.data_4_4[i][j]);
            EXPECT_EQ(matM.data[i * 4 + j], matM.data4V[i].xyzw[j]);

        }

    }
}
TEST(CLASS_NAME, Transform)
{
    float trX = RAND_FLOAT;
    float trY = RAND_FLOAT;
    float trZ = RAND_FLOAT;

    float rtX = RAND_FLOAT;
    float rtY = RAND_FLOAT;
    float rtZ = RAND_FLOAT;

    float scX = RAND_FLOAT;
    float scY = RAND_FLOAT;
    float scZ = RAND_FLOAT;

    Mat4 matM = Mat4::Transform({ trX , trY, trZ }, { rtX ,rtY, rtZ }, { scX, scY, scZ });
    glm::mat4 matG = glm::translate(glm::vec3(trX, trY, trZ)) *
        glm::rotate(rtX,glm::vec3(1 ,0, 0 ))*
        glm::rotate(rtY,glm::vec3(0, 1, 0))*
        glm::rotate(rtZ,glm::vec3(0, 0, 1))*
        glm::scale(glm::vec3(scX, scY, scZ ));


    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, Rotate)
{
    float rtX = RAND_FLOAT;
    float rtY = RAND_FLOAT;
    float rtZ = RAND_FLOAT;

    Mat4 matM = Mat4::Rotate({ rtX ,rtY, rtZ });
    glm::mat4 matG = glm::rotate(rtX, glm::vec3(1, 0, 0)) *
        glm::rotate(rtY, glm::vec3(0, 1, 0)) *
        glm::rotate(rtZ, glm::vec3(0, 0, 1));

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, RotateX)
{
    float rtX = RAND_FLOAT;

    Mat4 matM = Mat4::RotateX(rtX);
    glm::mat4 matG = glm::rotate(rtX, glm::vec3(1, 0, 0));

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, RotateY)
{
    float rtY = RAND_FLOAT;

    Mat4 matM = Mat4::RotateY(rtY);
    glm::mat4 matG = glm::rotate(rtY, glm::vec3(0, 1, 0));

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, RotateZ)
{
    float rtZ = RAND_FLOAT;

    Mat4 matM = Mat4::RotateZ(rtZ);
    glm::mat4 matG = glm::rotate(rtZ, glm::vec3(0, 0, 1));

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, Translate)
{
    float trX = RAND_FLOAT;
    float trY = RAND_FLOAT;
    float trZ = RAND_FLOAT;

    Mat4 matM = Mat4::Translate({ trX , trY, trZ });
    glm::mat4 matG = glm::translate(glm::vec3(trX, trY, trZ));

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, Scale)
{
    float scX = RAND_FLOAT;
    float scY = RAND_FLOAT;
    float scZ = RAND_FLOAT;

    Mat4 matM = Mat4::Scale({ scX, scY, scZ });
    glm::mat4 matG = glm::scale(glm::vec3(scX, scY, scZ));

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, Determinant)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matM(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    glm::mat4 matG(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    EXPECT_EQ(matM.Determinant(), glm::determinant(matG));
}
TEST(CLASS_NAME, Trace)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matM(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    glm::mat4 matG(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);
    EXPECT_EQ(matM.Trace(),(matG[0][0]+ matG[1][1]+ matG[2][2]+ matG[3][3]));
}



TEST(CLASS_NAME, Inverse)
{
    
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matM(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    glm::mat4 matG(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    matM.Inverse();
    matG = glm::inverse(matG);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_NEAR(matM[i][j], matG[i][j], 5e-5);
}



TEST(CLASS_NAME, Transpose)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matM(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    glm::mat4 matG(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    matM.Transpose();
    matG = glm::transpose(matG);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, OperatorEqualFloatArray)
{
    float data[16] = { RAND_FLOAT,RAND_FLOAT,RAND_FLOAT,RAND_FLOAT,
        RAND_FLOAT,RAND_FLOAT,RAND_FLOAT,RAND_FLOAT,
        RAND_FLOAT,RAND_FLOAT,RAND_FLOAT,RAND_FLOAT,
        RAND_FLOAT,RAND_FLOAT,RAND_FLOAT,RAND_FLOAT };

    Mat4 matMA(data);
    Mat4 matMB;

    matMB = data;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matMA[i][j], matMB[i][j]);
}
TEST(CLASS_NAME, Negate)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matMA(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    Mat4 matMB(-matMA);
    

    for (int i = 0; i < 16; i++)
        EXPECT_EQ(-matMA.data[i], matMB.data[i]);
}
TEST(CLASS_NAME, IsEqual)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matMA(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    Mat4 matMB(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    Mat4 matMC(_2, _1, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    
    EXPECT_EQ(matMA == matMB, true);
    EXPECT_EQ(matMA == matMC, false);
}
TEST(CLASS_NAME, IsNotEqual)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matMA(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    Mat4 matMB(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    Mat4 matMC(_2, _1, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);


    EXPECT_EQ(matMA != matMB, false);
    EXPECT_EQ(matMA != matMC, true);
}
TEST(CLASS_NAME, ArrayOperator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matM(_1, _2, _3, _4,
        _5, _6, _7, _8,
        _9, _10, _11, _12,
        _13, _14, _15, _16);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matM.data_4_4[i][j]);
}

TEST(CLASS_NAME, Mat4PlusMat4Operator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matMA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    Mat4 matMB(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1);


    glm::mat4 matGA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    glm::mat4 matGB(_16, _15, _14, _13, _12, _11, _10, _9,  _8, _7, _6, _5, _4, _3, _2, _1);

    Mat4 matMR = matMA + matMB;
    glm::mat4 matGR = matGA + matGB;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matMR[i][j], matGR[i][j]);
}
TEST(CLASS_NAME, Mat4MinusMat4Operator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matMA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    Mat4 matMB(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1);


    glm::mat4 matGA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    glm::mat4 matGB(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1);

    Mat4 matMR = matMA - matMB;
    glm::mat4 matGR = matGA - matGB;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matMR[i][j], matGR[i][j]);
}
TEST(CLASS_NAME, Mat4TimeMat4Operator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matMA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    Mat4 matMB(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1);


    glm::mat4 matGA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    glm::mat4 matGB(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1);

    Mat4 matMR = matMA * matMB;
    glm::mat4 matGR = matGA * matGB;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matMR[i][j], matGR[i][j]);
}

TEST(CLASS_NAME, Mat4PlusEqualMat4Operator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matMA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    Mat4 matMB(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1);


    glm::mat4 matGA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    glm::mat4 matGB(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1);

    matMA += matMB;
    matGA += matGB;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matMA[i][j], matGA[i][j]);
}
TEST(CLASS_NAME, Mat4MinusEqualMat4Operator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matMA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    Mat4 matMB(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1);


    glm::mat4 matGA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    glm::mat4 matGB(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1);

    matMA -= matMB;
    matGA -= matGB;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matMA[i][j], matGA[i][j]);
}
TEST(CLASS_NAME, Mat4TimeEqualMat4Operator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    Mat4 matMA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    Mat4 matMB(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1);


    glm::mat4 matGA(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    glm::mat4 matGB(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1);

    matMA *= matMB;
    matGA *= matGB;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matMA[i][j], matGA[i][j]);
}

TEST(CLASS_NAME, Mat4TimeScaOperator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    float _sca = RAND_FLOAT;

    Mat4 matM(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);


    glm::mat4 matG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);

    Mat4 matMR = matM * _sca;
    glm::mat4 matGR = matG * _sca;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matMR[i][j], matGR[i][j]);
}
TEST(CLASS_NAME, Mat4DivScaOperator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    float _sca;
    do {
        _sca = RAND_FLOAT;
    } while (!_sca);

    Mat4 matM(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);


    glm::mat4 matG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);

    Mat4 matMR = matM / _sca;
    glm::mat4 matGR = matG / _sca;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matMR[i][j], matGR[i][j]);
}

TEST(CLASS_NAME, Mat4TimeEqualScaOperator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    float _sca = RAND_FLOAT;

    Mat4 matM(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);


    glm::mat4 matG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);

    matM *= _sca;
    matG *= _sca;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}
TEST(CLASS_NAME, Mat4DivEqualScaOperator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    float _sca;
    do {
        _sca = RAND_FLOAT;
    } while (!_sca);

    Mat4 matM(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);


    glm::mat4 matG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);

    matM /= _sca;
    matG /= _sca;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_EQ(matM[i][j], matG[i][j]);
}

TEST(CLASS_NAME, Mat4TimeVec3Operator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    float _x = RAND_FLOAT;
    float _y = RAND_FLOAT;
    float _z = RAND_FLOAT;

    Mat4 matM(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    Vec3 vecM(_x, _y, _z);


    glm::mat4 matG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    glm::vec4 vecG(_x, _y, _z, 1);

    Vec3 vecMR = matM * vecM;
    glm::vec3 vecGR = vecG * matG;

    for (int i = 0; i < 3; i++)
        EXPECT_EQ(vecMR[i], vecGR[i]);
}
TEST(CLASS_NAME, Mat4TimeVec4Operator)
{
    float _1 = RAND_FLOAT;
    float _2 = RAND_FLOAT;
    float _3 = RAND_FLOAT;
    float _4 = RAND_FLOAT;
    float _5 = RAND_FLOAT;
    float _6 = RAND_FLOAT;
    float _7 = RAND_FLOAT;
    float _8 = RAND_FLOAT;
    float _9 = RAND_FLOAT;
    float _10 = RAND_FLOAT;
    float _11 = RAND_FLOAT;
    float _12 = RAND_FLOAT;
    float _13 = RAND_FLOAT;
    float _14 = RAND_FLOAT;
    float _15 = RAND_FLOAT;
    float _16 = RAND_FLOAT;

    float _x = RAND_FLOAT;
    float _y = RAND_FLOAT;
    float _z = RAND_FLOAT;
    float _w = RAND_FLOAT;

    Mat4 matM(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    Vec4 vecM(_x, _y, _z, _w);


    glm::mat4 matG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16);
    glm::vec4 vecG(_x, _y, _z, _w);

    Vec4 vecMR = matM * vecM;
    glm::vec4 vecGR = vecG * matG;

    for (int i = 0; i < 4; i++)
        EXPECT_EQ(vecMR[i], vecGR[i]);
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