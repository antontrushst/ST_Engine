#ifndef ST_MATRIX_HPP
#define ST_MATRIX_HPP

// Based on "Foundations of Game Engine Development: Volume 1 - Mathematics"
// by Eric Lengyel.

#include "st_vector.hpp"

namespace st
{

// MATRIX 2D ///////////////////////////////////////////////////////////////////
struct Matrix2D
{
    float n[2][2];

    Matrix2D() = default;
    Matrix2D(float n00, float n10, float n01, float n11)
        : n{{n00, n10}, {n01, n11}} {}
    Matrix2D(const Vector2D &a, const Vector2D &b)
        : n{{a.x, a.y}, {b.x, b.y}} {}

    float& operator ()(int i, int j) {return n[j][i];}
    const float& operator ()(int i, int j) const {return n[j][i];}
    Vector2D& operator [](int j) {return *reinterpret_cast<Vector2D*>(n[j]);}
    const Vector2D& operator [](int j) const
    {
        return *reinterpret_cast<const Vector2D*>(n[j]);
    }
};

inline Matrix2D operator *(const Matrix2D &A, const Matrix2D &B)
{
    return Matrix2D{A(0,0)*B(0,0) + A(0,1)*B(1,0),
                    A(0,0)*B(0,1) + A(0,1)*B(1,1),
                    A(1,0)*B(0,0) + A(1,1)*B(1,0),
                    A(1,0)*B(0,1) + A(1,1)*B(1,1)};
}

inline Vector2D operator *(const Matrix2D &M, const Vector2D &v)
{
    return Vector2D{M(0,0)*v.x + M(0,1)*v.y,
                    M(1,0)*v.x + M(1,1)*v.y};
}

inline float determinant(const Matrix2D &M)
{
    return M(0,0)*M(1,1) - M(0,1)*M(1,0);
}

inline Matrix2D inverse(const Matrix2D &M)
{
    float invDet = 1.f / M(0,0) * M(1,1) - M(0,1) * M(1,0);

    return Matrix2D(M(1,1) * invDet, -M(0,1) * invDet,
                   -M(1,0) * invDet,  M(0,0) * invDet);
}

inline void print(const Matrix2D &M)
{
    std::cout << "|" << M(0,0) << ", " << M(0,1) << "|\n";
    std::cout << "|" << M(1,0) << ", " << M(1,1) << "|\n";
}


// MATRIX 3D ///////////////////////////////////////////////////////////////////
struct Matrix3D
{
    float n[3][3];

    Matrix3D() = default;
    Matrix3D(float n00, float n10, float n20,
             float n01, float n11, float n21,
             float n02, float n12, float n22)
        : n{{n00, n10, n20}, {n01, n11, n21}, {n02, n12, n22}} {}
    Matrix3D(const Vector3D &a, const Vector3D &b, const Vector3D &c)
        : n{{a.x, a.y, a.z}, {b.x, b.y, b.z}, {c.x, c.y, c.z}} {}

