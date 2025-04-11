#pragma once
#include <math.h>
#include <string>
constexpr auto PI = 3.1415927f;


class Vector3f
{
public:

    float x;
    float y;
    float z;

    Vector3f() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3f(const float x, const float y, const float z) : x(x), y(y), z(z) {}
    Vector3f operator + (const Vector3f& rhs) const { return Vector3f(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vector3f operator - (const Vector3f& rhs) const { return Vector3f(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vector3f operator * (const float& rhs) const { return Vector3f(x * rhs, y * rhs, z * rhs); }
    Vector3f operator / (const float& rhs) const { return Vector3f(x / rhs, y / rhs, z / rhs); }
    Vector3f& operator += (const Vector3f& rhs) { return *this = *this + rhs; }
    Vector3f& operator += (const float rhs) { return *this = *this + Vector3f(rhs, rhs, rhs); }
    Vector3f& operator -= (const Vector3f& rhs) { return *this = *this - rhs; }
    Vector3f& operator *= (const float& rhs) { return *this = *this * rhs; }
    Vector3f& operator /= (const float& rhs) { return *this = *this / rhs; }
    bool operator == (const Vector3f& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    float Length() const { return sqrtf(x * x + y * y + z * z); } //AKA Magnitude
    Vector3f Normalize() const { return *this * (1 / Length()); } // 1/ Length() 
    float Distance(const Vector3f& rhs) const { return (*this - rhs).Length(); }
    Vector3f& abs() { x = fabs(x); y = fabs(y); z = fabs(z); return *this; }
    void NormalizeAngle(float yawMin = -90, float yawMax = 90, float pitchMin = 0, float pitchMax = 360);
    std::string ToString();

};

struct Vec4
{
    float x;
    float y;
    float z;
    float w;
};

using Vec3 = Vector3f;
using vec3 = Vector3f;
using Vec = Vector3f;
using vec = Vector3f;

Vec3 OpenGLWorldToScreen(Vec3& pos, const float matrix[16], int windowWidth, int windowHeight);
Vec3 DirectXWorldToScreen(Vec3& pos, const float matrix[16], int windowWidth, int windowHeight);
Vec3 OpenGLWorldToScreen(Vec3& pos);
Vec3 CalcAngle(const Vec3& origin, const Vec3& target, bool invertYaw = false, bool invertPitch = false);

Vec3 DegreesToRadians(const Vec3& vec);
float DegreesToRadians(float num);
Vec3 RadiansToDegrees(const Vec3& vec);
float RadiansToDegrees(float num);
