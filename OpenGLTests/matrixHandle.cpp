#include"matrixHandle.h"
//#include <iostream>
#include<cmath>

matrix_3x3::matrix_3x3() : m_col_1(&m_data[0]), m_col_2((&m_data[0])+3), m_col_3((&m_data[0])+6)
{
    m_col_1[0] = 1.0f;      m_col_2[0] = 0.0f;      m_col_3[0] = 0.0f;
    m_col_1[1] = 0.0f;      m_col_2[1] = 1.0f;      m_col_3[1] = 0.0f;
    m_col_1[2] = 0.0f;      m_col_2[2] = 0.0f;      m_col_3[2] = 1.0f;
}

matrix_3x3::matrix_3x3(float diagonal) : m_col_1(&m_data[0]), m_col_2((&m_data[0]) + 3), m_col_3((&m_data[0]) + 6)
{
    m_col_1[0] = diagonal;  m_col_2[0] = 0.0f;      m_col_3[0] = 0.0f;
    m_col_1[1] = 0.0f;      m_col_2[1] = diagonal;  m_col_3[1] = 0.0f;
    m_col_1[2] = 0.0f;      m_col_2[2] = 0.0f;      m_col_3[2] = diagonal;
}

matrix_3x3::matrix_3x3(float _1x1, float _2x2, float _3x3) : 
    m_col_1(&m_data[0]), m_col_2((&m_data[0]) + 3), m_col_3((&m_data[0]) + 6)
{
    m_col_1[0] = _1x1;      m_col_2[0] = 0.0f;      m_col_3[0] = 0.0f;
    m_col_1[1] = 0.0f;      m_col_2[1] = _2x2;      m_col_3[1] = 0.0f;
    m_col_1[2] = 0.0f;      m_col_2[2] = 0.0f;      m_col_3[2] = _3x3;
}

matrix_3x3::matrix_3x3(float _1x1, float _2x1, float _3x1, float _1x2, float _2x2, float _3x2, float _1x3, float _2x3, float _3x3) : 
    m_col_1(&m_data[0]), m_col_2((&m_data[0]) + 3), m_col_3((&m_data[0]) + 6)
{
    this->equal(_1x1, _2x1, _3x1, _1x2, _2x2, _3x2, _1x3, _2x3, _3x3);
    /*
    m_col_1[0] = _1x1;      m_col_2[0] = _1x2;      m_col_3[0] = _1x3;
    m_col_1[1] = _2x1;      m_col_2[1] = _2x2;      m_col_3[1] = _2x3;
    m_col_1[2] = _3x1;      m_col_2[2] = _3x2;      m_col_3[2] = _3x3;
    */
}

matrix_3x3::matrix_3x3(vector_3& vec, float ang) : m_col_1(&m_data[0]), m_col_2((&m_data[0]) + 3), m_col_3((&m_data[0]) + 6)
{
    rodrigues(vec, ang);
}

vector_3& matrix_3x3::matrix_3x3::operator[] (int idx)
{
    //_ASSERT_EXPR(idx > 0 && idx < 3, "bad para in matrix_3x3[]");
    switch (idx)
    {
    case 0:
        return m_col_1;
    case 1:
        return m_col_2;
    default:
        return m_col_3;
        /*
    default:
        return nullptr;
        */
    }
}

matrix_3x3& matrix_3x3::operator* (matrix_3x3& mat)
{
    m_col_1[0] = m_col_1[0] * mat[0][0]  +  m_col_2[0] * mat[0][1]  +  m_col_3[0] * mat[0][2];
    m_col_1[1] = m_col_1[1] * mat[0][0]  +  m_col_2[1] * mat[0][1]  +  m_col_3[1] * mat[0][2];
    m_col_1[2] = m_col_1[2] * mat[0][0]  +  m_col_2[2] * mat[0][1]  +  m_col_3[2] * mat[0][2];

    m_col_2[0] = m_col_1[0] * mat[1][0]  +  m_col_2[0] * mat[1][1]  +  m_col_3[0] * mat[1][2];
    m_col_2[1] = m_col_1[1] * mat[1][0]  +  m_col_2[1] * mat[1][1]  +  m_col_3[1] * mat[1][2];
    m_col_2[2] = m_col_1[2] * mat[1][0]  +  m_col_2[2] * mat[1][1]  +  m_col_3[2] * mat[1][2];

    m_col_3[0] = m_col_1[0] * mat[2][0]  +  m_col_2[0] * mat[2][1]  +  m_col_3[0] * mat[2][2];
    m_col_3[1] = m_col_1[1] * mat[2][0]  +  m_col_2[1] * mat[2][1]  +  m_col_3[1] * mat[2][2];
    m_col_3[2] = m_col_1[2] * mat[2][0]  +  m_col_2[2] * mat[2][1]  +  m_col_3[2] * mat[2][2];

    return *this;
}

matrix_3x3& matrix_3x3::operator* (float factor)
{
    m_col_1[0] *= factor;
    m_col_1[1] *= factor;
    m_col_1[2] *= factor;

    m_col_2[0] *= factor;
    m_col_2[1] *= factor;
    m_col_2[2] *= factor;

    m_col_3[0] *= factor;
    m_col_3[1] *= factor;
    m_col_3[2] *= factor;

    return *this;
}

