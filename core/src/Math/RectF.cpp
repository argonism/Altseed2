﻿#include "RectF.h"
#include "RectI.h"
#include "Vector2DF.h"

using namespace std;

namespace altseed {
RectF::RectF() : X(0), Y(0), Width(0), Height(0) {}

RectF::RectF(float x, float y, float width, float height) : X(x), Y(y), Width(width), Height(height) {}

RectF::RectF(Vector2DF position, Vector2DF size) : X(position.X), Y(position.Y), Width(size.X), Height(size.Y) {}

Vector2DF RectF::GetPosition() const { return Vector2DF(X, Y); }

Vector2DF RectF::GetSize() const { return Vector2DF(Width, Height); }

array<Vector2DF, 4> RectF::GetVertexes() const {
    array<Vector2DF, 4> ret;

    ret[0] = Vector2DF(X, Y);
    ret[1] = Vector2DF(X + Width, Y);
    ret[2] = Vector2DF(X + Width, Y + Height);
    ret[3] = Vector2DF(X, Y + Height);

    return ret;
}

bool RectF::operator!=(const RectF& right) { return X != right.X || Y != right.Y || Width != right.Width || Height != right.Height; }

RectI RectF::ToI() const { return RectI((int32_t)X, (int32_t)Y, (int32_t)Width, (int32_t)Height); }
}  // namespace altseed