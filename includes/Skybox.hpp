#ifndef SKYBOX_HPP
# define SKYBOX_HPP

#include "libs.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

class	Skybox
{
	public:
		~Skybox();
		Skybox(const std::vector<std::string> &faces);
		void	draw(Camera &camera, Shader &shader);

		unsigned int	ID;
		unsigned int	VAO;
		unsigned int	VBO;
		mat4    		model;
};

#endif