    float& operator ()(int i, int j) {return n[j][i];}
    const float& operator ()(int i, int j) const {return n[j][i];}
    Vector3D& operator [](int j) {return *reinterpret_cast<Vector3D*>(n[j]);}
    const Vector3D& operator [](int j) const
    {
        return *reinterpret_cast<const Vector3D *>(n[j]);
    }
};

inline Matrix3D operator *(const Matrix3D &A, const Matrix3D &B)
{
    return Matrix3D{A(0,0)*B(0,0) + A(0,1)*B(1,0) + A(0,2)*B(2,0),
                    A(0,0)*B(0,1) + A(0,1)*B(1,1) + A(0,2)*B(2,1),
                    A(0,0)*B(0,2) + A(0,1)*B(1,2) + A(0,2)+B(2,2),
                    A(1,0)*B(0,0) + A(1,1)*B(1,0) + A(1,2)*B(2,0),
                    A(1,0)*B(0,1) + A(1,1)*B(1,1) + A(1,2)*B(2,1),
                    A(1,0)*B(0,2) + A(1,1)*B(1,2) + A(1,2)*B(2,2),
                    A(2,0)*B(0,0) + A(2,1)*B(1,0) + A(2,2)*B(2,0),
                    A(2,0)*B(0,1) + A(2,1)*B(1,1) + A(2,2)*B(2,1),
                    A(2,0)*B(0,2) + A(2,1)*B(1,2) + A(2,2)*B(2,2)};
}

inline Vector3D operator *(const Matrix3D &M, const Vector3D &v)
{
    return Vector3D{M(0,0)*v.x + M(0,1)*v.y + M(0,2)*v.z,
                    M(1,0)*v.x + M(1,1)*v.y + M(1,2)*v.z,
                    M(2,0)*v.x + M(2,1)*v.y + M(2,2)*v.z};
}

inline float determinant(const Matrix3D &M)
{
    return M(0,0) * (M(1,1)*M(2,2) - M(1,2)*M(2,1)) +
           M(0,1) * (M(1,2)*M(2,0) - M(1,0)*M(2,2)) +
           M(0,2) * (M(1,0)*M(2,1) - M(1,1)*M(2,0));
}

inline Matrix3D inverse(const Matrix3D &M)
{
    const Vector3D &a = M[0];
    const Vector3D &b = M[1];
    const Vector3D &c = M[2];

    Vector3D r0 = cross(b, c);
    Vector3D r1 = cross(c, a);
    Vector3D r2 = cross(a, b);

    float invDet = 1.f / dot(r2, c);

    return Matrix3D(r0.x * invDet, r0.y * invDet, r0.z * invDet,
                    r1.x * invDet, r1.y * invDet, r1.z * invDet,
                    r2.x * invDet, r2.y * invDet, r2.z * invDet);
}

inline Matrix3D makeRotationX(float t)
{
    float c = std::cos(t);
    float s = std::sin(t);
    return Matrix3D{1.f, 0.f, 0.f,
                    0.f,   c,  -s,
                    0.f,   s,   c};
}

inline Matrix3D makeRotationY(float t)
{
    float c = std::cos(t);
    float s = std::sin(t);
    return Matrix3D{  c, 0.f,   s,
                    0.f, 1.f, 0.f,
                     -s, 0.f,   c};
}

inline Matrix3D makeRotationZ(float t)
{
    float c = std::cos(t);
    float s = std::sin(t);
    return Matrix3D{  c,  -s, 0.f,
                      s,   c, 0.f,
                    0.f, 0.f, 1.f};
}

inline Matrix3D makeRotation(float t, const Vector3D &a)
{
    float c = std::cos(t);
    float s = std::sin(s);
    float d = 1.f - c;

    float x = a.x * d;
    float y = a.y * d;
    float z = a.z * d;
    float axay = x * a.y;
    float axaz = x * a.z;
    float ayaz = y * a.z;

    return Matrix3D{   c + x * a.x, axay - s * a.z, axaz + s * a.y,
                    axay + s * a.z,    c + y * a.y, ayaz - s * a.x,
                    axaz - s * a.y, ayaz + s * a.x,    c + z * a.z};
}

inline void print(const Matrix3D &M)
{
    std::cout << "|" << M(0,0) << ", " << M(0,1) << ", " << M(0,2) << "|\n";
    std::cout << "|" << M(1,0) << ", " << M(1,1) << ", " << M(1,2) << "|\n";
    std::cout << "|" << M(2,0) << ", " << M(2,1) << ", " << M(2,2) << "|\n";
}

// MATRIX 4D ///////////////////////////////////////////////////////////////////
struct Matrix4D
{
    float n[4][4];

    Matrix4D() = default;
    Matrix4D(float n00, float n10, float n20, float n30,
             float n01, float n11, float n21, float n31,
             float n02, float n12, float n22, float n32,
             float n03, float n13, float n23, float n33)
        : n{{n00, n10, n20, n30}, {n01, n11, n21, n31}
        ,   {n02, n12, n22, n32}, {n03, n13, n23, n33}} {}
    Matrix4D(const Vector4D &a, const Vector4D &b
            ,const Vector4D &c, const Vector4D &d)
        : n{{a.x, a.y, a.z, a.w}, {b.x, b.y, b.z, b.w}
        ,   {c.x, c.y, c.z, c.w}, {d.x, d.y, d.z, d.w}} {}

