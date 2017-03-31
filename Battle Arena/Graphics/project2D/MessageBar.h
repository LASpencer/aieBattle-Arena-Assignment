#pragma once
#include "Renderer2D.h"
#include <string>
#include "Font.h"

/*
MessageBar
Contains a message, and draws a coloured box containing that message
*/
class MessageBar
{
public:
	MessageBar();
	MessageBar(std::string message, float xPos = 0, float yPos = 0, unsigned int boxColour = BOX_COLOUR, unsigned int  textColour = TEXT_COLOUR);
	~MessageBar();

	void SetPosition(float xPos, float yPos);
	void SetMessage(std::string message);
	void SetMessage(const char* message);
	void SetBoxColour(unsigned int boxColour);
	void SetTextColour(unsigned int textColour);

	void draw(aie::Renderer2D &renderer);

	static const float CHARACTER_WIDTH;
	static const float CHARACTER_HEIGHT;
	static const unsigned int BOX_COLOUR = 0x101010FF;
	static const unsigned int TEXT_COLOUR = 0xFFFFFFFF;


private:
	// Message to be printed
	char* m_message;
	// Position of message bar
	float m_xPos;
	float m_yPos;
	// Colour of box
	unsigned int m_boxColour;
	//Colour of message text
	unsigned int m_textColour;
	aie::Font m_font;
};

