#pragma once
#include<utility>
#include"global.h"
#include<math.h>
#include<memory>
#include"template_SFINAE.h"

namespace matrix
{
    template<typename T = float, unsigned int U = 3>
    class coloum
    {
    public:
        coloum() = default;
        ~coloum() = default;
        T& operator[](int i);
        T dot(const coloum<T, U> &col);
        template<TMPL_COND_DECL(U==3)>
        coloum<T, U>&& cross(const coloum<T, U>& col);
        bool isNormalized();
        void normalize();

    public:
        typedef T _dataType;
        enum { length = U };
        T m_data[U];
    };

    template<typename T = float, unsigned int U = 3, unsigned int V = 3>
    class matrix
    {
    public:
        matrix() = default;
        template<TMPL_COND_DECL(U == V)>
        matrix(T i);
        ~matrix() = default;
        coloum<T, V>& operator[](int i);
        matrix<T, U, V>&& operator*(float f);
        coloum<T, V>&& operator*(const coloum<T, U> &col);
        template<typename W, int U2 = W::coloumLength, TMPL_COND_DECL(W::lineLength == V)>
        matrix<T, U, U2>&& operator*(const W &mat);
        template<TMPL_COND_DECL((U == 2) && (V == 2))>
        void normalize();
        template<TMPL_COND_DECL((U == 3) && (V == 3)), TMPL_SIG_DECL>
        void normalize();
        template<TMPL_COND_DECL((U == 4) && (V == 4)), TMPL_SIG_DECL, TMPL_SIG_DECL>
        void normalize();
        template<TMPL_COND_DECL((U == 3) && (V == 3))>
        matrix<T, U, V>&& rodrigues(const coloum<T, U>& axis, float angle);
        template<TMPL_COND_DECL((U == 4) && (V == 4)), TMPL_SIG_DECL>
        matrix<T, U, V>&& rodrigues(const coloum<T, U>& axis, float angle);
    public:
        typedef T _dataType;
        enum { lineLength = U, coloumLength = V };

        enum { diffSignature };
        union 
        {
            T m_data[U*V];
            coloum<T, V> m_colom[U];
        };
        bool bNormal;
    };

    /*template<typename T, unsigned int U>
    inline coloum<T, U>::coloum()
    {
        for (auto &i : m_data) i = 0;
    }*/

    template<typename T, unsigned int U>
    inline T& coloum<T, U>::operator[](int i)
    {
        return m_data[i];
    }

    template<typename T, unsigned int U>
    inline T coloum<T, U>::dot(const coloum<T, U>& col)
    {
        T r = 0.0;
        for (int i = 0; i < U; ++i)
        {
            r += col[i] * (*this)[i];
        }
        return r;
    }

    // template<>
   
    template<typename T, unsigned int U>
    template<TMPL_COND_DEFI>
    inline coloum<T, U>&& coloum<T, U>::cross(const coloum<T, U>& col)
    {
        coloum<T, U> col_r;
        col_r[0] -= (*this)[1] * col[2];
        col_r[0] += (*this)[2] * col[1];

        col_r[1] += (*this)[0] * col[2];
        col_r[1] -= (*this)[2] * col[0];

        col_r[2] -= (*this)[0] * col[1];
        col_r[2] += (*this)[1] * col[0];
        return std::move(col_r);
    }
    /*
    template<typename T, unsigned int U>
    inline coloum<T, U>&& coloum<T, U>::cross(const coloum<T, U>& col)
    {   
        coloum<T, U> col_r;
        for (int i = 0, k; i < U; ++i)
        {
            for (int j = U, l; j > 0; --j)
            {
                l = j - i;
                if (l < 0) l += U;
                k = (l != 0 ? 1 : 0);
                col_r[i] += k * (*this)[i] * col[l] * ((j + i) % 2 - 0.5) * 2;
            }
        }
        return std::move(col_r);
    }
    */
    template<typename T, unsigned int U>
    inline bool coloum<T, U>::isNormalized()
    {
        T acc = 0.0;
        for (int i = 0; i < U; i++)
        {
            acc += m_data[i] * m_data[i];
        }
        if ((acc - 1.0 < 0.00001) || (1.0 - acc < 0.00001)) return true;
        else return false;
    }

