/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/25 11:56:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"
#include "Font.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "ComputeShader.hpp"
#include "Particles.hpp"
#include "Texture.hpp"
#include "Emitter.hpp"
#include "Skybox.hpp"
#include "EmittersManager.hpp"

float	FOV = 65;
float	SCREEN_WIDTH = 1100;
float	SCREEN_HEIGHT = 900;
float	RENDER_DISTANCE = 1000;

bool	F3 = false;
bool	PAUSED = false;
bool	SKYBOX_ACTIVE = false;

int		currentFPS = 60;

Font				*FONT;
Window				*WINDOW;
Camera				*CAMERA;
Skybox				*SKYBOX;

TextureManager		*TEXTURE_MANAGER;
EmittersManager		*EMITTERS;

ShaderManager		*SHADER_MANAGER;
ComputeShader		*COMPACT_SHADER;
ComputeShader		*COMPUTE_SHADER;
ComputeShader		*LOAD_SHADER;

Particles			*MAIN_PARTICLES;

unsigned int		GLOBAL_PARTICLES_COUNT = 0;

bool				CAMERA_3D = false;

float				lastX = SCREEN_WIDTH / 2;
float				lastY = SCREEN_HEIGHT / 2;

int 				LOAD_PARTICLES = 1000;
bool				GRAVITY_CENTER_ACTIVATED = false;
bool				PARTICLE_SHAPE = true;
float				MAX_PARTICLE_SIZE = 30.0;
bool				VELOCITY_COLOR = false;

enum particlesColor
{
	RED_YELLOW,
	PURPLE_PINK,
	GREEN_BLUE,
	BLACK_WHITE,
	MAX_COLOR
};

particlesColor	CURRENT_COLOR_SET;

void	keyboard_input(GLFWwindow *window, unsigned int key)
{
	(void)window;(void)key;
}

void	key_hook(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)window;(void)key;(void)scancode;(void)action;(void)mods;

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
		EMITTERS->addEmitter();
	if (key == GLFW_KEY_V && (action == GLFW_PRESS || action == GLFW_REPEAT))
		VELOCITY_COLOR = !VELOCITY_COLOR;
	if (key == GLFW_KEY_M && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		int tmp = static_cast<int>(CURRENT_COLOR_SET);
		tmp++;
		if (tmp == particlesColor::MAX_COLOR)
			tmp = particlesColor::RED_YELLOW;
		if (tmp == particlesColor::RED_YELLOW)
		{
			MAIN_PARTICLES->setNearColor(glm::vec3(1.0, 1.0, 0.0));
			MAIN_PARTICLES->setFarColor(glm::vec3(1.0, 0.0, 0.0));
		}
		else if (tmp == particlesColor::GREEN_BLUE)
		{
			MAIN_PARTICLES->setNearColor(glm::vec3(0.0, 1.0, 1.0));
			MAIN_PARTICLES->setFarColor(glm::vec3(0.4, 1.0, 0.0));
		}
		else if (tmp == particlesColor::PURPLE_PINK)
		{
			MAIN_PARTICLES->setNearColor(glm::vec3(1.0, 0.0, 1.0));
			MAIN_PARTICLES->setFarColor(glm::vec3(0.5, 0.0, 1.0));
		}
		else if (tmp == particlesColor::BLACK_WHITE)
		{
			MAIN_PARTICLES->setNearColor(glm::vec3(1.0, 1.0, 1.0));
			MAIN_PARTICLES->setFarColor(glm::vec3(0.3, 0.3, 0.3));
		}
		CURRENT_COLOR_SET = static_cast<particlesColor>(tmp);
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
		SKYBOX_ACTIVE = !SKYBOX_ACTIVE;
}

void	build(ShaderManager *shader)
{
	shader->load({"text", TEXT_VERT_SHADER, TEXT_FRAG_SHADER});
	shader->load({"draw", "shaders/draw.vs", "shaders/draw.fs"});
	shader->load({"skybox", SKYBOX_VERT_SHADER, SKYBOX_FRAG_SHADER});
	shader->get("text")->use();
	shader->get("text")->setInt("tex0", 0);
	shader->get("skybox")->use();
	shader->get("skybox")->setInt("skybox", 0);
}

