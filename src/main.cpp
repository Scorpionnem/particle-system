/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/15 14:25:11 by mbatty           ###   ########.fr       */
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
bool	PAUSED = false;

Font				*FONT;
ShaderManager		*SHADER_MANAGER;
TextureManager		*TEXTURE_MANAGER;
InterfaceManager	*INTERFACES_MANAGER;
Camera				*CAMERA;
Window				*WINDOW;

ComputeShader	*COMPUTE_SHADER;
ComputeShader	*LOAD_SHADER;

bool			CAMERA_3D = false;

float lastX = SCREEN_WIDTH / 2;
float lastY = SCREEN_HEIGHT / 2;

bool	TEXTBOX_FOCUSED = false;

void	keyboard_input(GLFWwindow *window, unsigned int key)
{
	(void)window;
	INTERFACES_MANAGER->current->charInput(key);
}

void	key_hook(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)window;(void)key;(void)scancode;(void)action;(void)mods;

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
		INTERFACES_MANAGER->current->specialInput(key);

	if (TEXTBOX_FOCUSED)
		return ;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		PAUSED = !PAUSED;
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		lastX = SCREEN_WIDTH / 2;
		lastY = SCREEN_HEIGHT / 2;
		CAMERA_3D = !CAMERA_3D;
		if (CAMERA_3D == false)
			glfwSetInputMode(WINDOW->getWindowData(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(WINDOW->getWindowData(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
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
		glm::vec2((SCREEN_WIDTH / 2) - (fps.length() * 15) / 2, 0),
		glm::vec2(fps.length() * 15, 15));

	glEnable(GL_DEPTH_TEST);
}

void	update(ShaderManager *shaders)
{
	Shader	*textShader = shaders->get("text");
	textShader->use();
	textShader->setFloat("time", glfwGetTime());
	textShader->setFloat("SCREEN_WIDTH", SCREEN_WIDTH);
	textShader->setFloat("SCREEN_HEIGHT", SCREEN_HEIGHT);
	textShader->setVec3("color", glm::vec3(0.5, 1, 0.5));
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
	
	PARTICLES_COUNT = count;
	
	unsigned int size = PARTICLES_COUNT * sizeof(glm::vec4);
	
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

	unsigned int size = PARTICLES_COUNT * sizeof(glm::vec4);

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

	loadParticles(1000000);
}

glm::vec3	attractorPos(0, 0, 0);

glm::vec3	NEAR_COLOR(1.0, 1.0, 0.0);
glm::vec3	FAR_COLOR(1.0, 0.0, 0.0);

float	MAX_PARTICLE_SIZE = 30.0;

float	GRADIENT_SCALE = 50.0;

glm::vec3 screenToWorldRay(float mouseX, float mouseY, int screenWidth, int screenHeight, const glm::mat4& projection, const glm::mat4& view)
{
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;
    glm::vec4 rayClip = glm::vec4(x, y, -1.0, 1.0);

    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

    glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));
    return (rayWorld);
}

glm::vec3 getGravityCenterFromMouseFixedDepth(float mouseX, float mouseY, int screenWidth, int screenHeight, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& camPos, float depth)
{
    glm::vec3 rayDir = screenToWorldRay(mouseX, mouseY, screenWidth, screenHeight, projection, view);
    return (camPos + rayDir * depth);
}

void	render()
{
	bool	mouseClicked;

	mouseClicked = glfwGetMouseButton(WINDOW->getWindowData(), GLFW_MOUSE_BUTTON_1);

	if (mouseClicked && !CAMERA_3D)
	{
		double mx, my;
		glfwGetCursorPos(WINDOW->getWindowData(), &mx, &my);
		attractorPos = getGravityCenterFromMouseFixedDepth( mx, my, SCREEN_WIDTH, SCREEN_HEIGHT, glm::perspective(glm::radians(FOV), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, RENDER_DISTANCE), CAMERA->getViewMatrix(), CAMERA->pos, 100.0f );
	}

	glUseProgram(SHADER_MANAGER->get("draw")->ID);	
	SHADER_MANAGER->get("draw")->setVec3("attractorPos", attractorPos);
	SHADER_MANAGER->get("draw")->setVec3("NEAR_COLOR", NEAR_COLOR);
	SHADER_MANAGER->get("draw")->setVec3("FAR_COLOR", FAR_COLOR);
	SHADER_MANAGER->get("draw")->setVec3("viewPos", CAMERA->pos);
	SHADER_MANAGER->get("draw")->setFloat("MAX_PARTICLE_SIZE", MAX_PARTICLE_SIZE);
	SHADER_MANAGER->get("draw")->setFloat("GRADIENT_SCALE", GRADIENT_SCALE);
	
	glUseProgram(COMPUTE_SHADER->ID);
	glUniform1f(glGetUniformLocation(COMPUTE_SHADER->ID, "time"), glfwGetTime());
	glUniform1i(glGetUniformLocation(COMPUTE_SHADER->ID, "mouseClicked"), mouseClicked);
	glUniform3fv(glGetUniformLocation(COMPUTE_SHADER->ID, "attractorPos"), 1, &attractorPos.x);
	glUniform1f(glGetUniformLocation(COMPUTE_SHADER->ID, "deltaTime"), WINDOW->getDeltaTime());

	if (!PAUSED)
	{
		glDispatchCompute(PARTICLES_COUNT, 1, 1);
	
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	Shader* particleShader = SHADER_MANAGER->get("draw");
	CAMERA->setViewMatrix(*particleShader);

	glBindVertexArray(posVAO);
	glDrawArrays(GL_POINTS, 0, PARTICLES_COUNT);
	glBindVertexArray(0);
}

void	frame_key_hook(Window &window)
{
	if (!CAMERA_3D || TEXTBOX_FOCUSED)
		return ;
		
	float cameraSpeed = 15.0f * window.getDeltaTime();
	float	speedBoost = 1.0f;

	if (glfwGetKey(window.getWindowData(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		speedBoost = 20.0f;
	
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_W) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos + CAMERA->front * (cameraSpeed * speedBoost);
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_S) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos - CAMERA->front * (cameraSpeed * speedBoost);
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_SPACE) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos + CAMERA->up * (cameraSpeed * speedBoost);
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos - CAMERA->up * (cameraSpeed * speedBoost);
		
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_A) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos - glm::cross(normalize(CAMERA->front), normalize(CAMERA->up)) * (cameraSpeed * speedBoost);
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_D) == GLFW_PRESS)
		CAMERA->pos = CAMERA->pos + glm::cross(normalize(CAMERA->front), normalize(CAMERA->up)) * (cameraSpeed * speedBoost);
		
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

void	move_mouse_hook(GLFWwindow* window, double xpos, double ypos)
{
	if (!CAMERA_3D)
		return ;
	(void)window;
	(void)xpos;
	(void)ypos;

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	CAMERA->yaw += xoffset;
	CAMERA->pitch += yoffset;
	
	if(CAMERA->pitch > 89.0f)
		CAMERA->pitch = 89.0f;
	if(CAMERA->pitch < -89.0f)
		CAMERA->pitch = -89.0f;
}

int	main(void)
{
	try {
		Window	window;
		WINDOW = &window;
	
		glfwSetInputMode(WINDOW->getWindowData(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(WINDOW->getWindowData(), move_mouse_hook);

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
		
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		CAMERA->pos.z = 50;

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
