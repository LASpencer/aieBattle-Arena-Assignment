#include "AgentHuman.h"
#include "Attack.h"
#include "Input.h"
#include "Creature.h"
#include "Battle.h"

const float AgentHuman::ATTACK_MENU_XPOS = 500;
const float AgentHuman::ATTACK_MENU_YPOS = 197;

AgentHuman::AgentHuman() : m_targetArrow(nullptr)
{
	m_attackMenu = AttackMenuBox(std::vector<Attack*>{}, ATTACK_MENU_XPOS, ATTACK_MENU_YPOS);
}

AgentHuman::AgentHuman(aie::Texture * targetArrow) : m_targetArrow(targetArrow), m_attackMenu(std::vector<Attack*>{}, ATTACK_MENU_XPOS, ATTACK_MENU_YPOS)
{
}


AgentHuman::~AgentHuman()
{
}

void AgentHuman::startTurn(CreatureArray * friends, CreatureArray * enemies, size_t position)
{
	Agent::startTurn(friends, enemies, position);
	// Put vector of current attacks in attack menu
	m_attackMenu.setContents(m_possibleAttacks);
	m_attackMenu.setPosition(ATTACK_MENU_XPOS - (position * Battle::POSITION_WIDTH), ATTACK_MENU_YPOS);
	m_pickingTarget = false;
}

void AgentHuman::update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	if (!m_pickingTarget) {										// If attack hasn't been chosen yet
		m_attackMenu.update(deltaTime);							// Update attack menu
		if (input->wasKeyPressed(aie::INPUT_KEY_SPACE)) {		// Check if player has chosen their attack
			m_chosenAttack = m_attackMenu.getCurrent();
			setTargetChoices();
			m_pickingTarget = true;
		}
	}
	else {
		if (input->wasKeyPressed(aie::INPUT_KEY_LEFT)) {
			switch (m_chosenAttack->getMainTarget()) {			// Check if player is chosing a friendly or enemy target
			case TargetType::SELF:
				break;
			case TargetType::FRIEND:
				incrementTargetChoice();						// For player team, left is higher index
				break;
			case TargetType::ENEMY:
				decrementTargetChoice();						// For enemy team, left is lower index
				break;
			}
		}
		else if (input->wasKeyPressed(aie::INPUT_KEY_RIGHT)) {
			switch (m_chosenAttack->getMainTarget()) {			// Check if player is chosing a friendly or enemy target
			case TargetType::SELF:
				break;
			case TargetType::FRIEND:
				decrementTargetChoice();						// For player team, right is lower index
				break;
			case TargetType::ENEMY:
				incrementTargetChoice();						// For enemy team, left is higher index
				break;
			}
		}
		else if (input->wasKeyPressed(aie::INPUT_KEY_SPACE)) {
			// Player has selected target
			m_target = *m_targetIter;
			m_decided = true;
		}
		else if (input->wasKeyPressed(aie::INPUT_KEY_BACKSPACE)) {
			// Player wants to change attack
			m_pickingTarget = false;
		}
	}
}

void AgentHuman::setTargetArrowSprite(aie::Texture * targetArrow)
{
	m_targetArrow = targetArrow;
}

void AgentHuman::draw(aie::Renderer2D & renderer)
{
	// Draw attack menu
	m_attackMenu.draw(renderer);
	if (m_pickingTarget) {
		float indicatorXPos;								// xPos of sprite chosen
		float indicatorYPos = Battle::GROUND_POS + 220;		// 20 pixels above sprite
		renderer.setRenderColour(0xFFFFFFFF);
		if(m_chosenAttack->isAreaEffect()){
			// If area effect attack chosen, draw arrows over all affected creatures
			size_t range[2];								// Start and end target positions
			m_chosenAttack->getAreaEffectRange(range);
			Creature** targetedCreature;
			float startPos;									// x coordinate of creature 0 in array
			float positionModifier;							// distance between creatures in same team
			switch (m_chosenAttack->getMainTarget()) {		// Check if friendly or enemy team
			case TargetType::SELF:
			case TargetType::FRIEND:
				startPos = Battle::PLAYER_TEAM_STARTPOS;
				positionModifier = -Battle::POSITION_WIDTH;	// Player creatures move left as index increases
				targetedCreature = m_friends->creature;
				if (range[1] > m_friends->size) {
					range[1] = m_friends->size-1;			// Check max target position is valid index
				}
				break;
			case TargetType::ENEMY:
				startPos = Battle::ENEMY_TEAM_STARTPOS;
				positionModifier = Battle::POSITION_WIDTH;	// Enemy creatures move right as index increases
				targetedCreature = m_enemies->creature;
				if (range[1] >= m_enemies->size) {
					range[1] = m_enemies->size-1;			// Check max target position is valid index
				}
				break;
			}
			for (size_t i = range[0]; i <= range[1]; ++i) {			// Iterate over all creatures in range
				indicatorXPos = startPos + positionModifier * i;	// Set target arrow position to above that creature
				if (targetedCreature[i]->isAlive()) {				// If creature alive, draw arrow over it
					renderer.drawSprite(m_targetArrow,indicatorXPos,indicatorYPos);
				}
			}
		}
		else {
			// If single target effect, draw arrow over currently selected target
			switch (m_chosenAttack->getMainTarget()) {
			case TargetType::SELF:
			case TargetType::FRIEND:
				// If Player team, increasing index moves arrow left (decrease x coordinate)
				indicatorXPos = Battle::PLAYER_TEAM_STARTPOS - *m_targetIter * Battle::POSITION_WIDTH;
				break;
			case TargetType::ENEMY:
				// If enemy team, increasing index moves arrow right (increase x coordinate)
				indicatorXPos = Battle::ENEMY_TEAM_STARTPOS + *m_targetIter*Battle::POSITION_WIDTH;
				break;
			}
			// Draw arrow over selected target
			renderer.drawSprite(m_targetArrow, indicatorXPos, indicatorYPos);
		}
	}
}

void AgentHuman::setTargetChoices()
{
	CreatureArray* tgtArray;

	// Empty targetChoices 
	m_targetChoices.clear();

	switch (m_chosenAttack->getMainTarget()) {
	case TargetType::SELF:
	case TargetType::FRIEND:
		tgtArray = m_friends;
		break;
	case TargetType::ENEMY:
		tgtArray = m_enemies;
		break;
	}
	if (m_chosenAttack->getMainTarget() == TargetType::SELF) {
		m_targetChoices.push_back(m_position);
	}
	else {
		for (size_t i = m_chosenAttack->getMinTgt(); i <= m_chosenAttack->getMaxTgt() && i<tgtArray->size; ++i) {
			if (tgtArray->creature[i]->isTargetable(m_chosenAttack->getMainTarget())) {
				m_targetChoices.push_back(i);
			}
		}
	}
	m_targetIter = m_targetChoices.begin();
}

void AgentHuman::incrementTargetChoice()
{
	++m_targetIter;
	if (m_targetIter >= m_targetChoices.end()) {
		--m_targetIter;
	}
}

void AgentHuman::decrementTargetChoice()
{
	if (m_targetIter > m_targetChoices.begin()) {
		--m_targetIter;
	}
}
