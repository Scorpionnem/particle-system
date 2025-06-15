/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIElement.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 12:42:04 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/13 16:25:29 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UIELEMENT_HPP
# define UIELEMENT_HPP

# include "libs.hpp"
# include "TextureManager.hpp"
# include "ShaderManager.hpp"
# include "Font.hpp"

# define DEFAULT_BUTTON_TEXTURE BUTTON_PATH
# define DEFAULT_BUTTON_CLICK_TEXTURE BUTTON_PRESSED_PATH
# define DEFAULT_SLIDER_BACKGROUND_TEXTURE SLIDER_BG_PATH

# define DEFAULT_BUTTON_SHADER "gui"

extern unsigned int	quadVAO;
extern unsigned int	quadVBO;

extern float	SCREEN_WIDTH;
extern float	SCREEN_HEIGHT;

enum UIAnchor
{
	NONE, //Not anchored
	TOP_LEFT, //Anchored on the top left of the screen
	TOP_RIGHT, //Anchored on the top right of the screen
	TOP_CENTER, //Anchored on the top center of the screen
	CENTER, //Anchored on the center of the screen
	CENTER_LEFT, //Anchored on the center left of the screen
	CENTER_RIGHT, //Anchored on the center right of the screen
	BOTTOM_LEFT, //Anchored on the bottom left of the screen
	BOTTOM_RIGHT, //Anchored on the bottom left of the screen
	BOTTOM_CENTER, //Anchored on the bottom left of the screen
	CENTER_HALF_LEFT, //Anchored on the half left of the screen
	CENTER_HALF_RIGHT //Anchored on the half right of the screen
};

class	UIElement
{
	public:
		UIElement(){}
		virtual ~UIElement(){}
		/*
			Draws the button, expects GL_DEPTH_TEST to be disabled so please use this function in drawUI
		*/
		virtual void	draw() = 0;
		/*
			@brief update button's variables

			Updates button's variables like its current used texture (normal/pressed)
		*/
        virtual void	update(glm::vec2 mousePos, bool mousePressed) = 0;
		/*
			Returns wether the mouse is in the button or not
		*/
		bool	isInside(glm::vec2 buttonPos, glm::vec2 size, glm::vec2 mousePos)
		{
			return mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + size.x && mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + size.y;
		}
		/*
			Simple function to init the quad "model" used for all buttons
			If the quad is already loaded, just returns
		*/
		void	initButtonQuad()
		{
			if (quadVAO != 0)
				return;
		
			if (DEBUG)
				std::cout << "Loading simple quad" << std::endl;
		
			float vertices[] =
			{
				0.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f
			};
		
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
		
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		
			glBindVertexArray(0);
		}
		void	anchorPos()
		{
			if (this->anchor == UIAnchor::TOP_CENTER)
			{
				this->pos.x = SCREEN_WIDTH / 2 - this->size.x / 2 + this->offset.x;
				this->pos.y = 0 + this->offset.y;
			}
			else if (this->anchor == UIAnchor::TOP_LEFT)
			{
				this->pos.x = 0 + this->offset.x;
				this->pos.y = 0 + this->offset.y;
			}
			else if (this->anchor == UIAnchor::TOP_RIGHT)
			{
				this->pos.x = SCREEN_WIDTH - this->size.x + this->offset.x;
				this->pos.y = 0 + this->offset.y;
			}
			else if (this->anchor == UIAnchor::CENTER)
			{
				this->pos.x = SCREEN_WIDTH / 2 - this->size.x / 2 + this->offset.x;
				this->pos.y = SCREEN_HEIGHT / 2 - this->size.y / 2 + this->offset.y;
			}
			else if (this->anchor == UIAnchor::CENTER_LEFT)
			{
				this->pos.x = 0 + this->offset.x;
				this->pos.y = SCREEN_HEIGHT / 2 - this->size.y / 2 + this->offset.y;
			}
			else if (this->anchor == UIAnchor::CENTER_RIGHT)
			{
				this->pos.x = SCREEN_WIDTH - this->size.x + this->offset.x;
				this->pos.y = SCREEN_HEIGHT / 2 - this->size.y / 2 + this->offset.y;
			}
			else if (this->anchor == UIAnchor::BOTTOM_CENTER)
			{
				this->pos.x = SCREEN_WIDTH / 2 - this->size.x / 2 + this->offset.x;
				this->pos.y = SCREEN_HEIGHT - this->size.y + this->offset.y;
			}
			else if (this->anchor == UIAnchor::BOTTOM_LEFT)
			{
				this->pos.x = 0 + this->offset.x;
				this->pos.y = SCREEN_HEIGHT - this->size.y + this->offset.y;
			}
			else if (this->anchor == UIAnchor::BOTTOM_RIGHT)
			{
				this->pos.x = SCREEN_WIDTH - this->size.x + this->offset.x;
				this->pos.y = SCREEN_HEIGHT - this->size.y + this->offset.y;
			}
			else if (this->anchor == UIAnchor::CENTER_HALF_LEFT)
			{
				this->pos.x = (SCREEN_WIDTH * 0.25) - this->size.x / 2 + this->offset.x;
				this->pos.y = SCREEN_HEIGHT / 2 - this->size.y / 2 + this->offset.y;
			}
			else if (this->anchor == UIAnchor::CENTER_HALF_RIGHT)
			{
				this->pos.x = (SCREEN_WIDTH * 0.75) - this->size.x / 2 + this->offset.x;
				this->pos.y = SCREEN_HEIGHT / 2 - this->size.y / 2 + this->offset.y;
			}
		}

