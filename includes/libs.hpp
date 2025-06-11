/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:10 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/11 00:53:06 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBS_HPP
# define LIBS_HPP

# define DEBUG 0

# include <iostream>
# include <string>
# include <fstream>
# include <sstream>
# include <istream>
# include <vector>
# include <map>
# include <cmath>
# include <unistd.h>
# include <exception>
# include <functional>
# include <array>
# include <algorithm>
# include <deque>

# include "vec2.hpp"
# include "vec3.hpp"
# include "vec4.hpp"
# include "mat4.hpp"

# include "strings.hpp"

# include "glad.h"
# include "glfw3.h"

# define WIN_NAME "particle-system"

inline float   clamp(float val, float min, float max)
{
    float   res = val;
    if (res < min)
        res = min;
    else if (res > max)
        res = max;
    return (res);
}

inline float   toRadians(float deg)
{
    return(deg * M_PI / 180.0f);
}

#endif
