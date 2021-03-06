﻿#include "Vector4DI.h"

namespace altseed {
Vector4DI::Vector4DI() : X(0), Y(0), Z(0), W(0) {}

Vector4DI::Vector4DI(int32_t x, int32_t y, int32_t z, int32_t w) : X(x), Y(y), Z(z), W(w) {}

bool Vector4DI::operator==(const Vector4DI& o) { return X == o.X && Y == o.Y && Z == o.Z && W == o.W; }

bool Vector4DI::operator!=(const Vector4DI& o) { return !(X == o.X && Y == o.Y && Z == o.Z && W == o.W); }

Vector4DI Vector4DI::operator-() { return Vector4DI(-X, -Y, -Z, -W); }

Vector4DI Vector4DI::operator+(const Vector4DI& o) const { return Vector4DI(X + o.X, Y + o.Y, Z + o.Z, W + o.W); }

Vector4DI Vector4DI::operator-(const Vector4DI& o) const { return Vector4DI(X - o.X, Y - o.Y, Z - o.Z, W - o.W); }

Vector4DI Vector4DI::operator*(const Vector4DI& o) const { return Vector4DI(X * o.X, Y * o.Y, Z * o.Z, W * o.W); }

Vector4DI Vector4DI::operator/(const Vector4DI& o) const { return Vector4DI(X / o.X, Y / o.Y, Z / o.Z, W / o.W); }

Vector4DI Vector4DI::operator*(const int32_t& o) const { return Vector4DI(X * o, Y * o, Z * o, W * o); }

Vector4DI Vector4DI::operator/(const int32_t& o) const { return Vector4DI(X / o, Y / o, Z * o, W * o); }

int32_t Vector4DI::Dot(const Vector4DI& v1, const Vector4DI& v2) { return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z + v1.W * v2.W; }

float Vector4DI::Distance(const Vector4DI& v1, const Vector4DI& v2) {
    int32_t dx = v1.X - v2.X;
    int32_t dy = v1.Y - v2.Y;
    int32_t dz = v1.Z - v2.Z;
    int32_t dw = v1.W - v2.W;
    return sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

Vector4DF Vector4DI::To4DF() const { return Vector4DF(X, Y, Z, W); }
}  // namespace altseed