/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/18 23:42:48 by mbatty           ###   ########.fr       */
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
#include "Particles.hpp"
#include "Texture.hpp"

float	FOV = 65;
float	SCREEN_WIDTH = 1100;
float	SCREEN_HEIGHT = 900;
float	RENDER_DISTANCE = 1000;

bool	F1 = false;
bool	F3 = false;
bool	lock_fps = true;
bool	PAUSED = false;

int currentFPS = 60;

Font				*FONT;
TextureManager		*TEXTURE_MANAGER;
InterfaceManager	*INTERFACES_MANAGER;
Camera				*CAMERA;
Window				*WINDOW;

ShaderManager		*SHADER_MANAGER;
ComputeShader		*COMPUTE_SHADER;
ComputeShader		*LOAD_SHADER;
ComputeShader		*COMPACT_SHADER;
Particles			*MAIN_PARTICLES;

unsigned int		GLOBAL_PARTICLES_COUNT = 0;

bool			CAMERA_3D = false;

float lastX = SCREEN_WIDTH / 2;
float lastY = SCREEN_HEIGHT / 2;

bool	TEXTBOX_FOCUSED = false;

unsigned int PARTICLES_CAPACITY = 0;

int 	LOAD_PARTICLES = 1000;
bool	GRAVITY_CENTER_ACTIVATED = false;
bool	PARTICLE_SHAPE = true;
float	MAX_PARTICLE_SIZE = 30.0;

class	Emitter
{
	public:
		Emitter(glm::vec3 pos): _particles(0)
		{
			_pos = pos;
		}
		~Emitter()
		{	
		}
		void	emit(int count)
		{
			glUseProgram(LOAD_SHADER->ID);
			glUniform1i(glGetUniformLocation(LOAD_SHADER->ID, "EMITTER"), true);
			glUniform3fv(glGetUniformLocation(LOAD_SHADER->ID, "EMITTER_POS"), 1, &_pos.x);
			_particles.loadParticles(count);
			glUniform1i(glGetUniformLocation(LOAD_SHADER->ID, "EMITTER"), false);
			glUseProgram(0);
		}
		void	render()
		{
			_particles.render();
		}
		void	update(bool paused, bool gravityCenterOn, glm::vec3 attractor, float deltaTime)
		{
			_particles.update(paused, gravityCenterOn, attractor, deltaTime);
		}
		Particles	_particles;
		glm::vec3		_pos;
};

std::vector<Emitter *> EMITTERS;

void	keyboard_input(GLFWwindow *window, unsigned int key)
{
	(void)window;
	if (INTERFACES_MANAGER->current)
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
	if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			LOAD_PARTICLES + 1000 < 10000000 ? LOAD_PARTICLES += 1000 : LOAD_PARTICLES = 10000000;
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			LOAD_PARTICLES - 1000 > 0 ? LOAD_PARTICLES -= 1000 : LOAD_PARTICLES = 0;
		}
		else
			MAIN_PARTICLES->loadParticles(LOAD_PARTICLES);
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
		GRAVITY_CENTER_ACTIVATED = !GRAVITY_CENTER_ACTIVATED;
	if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		float	particleSize = MAIN_PARTICLES->getParticleSize();
		ParticleShape	particleShape = MAIN_PARTICLES->getParticleShape();
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			particleSize + 10 < 1000 ? particleSize += 10 : particleSize = 1000;
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			particleSize - 10 > 0 ? particleSize -= 10 : particleSize = 0;
		else
		{
			if (particleShape == CIRCLE)
				particleShape = SQUARE;
			else
				particleShape = CIRCLE;
		}
		MAIN_PARTICLES->setParticleSize(particleSize);
		MAIN_PARTICLES->setParticleShape(particleShape);
	}
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
	{
		F3 = !F3;
		if (F3)
			glfwSwapInterval(0);
		else
			glfwSwapInterval(1);
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		EMITTERS.push_back(new Emitter(CAMERA->pos));
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
	currentFPS = (int)(1.0f / WINDOW->_deltaTime);
	return (std::to_string((int)(1.0f / WINDOW->_deltaTime)) + " fps");
}

void	drawUI()
{
	glDisable(GL_DEPTH_TEST);

	static std::string	fps = "0 fps";
	std::string			particles_count;
	std::string			load_particles;

	if ((int)WINDOW->_lastFrame != (int)WINDOW->_currentFrame)
		fps = getFPSString();
	FONT->putString(fps, *SHADER_MANAGER->get("text"),
		glm::vec2((SCREEN_WIDTH / 2) - (fps.length() * 15) / 2, 0),
		glm::vec2(fps.length() * 15, 15));
	
	particles_count = std::to_string(GLOBAL_PARTICLES_COUNT) + " particles";

	FONT->putString(particles_count, *SHADER_MANAGER->get("text"),
		glm::vec2((SCREEN_WIDTH / 2) - (particles_count.length() * 15) / 2, 16),
		glm::vec2(particles_count.length() * 15, 15));
	
	glEnable(GL_DEPTH_TEST);
}

