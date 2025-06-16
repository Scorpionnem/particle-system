/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Particles.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 22:14:23 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/17 01:15:51 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLES_HPP
# define PARTICLES_HPP

# include "libs.hpp"
# include "ShaderManager.hpp"
# include "ComputeShader.hpp"
# include "Camera.hpp"

extern ComputeShader	*LOAD_SHADER;
extern ShaderManager	*SHADER_MANAGER;
extern ComputeShader	*COMPUTE_SHADER;
extern bool				PAUSED;
extern Camera			*CAMERA;
extern unsigned int		GLOBAL_PARTICLES_COUNT;

enum	ParticleShape
{
	SQUARE,
	CIRCLE	
};

class	Particles
{
	public:
		Particles(int particlesCount)
		{
			glEnable(GL_PROGRAM_POINT_SIZE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
			_nearColor = glm::vec3(1.0, 1.0, 0.0);
			_farColor = glm::vec3(1.0, 0.0, 0.0);
			_particleSize = 30;
			_particleShape = CIRCLE;

			glGenVertexArrays(1, &posVAO);
			glCreateBuffers(1, &posBuf);
			glCreateBuffers(1, &velBuf);
			loadParticles(particlesCount);
		}
		~Particles()
		{
			glDeleteVertexArrays(1, &posVAO);
			glDeleteBuffers(1, &posBuf);
			glDeleteBuffers(1, &velBuf);
		}

		void render()
		{
			Shader* particleShader = SHADER_MANAGER->get("draw");
			CAMERA->setViewMatrix(*particleShader);
		
			glBindVertexArray(posVAO);
			glDrawArrays(GL_POINTS, 0, _usedParticles);
			glBindVertexArray(0);
		}
		void update(bool paused, bool gravityCenterOn, glm::vec3 attractor, float deltaTime)
		{
			glUseProgram(SHADER_MANAGER->get("draw")->ID);
			SHADER_MANAGER->get("draw")->setVec3("MAIN_ATTRACTOR", attractor);
			SHADER_MANAGER->get("draw")->setVec3("NEAR_COLOR", _nearColor);
			SHADER_MANAGER->get("draw")->setVec3("FAR_COLOR", _farColor);
			SHADER_MANAGER->get("draw")->setVec3("viewPos", CAMERA->pos);
			SHADER_MANAGER->get("draw")->setFloat("MAX_PARTICLE_SIZE", _particleSize);
			SHADER_MANAGER->get("draw")->setInt("PARTICLE_SHAPE", _particleShape);
			SHADER_MANAGER->get("draw")->setFloat("GRADIENT_SCALE", 50);

			glUseProgram(COMPUTE_SHADER->ID);
			glUniform1f(glGetUniformLocation(COMPUTE_SHADER->ID, "time"), glfwGetTime());
			glUniform1i(glGetUniformLocation(COMPUTE_SHADER->ID, "mouseClicked"), gravityCenterOn);
			glUniform3fv(glGetUniformLocation(COMPUTE_SHADER->ID, "MAIN_ATTRACTOR"), 1, &attractor.x);
			glUniform1f(glGetUniformLocation(COMPUTE_SHADER->ID, "deltaTime"), deltaTime);

			if (paused)
				return ;

			glDispatchCompute(_usedParticles, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}
		void loadParticles(unsigned int countToAdd)
		{
			if (_usedParticles + countToAdd > 10000000)
				return ;
		
			unsigned int newTotal = _usedParticles + countToAdd;
		
			if (newTotal > _particlesCapacity)
			{
				_particlesCapacity = std::max(newTotal, _particlesCapacity * 3 / 2 + 1);
			
				reallocBuffer(posBuf, _usedParticles * sizeof(glm::vec4), _particlesCapacity * sizeof(glm::vec4));
				reallocBuffer(velBuf, _usedParticles * sizeof(glm::vec4), _particlesCapacity * sizeof(glm::vec4));
				
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuf);

				glBindVertexArray(posVAO);
				glBindBuffer(GL_ARRAY_BUFFER, posBuf);
				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
				glEnableVertexAttribArray(0);
			}

			glUseProgram(LOAD_SHADER->ID);
			glUniform1ui(glGetUniformLocation(LOAD_SHADER->ID, "u_Offset"), _usedParticles);
			glUniform1ui(glGetUniformLocation(LOAD_SHADER->ID, "u_Count"), countToAdd);
			glUniform1i(glGetUniformLocation(LOAD_SHADER->ID, "shape"), _particleShape);

			glUseProgram(LOAD_SHADER->ID);
			glDispatchCompute(countToAdd, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

			_usedParticles = newTotal;
			GLOBAL_PARTICLES_COUNT += countToAdd;
		}
		void	setAttractor(glm::vec3 attractor)
		{
			SHADER_MANAGER->get("draw")->setVec3("MAIN_ATTRACTOR", attractor);
		}
		void	setParticleSize(float size) {this->_particleSize = size;}
		float	getParticleSize() {return (this->_particleSize);}
		void	setParticleShape(ParticleShape shape) {this->_particleShape = shape;}
		ParticleShape	getParticleShape() {return (this->_particleShape);}
	private:	
		void reallocBuffer(GLuint &buffer, GLsizeiptr oldSize, GLsizeiptr newSize)
		{
			GLuint temp;
			glCreateBuffers(1, &temp);
		
			glBindBuffer(GL_COPY_READ_BUFFER, buffer);
			glBindBuffer(GL_COPY_WRITE_BUFFER, temp);
		
			glBufferData(GL_COPY_WRITE_BUFFER, newSize, NULL, GL_DYNAMIC_DRAW);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, oldSize);
		
			glDeleteBuffers(1, &buffer);
			buffer = temp;
		}
		
		unsigned int	posVAO = 0;
		unsigned int	posBuf = 0;
		unsigned int	velBuf = 0;
		unsigned int	_usedParticles = 0;
		unsigned int	_particlesCapacity = 0;
		glm::vec3		_farColor;
		glm::vec3		_nearColor;
		float			_particleSize;
		ParticleShape	_particleShape;
};

#endif
