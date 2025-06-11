/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:47:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/01 15:10:03 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

mat4 ortho(float left, float right, float bottom, float top)
{
    const float near = -1.0f;
    const float far = 1.0f;

    mat4 m = mat4::identity();

    m(0, 0) = 2.0f / (right - left);
    m(1, 1) = 2.0f / (top - bottom);
    m(2, 2) = -2.0f / (far - near);

    m(3, 0) = -(right + left) / (right - left);
    m(3, 1) = -(top + bottom) / (top - bottom);
    m(3, 2) = -(far + near) / (far - near);

    return (m);
}

vec4 getCol(const mat4 &m, int col)
{
    return vec4(m(col, 0), m(col, 1), m(col, 2), m(col, 3));
}

void setCol(mat4 &m, int col, const vec4 &v)
{
    m(col, 0) = v.x;
    m(col, 1) = v.y;
    m(col, 2) = v.z;
    m(col, 3) = v.w;
}

mat4 perspective(float fovDeg, float aspect, float near, float far)
{
    float fovRad = fovDeg * M_PI / 180.0f;
    float f = 1.0f / std::tan(fovRad / 2.0f);

    mat4 m;
    m.data[0] = f / aspect;
    m.data[5] = f;
    m.data[10] = (far + near) / (near - far);
    m.data[11] = -1.0f;
    m.data[14] = (2.0f * far * near) / (near - far);
    return (m);
}

mat4 lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
    vec3 f = (center - eye).normalize();
    vec3 s = f.cross(up).normalize();
    vec3 u = s.cross(f);

    mat4 m = mat4::identity();
    m(0, 0) = s.x; m(1, 0) = s.y; m(2, 0) = s.z;
    m(0, 1) = u.x; m(1, 1) = u.y; m(2, 1) = u.z;
    m(0, 2) = -f.x; m(1, 2) = -f.y; m(2, 2) = -f.z;
    m(3, 0) = -s.dot(eye);
    m(3, 1) = -u.dot(eye);
    m(3, 2) = f.dot(eye);
    return (m);
}

mat4 translate(const vec3 &t)
{
    mat4 m = mat4::identity();
    m(0, 3) = t.x;
    m(1, 3) = t.y;
    m(2, 3) = t.z;
    return (m);
}

mat4 translate(const mat4 &m, const vec3 &v)
{
    mat4 result = m;

    vec4 col0 = getCol(m, 0);
    vec4 col1 = getCol(m, 1);
    vec4 col2 = getCol(m, 2);
    vec4 col3 = getCol(m, 3);

    vec4 translation = col0 * v.x + col1 * v.y + col2 * v.z + col3;

    setCol(result, 3, translation);

    return (result);
}

mat4 scale(const vec3 &s)
{
    mat4 m = mat4::identity();
    m(0, 0) = s.x;
    m(1, 1) = s.y;
    m(2, 2) = s.z;
    return (m);
}

mat4 scale(mat4 &mat, const vec3 &s)
{
    (void)mat;
    return (mat * scale(s));
}

mat4 rotate(mat4 &mat, float angleRad, const vec3 &axis)
{
    return (mat * rotate(angleRad, axis));
}

mat4 rotate(float angleRad, const vec3 &axis)
{
    vec3 a = axis.normalize();
    float c = std::cos(angleRad);
    float s = std::sin(angleRad);
    float oneMinusC = 1.0f - c;

    mat4 m = mat4::identity();
    m(0, 0) = c + a.x * a.x * oneMinusC;
    m(0, 1) = a.x * a.y * oneMinusC + a.z * s;
    m(0, 2) = a.x * a.z * oneMinusC - a.y * s;

    m(1, 0) = a.y * a.x * oneMinusC - a.z * s;
    m(1, 1) = c + a.y * a.y * oneMinusC;
    m(1, 2) = a.y * a.z * oneMinusC + a.x * s;

    m(2, 0) = a.z * a.x * oneMinusC + a.y * s;
    m(2, 1) = a.z * a.y * oneMinusC - a.x * s;
    m(2, 2) = c + a.z * a.z * oneMinusC;

    return (m);
}
