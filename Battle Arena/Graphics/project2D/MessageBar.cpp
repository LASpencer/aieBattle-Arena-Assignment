#include "MessageBar.h"
#include "Font.h"

const float MessageBar::CHARACTER_WIDTH = 20;
const float MessageBar::CHARACTER_HEIGHT = 36;

MessageBar::MessageBar() : m_message(nullptr),m_xPos(0),m_yPos(0),m_boxColour(BOX_COLOUR),m_textColour(TEXT_COLOUR), m_font("./font/consolas.ttf", 32)
{
}

MessageBar::MessageBar(std::string message, float xPos, float yPos, unsigned int boxColour, unsigned int  textColour) : m_message(nullptr), m_xPos(xPos), m_yPos(yPos), m_boxColour(boxColour), m_textColour(textColour), m_font("./font/consolas.ttf", 32)
{
	
	setMessage(message);
}


MessageBar::~MessageBar()
{
	if (m_message != nullptr) {
		delete[] m_message;
	}
}

void MessageBar::setPosition(float xPos, float yPos)
{
	m_xPos = xPos;
	m_yPos = yPos;
}

void MessageBar::setMessage(std::string message)
{
	// Delete old message
	if (m_message != nullptr) {
		delete[] m_message;
	}
	// Allocate memory for message
	m_message = new char[message.length() + 1];
	// copy message to m_message
	message.copy(m_message, message.length(), 0);
	m_message[message.length()] = 0;
}

void MessageBar::setMessage(const char * message)
{
	// Delete old message
	if (m_message != nullptr) {
		delete[] m_message;
	}
	// Allocate memory for message
	m_message = new char[strlen(message) + 1];
	// Copy message to m_message
	strcpy(m_message, message);
}

void MessageBar::setBoxColour(unsigned int boxColour)
{
	m_boxColour = boxColour;
}

void MessageBar::setTextColour(unsigned int textColour)
{
	m_textColour = textColour;
}

void MessageBar::draw(aie::Renderer2D & renderer)
{
	if (m_message != nullptr) {
		//Calculate message width
		float width = renderer.measureTextWidth(&m_font, m_message);
		// Draw background box
		renderer.setRenderColour(m_boxColour);
		renderer.drawBox(m_xPos, m_yPos, width+6, CHARACTER_HEIGHT + 2);
		// Print message
		renderer.setRenderColour(m_textColour);
		renderer.drawText(&m_font, m_message, m_xPos - (width / 2), m_yPos - (CHARACTER_HEIGHT / 4));
	}
}
