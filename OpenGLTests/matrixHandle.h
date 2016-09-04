#pragma once
#include<array>
using namespace std;

class vector_3
{
public:
    vector_3(float&, float&, float&);
    vector_3(float*);
    void init(float);
    void init(float, float, float);
    void normalize();
    bool isNormalized();
    float& operator[](int idx);

private:

public:
    float* m_vec_x;
    float* m_vec_y;
    float* m_vec_z;

private:
    bool m_isNormalized;
};

class matrix_3x3
{
public:
    // 实现为列优先，这里示范的格式(行优先)是错的
    matrix_3x3();
    matrix_3x3(float diagonal);
    matrix_3x3(float _1x1, float _2x2, float _3x3);
    matrix_3x3(float _1x1, float _2x1, float _3x1, float _1x2, float _2x2, float _3x2, float _1x3, float _2x3, float _3x3);
    matrix_3x3(vector_3&, float);
    matrix_3x3(array<float, 3>, float);
    void normalize();
    vector_3& operator[] (int idx);
    matrix_3x3& operator* (matrix_3x3& mat);
    vector_3& operator* (vector_3& vec);
    matrix_3x3& operator* (float factor); 
    matrix_3x3& operator+ (matrix_3x3& mat);
    void operator= (matrix_3x3& mat);
    void equal (float _1x1, float _2x1, float _3x1, float _1x2, float _2x2, float _3x2, float _1x3, float _2x3, float _3x3);
    void rotate(float, float, float, float);

private:
    void rodrigues(vector_3& axis, float angle);

public:

private:
    array<float, 9> m_data;
    vector_3 m_col_1;
    vector_3 m_col_2;
    vector_3 m_col_3;
};
