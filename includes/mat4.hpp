/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:28:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/01 15:09:02 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAT4_HPP
# define MAT4_HPP

#include "libs.hpp"

class mat4
{
	public:
    	float data[16];
	
    	mat4()
		{
    	    std::fill(data, data + 16, 0.0f);
    	}
        mat4(float val)
		{
    	    std::fill(data, data + 16, 0.0f);
            this->data[0] = this->data[5] = this->data[10] = this->data[15] = val;
    	}
        mat4(const mat4 &cpy)
		{
            *this = cpy;
    	}
        mat4 &operator=(const mat4 &cpy)
        {
            for (int i = 0; i < 16; i++)
                this->data[i] = cpy.data[i];
            return (*this);
        }
	
    	static mat4 identity()
		{
    	    mat4 m;
    	    std::fill(m.data, m.data + 16, 0.0f);
    	    m.data[0] = m.data[5] = m.data[10] = m.data[15] = 1.0f;
    	    return (m);
    	}
    	float &operator()(int col, int row)
		{
			return (data[col * 4 + row]);
		}
    	const float &operator()(int col, int row) const
		{
			return (data[col * 4 + row]);
		}
};

inline mat4 operator*(const mat4 &a, const mat4 &b)
{
    mat4 result;
    for (int col = 0; col < 4; ++col)
	{
        for (int row = 0; row < 4; ++row)
		{
            result(col, row) = 0.0f;
            for (int k = 0; k < 4; ++k)
                result(col, row) += a(k, row) * b(col, k);
        }
    }
    return (result);
}

vec4 getCol(const mat4 &m, int col);

void setCol(mat4 &m, int col, const vec4 &v);

mat4 perspective(float fovDeg, float aspect, float near, float far);

mat4 lookAt(const vec3 &eye, const vec3 &center, const vec3 &up);

mat4 translate(const vec3 &t);

mat4 translate(const mat4 &m, const vec3 &v);

mat4 scale(const vec3 &s);

mat4 scale(mat4 &mat, const vec3 &s);

mat4 ortho(float left, float right, float bottom, float top);

mat4 rotate(float angleRad, const vec3 &axis);

mat4 rotate(mat4 &mat, float angleRad, const vec3 &axis);

#endif