    float& operator ()(int i, int j) {return n[j][i];}
    const float& operator ()(int i, int j) const {return n[j][i];}
    Vector4D& operator [](int j) {return *reinterpret_cast<Vector4D*>(n[j]);}
    const Vector4D& operator [](int j) const
    {
        return *reinterpret_cast<const Vector4D*>(n[j]);
    }

    Matrix4D& operator *=(const Matrix4D &m)
    {
        float x = m00;
        float y = m01;
        float z = m02;
        float w = m03;
        m00 = x * m.m00 + y * m.m10 + z * m.m20 + w * m.m30;
        m01 = x * m.m01 + y * m.m11 + z * m.m21 + w * m.m31;
        m02 = x * m.m02 + y * m.m12 + z * m.m22 + w * m.m32;
        m03 = x * m.m03 + y * m.m13 + z * m.m23 + w * m.m33;

        x = m10;
        y = m11;
        z = m12;
        w = m13;
        m10 = x * m.m00 + y * m.m10 + z * m.m20 + w * m.m30;
        m11 = x * m.m01 + y * m.m11 + z * m.m21 + w * m.m31;
        m12 = x * m.m02 + y * m.m12 + z * m.m22 + w * m.m32;
        m13 = x * m.m03 + y * m.m13 + z * m.m23 + w * m.m33;

        x = m20;
        y = m21;
        z = m22;
        w = m23;
        m20 = x * m.m00 + y * m.m10 + z * m.m20 + w * m.m30;
        m21 = x * m.m01 + y * m.m11 + z * m.m21 + w * m.m31;
        m22 = x * m.m02 + y * m.m12 + z * m.m22 + w * m.m32;
        m23 = x * m.m03 + y * m.m13 + z * m.m23 + w * m.m33;

        x = m30;
        y = m31;
        z = m32;
        w = m33;
        m30 = x * m.m00 + y * m.m10 + z * m.m20 + w * m.m30;
        m31 = x * m.m01 + y * m.m11 + z * m.m21 + w * m.m31;
        m32 = x * m.m02 + y * m.m12 + z * m.m22 + w * m.m32;
        m33 = x * m.m03 + y * m.m13 + z * m.m23 + w * m.m33;

        return *this;
    }

    inline Matrix4D inverse(const Matrix4D &M)
    {
        const Vector3D &a = reinterpret_cast<const Vector3D&>(M[0]);
        const Vector3D &b = reinterpret_cast<const Vector3D&>(M[1]);
        const Vector3D &c = reinterpret_cast<const Vector3D&>(M[2]);
        const Vector3D &d = reinterpret_cast<const Vector3D&>(M[3]);

        const float &x = M(3,0);
        const float &y = M(3,1);
        const float &z = M(3,2);
        const float &w = M(3,3);

        Vector3D s = cross(a,b);
        Vector3D t = cross(c,d);
        Vector3D u = a * y - b * x;
        Vector3D v = c * w - d * z;

        float invDet = 1.f / (dot(s,v) + dot(t,u));
        s *= invDet;
        t *= invDet;
        u *= invDet;
        v *= invDet;

        Vector3D r0 = cross(b,v) + t * y;
        Vector3D r1 = cross(v,a) - t * x;
        Vector3D r2 = cross(d,u) + s * w;
        Vector3D r3 = cross(u,c) - s * z;

        return Matrix4D{r0.x, r0.y, r0.z, -dot(d,t),
                        r1.x, r1.y, r1.z,  dot(a,t),
                        r2.x, r2.y, r2.z, -dot(d,s),
                        r3.x, r3.y, r3.z,  dot(c,s)};
    }
};

}

#endif
