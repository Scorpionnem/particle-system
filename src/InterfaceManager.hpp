/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InterfaceManager.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 22:00:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/13 16:26:14 by mbatty           ###   ########.fr       */
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

void	buildMainInterface();

inline void	build(InterfaceManager *interfaces)
{
	(void)interfaces;
	buildMainInterface();
}

#endif
