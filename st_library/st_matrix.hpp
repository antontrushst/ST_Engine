#ifndef ST_MATRIX_HPP
#define ST_MATRIX_HPP

// Based on "Foundations of Game Engine Development: Volume 1 - Mathematics"
// by Eric Lengyel.

namespace st
{

struct Matrix3D
{
private:
    float n[3][3];
public:
    Matrix3D() = default;
    Matrix3D(float n00, float n10, float n20,
             float n01, float n11, float n21,
             float n02, float n12, float n22)
//        : n[0][0]{n00}, n[0][1]{n10}, n[0][2]{n20}
//        , n[1][0]{n01}, n[1][1]{n11}, n[1][2]{n21}
//        , n[2][0]{n02}, n[2][1]{n12}, n[2][2]{n22} {}
        : n{{n00, n10, n20}, {n01, n11, n21}, {n02, n12, n22}} {}
    Matrix3D(const Vector3D &a, const Vector3D &b, const Vector3D &c)
//        : n[0][0]{a.x}, n[0][1]{a.y}, n[0][2]{a.z}
//        , n[1][0]{b.x}, n[1][1]{b.y}, n[1][2]{b.z}
//        , n[2][0]{c.x}, n[2][1]{c.y}, n[2][2]{c.z} {}
        : n{{a.x, a.y, a.z}, {b.x, b.y, b.z}, {c.x, c.y, c.z}} {}

    float& operator ()(int i, int j) {return n[j][i];}
    const float& operator ()(int i, int j) const {return n[j][i];}
    Vector3D& operator [](int j) {return *reinterpret_cast<Vector3D *>(n[j]);}
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

inline void print(const Matrix3D &M)
{
    std::cout << "|" << M(0,0) << ", " << M(0,1) << ", " << M(0,2) << "|\n";
    std::cout << "|" << M(1,0) << ", " << M(1,1) << ", " << M(1,2) << "|\n";
    std::cout << "|" << M(2,0) << ", " << M(2,1) << ", " << M(2,2) << "|\n";
}

}

#endif
