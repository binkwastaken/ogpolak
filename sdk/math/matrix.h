#pragma once
#include <math.h>
#include <iostream>
#include <iomanip>

class Matrix2x4_t {
public:
    float data[2][4];

    Matrix2x4_t() {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 4; ++j)
                data[i][j] = 0.0f;
    }
};

class Matrix3x3_t {
public:
    float data[3][3];

    Matrix3x3_t() {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                data[i][j] = 0.0f;
    }
};

class Matrix3x4_t {
public:
    float data[3][4];

    Matrix3x4_t() {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 4; ++j)
                data[i][j] = 0.0f;
    }
};