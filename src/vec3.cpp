/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:54:24 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/01 15:10:53 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

vec3 minvec3(const vec3 &a, const vec3 &b)
{
    return vec3(
        (a.x < b.x) ? a.x : b.x,
        (a.y < b.y) ? a.y : b.y,
        (a.z < b.z) ? a.z : b.z
    );
}

vec3 maxvec3(const vec3 &a, const vec3 &b)
{
    return vec3(
        (a.x > b.x) ? a.x : b.x,
        (a.y > b.y) ? a.y : b.y,
        (a.z > b.z) ? a.z : b.z
    );
}

vec3 vec3::operator+(const vec3 &op) const
{
	return (vec3(x + op.x, y + op.y, z + op.z));
}

vec3 vec3::operator-(const vec3 &op) const
{
	return (vec3(x - op.x, y - op.y, z - op.z));
}

vec3 vec3::operator*(const vec3 &op) const
{
	return (vec3(x * op.x, y * op.y, z * op.z));
}

vec3 vec3::operator/(const vec3 &op) const
{
	return (vec3(x / op.x, y / op.y, z / op.z));
}

vec3	vec3::cross(const vec3 &op) const
{
	return (vec3(
		y * op.z - z * op.y,
        z * op.x - x * op.z,
        x * op.y - y * op.x
	));
}

float	vec3::dot(const vec3 &op) const
{
	return (x * op.x + y * op.y + z * op.z);
}

float	vec3::length() const
{
    return (std::sqrt(x * x + y * y + z * z));
}

vec3	vec3::normalize() const
{
	float len = this->length();
	return (len > 0.0f ? *this / len : vec3(0, 0, 0));
}
