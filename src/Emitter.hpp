/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Emitter.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 17:28:48 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/23 08:30:45 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EMITTER_HPP
# define EMITTER_HPP

# include "libs.hpp"
# include "ComputeShader.hpp"
# include "Particles.hpp"

extern ComputeShader		*LOAD_SHADER;

class	Emitter
{
	public:
		Emitter(glm::vec3 pos);
		~Emitter() {}
		void	compact(void);
		void	emit(int count);
		void	render();
		void	update(bool paused, bool gravityCenterOn, glm::vec3 attractor, glm::vec3 secondaryAttractor, float deltaTime);
	private:
		Particles	_particles;
		glm::vec3		_pos;
};

#endif
