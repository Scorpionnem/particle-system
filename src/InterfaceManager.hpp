/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InterfaceManager.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 22:00:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/12 14:42:08 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACEMANAGER_HPP
# define INTERFACEMANAGER_HPP

# include "Interface.hpp"
# include "Window.hpp"

extern Window	*WINDOW;

enum InterfaceID
{
	MAIN_INTERFACE,
	MAX_INTERFACE //Not used as an interface id, its only to create the array of interfaces in InterfaceManager
};

class	InterfaceManager
{
	public:
		InterfaceManager()
		{
			for (int i = 0; i < MAX_INTERFACE; i++)
				interfaces[i] = new Interface();
			current = interfaces[MAIN_INTERFACE];
		}
		~InterfaceManager()
		{
			for (auto *interface : interfaces)
				delete interface;
		}
		void	update()
		{
			if (current)
				current->update();
		}
		void	draw()
		{
			if (current)
				current->draw();
		}
		Interface *get(const InterfaceID id)
		{
			if (id >= MAX_INTERFACE)
				return (NULL);
			return (interfaces[id]);
		}
		std::array<Interface *, MAX_INTERFACE>	interfaces;
		Interface								*current;
};

extern InterfaceManager	*INTERFACES_MANAGER;

inline void	gotoInterface(void *data)
{
	INTERFACES_MANAGER->current = static_cast<Interface *>(data);
}


inline void	buildMainInterface(InterfaceManager *interfaces)
{
	extern vec3	NEAR_COLOR;
	extern vec3	FAR_COLOR;
	extern float MAX_PARTICLE_SIZE;
	extern float GRADIENT_SCALE;
	
	Interface	*main = interfaces->get(MAIN_INTERFACE);

	main->onUpdate = [](void *data)
	{
		(void)data;
	};
	main->addButton(UIAnchor::TOP_LEFT, "quit", vec2(0, 0), vec2(75, 30), [](void *data){(void)data;glfwSetWindowShouldClose(WINDOW->getWindowData(), true);}, NULL);
	main->addSlider(UIAnchor::TOP_LEFT, "red", vec2(75, 0), vec2(100, 10), [](float value, void *data){(void)data;NEAR_COLOR.x = value;}, NULL);
	main->addSlider(UIAnchor::TOP_LEFT, "green", vec2(75, 10), vec2(100, 10), [](float value, void *data){(void)data;NEAR_COLOR.y = value;}, NULL);
	main->addSlider(UIAnchor::TOP_LEFT, "blue", vec2(75, 20), vec2(100, 10), [](float value, void *data){(void)data;NEAR_COLOR.z = value;}, NULL);
	main->addSlider(UIAnchor::TOP_LEFT, "red", vec2(175, 0), vec2(100, 10), [](float value, void *data){(void)data;FAR_COLOR.x = value;}, NULL);
	main->addSlider(UIAnchor::TOP_LEFT, "green", vec2(175, 10), vec2(100, 10), [](float value, void *data){(void)data;FAR_COLOR.y = value;}, NULL);
	main->addSlider(UIAnchor::TOP_LEFT, "blue", vec2(175, 20), vec2(100, 10), [](float value, void *data){(void)data;FAR_COLOR.z = value;}, NULL);
	main->addSlider(UIAnchor::TOP_LEFT, "size", vec2(275, 0), vec2(100, 30), [](float value, void *data){(void)data;MAX_PARTICLE_SIZE = 300 * value;}, NULL);
	main->addSlider(UIAnchor::TOP_LEFT, "gradient", vec2(375, 0), vec2(100, 30), [](float value, void *data){(void)data;GRADIENT_SCALE = 300 * value;}, NULL);
}

inline void	build(InterfaceManager *interfaces)
{
	buildMainInterface(interfaces);
}

#endif
