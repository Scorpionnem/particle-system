/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Particles.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 22:14:23 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/19 20:00:21 by mbatty           ###   ########.fr       */
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
extern ComputeShader			*COMPACT_SHADER;
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
			glCreateBuffers(1, &counterBuf);
			glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, counterBuf); //Generate the counter atomic buffer
			glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
			loadParticles(particlesCount);
		}
		~Particles()
		{
			glDeleteVertexArrays(1, &posVAO);
			glDeleteBuffers(1, &posBuf);
			glDeleteBuffers(1, &velBuf);
			glDeleteBuffers(1, &counterBuf);
		}

		void resetAtomicCounter(unsigned int buf)
		{
			glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, buf);
			unsigned int val[1] = {0};
			glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(unsigned int), val);
			glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
		}
		unsigned int getAtomicCounter(unsigned int buf)
		{
			GLuint val[1];
			glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, buf);
			glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(unsigned int), val);
			glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
			return (val[0]);
		}
		void render()
		{
			if (_usedParticles <= 0)
				return ;
				
			Shader* particleShader = SHADER_MANAGER->get("draw");
			CAMERA->setViewMatrix(*particleShader);
		
			glUseProgram(SHADER_MANAGER->get("draw")->ID);
			SHADER_MANAGER->get("draw")->setVec3("NEAR_COLOR", _nearColor);
			SHADER_MANAGER->get("draw")->setVec3("FAR_COLOR", _farColor);
			SHADER_MANAGER->get("draw")->setVec3("viewPos", CAMERA->pos);
			SHADER_MANAGER->get("draw")->setFloat("MAX_PARTICLE_SIZE", _particleSize);
			SHADER_MANAGER->get("draw")->setInt("PARTICLE_SHAPE", _particleShape);
			SHADER_MANAGER->get("draw")->setFloat("GRADIENT_SCALE", 50);

			glBindVertexArray(posVAO);
			glDrawArrays(GL_POINTS, 0, _usedParticles);
			glBindVertexArray(0);
		}
		void update(bool paused, bool gravityCenterOn, glm::vec3 attractor, float deltaTime)
		{
			if (_usedParticles <= 0)
				return ;
				
			glUseProgram(SHADER_MANAGER->get("draw")->ID);
			SHADER_MANAGER->get("draw")->setVec3("MAIN_ATTRACTOR", attractor);

			glUseProgram(COMPUTE_SHADER->ID);
			glUniform1f(glGetUniformLocation(COMPUTE_SHADER->ID, "time"), glfwGetTime());
			glUniform1i(glGetUniformLocation(COMPUTE_SHADER->ID, "mouseClicked"), gravityCenterOn);
			glUniform3fv(glGetUniformLocation(COMPUTE_SHADER->ID, "MAIN_ATTRACTOR"), 1, &attractor.x);
			glUniform1f(glGetUniformLocation(COMPUTE_SHADER->ID, "deltaTime"), deltaTime);

			if (paused)
				return ;

			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuf);
			glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, counterBuf);
			resetAtomicCounter(counterBuf);
			
			glUseProgram(COMPUTE_SHADER->ID);
			glDispatchCompute(_usedParticles / 10, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

			_liveParticlesCount = getAtomicCounter(counterBuf);
			GLOBAL_PARTICLES_COUNT += _liveParticlesCount;
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

				glBindVertexArray(posVAO);
				glBindBuffer(GL_ARRAY_BUFFER, posBuf);
				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
				glEnableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, velBuf);
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
				glEnableVertexAttribArray(1);
			}

			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuf);

			glUseProgram(LOAD_SHADER->ID);
			glUniform1ui(glGetUniformLocation(LOAD_SHADER->ID, "u_Offset"), _usedParticles);
			glUniform1ui(glGetUniformLocation(LOAD_SHADER->ID, "u_Count"), countToAdd);
			glUniform1i(glGetUniformLocation(LOAD_SHADER->ID, "shape"), _particleShape);

			glUseProgram(LOAD_SHADER->ID);
			glDispatchCompute(countToAdd / 10, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

			_usedParticles = newTotal;
			_liveParticlesCount += countToAdd;
		}
		void	compactParticles()
		{
			if (_usedParticles <= 0 || _liveParticlesCount <= 0)
				return ;

			resetAtomicCounter(counterBuf);

			GLuint tempPosBuf, tempVelBuf;
			glCreateBuffers(1, &tempPosBuf);
			glCreateBuffers(1, &tempVelBuf);
			glNamedBufferData(tempPosBuf, _usedParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
			glNamedBufferData(tempVelBuf, _usedParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

			glUseProgram(COMPACT_SHADER->ID);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuf);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, tempPosBuf);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, tempVelBuf);
			glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, counterBuf);

			glUseProgram(COMPACT_SHADER->ID);
			glDispatchCompute(_usedParticles / 100, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

			_liveParticlesCount = getAtomicCounter(counterBuf);

			if (_liveParticlesCount == 0)
			{
				glDeleteBuffers(1, &tempPosBuf);
				glDeleteBuffers(1, &tempVelBuf);
				_usedParticles = 0;
				_particlesCapacity = 0;
				return;
			}

			//Size down used buffers and copy from temp

			reallocBuffer(posBuf, tempPosBuf, _liveParticlesCount * sizeof(glm::vec4));
			reallocBuffer(velBuf, tempVelBuf, _liveParticlesCount * sizeof(glm::vec4));

			glDeleteBuffers(1, &tempPosBuf);
			glDeleteBuffers(1, &tempVelBuf);

			_usedParticles = _liveParticlesCount;
			_particlesCapacity = _liveParticlesCount;

			glBindVertexArray(posVAO);
			glBindBuffer(GL_ARRAY_BUFFER, posBuf);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, velBuf);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(1);
		}
		void	setAttractor(glm::vec3 attractor)
		{
			SHADER_MANAGER->get("draw")->setVec3("MAIN_ATTRACTOR", attractor);
		}
		void	setParticleSize(float size) {this->_particleSize = size;}
		float	getParticleSize() {return (this->_particleSize);}
		void	setParticleShape(ParticleShape shape) {this->_particleShape = shape;}
		ParticleShape	getParticleShape() {return (this->_particleShape);}
		unsigned int getParticlesCount() {return (this->_liveParticlesCount);}
	// private:	
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
		void reallocBuffer(GLuint &dest, GLuint &src, GLsizeiptr srcSize)
		{
			glBindBuffer(GL_COPY_READ_BUFFER, src);
			glBindBuffer(GL_COPY_WRITE_BUFFER, dest);
		
			glBufferData(GL_COPY_WRITE_BUFFER, srcSize, NULL, GL_DYNAMIC_DRAW);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, srcSize);
		}

		unsigned int	posVAO = 0;
		unsigned int	posBuf = 0;
		unsigned int	velBuf = 0;
		unsigned int	counterBuf = 0;
		unsigned int	_usedParticles = 0;
		unsigned int	_particlesCapacity = 0;
		unsigned int	_liveParticlesCount = 0;
		glm::vec3		_farColor;
		glm::vec3		_nearColor;
		float			_particleSize;
		ParticleShape	_particleShape;
};

#endif
