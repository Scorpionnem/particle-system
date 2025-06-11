/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Terminal.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:24:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/05/22 14:17:46 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINAL_HPP
# define TERMINAL_HPP

# include "libs.hpp"

void	terminal_special_keys(GLFWwindow *window, int key, int scancode, int action, int mods);
void	terminal_keyboard_input(GLFWwindow *window, unsigned int key);

extern bool			isTerminalOn;
extern std::string	terminalInput;
extern std::string::iterator	terminalCursor;
extern std::string	terminalReturn;
extern float	terminalReturnTime;

#endif