/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:54:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/07 22:35:51 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"
#include "Camera.hpp"

Camera::Camera()
{
	this->yaw = -90.0f;
	this->pitch = 0.0f;
	this->deltaTime = 0.0f;
	this->lastFrame = 0.0f;
	pos = vec3(0.0f, 0.0f, 0.0f);
	front = vec3(0.0f, 0.0f, -1.0f);
	up = vec3(0.0f, 1.0f, 0.0f);
}

void	Camera::update()
{
	this->direction.x = cos(toRadians(yaw)) * cos(toRadians(pitch));
	this->direction.y = sin(toRadians(pitch));
	this->direction.z = sin(toRadians(yaw)) * cos(toRadians(pitch));
	front = direction.normalize();
}

void	Camera::setViewMatrix(Shader &shader)
{
	mat4	view = this->getViewMatrix();
	mat4	projection = perspective(FOV, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, RENDER_DISTANCE);

	shader.use();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
}

mat4	Camera::getViewMatrix()
{
	return (lookAt(pos, pos + front, up));
}
