#ifndef ST_VECTOR_HPP
#define ST_VECTOR_HPP

#include <cmath>

namespace st
{
// VECTOR 2D /////////////////////////////////////////////////////////////////// 
struct Vector2D
{
    float x, y;

    Vector2D() = default;
    Vector2D(float a, float b)
        : x{a}, y{b}
    {}

    float&       operator [](int index) {return ((&x)[index]);}
    const float& operator [](int index) const {return ((&x)[index]);}
    Vector2D&    operator +=(const Vector2D &v)
    {
        x+=v.x; y+=v.y;
        return *this;
    }
    Vector2D&    operator -=(const Vector2D &v)
    {
        x-=v.x; y-=v.y;
        return *this;
    }
    Vector2D&    operator *=(float s)
    {
        x*=s; y*=s;
        return *this;
    }
    Vector2D&    operator /=(float s)
    {
        s = 1.0f/s;
        x*=s; y*=s;
        return *this;
    }
};

inline Vector2D operator +(const Vector2D &a, const Vector2D &b)
{
    return Vector2D{a.x + b.x, a.y + b.y};
}

inline Vector2D operator -(const Vector2D &a, const Vector2D &b)
{
    return Vector2D{a.x - b.x, a.y - b.y};
}

inline Vector2D operator *(const Vector2D &v, float s)
{
    return Vector2D{v.x * s, v.y * s};
}

inline Vector2D operator /(const Vector2D &v, float s)
{
    s = 1.0f / s;
    return Vector2D{v.x * s, v.y * s};
}

inline Vector2D operator -(const Vector2D &v)
{
    return Vector2D{-v.x, -v.y};
}

inline float magnitude(const Vector2D &v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

inline Vector2D normalize(const Vector2D &v)
{
    return (v / magnitude(v));
}

// VECTOR 3D ///////////////////////////////////////////////////////////////////
struct Vector3D
{
    float x, y, z;

    Vector3D() = default;
    Vector3D(float a, float b, float c)
        : x{a}, y{b}, z{c}
    {}

    float&       operator [](int index) {return ((&x)[index]);}
    const float& operator [](int index) const {return ((&x)[index]);}
    Vector3D&    operator +=(const Vector3D &v)
    {
        x+=v.x; y+=v.y; z+=v.z;
        return *this;
    }
    Vector3D&    operator -=(const Vector3D &v)
    {
        x-=v.x; y-=v.y; z-=v.z;
        return *this;
    }
    Vector3D&    operator *=(float s)
    {
        x*=s; y*=s; z*=s;
        return *this;
    }
    Vector3D&    operator /=(float s)
    {
        s = 1.0f/s;
        x*=s; y*=s; z*=s;
        return *this;
    }
};

#ifdef SFML_GRAPHICS_HPP
// SFML specific vectors
#endif

inline Vector3D operator +(const Vector3D &a, const Vector3D &b)
{
    return Vector3D{a.x + b.x, a.y + b.y, a.z + b.z};
}

inline Vector3D operator -(const Vector3D &a, const Vector3D &b)
{
    return Vector3D{a.x - b.x, a.y - b.y, a.z - b.z};
}

inline Vector3D operator *(const Vector3D &v, float s)
{
    return Vector3D{v.x * s, v.y * s, v.z * s};
}

inline Vector3D operator /(const Vector3D &v, float s)
{
    s = 1.0f / s;
    return Vector3D{v.x * s, v.y * s, v.z * s};
}

inline Vector3D operator -(const Vector3D &v)
{
    return Vector3D{-v.x, -v.y, -v.z};
}

inline float magnitude(const Vector3D &v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline Vector3D normalize(const Vector3D &v)
{
    return (v / magnitude(v));
}

}
#endif
