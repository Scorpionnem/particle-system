/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:52:39 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/01 15:09:32 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC4_HPP
# define VEC4_HPP

# include "libs.hpp"

class vec4
{
    public:
        float x, y, z, w;

	    ~vec4(){}
        vec4(float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw)
	    {}
        vec4() : x(0), y(0), z(0), w(0)
	    {}
        vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
	    {}
    
        vec4 operator+(const vec4 &v) const
	    {
	    	return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
	    }
        vec4 operator-(const vec4 &v) const
	    {
	    	return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
	    }
        vec4 operator*(float s) const
	    {
	    	return vec4(x * s, y * s, z * s, w * s);
	    }
        vec4 operator/(float s) const
	    {
	    	return vec4(x / s, y / s, z / s, w / s);
	    }
    
        float dot(const vec4 &v) const
	    {
	    	return (x * v.x + y * v.y + z * v.z + w * v.w);
	    }
        float length() const
	    {
	    	return (std::sqrt(x * x + y * y + z * z + w * w));
	    }
        vec4 normalize() const
	    {
            float len = length();
            return len > 0.0f ? *this / len : vec4(0, 0, 0, 0);
        }
};

#endif