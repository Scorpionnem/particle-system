/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:19:20 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/13 16:45:23 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InterfaceManager.hpp"

extern InterfaceManager	*INTERFACES_MANAGER;
extern Window			*WINDOW;

extern void	loadParticles(unsigned int count);

void	buildMainInterface()
{
	Interface	*current = INTERFACES_MANAGER->get(MAIN_INTERFACE);

	extern glm::vec3	NEAR_COLOR;
	extern glm::vec3	FAR_COLOR;
	extern float MAX_PARTICLE_SIZE;
	extern float GRADIENT_SCALE;
	
	Interface	*main = INTERFACES_MANAGER->get(MAIN_INTERFACE);

	main->onUpdate = [](void *data)
	{
		(void)data;
	};
	current->elements.push_back(new TextBox(UIAnchor::BOTTOM_RIGHT, "", glm::vec2(0, -130), glm::vec2(150, 30), [](std::string &input){MAX_PARTICLE_SIZE = std::atoi(input.c_str());}));
	current->elements.push_back(new Text(UIAnchor::BOTTOM_RIGHT, "max particle size", glm::vec2(-150, -130), glm::vec2(150, 30)));
	current->elements.push_back(new TextBox(UIAnchor::BOTTOM_RIGHT, "", glm::vec2(0, -100), glm::vec2(150, 30), [](std::string &input){GRADIENT_SCALE = std::atoi(input.c_str());}));
	current->elements.push_back(new Text(UIAnchor::BOTTOM_RIGHT, "gradient scale", glm::vec2(-150, -100), glm::vec2(150, 30)));
	current->elements.push_back(new TextBox(UIAnchor::BOTTOM_RIGHT, "", glm::vec2(0, -160), glm::vec2(150, 30), [](std::string &input){loadParticles(std::atoi(input.c_str()));}));
	current->elements.push_back(new Text(UIAnchor::BOTTOM_RIGHT, "particles count", glm::vec2(-150, -160), glm::vec2(150, 30)));
}

	// current->elements.push_back(new Text(UIAnchor::BOTTOM_RIGHT, "far color", glm::vec2(-150, -30), glm::vec2(150, 30)));
	// main->addSlider(UIAnchor::BOTTOM_RIGHT, "red", glm::vec2(0, -40), glm::vec2(150, 10), [](float value, void *data){(void)data;FAR_COLOR.x = value;}, NULL);
	// main->addSlider(UIAnchor::BOTTOM_RIGHT, "green", glm::vec2(0, -30), glm::vec2(150, 10), [](float value, void *data){(void)data;FAR_COLOR.y = value;}, NULL);
	// main->addSlider(UIAnchor::BOTTOM_RIGHT, "blue", glm::vec2(0, -20), glm::vec2(150, 10), [](float value, void *data){(void)data;FAR_COLOR.z = value;}, NULL);
	// current->elements.push_back(new Text(UIAnchor::BOTTOM_RIGHT, "near color", glm::vec2(-150, -60), glm::vec2(150, 30)));
	// main->addSlider(UIAnchor::BOTTOM_RIGHT, "red", glm::vec2(0, -70), glm::vec2(150, 10), [](float value, void *data){(void)data;FAR_COLOR.x = value;}, NULL);
	// main->addSlider(UIAnchor::BOTTOM_RIGHT, "green", glm::vec2(0, -60), glm::vec2(150, 10), [](float value, void *data){(void)data;FAR_COLOR.y = value;}, NULL);
	// main->addSlider(UIAnchor::BOTTOM_RIGHT, "blue", glm::vec2(0, -50), glm::vec2(150, 10), [](float value, void *data){(void)data;FAR_COLOR.z = value;}, NULL);

// current->elements.push_back(new TextBox(UIAnchor::CENTER, "enter username", glm::vec2(0, 0), glm::vec2(200, 40), [](std::string &input){std::cout << input << std::endl;}));
// inline void	buildMainInterface(InterfaceManager *interfaces)
// {
// 	extern glm::vec3	NEAR_COLOR;
// 	extern glm::vec3	FAR_COLOR;
// 	extern float MAX_PARTICLE_SIZE;
// 	extern float GRADIENT_SCALE;
	
// 	Interface	*main = interfaces->get(MAIN_INTERFACE);

// 	main->onUpdate = [](void *data)
// 	{
// 		(void)data;
// 	};
// 	main->addButton(UIAnchor::TOP_LEFT, "quit", glm::vec2(0, 0), glm::vec2(75, 30), [](void *data){(void)data;glfwSetWindowShouldClose(WINDOW->getWindowData(), true);}, NULL);
// 	main->addSlider(UIAnchor::TOP_LEFT, "red", glm::vec2(75, 0), glm::vec2(100, 10), [](float value, void *data){(void)data;NEAR_COLOR.x = value;}, NULL);
// 	main->addSlider(UIAnchor::TOP_LEFT, "green", glm::vec2(75, 10), glm::vec2(100, 10), [](float value, void *data){(void)data;NEAR_COLOR.y = value;}, NULL);
// 	main->addSlider(UIAnchor::TOP_LEFT, "blue", glm::vec2(75, 20), glm::vec2(100, 10), [](float value, void *data){(void)data;NEAR_COLOR.z = value;}, NULL);
// 	main->addSlider(UIAnchor::TOP_LEFT, "red", glm::vec2(175, 0), glm::vec2(100, 10), [](float value, void *data){(void)data;FAR_COLOR.x = value;}, NULL);
// 	main->addSlider(UIAnchor::TOP_LEFT, "green", glm::vec2(175, 10), glm::vec2(100, 10), [](float value, void *data){(void)data;FAR_COLOR.y = value;}, NULL);
// 	main->addSlider(UIAnchor::TOP_LEFT, "blue", glm::vec2(175, 20), glm::vec2(100, 10), [](float value, void *data){(void)data;FAR_COLOR.z = value;}, NULL);
// 	main->addSlider(UIAnchor::TOP_LEFT, "size", glm::vec2(275, 0), glm::vec2(100, 30), [](float value, void *data){(void)data;MAX_PARTICLE_SIZE = 300 * value;}, NULL);
// 	main->addSlider(UIAnchor::TOP_LEFT, "gradient", glm::vec2(375, 0), glm::vec2(100, 30), [](float value, void *data){(void)data;GRADIENT_SCALE = 300 * value;}, NULL);
// }
