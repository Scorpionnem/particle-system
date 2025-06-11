/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:55:06 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/10 22:33:01 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "libs.hpp"
# include "Shader.hpp"

extern float	FOV;
extern float	RENDER_DISTANCE;
extern float	SCREEN_WIDTH;
extern float	SCREEN_HEIGHT;

class Camera
{
	public:
		Camera();
		void	update();
		mat4	getViewMatrix();
		vec3 direction;
		void	setViewMatrix(Shader &shader);
		vec3	pos;
		vec3	front;
		vec3	up;
		float	deltaTime;
		float	lastFrame;
		float	yaw;
		float	pitch;
};

#endif