vector_3& matrix_3x3::operator* (vector_3& vec)
{
    float buf[3] = {};
    buf[0] = m_col_1[0] * vec[0] + m_col_2[0] * vec[1] + m_col_3[0] * vec[2];
    buf[1] = m_col_1[1] * vec[0] + m_col_2[1] * vec[1] + m_col_3[1] * vec[2];
    buf[2] = m_col_1[2] * vec[0] + m_col_2[2] * vec[1] + m_col_3[2] * vec[2];
    vec[0] = buf[0]; vec[1] = buf[1]; vec[2] = buf[2];

    return vec;
}

matrix_3x3& matrix_3x3::operator+ (matrix_3x3& mat)
{
    m_col_1[0] += mat[0][0];
    m_col_1[1] += mat[0][1];
    m_col_1[2] += mat[0][2];

    m_col_2[0] += mat[1][0];
    m_col_2[1] += mat[1][1];
    m_col_2[2] += mat[1][2];

    m_col_3[0] += mat[2][0];
    m_col_3[1] += mat[2][1];
    m_col_3[2] += mat[2][2];

    return *this;
}

void matrix_3x3::operator= (matrix_3x3& mat)
{
    this->equal(mat[0][0], mat[1][0], mat[2][0], mat[0][1], mat[1][1], mat[2][1], mat[0][2], mat[1][2], mat[2][2]);
}

void matrix_3x3::equal (float _1x1, float _2x1, float _3x1, float _1x2, float _2x2, float _3x2, float _1x3, float _2x3, float _3x3)
{
    m_col_1[0] = _1x1;      m_col_2[0] = _1x2;      m_col_3[0] = _1x3;
    m_col_1[1] = _2x1;      m_col_2[1] = _2x2;      m_col_3[1] = _2x3;
    m_col_1[2] = _3x1;      m_col_2[2] = _3x2;      m_col_3[2] = _3x3;
}

void matrix_3x3::rodrigues (vector_3& axis, float angle)
{
    if (!axis.isNormalized()) axis.normalize();
    matrix_3x3 id(1.0f);
    float x = axis[0], y = axis[1], z = axis[2];
    matrix_3x3 aat(x*x, y*x, z*x,
                   x*y, y*y, z*y,
                   x*z, y*z, z*z);
    //列主序输入，列向量
    matrix_3x3 atsym(0, -z,  y,
                     z,  0, -x,
                    -y,  x,  0);
    *this = id * cosf(angle) + aat * (1.0f - cosf(angle)) + atsym * sinf(angle);
}

void matrix_3x3::rotate(float x, float y, float z, float angle)
{
    vector_3 axis(x, y, z);
    rodrigues(axis, angle);
}

void matrix_3x3::normalize()
{
    // 互相垂直
    m_col_1[2] = m_col_2[0] * m_col_3[1] - m_col_2[1] * m_col_3[0];
    m_col_2[2] = m_col_3[0] * m_col_1[1] - m_col_3[1] * m_col_1[0];
    m_col_3[2] = m_col_1[0] * m_col_2[1] - m_col_1[1] * m_col_2[0];

    m_col_1[0] = m_col_2[1] * m_col_3[2] - m_col_2[2] * m_col_3[1];
    m_col_2[0] = m_col_3[1] * m_col_1[2] - m_col_3[2] * m_col_1[1];
    m_col_3[0] = m_col_1[1] * m_col_2[2] - m_col_1[2] * m_col_2[1];

    // 归一化
    float factor;
    factor = 1 / (sqrt((m_col_1[0] * m_col_1[0]) + (m_col_2[0] * m_col_2[0]) + (m_col_3[0] * m_col_3[0])));
    m_col_1[0] *= factor;
    m_col_2[0] *= factor;
    m_col_3[0] *= factor;

    factor = 1 / (sqrt((m_col_1[1] * m_col_1[1]) + (m_col_2[1] * m_col_2[1]) + (m_col_3[1] * m_col_3[1])));
    m_col_1[1] *= factor;
    m_col_2[1] *= factor;
    m_col_3[1] *= factor;

    factor = 1 / (sqrt((m_col_1[2] * m_col_1[2]) + (m_col_2[2] * m_col_2[2]) + (m_col_3[2] * m_col_3[2])));
    m_col_1[2] *= factor;
    m_col_2[2] *= factor;
    m_col_3[2] *= factor;

}

vector_3::vector_3(float& x, float& y, float& z) : m_isNormalized(false)
{
    m_vec_x = &x;
    m_vec_y = &y;
    m_vec_z = &z;
}

vector_3::vector_3(float* p) : m_isNormalized(false)
{
    m_vec_x = p;
    m_vec_y = p + 1;
    m_vec_z = p + 2;
}

void vector_3::init(float f)// : m_isNormalized(false)
{
    *m_vec_x = f;
    *m_vec_y = f;
    *m_vec_z = f;
}

void vector_3::init(float _1, float _2, float _3)// : m_isNormalized(false)
{
    *m_vec_x = _1;
    *m_vec_y = _2;
    *m_vec_z = _3;
}

void vector_3::normalize()
{
    float factor = 1 / (sqrt((*m_vec_x * *m_vec_x) + (*m_vec_y * *m_vec_y) + (*m_vec_z * *m_vec_z)));
    *m_vec_x *= factor;
    *m_vec_y *= factor;
    *m_vec_z *= factor;
    m_isNormalized = true;
}

bool vector_3::isNormalized()
{
    return m_isNormalized;
}

float& vector_3::operator[](int idx)
{
    //return *(m_vec + idx);
    switch (idx)
    {
    case 0:
        return *m_vec_x;
    case 1:
        return *m_vec_y;
    case 2:
        return *m_vec_z;
    default:
        //std::cerr << "vector [idx] out of range" << std::endl;
        throw "vector [idx] out of range";
    }
}