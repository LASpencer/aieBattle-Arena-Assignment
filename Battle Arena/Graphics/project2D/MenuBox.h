#pragma once
#include <vector>
#include <string>
#include "Renderer2D.h"
#include "Font.h"
#include "Input.h"

struct OptionInfo {
	char* name;			// String to display for option
	size_t length;		// Length of string
	bool chosen;		// Is this the currently chosen option?
};

/*MenuBox class
Displays a box with a list of possible options. User can set their choice using arrow keys
*/
class MenuBox
{
public:
	static const float MARGIN;	
	static const float LINE_HEIGHT;	
	static const float TEXT_OFFSET;	

	MenuBox() : m_xPos(0), m_yPos(0), m_boxColour(0x202020FF), m_textColour(0xFFFFFFFF), m_selectColour(0x22AAAAFF), m_font("./font/consolas.ttf", 32)
	{
	}

	MenuBox(float xPos, float yPos, unsigned int boxColour, unsigned int textColour, unsigned int selectColour) : m_xPos(xPos), m_yPos(yPos), m_boxColour(boxColour), m_textColour(textColour), m_selectColour(selectColour), m_font("./font/consolas.ttf", 32)
	{
	}

	~MenuBox()
	{
	}

	// Check if user is pressing arrow keys and change selection if so
	virtual void update(float deltaTime) = 0;

	// Draw the menuBox
	virtual void draw(aie::Renderer2D &renderer) = 0;
	
	// Sets position of top left corner of menu
	void setPosition(float xPos, float yPos) {
		m_xPos = xPos;
		m_yPos = yPos;
	}

	/* Sets colour of the menu box
	colour = RGBA value as 4 byte unsigned integer
	*/
	void setBoxColour(unsigned int colour) {
		m_boxColour = colour;
	}

	/* Sets colour of unselected options
	colour = RGBA value as 4 byte unsigned integer
	*/
	void setTextColour(unsigned int colour) {
		m_textColour = colour;
	}

	/* Sets colour of the currently selected option
	colour = RGBA value as 4 byte unsigned integer
	*/
	void setSelectColour(unsigned int colour) {
		m_selectColour = colour;
	}

protected:

	// Position of top left corner of menu
	float m_xPos;
	float m_yPos;
	// Background and text colour
	unsigned int m_boxColour;
	unsigned int m_textColour;
	unsigned int m_selectColour;
	// Font
	aie::Font m_font;
};