		glm::vec2						offset;
		glm::vec2						pos;
		glm::vec2						size;
		UIAnchor					anchor = UIAnchor::NONE;
};

/*
	@brief	Represents a simple clickable UI element

	When initialized the button will use basic textures/shaders from the Texture/ShaderManager, use the setter functions to use custom shaders
*/
class	Button : public UIElement
{
	public:
		/*
			@brief	Simple button

			This button will be fixed on its pos, use a anchored constructor to have
			your button anchored even on resize of the screen.

			@param	label       Label of the button to be displayed
			@param	pos         Position of the button on screen
			@param	size        Size of the button
			@param	onClick     function to be called when button is clicked
			@param	clickData   Data passed to the onClick function
		*/
		Button(std::string label, glm::vec2 pos, glm::vec2 size, std::function<void(void*)> onClick, void *clickData)
		{
			this->label = label;
			this->pos = pos;
			this->size = size;
			this->onClick = onClick;
			this->clickData = clickData;

			loadDefaultAssets();
		}
		/*
			@brief	Simple anchored button

			Constructor for anchored buttons, the pos argument will be added to the anchor point so
			use positive/negative values to move the button around its anchor

			@param	anchor      Where the button should be anchored
			@param	label       Label of the button to be displayed
			@param	offset      Offset from anchor
			@param	size        Size of the button
			@param	onClick     function to be called when button is clicked, NULL to do nothing
			@param	clickData   Data passed to the onClick function
		*/
		Button(UIAnchor anchor, std::string label, glm::vec2 offset, glm::vec2 size, std::function<void(void*)> onClick, void *clickData)
		{
			this->label = label;
			this->offset = offset;
			this->pos = glm::vec2(0);
			this->size = size;
			this->onClick = onClick;
			this->clickData = clickData;
			this->anchor = anchor;

			loadDefaultAssets();
		}
		~Button(){}
		void	loadDefaultAssets()
		{
			this->texture = TEXTURE_MANAGER->get(DEFAULT_BUTTON_TEXTURE);
			this->clickTexture = TEXTURE_MANAGER->get(DEFAULT_BUTTON_CLICK_TEXTURE);

			this->fontShader = SHADER_MANAGER->get("text");
			this->buttonShader = SHADER_MANAGER->get("gui");
			this->font = FONT;
		}
		// Sets custom button assets, if argument is NULL, asset wont be set
		void	setAssets(Texture *texture, Texture *clickTexture, Shader *buttonShader, Shader *fontShader, Font *font)
		{
			if (texture)
				this->texture = texture;
			if (clickTexture)
				this->clickTexture = clickTexture;
			if (buttonShader)
				this->buttonShader = buttonShader;
			if (fontShader)
				this->fontShader = fontShader;
			if (font)
				this->font = font;
		}
		
		void	draw()
		{
			if (!this->buttonShader || !this->currentTexture)
				return ;
			
			initButtonQuad();
			
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(this->pos.x, this->pos.y, 0.0f));
			model = glm::scale(model, glm::vec3(this->size.x, this->size.y, 1.0f));
			glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

			this->buttonShader->use();
			this->buttonShader->setMat4("model", model);
			this->buttonShader->setMat4("projection", projection);
			
