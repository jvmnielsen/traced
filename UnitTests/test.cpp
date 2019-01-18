#include "pch.h"
#include "../src/math/math_util.hpp"
#include "../src/math/vec3.hpp"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(ClampTest, ValueAbove) {
    ASSERT_EQ(Math::Clamp(0, 255, 600), 255);
}

TEST(ClampTest, ValueBelow) {
    ASSERT_EQ(Math::Clamp(30, 600, -20), 30);
}

TEST(ClampTest, ValueInRange) {
    ASSERT_EQ(Math::Clamp(1, 40, 25), 25);
}

TEST(ClampTest, AllNegative) {
    ASSERT_EQ(Math::Clamp(-41, -2, 25), -2);
}

TEST(DegreeToRadianTest, RightAngle) {
    ASSERT_NEAR(Math::DegreeToRadian(90), 1.5708, Math::Epsilon);
}

TEST(DegreeToRadianTest, ObtuseAngle) {
    ASSERT_NEAR(Math::DegreeToRadian(130), 2.26893, Math::Epsilon);
}

TEST(QuadraticSolverTest, Integer) {

    const int a = 2;
    const int b = 5;
    const int c = -3;

    float solutionOne;
    float solutiontwo;

    Math::SolveQuadratic(a, b, c, solutionOne, solutiontwo);

    ASSERT_NEAR(solutionOne, -3, Math::Epsilon);
    ASSERT_NEAR(solutiontwo, 0.5, Math::Epsilon);
}

TEST(QuadraticSolverTest, Float) {

    const auto a = -2.34f;
    const auto b = 22.3f;
    const auto c = -11.933f;

    float solutionOne;
    float solutiontwo;

    Math::SolveQuadratic(a, b, c, solutionOne, solutiontwo);

    ASSERT_NEAR(solutionOne, 0.56909679, Math::Epsilon);
    ASSERT_NEAR(solutiontwo, 8.9608177, Math::Epsilon);
}


TEST(Vec3Test, Equal) {

    Vec3f v1{0.2f, 12.339f, 7.3392f};
    Vec3f v2{0.2f, 12.339f, 7.3392f};

    ASSERT_EQ(v1, v2);
}

TEST(Vec3Test, Length) {

    Vec3f vec{0.2f, 12.339f, 7.3392f};
    
    ASSERT_NEAR(vec.LengthSquared(), 206.15477764, Math::Epsilon);

    ASSERT_NEAR(vec.Length(), 14.3580910166, Math::Epsilon);
}


TEST(Vec3Test, ScalingByConstant) {

    Vec3f v1{0.2f, -12.339f, 7.3392f};
    auto v2 = 2.3f * v1;

    v1 *= 2.3f;

    ASSERT_EQ(v1, Vec3f(0.46f, -28.3797, 16.88016f));
    ASSERT_EQ(v2, Vec3f(0.46f, -28.3797, 16.88016f));

    v1 /= 2.3f;
    v2 = v2 / 2.3f;

    ASSERT_EQ(v1, Vec3f(0.2f, -12.339f, 7.3392f));
    ASSERT_EQ(v1, Vec3f(0.2f, -12.339f, 7.3392f));
}

// write custom float logic for checking vecs
/*
TEST(Vec3Test, VectorAddition) {

    Vec3f v1{2.5f, 9.1f, 2.0f};
    Vec3f v2{0.5f, 4.3f, 3.1f};
    auto v3 = v1 + v2;
    auto v4 = v1 - v2;

    ASSERT_EQ(v3, Vec3f(3.0f, 13.4f, 5.1f));
    ASSERT_EQ(v4, Vec3f(2.0f, 4.8f, -1.1f));

    v1 += v2;
    v2 -= v1;

    ASSERT_EQ(v1, Vec3f(3.0f, 13.4f, 5.1f));
    ASSERT_EQ(v2, Vec3f(2.0f, 4.8f, -1.1f));
}
*/