    //U = 3时的实现
    template<typename T, unsigned int U>
    inline void coloum<T, U>::normalize()
    {
        T acc = 0.0;
        for (auto &i : m_data)
        {
            acc += i * i;
        }
        if (!((acc - 1.0 < 0.00001) || (1.0 - acc < 0.00001)))
        {
            double factor = sqrt(acc);
            for (auto &i : m_data) { i /= factor; }
        }
    }

    /*template<typename T, unsigned int U, unsigned int V>
    inline matrix<T, U, V>::matrix()
    {
        memset(m_data, 0, sizeof(m_data));
        bNormal = false;
    }*/

    template<typename T, unsigned int U, unsigned int V>
    template<TMPL_COND_DEFI>
    inline matrix<T, U, V>::matrix(T i)
    {
        for (int j = 0; j < U; ++j) (*this)[j][j] = i;
        bNormal = false;
    }

    template<typename T, unsigned int U, unsigned int V>
    inline coloum<T, V>& matrix<T, U, V>::operator[](int i)
    {
        return m_colom[i];
    }

    template<typename T, unsigned int U, unsigned int V>
    inline matrix<T, U, V>&& matrix<T, U, V>::operator*(float f)
    {
        matrix<T, U, V>mat;
        for (int i = 0; i < U; ++i)
        {
            for (int j = 0; j < V; ++j)
                mat[i][j] *= f;
        }
        return std::move(mat);
    }

    template<typename T, unsigned int U, unsigned int V>
    inline coloum<T, V>&& matrix<T, U, V>::operator*(const coloum<T, U> &col)
    {
        coloum<T, V> col_r;
        for (int i = 0; i < V; ++i)
        {
            for (int j = 0; j < U; ++j)
            {
                col_r[i] += (*this)[i][j] * col[j];
            }
        }
        return std::move(col_r);
    }

    template<typename T, unsigned int U, unsigned int V>
    template<TMPL_COND_DEFI>
    inline void matrix<T, U, V>::normalize()
    {
        if (bNormal) return;
        else
        {
            T x = (*this)[0][0];
            T y = (*this)[0][1];
            coloum<T, 2> temp;
            temp[0] = y;
            temp[1] = -x;
            if ((*this)[0].dot(temp) > 0)
            {
                (*this)[1][0] = y;
                (*this)[1][1] = -x;
            }
            else
            {
                (*this)[1][0] = -y;
                (*this)[1][1] = x;
            }
            bNormal = true;
        }
    }

    template<typename T, unsigned int U, unsigned int V>
    template<TMPL_COND_DEFI, TMPL_SIG_DEFI>
    inline void matrix<T, U, V>::normalize()
    {
        if (bNormal) return;
        else
        {
            coloum<T, 3> col_z = (*this)[0].cross((*this)[1]);
            coloum<T, 3> col_y = col_z.cross((*this)[0]);
            col_z.normalize();
            col_y.normalize();
            (*this)[0].normalize();

            (*this)[1][0] = col_y[0];
            (*this)[1][1] = col_y[1];
            (*this)[1][2] = col_y[2];

            (*this)[2][0] = col_z[0];
            (*this)[2][1] = col_z[1];
            (*this)[2][2] = col_z[2];
        }
        bNormal = true;
    }