std::string	getFPSString()
{
	currentFPS = (int)(1.0f / WINDOW->_deltaTime);
	return (std::to_string(currentFPS) + " fps");
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

glm::vec3 getGravityCenter(float mouseX, float mouseY, int screenWidth, int screenHeight, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& camPos, float depth)
{
    glm::vec3 rayDir = screenToWorldRay(mouseX, mouseY, screenWidth, screenHeight, projection, view);
    return (camPos + rayDir * depth);
}

void	frame_key_hook(Window &window)
{
	if (!CAMERA_3D)
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
	<< "F3              Unlock FPS\n"
	<< "V               Visualize velocity as color\n"
	<< "J               Toggle skybox"
	<< std::endl;
}

struct	Engine //Simple struct to make the main more light (Just initiates all of the global variables)
{
	Engine()
	{
		WINDOW = &this->window;
		CAMERA = &this->camera;
		FONT = &this->font;
		SHADER_MANAGER = &this->shaderManager;
		build(SHADER_MANAGER);
		TEXTURE_MANAGER = &this->textureManager;
	}
	Window				window;
	Camera				camera;
	Font				font;
	ShaderManager		shaderManager;
	TextureManager		textureManager;
};

void	render()
{
	if (SKYBOX_ACTIVE)
		SKYBOX->draw(*CAMERA, *SHADER_MANAGER->get("skybox"));

	MAIN_PARTICLES->render();

	EMITTERS->render();
	
	drawUI();
}

void	tryCompactEmitters()
{
	static int frame = 0;
	
	frame++;
	if (frame >= currentFPS)
	{
		EMITTERS->compact();
		frame = 0;
	}
}

void	update()
{
	static glm::vec3	attractor(0);
	static glm::vec3	secondaryAttractor(0);
	static glm::vec3	mousePos;
	bool	leftMouseClicked;
	bool	rightMouseClicked;

	SHADER_MANAGER->get("draw")->setBool("VELOCITY_COLOR", VELOCITY_COLOR);

	if (!PAUSED)
		GLOBAL_PARTICLES_COUNT = 0;

	leftMouseClicked = glfwGetMouseButton(WINDOW->getWindowData(), GLFW_MOUSE_BUTTON_1);
	rightMouseClicked = glfwGetMouseButton(WINDOW->getWindowData(), GLFW_MOUSE_BUTTON_2);
	
	if (!CAMERA_3D)
	{
		double	mouseX, mouseY;
		glfwGetCursorPos(WINDOW->getWindowData(), &mouseX, &mouseY);
		mousePos = getGravityCenter(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT, glm::perspective(glm::radians(FOV), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, RENDER_DISTANCE), CAMERA->getViewMatrix(), CAMERA->pos, 100.0f);
		if (leftMouseClicked)
		{
			attractor = mousePos;
			secondaryAttractor = attractor;
		}
		else if (rightMouseClicked)
			secondaryAttractor = mousePos;
	}

	SHADER_MANAGER->get("draw")->setVec3("MOUSE_POS", mousePos);

	MAIN_PARTICLES->update(PAUSED, GRAVITY_CENTER_ACTIVATED, attractor, secondaryAttractor, WINDOW->getDeltaTime());
	EMITTERS->update(attractor, secondaryAttractor);

	tryCompactEmitters();
}

void	glBugReport(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	(void)userParam;
	
	if (type == GL_DEBUG_TYPE_PERFORMANCE || severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		// std::cout << "---WARNING---" << std::endl;
		return ;
	else
		std::cout << "---ERROR---" << std::endl;
	std::cout << "source: " << source << std::endl;
	std::cout << "type: " << type << std::endl;
	std::cout << "id: " << id << std::endl;
	std::cout << "severity: " << severity << std::endl;
	std::cout << "length: " << length << std::endl;
	std::cout << "message: " << message << std::endl;
	std::cout << "-----------" << std::endl;
}

/*

Manda:
DONE	OpenGL 4.x (Using OpenGL 4.3)
DONE	Never allocate on RAM
DONE	1 Million 60FPS (500FPS)
DONE	3 Million 20FPS (250FPS)
DONE~	Interoperability (glMemoryBarrier is a memory synchronization function)
DONE	Print FPS on window
DONE	Color based on distance to cursor

Bonuses:
DONE	-Changing particles shape
DONE	-Changing particles size
DONE	-Particles fade out and go smaller as they die
DONE	-Show velocity as color mode
DONE	-Different color sets (green blue, red yellow, purple pink, gray white)
DONE	-2 attraction points
DONE	Skybox

For evaluation:
- Use nvidia-smi to check VRAM usage
- Use htop to check RAM usage
*/

#include <csignal>

void	handleSIGINT(int)
{
	glfwSetWindowShouldClose(WINDOW->getWindowData(), true);
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
	std::signal(SIGINT, handleSIGINT);
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

		EmittersManager	emitters;
		EMITTERS = &emitters;

		Skybox	skybox({SKYBOX_PATHES});
		SKYBOX = &skybox;

		CAMERA->pos.z = 50;

		// glEnable(GL_DEBUG_OUTPUT); //Uncomment to have GL's debug output (Errors)
		// glDebugMessageCallback(glBugReport, NULL);

		while (WINDOW->up())
		{
			WINDOW->loopStart();
			CAMERA->update();
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