			this->currentTexture->use();

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			font->putString(this->label, *this->fontShader, glm::vec2(this->pos.x, this->pos.y - this->size.y / 8), this->size);
		}
		void	update(glm::vec2 mousePos, bool mousePressed)
		{
			bool inside = isInside(this->pos, this->size, mousePos);

			this->currentTexture = this->texture;
			
			if (this->anchor != NONE)
				anchorPos();

			if (mousePressed && !this->previousMousePressed)
    			this->wasPressedInside = inside;

    		if (mousePressed)
    		{
    			if (inside && this->wasPressedInside)
    				this->currentTexture = this->clickTexture;
    		}
    		else
    		{
    			if (this->wasPressedInside && inside && onClick)
    				this->onClick(clickData);
    			this->wasPressedInside = false;
    		}

			this->previousMousePressed = mousePressed;
		}

		bool						wasPressedInside = false;
		bool						previousMousePressed = false;

		std::function<void(void*)>	onClick = NULL;
		void						*clickData = NULL;

		std::string					label;

		Texture						*texture = NULL;
		Texture						*clickTexture = NULL;
		Texture						*currentTexture = NULL;

		Shader						*buttonShader = NULL;
		Shader						*fontShader = NULL;
		
		Font						*font = NULL;
};

/*
	@brief	Represents a simple slider UI element

	When initialized the slider will use basic textures/shaders from the Texture/ShaderManager, use the setter functions to use custom shaders
*/
class	Slider : public UIElement
{
	public:
		/*
		*/
		Slider(std::string label, glm::vec2 pos, glm::vec2 size, std::function<void(float, void*)> onChange, void *clickData)
		{
			this->label = label;
			this->pos = pos;
			this->size = size;
			this->onChange = onChange;
			this->clickData = clickData;

			loadDefaultAssets();
		}
		/*
		*/
		Slider(UIAnchor anchor, std::string label, glm::vec2 offset, glm::vec2 size, std::function<void(float, void*)> onChange, void *clickData)
		{
			this->label = label;
			this->offset = offset;
			this->pos = glm::vec2(0);
			this->size = size;
			this->onChange = onChange;
			this->clickData = clickData;
			this->anchor = anchor;

			loadDefaultAssets();
		}
		~Slider(){}
		void	loadDefaultAssets()
		{
			this->texture = TEXTURE_MANAGER->get(DEFAULT_BUTTON_TEXTURE);
			this->clickTexture = TEXTURE_MANAGER->get(DEFAULT_BUTTON_CLICK_TEXTURE);
			this->backgroundTexture = TEXTURE_MANAGER->get(DEFAULT_SLIDER_BACKGROUND_TEXTURE);

			this->fontShader = SHADER_MANAGER->get("text");
			this->buttonShader = SHADER_MANAGER->get("gui");
			this->font = FONT;
		}
		void	setAssets(Texture *texture, Texture *clickTexture, Shader *buttonShader, Shader *fontShader, Font *font)
		{
			if (texture)
				this->texture = texture;
			if (clickTexture)
				this->clickTexture = clickTexture;
			if (buttonShader)
				this->buttonShader = buttonShader;
			if (fontShader)
				this->fontShader = fontShader;
			if (font)
				this->font = font;
		}
		void	setAssets(Texture *texture, Texture *clickTexture, Texture *backgroundTexture, float sliderWidth, Shader *buttonShader, Shader *fontShader, Font *font)
		{
			if (texture)
				this->texture = texture;
			if (clickTexture)
				this->clickTexture = clickTexture;
			if (backgroundTexture)
				this->backgroundTexture = backgroundTexture;
			if (sliderWidth > 0 && sliderWidth <= this->size.x)
				this->sliderWidth = sliderWidth;
			if (buttonShader)
				this->buttonShader = buttonShader;
			if (fontShader)
				this->fontShader = fontShader;
			if (font)
				this->font = font;
		}
		void	draw()
		{
			if (!this->buttonShader || !this->currentTexture || !this->backgroundTexture)
				return ;

			initButtonQuad();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(this->pos.x, this->pos.y, 0.0f));
			model = glm::scale(model, glm::vec3(this->size.x, this->size.y, 1.0f));

			glm::mat4 sliderModel = glm::translate(glm::mat4(1.0f), glm::vec3(this->sliderPos.x, this->sliderPos.y, 0.0f));
			sliderModel = glm::scale(sliderModel, glm::vec3(this->sliderWidth, this->size.y, 1.0f));

			glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

			this->buttonShader->use();
			this->buttonShader->setMat4("projection", projection);
			
			glBindVertexArray(quadVAO);
			
			this->backgroundTexture->use();
			this->buttonShader->setMat4("model", model); //Draws the background
			glDrawArrays(GL_TRIANGLES, 0, 6);
			
