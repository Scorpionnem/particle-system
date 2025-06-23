/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Particles.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 22:14:23 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/22 17:27:05 by mbatty           ###   ########.fr       */
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
		Particles(int particlesCount);
		~Particles();
		
		void render();
		void update(bool paused, bool gravityCenterOn, glm::vec3 attractor, glm::vec3 secondaryAttractor, float deltaTime);
		void loadParticles(unsigned int countToAdd);
		
		void				compactParticles();
		void				setAttractor(glm::vec3 attractor) {SHADER_MANAGER->get("draw")->setVec3("MAIN_ATTRACTOR", attractor);}
		void				setParticleSize(float size) {this->_particleSize = size;}
		float				getParticleSize() {return (this->_particleSize);}
		void				setParticleShape(ParticleShape shape) {this->_particleShape = shape;}
		ParticleShape		getParticleShape() {return (this->_particleShape);}
		unsigned int		getParticlesCount() {return (this->_liveParticlesCount);}
		void				setNearColor(glm::vec3 color) {this->_nearColor = color;}
		void				setFarColor(glm::vec3 color) {this->_farColor = color;}
		
	private:
		void reallocBuffer(unsigned int &buffer, long int oldSize, long int newSize)
		{
			unsigned int temp;
			glCreateBuffers(1, &temp);
		
			glBindBuffer(GL_COPY_READ_BUFFER, buffer);
			glBindBuffer(GL_COPY_WRITE_BUFFER, temp);
		
			glBufferData(GL_COPY_WRITE_BUFFER, newSize, NULL, GL_DYNAMIC_DRAW);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, oldSize);
		
			glDeleteBuffers(1, &buffer);
			buffer = temp;
		}
		void bufferCpy(unsigned int &dest, unsigned int &src, long int srcSize)
		{
			glBindBuffer(GL_COPY_READ_BUFFER, src);
			glBindBuffer(GL_COPY_WRITE_BUFFER, dest);
		
			glBufferData(GL_COPY_WRITE_BUFFER, srcSize, NULL, GL_DYNAMIC_DRAW);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, srcSize);
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
			unsigned int val[1];
			glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, buf);
			glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(unsigned int), val);
			glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
			return (val[0]);
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
