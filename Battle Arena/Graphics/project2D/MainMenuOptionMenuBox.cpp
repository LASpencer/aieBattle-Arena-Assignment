#include "MainMenuOptionMenuBox.h"
#include "MainMenu.h"


MainMenuOptionMenuBox::MainMenuOptionMenuBox():m_contents(), MenuBox()
{

	m_current = m_contents.begin();
}

MainMenuOptionMenuBox::MainMenuOptionMenuBox(std::vector<MainMenuOption>& contents, float xPos, float yPos, unsigned int boxColour, unsigned int textColour, unsigned int selectColour) :m_contents(contents), MenuBox(xPos, yPos, boxColour, textColour, selectColour)
{
	m_current = m_contents.begin();
}

MainMenuOptionMenuBox::MainMenuOptionMenuBox(const MainMenuOptionMenuBox & m) : m_contents(m.m_contents), MenuBox(m.m_xPos, m.m_yPos, m.m_boxColour, m.m_textColour, m.m_selectColour)
{
	m_current = m_contents.begin();
}


MainMenuOptionMenuBox::~MainMenuOptionMenuBox()
{
}

void MainMenuOptionMenuBox::update(float deltaTime)
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

void MainMenuOptionMenuBox::draw(aie::Renderer2D & renderer)
{
	if (!m_contents.empty()) {
		// Get each option from iterator
		OptionInfo* options = new OptionInfo[m_contents.size()];
		size_t arraySize = 0;
		float maxWidth = 0;
		for (contentIterator it = m_contents.begin(); it != m_contents.end(); ++it) {
			// Get name of option
			std::string name = (*it).GetName();
			// allocate memory for name
			options[arraySize].name = new char[name.length() + 1];
			options[arraySize].length = name.length();
			// Copy name into OptionInfo
			name.copy(options[arraySize].name, options[arraySize].length, 0);
			options[arraySize].name[options[arraySize].length] = 0;				// Add terminating 0
			// Check if this is currently selected option
			if (it == m_current) {
				options[arraySize].chosen = true;
			}
			else {
				options[arraySize].chosen = false;
			}
			// Check if this is new widest text in box
			float width = renderer.measureTextWidth(&m_font, options[arraySize].name);
			if (width > maxWidth) {
				maxWidth = width;
			}
			++arraySize;
		}
		// Draw box
		renderer.setRenderColour(m_boxColour);
		renderer.drawBox((m_xPos + MARGIN + maxWidth / 2), m_yPos - (float)((LINE_HEIGHT / 2)*(arraySize)), maxWidth + (2 * MARGIN), arraySize * LINE_HEIGHT); 
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
			renderer.drawText(&m_font, options[i].name, (m_xPos + MARGIN), (m_yPos - (LINE_HEIGHT / 2 + TEXT_OFFSET + (LINE_HEIGHT * i)))); 
			// Deallocate string memory after writing
			delete[] options[i].name;
		}
		// Delete array
		delete[] options;
	}
}

void MainMenuOptionMenuBox::setContents(std::vector<MainMenuOption>& contents)
{
	m_contents = contents;
	m_current = m_contents.begin();
}

MainMenuOption MainMenuOptionMenuBox::getCurrent()
{
	return *m_current;
}
