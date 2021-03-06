﻿#pragma once
#include "Vector2DI.h"
#include "Vector3DI.h"

namespace altseed {
/**
        @brief	3×3行列を表す構造体。
        @note
        [0,0][0,1]
        [1,0][1,1]
        */
struct Matrix33I {
public:

    Matrix33I();


    int32_t Values[3][3];


    Matrix33I& SetIdentity();

    /**
    @brief	転置行列を設定する。
    @return	このインスタンスへの参照
    */
    Matrix33I& SetTransposed();

    /**
    @brief	逆行列を設定する。
    @return	このインスタンスへの参照
    */
    Matrix33I& SetInverted();

    /**
    @brief	逆行列を取得する。
    @return	逆行列
    */
    Matrix33I GetInverted();


    Matrix33I& SetTranslation(int32_t x, int32_t y);

    
    Matrix33I& SetScale(int32_t x, int32_t y);

    /**
    @brief	行列でベクトルを変形させる。
    @param	in	変形前ベクトル
    @return	変形後ベクトル
    */
    Vector2DI Transform2D(const Vector2DI& in) const;

    /**
    @brief	行列でベクトルを変形させる。
    @param	in	変形前ベクトル
    @return	変形後ベクトル
    */
    Vector3DI Transform3D(const Vector3DI& in) const;


    Matrix33I operator*(const Matrix33I& right);

    Vector3DI operator*(const Vector3DI& right);
};
}  // namespace altseed