/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 22:01:22 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/13 16:21:28 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_HPP
# define INTERFACE_HPP

# include "libs.hpp"
# include "UIElement.hpp"
# include "Window.hpp"

extern Window	*WINDOW;

class	Interface
{
	public:
		Interface(){}
		~Interface()
		{
			for (auto *element : elements)
				delete element;
		}

		void	addButton(UIAnchor anchor, std::string label, glm::vec2 offset, glm::vec2 size, std::function<void(void*)> onClick, void *clickData)
		{
			elements.push_back(new Button(anchor, label, offset, size, onClick, clickData));
		}
		void	addButton(std::string label, glm::vec2 pos, glm::vec2 size, std::function<void(void*)> onClick, void *clickData)
		{
			elements.push_back(new Button(label, pos, size, onClick, clickData));
		}
		void	addSlider(UIAnchor anchor, std::string label, glm::vec2 offset, glm::vec2 size, std::function<void(float, void*)> onClick, void *clickData)
		{
			elements.push_back(new Slider(anchor, label, offset, size, onClick, clickData));
		}
		void	addSlider(std::string label, glm::vec2 pos, glm::vec2 size, std::function<void(float, void*)> onClick, void *clickData)
		{
			elements.push_back(new Slider(label, pos, size, onClick, clickData));
		}

		void	draw()
		{
			if (onDraw)
				onDraw(drawData);
			for (auto *element : elements)
				element->draw();
		}
		void	update()
		{
			if (onUpdate)
				onUpdate(updateData);
			double mouseX, mouseY;
			bool mousePressed = glfwGetMouseButton(WINDOW->getWindowData(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
			glfwGetCursorPos(WINDOW->getWindowData(), &mouseX, &mouseY);
			for (auto *element : elements)
				element->update(glm::vec2(mouseX, mouseY), mousePressed);
		}
		void	charInput(char c)
		{
			for (auto *element : elements)
			{
				if (TextBox *current = dynamic_cast<TextBox *>(element))
				{
					current->onCharInput(c);
				}
			}
		}
		void	specialInput(int key)
		{
			for (auto *element : elements)
			{
				if (TextBox *current = dynamic_cast<TextBox *>(element))
				{
					current->onSpecialInput(key);
				}
			}
		}
		std::vector<UIElement *>	elements;
		std::function<void(void*)>	onUpdate = NULL;
		std::function<void(void*)>	onDraw = NULL;
		void						*updateData = NULL;
		void						*drawData = NULL;
};

#endif