			this->currentTexture->use();
			this->buttonShader->setMat4("model", sliderModel); //Draws the slider
			glDrawArrays(GL_TRIANGLES, 0, 6);
			
			glBindVertexArray(0);
			font->putString(this->label, *this->fontShader, glm::vec2(this->pos.x, this->pos.y - this->size.y / 8), this->size);
		}
		void	update(glm::vec2 mousePos, bool mousePressed)
		{
			if (this->anchor != NONE)
			{
				anchorPos();
				setSlider(value);
			}
			
			float	minCenter = pos.x + sliderWidth * 0.5f;
    		float	maxCenter = pos.x + this->size.x - sliderWidth * 0.5f;
			bool	insideSlider = isInside(this->sliderPos, glm::vec2(sliderWidth, this->size.y), mousePos);
			bool	insideButton = isInside(this->pos, size, mousePos);
			float	previousValue = value;

			this->currentTexture = this->texture;

			if (mousePressed && !this->previousMousePressed)
			{
				if (insideSlider || insideButton)
					this->dragging = true;
			}

			if (mousePressed && this->dragging)
			{
				this->currentTexture = this->clickTexture;
				this->sliderPos.x = mousePos.x - (sliderWidth / 2);
			}
			
			if (!mousePressed)
				this->dragging = false;

			sliderPos.x = glm::clamp(sliderPos.x, pos.x, pos.x + size.x - (sliderWidth));
			this->previousMousePressed = mousePressed;
			float sliderCenter = sliderPos.x + (sliderWidth / 2);
    		float normalizedValue = (sliderCenter - minCenter) / (maxCenter - minCenter);
    		normalizedValue = glm::clamp(normalizedValue, 0.0f, 1.0f);
            
    		value = normalizedValue;
			if (value != previousValue && onChange)
				onChange(value, clickData);
		}
		void    setSlider(float value)
		{
			value = glm::clamp(value, 0.0f, 1.0f);
			this->value = value;
		
			float minCenter = pos.x + (sliderWidth / 2);
			float maxCenter = pos.x + size.x - (sliderWidth / 2);
			float centerX = minCenter + value * (maxCenter - minCenter);
		
			sliderPos.x = centerX - (sliderWidth / 2);
			sliderPos.y = pos.y;
		}

		bool						wasPressedInside = false;
		bool						previousMousePressed = false;
		bool						dragging = false;

		std::function<void(float, void*)>	onChange = NULL;
		void								*clickData = NULL;

		glm::vec2						sliderPos;
		float						sliderWidth = 30;
		float						value = 0;
		std::string					label;

		Texture						*texture = NULL;
		Texture						*clickTexture = NULL;
		Texture						*backgroundTexture = NULL;
		Texture						*currentTexture = NULL;

		Shader						*buttonShader = NULL;
		Shader						*fontShader = NULL;
		
		Font						*font = NULL;
};

class	Text : public UIElement
{
	public:
		Text()
		{

		}
		~Text(){}
		Text(UIAnchor anchor, std::string label, glm::vec2 offset, glm::vec2 size)
		{
			this->label = label;
			this->offset = offset;
			this->pos = glm::vec2(0);
			this->size = size;
			this->anchor = anchor;

			loadDefaultAssets();
		}
		void	loadDefaultAssets()
		{
			this->fontShader = SHADER_MANAGER->get("text");
			this->font = FONT;
		}
		void	draw()
		{
			if (!this->font || !this->fontShader)
				return ;

			font->putString(this->label, *this->fontShader, glm::vec2(this->pos.x, this->pos.y - this->size.y / 8), this->size);
		}
		void	update(glm::vec2 mousePos, bool mousePressed)
		{
			(void)mousePos;(void)mousePressed;
			if (this->anchor != NONE)
				anchorPos();
		}
		Shader						*fontShader = NULL;
		Font						*font = NULL;

		std::string					label;
};

class	Image : public UIElement
{
	public:
		Image()
		{

		}
		~Image(){}
		Image(UIAnchor anchor, Texture *texture, glm::vec2 offset, glm::vec2 size)
		{
			this->offset = offset;
			this->pos = glm::vec2(0);
			this->size = size;
			this->anchor = anchor;
			this->texture = texture;

			loadDefaultAssets();
		}
		void	loadDefaultAssets()
		{
			if (!texture)
				this->texture = TEXTURE_MANAGER->get(MBATTY_TX_PATH);
			this->buttonShader = SHADER_MANAGER->get("gui");
		}
		void	draw()
		{
			if (!this->texture || !this->buttonShader)
				return ;

			initButtonQuad();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(this->pos.x, this->pos.y, 0.0f));
			model = glm::scale(model, glm::vec3(this->size.x, this->size.y, 1.0f));

			glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

			this->buttonShader->use();
			this->buttonShader->setMat4("projection", projection);
			
			glBindVertexArray(quadVAO);
			this->texture->use();
			this->buttonShader->setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		void	update(glm::vec2 mousePos, bool mousePressed)
		{
			(void)mousePos;(void)mousePressed;
			if (this->anchor != NONE)
				anchorPos();
		}
		Shader						*buttonShader = NULL;
		Texture						*texture = NULL;
};

class	TextBox : public UIElement
{
	public:
		TextBox(UIAnchor anchor, std::string display, glm::vec2 offset, glm::vec2 size, std::function<void(std::string &)> onConfirm)
		{
			this->offset = offset;
			this->pos = glm::vec2(0);
			this->size = size;
			this->onConfirm = onConfirm;
			this->anchor = anchor;
			this->display = display;

			loadDefaultAssets();
		}
		~TextBox(){}
		void	loadDefaultAssets()
		{
			this->texture = TEXTURE_MANAGER->get(DEFAULT_BUTTON_TEXTURE);
			this->clickTexture = TEXTURE_MANAGER->get(DEFAULT_BUTTON_CLICK_TEXTURE);

			this->fontShader = SHADER_MANAGER->get("text");
			this->buttonShader = SHADER_MANAGER->get("gui");
			this->font = FONT;
		}
		// Sets custom button assets, if argument is NULL, asset wont be set
		void	setAssets(Texture *texture, Texture *clickTexture, Shader *buttonShader, Shader *fontShader, Font *font)
		{
			if (texture)
				this->texture = texture;
			if (clickTexture)
				this->clickTexture = clickTexture;
			if (buttonShader)
				this->buttonShader = buttonShader;
			if (fontShader)
				this->fontShader = fontShader;
			if (font)
				this->font = font;
		}
		
		void	draw()
		{
			if (!this->buttonShader || !this->currentTexture)
				return ;
			
			initButtonQuad();
			
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(this->pos.x, this->pos.y, 0.0f));
			model = glm::scale(model, glm::vec3(this->size.x, this->size.y, 1.0f));
			glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

			this->buttonShader->use();
			this->buttonShader->setMat4("model", model);
			this->buttonShader->setMat4("projection", projection);
			
			this->currentTexture->use();

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			if (!focused && !input.size())
				font->putString(this->display, *this->fontShader, glm::vec2(this->pos.x, this->pos.y - this->size.y / 8), this->size);
			else if (!focused)
				font->putString(this->input, *this->fontShader, glm::vec2(this->pos.x, this->pos.y - this->size.y / 8), this->size);
			else
				font->putString(this->input + "_", *this->fontShader, glm::vec2(this->pos.x, this->pos.y - this->size.y / 8), this->size);
		}
		void	update(glm::vec2 mousePos, bool mousePressed)
		{
			bool inside = isInside(this->pos, this->size, mousePos);

			this->currentTexture = this->texture;
			
			if (this->anchor != NONE)
				anchorPos();

			if (mousePressed && !this->previousMousePressed)
			{
    			this->wasPressedInside = inside;
			}

    		if (mousePressed)
    		{
				if (inside && this->wasPressedInside)
					this->currentTexture = this->clickTexture;
				if (!inside)
					focused = false;
    		}
    		else
    		{
    			if (this->wasPressedInside && inside)
    				focused = true;
    			this->wasPressedInside = false;
    		}

			this->previousMousePressed = mousePressed;
		}
		void	onCharInput(char c)
		{
			if (!focused)
				return ;
			input += c;
		}
		void	onSpecialInput(int key)
		{
			if (!focused)
				return ;
			if (input.size() && key == GLFW_KEY_BACKSPACE)
				input.pop_back();
			if (key == GLFW_KEY_ENTER)
			{
				focused = false;
				if (onConfirm)
					onConfirm(input);
			}
		}

		bool						wasPressedInside = false;
		bool						previousMousePressed = false;

		std::function<void(std::string &)>	onConfirm = NULL;
		std::string							input;
		bool								focused = false;

		std::string							display;

		Texture						*texture = NULL;
		Texture						*clickTexture = NULL;
		Texture						*currentTexture = NULL;

		Shader						*buttonShader = NULL;
		Shader						*fontShader = NULL;
		
		Font						*font = NULL;
};

#endif
