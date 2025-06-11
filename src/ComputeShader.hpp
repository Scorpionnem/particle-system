/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ComputeShader.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 23:01:15 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/10 23:06:50 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTESHADER_HPP
# define COMPUTESHADER_HPP

# include "libs.hpp"

class	ComputeShader
{
	public:
		ComputeShader(const char *computePath)
		{
			if (DEBUG)
				std::cout << "Loading shader: " << computePath << std::endl;
		
			std::string		shaderCodeFile;
			std::ifstream	shaderFile;

			shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				shaderFile.open(computePath);
				std::stringstream shaderFileStream;
				shaderFileStream << shaderFile.rdbuf();
				shaderFile.close();
				shaderCodeFile = shaderFileStream.str();
			}
			catch(...)
			{
				throw std::runtime_error("Failed to read shader");
			}
		
			const char	*shaderCode = shaderCodeFile.c_str();
			
			this->ID = loadShaderProgram(shaderCode);
			if (!this->ID)
				throw std::runtime_error("Failed to load shader program");
		
		}
		int	loadComputeShader(const char *shader)
		{
			int				success;
			char			infoLog[512];
			unsigned int	res = glCreateShader(GL_COMPUTE_SHADER);

			glShaderSource(res, 1, &shader, NULL);
			glCompileShader(res);
			glGetShaderiv(res, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(res, 512, NULL, infoLog);
				std::cout << infoLog << std::endl;
				return (0);
			}
			return (res);
		}
		int	loadShaderProgram(const char *compute)
		{
			unsigned int	res;
			int				success;
			char			infoLog[512];
			int				computeID;
		
			computeID = loadComputeShader(compute);
			if (!computeID)
				return (0);
		
			res = glCreateProgram();
			glAttachShader(res, computeID);
			glLinkProgram(res);
			glGetProgramiv(res, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(res, 512, NULL, infoLog);
				std::cout << infoLog << std::endl;
				return (0);
			}
			glDeleteShader(computeID);
			return (res);
		}
		unsigned int	ID;
};

#endif
