#pragma once
#include <vector>
#include <string>
#include "Renderer2D.h"
#include "Font.h"
#include "Input.h"

//TODO change MenuBox into abstract class, with derived classes for each type needed

struct OptionInfo {
	char* name;
	size_t length;
	bool chosen;
};

template<class T>
class MenuBox
{
public:
	static const int MARGIN = 3;		//TODO these should be floats, use base class to work around issue
	static const int LINE_HEIGHT = 40;
	static const int TEXT_OFFSET = 9;

	MenuBox() : m_contents(), m_xPos(0), m_yPos(0), m_boxColour(0x202020FF), m_textColour(0xFFFFFFFF), m_selectColour(0x22AAAAFF), m_font("./font/consolas.ttf", 32)
	{
		m_current = m_contents.begin();		//HACK test if this works in iterator list
	}

	MenuBox(std::vector<T*> &contents, float xPos = 0, float yPos = 0, unsigned int boxColour = 0x202020FF, unsigned int textColour = 0xFFFFFFFF, unsigned int selectColour = 0xAAAA22FF) : m_contents(contents), m_xPos(xPos), m_yPos(yPos), m_boxColour(boxColour), m_textColour(textColour), m_selectColour(selectColour), m_font("./font/consolas.ttf", 32)
	{
		m_current = m_contents.begin();
	}

	MenuBox(const MenuBox<T> &m) : m_contents(m.m_contents), m_xPos(m.m_xPos), m_yPos(m.m_yPos), m_boxColour(m.m_boxColour), m_textColour(m.m_textColour), m_selectColour(m.m_selectColour), m_font(m.m_font)
	{
		m_current = m_contents.begin();
	}

	~MenuBox()
	{
	}

	void Update(float deltaTime)
	{
		//up and down direction keys scroll through menu
		aie::Input* input = aie::Input::getInstance();
		if (input->wasKeyPressed(aie::INPUT_KEY_DOWN)) {
			++m_current;
			if (m_current >= m_contents.end()) {	//If at or past end, undo move
				--m_current;
			}
		}
		else if (input->wasKeyPressed(aie::INPUT_KEY_UP)) {	
			if (m_current != m_contents.begin()) {
				--m_current;
			}
		}
	}

	void Draw(aie::Renderer2D &renderer)
	{
		if (!m_contents.empty()) {
			// Get each option from iterator
			OptionInfo* options = new OptionInfo[m_contents.size()];
			size_t arraySize = 0;
			float maxWidth = 0;
			for (contentIterator it = m_contents.begin(); it != m_contents.end(); ++it) {
				std::string name = (*it)->GetName();
				options[arraySize].name = new char[name.length() + 1];
				options[arraySize].length = name.length();
				name.copy(options[arraySize].name, options[arraySize].length, 0);
				options[arraySize].name[options[arraySize].length] = 0;
				if (it == m_current) {
					options[arraySize].chosen = true;
				}
				else {
					options[arraySize].chosen = false;
				}
				float width = renderer.measureTextWidth(&m_font, options[arraySize].name);
				if (width > maxWidth) {
					maxWidth = width;
				}
				++arraySize;
			}
			// Draw box
			renderer.setRenderColour(m_boxColour);
			renderer.drawBox((m_xPos + MARGIN + maxWidth / 2), m_yPos- (float)((LINE_HEIGHT / 2)*(arraySize)), maxWidth + (2*MARGIN), arraySize * LINE_HEIGHT); //HACK fix type conversion
			// Draw text (if current, use selectColour for text)
			for (size_t i = 0; i < arraySize; ++i) {
				//set colour
				if (options[i].chosen) {
					renderer.setRenderColour(m_selectColour);
				}
				else {
					renderer.setRenderColour(m_textColour);
				}
				// Draw the text
				renderer.drawText(&m_font, options[i].name, (m_xPos+MARGIN), (m_yPos - (LINE_HEIGHT/2 + TEXT_OFFSET + (LINE_HEIGHT * i)))); //HACK fix type conversion
																																  // Delete text after writing
				delete[] options[i].name;
			}
			// Delete array
			delete[] options;
		}
	}

	void SetContents(std::vector<T*> &contents) 
	{
		m_contents = contents;
		m_current = m_contents.begin();
	}

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

	T* GetCurrent() {
		return *m_current;
	}

private:
	//HACK not sure if this is right
	typedef typename std::vector<T*>::iterator contentIterator;

	// Menu contents
	std::vector<T*>	m_contents;
	contentIterator m_current;
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

