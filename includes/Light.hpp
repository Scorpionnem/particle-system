/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Light.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:41:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/02 13:07:56 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_HPP
# define LIGHT_HPP

# include "libs.hpp"
# include "Mesh.hpp"
# include "Shader.hpp"
# include "Camera.hpp"

class	Light
{
	public:
		Light();
		~Light();
		vec3	pos;
		vec3	color;
		void	update(Shader &shader);
		bool	move = true;
};

#endif