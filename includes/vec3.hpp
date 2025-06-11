/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:50:41 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/01 15:09:23 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC3_HPP
# define VEC3_HPP

# include "libs.hpp"

class	vec3
{
	public:
		float x;
		float y;
		float z;
		
		~vec3(){}
		vec3() : x(0), y(0), z(0)
		{}
		vec3(float xyz) : x(xyz), y(xyz), z(xyz)
		{}
		vec3(float x, float y, float z) : x(x), y(y), z(z)
		{}
		
		vec3 operator+(const vec3 &op) const;
		vec3 operator-(const vec3 &op) const;
		vec3 operator*(const vec3 &op) const;
		vec3 operator/(const vec3 &op) const;
		float	dot(const vec3 &op) const;
		vec3	cross(const vec3 &op) const;
		float	length(void) const;
		vec3	normalize(void) const;
};

vec3 minvec3(const vec3 &a, const vec3 &b);

vec3 maxvec3(const vec3 &a, const vec3 &b);

#endif