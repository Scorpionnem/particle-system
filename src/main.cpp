/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/11 02:00:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"
#include "Font.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "InterfaceManager.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "ComputeShader.hpp"

float	FOV = 65;
float	SCREEN_WIDTH = 1100;
float	SCREEN_HEIGHT = 900;
float	RENDER_DISTANCE = 1000;

bool	F1 = false;
bool	F3 = false;
bool	lock_fps = true;

Font				*FONT;
ShaderManager		*SHADER_MANAGER;
TextureManager		*TEXTURE_MANAGER;
InterfaceManager	*INTERFACES_MANAGER;
Camera				*CAMERA;
Window				*WINDOW;

ComputeShader	*COMPUTE_SHADER;
ComputeShader	*LOAD_SHADER;

void	interpolateTo(float &float1, float &float2, float deltaTime)
{
	float1 += (float2 - float1) * 4.0f * deltaTime;

	float2 = 1.0f - float1;
}

void	closeWindow()
{
	glfwSetWindowShouldClose(WINDOW->getWindowData(), true);
}

void	key_hook(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)window;(void)key;(void)scancode;(void)action;(void)mods;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
}

void	build(ShaderManager *shader)
{
	shader->load({"gui", GUI_VERT_SHADER, GUI_FRAG_SHADER});
	shader->load({"text", TEXT_VERT_SHADER, TEXT_FRAG_SHADER});
	shader->load({"draw", "shaders/draw.vs", "shaders/draw.fs"});
	shader->get("text")->use();
	shader->get("text")->setInt("tex0", 0);
}

std::string	getFPSString()
{
	std::stringstream	strs;
	strs << (int)(1.0f / WINDOW->_deltaTime) << " fps";

	std::string	str = strs.str();
	return (str);
}

void	drawUI()
{
	glDisable(GL_DEPTH_TEST);
	INTERFACES_MANAGER->draw();

	static std::string	fps;

	if ((int)WINDOW->_lastFrame != (int)WINDOW->_currentFrame)
		fps = getFPSString();
	FONT->putString(fps, *SHADER_MANAGER->get("text"),
		vec2(SCREEN_WIDTH - fps.length() * 15, 15 * 0),
		vec2(fps.length() * 15, 15));

	glEnable(GL_DEPTH_TEST);
}

void	update(ShaderManager *shaders)
{
	Shader	*textShader = shaders->get("text");
	textShader->use();
	textShader->setFloat("time", glfwGetTime());
	textShader->setFloat("SCREEN_WIDTH", SCREEN_WIDTH);
	textShader->setFloat("SCREEN_HEIGHT", SCREEN_HEIGHT);
	textShader->setVec3("color", vec3(0.5, 1, 0.5));
	textShader->setBool("rainbow", false);
}

unsigned int	PARTICLES_COUNT = 0;

unsigned int	posBuf = 0;
unsigned int	velBuf = 0;
unsigned int	posVAO = 0;

void	loadParticles(unsigned int count)
{
	glUseProgram(LOAD_SHADER->ID);
	glUniform1f(glGetUniformLocation(LOAD_SHADER->ID, "oldParticleCount"), PARTICLES_COUNT);
	
	PARTICLES_COUNT += count;
	
	unsigned int size = PARTICLES_COUNT * sizeof(vec4);
	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

	glUseProgram(LOAD_SHADER->ID);
	glDispatchCompute(PARTICLES_COUNT, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void	initBuffers()
{
	glCreateBuffers(1, &posBuf);
	glCreateBuffers(1, &velBuf);

	unsigned int size = PARTICLES_COUNT * sizeof(vec4);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &posVAO);
	glBindVertexArray(posVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	loadParticles(10000000);
}

void	render()
{
	glUseProgram(COMPUTE_SHADER->ID);
	glUniform1f(glGetUniformLocation(COMPUTE_SHADER->ID, "time"), glfwGetTime());
	
	glDispatchCompute(PARTICLES_COUNT, 1, 1);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader* particleShader = SHADER_MANAGER->get("draw");
	CAMERA->setViewMatrix(*particleShader);

	glBindVertexArray(posVAO);
	glPointSize(2.0f);
	glDrawArrays(GL_POINTS, 0, PARTICLES_COUNT);
	glBindVertexArray(0);
}

void	frame_key_hook(Window &window)
{
	float cameraSpeed = 15.0f * window.getDeltaTime();
	float	speedBoost = 1.0f;

	if (glfwGetKey(window.getWindowData(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		loadParticles(100);
		speedBoost = 20.0f;
	}
	
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_W) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos + CAMERA->front * (cameraSpeed * speedBoost);
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_S) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos - CAMERA->front * (cameraSpeed * speedBoost);
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_SPACE) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos + CAMERA->up * (cameraSpeed * speedBoost);
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos - CAMERA->up * (cameraSpeed * speedBoost);
		
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_A) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos -  CAMERA->front.cross(CAMERA->up).normalize() * (cameraSpeed * speedBoost);
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_D) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos + CAMERA->front.cross(CAMERA->up).normalize() * (cameraSpeed * speedBoost);
		
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_LEFT) == GLFW_PRESS)
		CAMERA->yaw -= (10.0f * cameraSpeed) * 1.f;
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_RIGHT) == GLFW_PRESS)
		CAMERA->yaw += (10.0f * cameraSpeed) * 1.f;
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_UP) == GLFW_PRESS)
		CAMERA->pitch += (10.0f * cameraSpeed) * 1.f;
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_DOWN) == GLFW_PRESS)
		CAMERA->pitch -= (10.0f * cameraSpeed) * 1.f;

	if (CAMERA->pitch > 89.0f)
		CAMERA->pitch = 89.0f;
	if (CAMERA->pitch < -89.0f)
		CAMERA->pitch = -89.0f;
}

int	main(void)
{
	try {
		Window	window;
		WINDOW = &window;
	
		Camera	camera;
		CAMERA = &camera;		

		Font	font;
		FONT = &font;

		ShaderManager	shaders;
		SHADER_MANAGER = &shaders;
		build(SHADER_MANAGER);
		
		TextureManager	textures;
		TEXTURE_MANAGER = &textures;

		InterfaceManager	interfaces;
		INTERFACES_MANAGER = &interfaces;
		build(INTERFACES_MANAGER);

		ComputeShader	computeShader("shaders/particles.cs");
		ComputeShader	loadShader("shaders/load_particles.cs");

		COMPUTE_SHADER = &computeShader;
		LOAD_SHADER = &loadShader;
		(void)computeShader;

		initBuffers();
		
		glfwSwapInterval(0);

		while (WINDOW->up())
		{
			WINDOW->loopStart();
			CAMERA->update();
			INTERFACES_MANAGER->update();
			update(SHADER_MANAGER);

			render();

			drawUI();

			frame_key_hook(*WINDOW);
			WINDOW->loopEnd();
		}
		glDeleteVertexArrays(1, &posVAO);
		glDeleteBuffers(1, &posBuf);
		glDeleteBuffers(1, &velBuf);
	} catch (const std::exception &e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return (1);
	}
}