    template<typename T, unsigned int U, unsigned int V>
    template<TMPL_COND_DEFI, TMPL_SIG_DEFI, TMPL_SIG_DEFI>
    inline void matrix<T, U, V>::normalize()
    {
        if (bNormal) return;
        else
        {
            coloum<T, 4> col_z_ = (*this)[0].cross((*this)[1]);
            coloum<T, 4> col_y_ = col_z.cross((*this)[0]);

            coloum<T, 3> col_z;
            col_z[0] = col_z_[0]; col_z[1] = col_z_[1]; col_z[2] = col_z_[2];

            coloum<T, 3> col_y;
            col_y[0] = col_y_[0]; col_y[1] = col_y_[1]; col_y[2] = col_y_[2];

            coloum<T, 3> col_x;
            col_x[0] = (*this)[0]; col_x[1] = (*this)[1]; col_x[2] = (*this)[2];

            col_z.normalize();
            col_y.normalize();
            col_x.normalize();

            (*this)[0][0] = col_x[0];
            (*this)[0][1] = col_x[1];
            (*this)[0][2] = col_x[2];

            (*this)[1][0] = col_y[0];
            (*this)[1][1] = col_y[1];
            (*this)[1][2] = col_y[2];

            (*this)[2][0] = col_z[0];
            (*this)[2][1] = col_z[1];
            (*this)[2][2] = col_z[2];
        }
        bNormal = true;
    }

    template<typename T, unsigned int U, unsigned int V>
    template<typename W, int U2, TMPL_COND_DEFI>
    inline matrix<T, U, U2>&& matrix<T, U, V>::operator*(const W &mat)
    {
        matrix<T, U, U2> mat_r;
        for (int i = 0; i < U; ++i)
        {
            for (int j = 0; j < U2; ++j)
            {
                for (int k = 0; k < V; ++k)
                {
                    mat_r += mat[k][j] * (*this)[i][k];
                }
            }
        }
        mat_r.bNormal = false;
        return std::move(mat_r);
    }

    template<typename T, unsigned int U, unsigned int V>
    template<TMPL_COND_DEFI>
    inline matrix<T, U, V>&& matrix<T, U, V>::rodrigues(const coloum<T, U>& axis, float angle)
    {
        coloum<T, U> _axis(axis);
        _axis.normalize();

        typedef matrix<T, 3, 3> matrix_3;

        matrix_3 mat_id(1);

        matrix_3 mat_aat;
        for (int i = 0; i < U; ++i)
        {
            for (int j = 0; j < V; ++j)
            {
                mat_aat[i][j] = _axis[i] * _axis[j];
            }
        }

        matrix_3 mat_atsym;
        mat_atsym[0][0] = 0;   mat_atsym[1][0] = -z;  mat_atsym[2][0] = y;
        mat_atsym[0][1] = z;   mat_atsym[1][1] = 0;   mat_atsym[2][1] = -x;
        mat_atsym[0][2] = -y;  mat_atsym[1][2] = x;   mat_atsym[2][2] = 0;

        matrix_3 mat_rst;
        mat_rst = mat_id * cosf(angle) + mat_aat * (1 - cosf(angle)) + mat_atsym * sinf(angle);
        return std::move(mat_rst);
    }

    template<typename T, unsigned int U, unsigned int V>
    template<TMPL_COND_DEFI, TMPL_SIG_DEFI>
    inline matrix<T, U, V>&& matrix<T, U, V>::rodrigues(const coloum<T, U>& axis, float angle)
    {
        coloum<T, 3> _sub_axis;
        _sub_axis[0] = axis[0];
        _sub_axis[1] = axis[1];
        _sub_axis[2] = axis[2];

        typedef matrix<T, 3, 3> matrix_3;
        typedef matrix<T, 4, 4> matrix_4;

        //matrix_4 mat_id(1);

        matrix_3 sub_mat, sub_rst_mat;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                sub_mat[i][j] = (*this)[i][j];
            }
        }
        sub_rst_mat = sub_mat.rodrigues(_sub_axis, angle);

        matrix_4 mat_rst;
        mat_rst[0][3] = 0.0f;  mat_rst[1][3] = 0.0f; mat_rst[2][3] = 0.0f;
        mat_rst[3][0] = 0.0f;  mat_rst[3][1] = 0.0f; mat_rst[3][2] = 0.0f; mat_rst[3][3] = 1.0f;
        return std::move(mat_rst);
    }
}

static_assert(std::is_pod<matrix::matrix<float, 3, 3>>::value, "");