void	update(ShaderManager *shaders)
{
	Shader	*textShader = shaders->get("text");
	Shader	*drawShader = shaders->get("draw");
	textShader->use();
	textShader->setFloat("time", glfwGetTime());
	textShader->setFloat("SCREEN_WIDTH", SCREEN_WIDTH);
	textShader->setFloat("SCREEN_HEIGHT", SCREEN_HEIGHT);
	drawShader->use();
	drawShader->setFloat("time", glfwGetTime());
}

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
}

void	move_mouse_hook(GLFWwindow* window, double xpos, double ypos)
{
	(void)window;
	if (!CAMERA_3D)
		return ;

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

void	printGuide()
{
	std::cout
	<< "--- particle-system ---\n"
	<< "Left Click      Sets main gravity center on cursor\n"
	<< "L               Load particles\n"
	<< "Up/Down + L     increase/decrease amount of particles loaded using L\n"
	<< "C               Toggles camera view\n"
	<< "WASD            Move camera around\n"
	<< "Ctrl            Increases camera speed\n"
	<< "M               Changes color mode\n"
	<< "G               Toggle gravity center\n"
	<< "P               Pause the simulation\n"
	<< "Q               Changes particle shape\n"
	<< "Up/Down + Q     Increase/decrease size of particles\n"
	<< "F3              Unlock FPS"
	<< std::endl;
}

struct	Engine
{
	Engine()
	{
		WINDOW = &this->window;
		CAMERA = &this->camera;
		FONT = &this->font;
		SHADER_MANAGER = &this->shaderManager;
		build(SHADER_MANAGER);
		TEXTURE_MANAGER = &this->textureManager;
		INTERFACES_MANAGER = &this->InterfaceManager;
		build(INTERFACES_MANAGER);
	}
	Window				window;
	Camera				camera;
	Font				font;
	ShaderManager		shaderManager;
	TextureManager		textureManager;
	InterfaceManager	InterfaceManager;
};

void	render()
{
	MAIN_PARTICLES->render();

	for (auto *emitter : EMITTERS)
		emitter->render();

	drawUI();
}

void	update()
{
	static int frame = 0;
	static glm::vec3	attractor(0);
	bool	leftMouseClicked;
	GLOBAL_PARTICLES_COUNT = 0;

	leftMouseClicked = glfwGetMouseButton(WINDOW->getWindowData(), GLFW_MOUSE_BUTTON_1);
	if (!CAMERA_3D)
	{
		double	mouseX, mouseY;
		glfwGetCursorPos(WINDOW->getWindowData(), &mouseX, &mouseY);
		if (leftMouseClicked)
			attractor = getGravityCenterFromMouseFixedDepth(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT, glm::perspective(glm::radians(FOV), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, RENDER_DISTANCE), CAMERA->getViewMatrix(), CAMERA->pos, 100.0f);
	}
	MAIN_PARTICLES->update(PAUSED, GRAVITY_CENTER_ACTIVATED, attractor, WINDOW->getDeltaTime());
	MAIN_PARTICLES->setAttractor(attractor);
	for (auto *emitter : EMITTERS)
	{
		if (GRAVITY_CENTER_ACTIVATED)
			emitter->emit(100);
		emitter->update(PAUSED, GRAVITY_CENTER_ACTIVATED, attractor, WINDOW->getDeltaTime());
	}

	frame++;
	if (frame >= currentFPS)
	{
		for (auto *emitter : EMITTERS)
			emitter->_particles.compactParticles();
		frame = 0;
	}
}

int	main(int ac, char **av)
{
	if (ac > 2) {
		std::cout << "Error\nToo many arguments" << std::endl;
		return (1);
	}
	int	count = 0;
	
	ac == 2 ? count = std::atoi(av[1]) : count = 100000;
	if (count > 10000000 || count < 0) {
		std::cout << "Error\nHEY! Don't you try to crash my PC! (Too many/few particles)" << std::endl;
		return (1);
	}
	printGuide();
	try {
		Engine	engine;

		ComputeShader	loadShader("shaders/load_particles.cs");
		ComputeShader	computeShader("shaders/particles.cs");
		ComputeShader	compactShader("shaders/compact.cs");
		COMPUTE_SHADER = &computeShader;
		LOAD_SHADER = &loadShader;
		COMPACT_SHADER = &compactShader;

		Particles	mainParticles(count);
		MAIN_PARTICLES = &mainParticles;

		CAMERA->pos.z = 50;

		while (WINDOW->up())
		{
			WINDOW->loopStart();
			CAMERA->update();
			INTERFACES_MANAGER->update();
			update(SHADER_MANAGER);

			update();

			render();

			frame_key_hook(*WINDOW);
			WINDOW->loopEnd();
		}
	} catch (const std::exception &e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return (1);
	}
}
