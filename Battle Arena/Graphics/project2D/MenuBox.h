#pragma once
#include <vector>
#include <string>
#include "Renderer2D.h"
#include "Font.h"
#include "Input.h"

struct OptionInfo {
	char* name;
	size_t length;
	bool chosen;
};

class MenuBox
{
public:
	static const int MARGIN = 3;		//TODO these should be floats, use base class to work around issue
	static const int LINE_HEIGHT = 40;
	static const int TEXT_OFFSET = 9;

	MenuBox() : m_xPos(0), m_yPos(0), m_boxColour(0x202020FF), m_textColour(0xFFFFFFFF), m_selectColour(0x22AAAAFF), m_font("./font/consolas.ttf", 32)
	{
	}

	MenuBox(float xPos, float yPos, unsigned int boxColour, unsigned int textColour, unsigned int selectColour) : m_xPos(xPos), m_yPos(yPos), m_boxColour(boxColour), m_textColour(textColour), m_selectColour(selectColour), m_font("./font/consolas.ttf", 32)
	{
	}

	~MenuBox()
	{
	}

	virtual void update(float deltaTime) = 0;

	virtual void draw(aie::Renderer2D &renderer) = 0;
	

	void SetPosition(float xPos, float yPos) {
		m_xPos = xPos;
		m_yPos = yPos;
	}

	void SetBoxColour(unsigned int colour) {
		m_boxColour = colour;
	}

	void SetTextColour(unsigned int colour) {
		m_textColour = colour;
	}

protected:

	// Position of menu
	float m_xPos;
	float m_yPos;
	// Background and text colour
	unsigned int m_boxColour;
	unsigned int m_textColour;
	unsigned int m_selectColour;
	// Font
	aie::Font m_font;
};

