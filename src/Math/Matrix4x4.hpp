#pragma once
#include <array>
#include "Point3.hpp"
#include "Vec3.hpp"

template< typename T >
class Matrix4x4
{
private:
    std::array<std::array<T, 4>, 4> m;
public:

    Matrix4x4
    (T a00, T a01, T a02, T a03,
     T a10, T a11, T a12, T a13,
     T a20, T a21, T a22, T a23,
     T a30, T a31, T a32, T a33)
        : m({a00, a01, a02, a03,
             a10, a11, a12, a13,
             a20, a21, a22, a23,
             a30, a31, a32, a33}) {}

    Matrix4x4() {}

    T& operator()(int i, int j) { return m[i][j]; }
    const T& operator()(int i, int j) const { return m[i][j]; }

    explicit Matrix4x4(const std::array<std::array<float, 4>, 4>& _m) : m(_m) {}

    explicit Matrix4x4(T val)
    {
        for (auto& subArray : m)
            for (auto& value : subArray)
                value = val;
    }

    Matrix4x4(const Matrix4x4& mtx) : m(mtx.m) { }

    Matrix4x4 Transpose() const
    {
        Matrix4x4 tmp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tmp.m[i][j] = m[j][i];

        return tmp;
    }

    Matrix4x4 getIdentity() const
    {
        return Matrix4x4{{ 1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              0, 0, 0, 1 }};
    }

    void setIdentity()
    {
        constexpr auto identity = std::array<std::array<T, 4>, 4>
        { 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1 };

        m = identity;
    }

    Matrix4x4 Invert() const
    {
        //static_assert(N == M, "Matrix is not square!");

        Matrix4x4 tmp = *this;
        auto invTmp = tmp.getIdentity();

        // Switch rows around so there is a leading number in each
        // Set the diagonal to 1 by dividing the row with the leading element
        // Reduce upper right-hand entries to zero by subtracting fractions of below rows
        // This is all done in parallel to the adjoined identity matrix

        for (int i = 0; i < 4; ++i)
        {
            if (tmp.m[i][i] == 0) // Our row had a leading zero
            {
                auto big = i;
                for (int j = 0; j < 4; ++j) // Check remaining rows
                {
                    if (std::abs(tmp.m[j][i]) > std::abs(tmp.m[big][i]))
                    {
                        big = j; // Another row has a (absolute) larger leading value
                    }
                }

                if (big != i)  // The matrix wasn't singular, i.e. another row could be switched in
                {
                    for (int k = 0; k < 4; ++k)
                    {
                        std::swap(tmp.m[i][k], tmp.m[big][k]); // swap rows
                        std::swap(invTmp.m[i][k], invTmp.m[big][k]);
                    }
                }
            }

            auto divisor = tmp.m[i][i];
            for (int j = 0; j < 4; ++j)
            {
                tmp.m[i][j] /= divisor;
                invTmp.m[i][j] /= divisor;
            }

            for (int j = 0; j < 4; ++j)
            {
                if (j != i && tmp.m[i][i] != 0)
                {
                    auto coefficient = tmp.m[j][i] / tmp.m[i][i];
                    if (coefficient != 0)
                    {
                        for (int k = 0; k < 4; ++k)
                        {
                            tmp.m[j][k] -= coefficient * tmp.m[i][k];
                            tmp.m[j][k] = 0;
                            invTmp.m[j][k] -= coefficient * invTmp.m[i][k];
                        }

                    }
                }
            }

        }

        return invTmp;
    }


    static void Multiply(const Matrix4x4& a, const Matrix4x4& b, Matrix4x4& c)
    {
        // rolled up version rather than writing out the arguments
        for (uint8_t i = 0; i < 4; ++i) {
            for (uint8_t j = 0; j < 4; ++j) {
                c.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] +
                    a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
            }
        }
    }

    Matrix4x4 operator*(const Matrix4x4& other) const
    {
        Matrix4x4 tmp;
        Multiply(*this, other, tmp);
        return tmp;
    }

    Point3f Multiply(const Point3f& point) const
    {
        //using Matrix<4, 4, T>::m; // tell the compiler that we are referring to a member of the class
                                  // alternatively use this->m
        auto a = point[0] * m[0][0] + point[1] * m[1][0] + point[2] * m[2][0] + m[3][0];
        auto b = point[0] * m[0][1] + point[1] * m[1][1] + point[2] * m[2][1] + m[3][1];
        auto c = point[0] * m[0][2] + point[1] * m[1][2] + point[2] * m[2][2] + m[3][2];
        auto w = point[0] * m[0][3] + point[1] * m[1][3] + point[2] * m[2][3] + m[3][3];

        return {a / w, b / w, c / w};
    }

    Vec3f Multiply(const Vec3f& vec) const
    {
        auto a = vec[0] * m[0][0] + vec[1] * m[1][0] + vec[2] * m[2][0] + m[3][0];
        auto b = vec[0] * m[0][1] + vec[1] * m[1][1] + vec[2] * m[2][1] + m[3][1];
        auto c = vec[0] * m[0][2] + vec[1] * m[1][2] + vec[2] * m[2][2] + m[3][2];
        auto w = vec[0] * m[0][3] + vec[1] * m[1][3] + vec[2] * m[2][3] + m[3][3];

        return {a, b, c};
    }

    void PrettyPrint() const
    {
        std::cout << "| " << std::setw(4) << m[0][0] << ", " << std::setw(4) << m[0][1] << ", " << std::setw(4) << m[0][2] << ", " << std::setw(4) << m[0][3] << std::setw(4) << " |\n";
        std::cout << "| " << std::setw(4) << m[1][0] << ", " << std::setw(4) << m[1][1] << ", " << std::setw(4) << m[1][2] << ", " << std::setw(4) << m[1][3] << std::setw(4) << " |\n";
        std::cout << "| " << std::setw(4) << m[2][0] << ", " << std::setw(4) << m[2][1] << ", " << std::setw(4) << m[2][2] << ", " << std::setw(4) << m[2][3] << std::setw(4) << " |\n";
        std::cout << "| " << std::setw(4) << m[3][0] << ", " << std::setw(4) << m[3][1] << ", " << std::setw(4) << m[3][2] << ", " << std::setw(4) << m[3][3] << std::setw(4) << " |\n";
    }
};
typedef Matrix4x4<float> Matrix4x4f;