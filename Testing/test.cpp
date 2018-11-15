#include "pch.h"



//constexpr static float Epsilon

TEST(ClampTest, ValueAbove) {
  ASSERT_EQ(Math::Clamp(0 , 255, 600), 255);
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

    ASSERT_NEAR(solutionOne, 0.5, Math::Epsilon);
    ASSERT_NEAR(solutiontwo, -3, Math::Epsilon);
}