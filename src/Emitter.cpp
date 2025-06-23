/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Emitter.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 08:27:49 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/23 08:32:25 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Emitter.hpp"

Emitter::Emitter(glm::vec3 pos): _particles(0)
{
	_pos = pos;
	_particles.setFarColor(glm::vec3(0.0, 1.0, 0.3));
	_particles.setNearColor(glm::vec3(0.0, 0.3, 1.0));
}

void	Emitter::emit(int count)
{
	glUseProgram(LOAD_SHADER->ID);
	glUniform1i(glGetUniformLocation(LOAD_SHADER->ID, "EMITTER"), true);
	glUniform3fv(glGetUniformLocation(LOAD_SHADER->ID, "EMITTER_POS"), 1, &_pos.x);
	_particles.loadParticles(count);
	glUniform1i(glGetUniformLocation(LOAD_SHADER->ID, "EMITTER"), false);
	glUseProgram(0);
}

void	Emitter::compact(void)
{
	_particles.compactParticles();
}

void	Emitter::render()
{
	_particles.setParticleSize(200);
	_particles.render();
}

void	Emitter::update(bool paused, bool gravityCenterOn, glm::vec3 attractor, glm::vec3 secondaryAttractor, float deltaTime)
{
	if (!paused)
		emit(100);
	_particles.update(paused, gravityCenterOn, attractor, secondaryAttractor, deltaTime);
}
