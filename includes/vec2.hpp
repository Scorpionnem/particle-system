/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:50:03 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/01 15:09:18 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC2_HPP
# define VEC2_HPP

# include "libs.hpp"

class vec2
{
    public:
        float x, y;
    
	    ~vec2(){}
	    vec2(float xy) : x(xy), y(xy)
	    {}
        vec2() : x(0), y(0)
	    {}
        vec2(float x, float y) : x(x), y(y)
	    {}
    
        vec2 operator+(const vec2 &op) const
	    {
	    	return (vec2(x + op.x, y + op.y));
	    }
        vec2 operator-(const vec2 &op) const
	    {
	    	return (vec2(x - op.x, y - op.y));
	    }
        vec2 operator*(float s) const
	    {
	    	return (vec2(x * s, y * s));
	    }
        vec2 operator/(float s) const
	    {
	    	return (vec2(x / s, y / s));
	    }
    
        float dot(const vec2 &op) const
	    {
	    	return (x * op.x + y * op.y);
	    }
        float length() const
	    {
	    	return (std::sqrt(x * x + y * y));
	    }
        vec2 normalize() const
	    {
            float len = length();
            return (len > 0.0f ? *this / len : vec2(0, 0));
        }
        float cross(const vec2 &v) const
        {
            return (x * v.y - y * v.x);
        }
};

#endif