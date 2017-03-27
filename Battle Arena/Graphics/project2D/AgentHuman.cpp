#include "AgentHuman.h"
#include "Attack.h"
#include "Input.h"
#include "Creature.h"
#include "Battle.h"

// TODO figure out best position
const float AgentHuman::ATTACK_MENU_XPOS = 500;
const float AgentHuman::ATTACK_MENU_YPOS = 197;

AgentHuman::AgentHuman() : m_targetArrow(nullptr)
{
	m_attackMenu = MenuBox<Attack>(std::vector<Attack*>{}, ATTACK_MENU_XPOS, ATTACK_MENU_YPOS);
}

AgentHuman::AgentHuman(aie::Texture * targetArrow) : m_targetArrow(targetArrow), m_attackMenu(std::vector<Attack*>{}, ATTACK_MENU_XPOS, ATTACK_MENU_YPOS)
{
}


AgentHuman::~AgentHuman()
{
}

void AgentHuman::StartTurn(CreatureArray * friends, CreatureArray * enemies, size_t position)
{
	Agent::StartTurn(friends, enemies, position);
	m_attackMenu.SetContents(m_possibleAttacks);
	m_attackMenu.SetPosition(ATTACK_MENU_XPOS - (position * Battle::POSITION_WIDTH), ATTACK_MENU_YPOS);
	m_pickingTarget = false;
}

void AgentHuman::Update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	if (!m_pickingTarget) {
		m_attackMenu.Update(deltaTime);
		if (input->wasKeyPressed(aie::INPUT_KEY_SPACE)) {
			m_chosenAttack = m_attackMenu.GetCurrent();
			SetTargetChoices();
			m_pickingTarget = true;
		}
	}
	else {
		if (input->wasKeyPressed(aie::INPUT_KEY_LEFT)) {
			switch (m_chosenAttack->GetMainTarget()) {
			case TargetType::SELF:
				break;
			case TargetType::FRIEND:
				IncrementTargetChoice();	
				break;
			case TargetType::ENEMY:
				DecrementTargetChoice();
				break;
			}
		}
		else if (input->wasKeyPressed(aie::INPUT_KEY_RIGHT)) {
			switch (m_chosenAttack->GetMainTarget()) {
			case TargetType::SELF:
				break;
			case TargetType::FRIEND:
				DecrementTargetChoice();
				break;
			case TargetType::ENEMY:
				IncrementTargetChoice();
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

void AgentHuman::SetTargetArrowSprite(aie::Texture * targetArrow)
{
	m_targetArrow = targetArrow;
}

void AgentHuman::Draw(aie::Renderer2D & renderer)
{
	m_attackMenu.Draw(renderer);
	if (m_pickingTarget) {
		float indicatorXPos;
		float indicatorYPos = Battle::GROUND_POS + 220;
		renderer.setRenderColour(0xFFFFFFFF);
		if(m_chosenAttack->IsAreaEffect()){
			size_t range[2];
			m_chosenAttack->GetAreaEffectRange(range);
			Creature** targetedCreature;
			float startPos;
			float positionModifier;
			switch (m_chosenAttack->GetMainTarget()) {
			case TargetType::SELF:
			case TargetType::FRIEND:
				startPos = Battle::PLAYER_TEAM_STARTPOS;
				positionModifier = -Battle::POSITION_WIDTH;
				targetedCreature = m_friends->creature;
				if (range[1] > m_friends->size) {
					range[1] = m_friends->size;
				}
				break;
			case TargetType::ENEMY:
				startPos = Battle::ENEMY_TEAM_STARTPOS;
				positionModifier = Battle::POSITION_WIDTH;
				targetedCreature = m_enemies->creature;
				if (range[1] >= m_enemies->size) {
					range[1] = m_enemies->size-1;
				}
				break;
			}

			for (size_t i = range[0]; i <= range[1]; ++i) {
				indicatorXPos = startPos + positionModifier * i;
				if (targetedCreature[i]->isAlive()) {
					renderer.drawSprite(m_targetArrow,indicatorXPos,indicatorYPos);
				}
			}
		}
		else {
			switch (m_chosenAttack->GetMainTarget()) {
			case TargetType::SELF:
			case TargetType::FRIEND:
				indicatorXPos = Battle::PLAYER_TEAM_STARTPOS - *m_targetIter * Battle::POSITION_WIDTH;
				break;
			case TargetType::ENEMY:
				indicatorXPos = Battle::ENEMY_TEAM_STARTPOS + *m_targetIter*Battle::POSITION_WIDTH;
				break;
			}
			renderer.drawSprite(m_targetArrow, indicatorXPos, indicatorYPos);
		}
	}
}

void AgentHuman::SetTargetChoices()
{
	CreatureArray* tgtArray;

	// Empty targetChoices 
	m_targetChoices.clear();

	switch (m_chosenAttack->GetMainTarget()) {
	case TargetType::SELF:
	case TargetType::FRIEND:
		tgtArray = m_friends;
		break;
	case TargetType::ENEMY:
		tgtArray = m_enemies;
		break;
	}
	if (m_chosenAttack->GetMainTarget() == TargetType::SELF) {
		m_targetChoices.push_back(m_position);
	}
	else {
		for (size_t i = m_chosenAttack->GetMinTgt(); i <= m_chosenAttack->GetMaxTgt() && i<tgtArray->size; ++i) {
			if (tgtArray->creature[i]->IsTargetable(m_chosenAttack->GetMainTarget())) {
				m_targetChoices.push_back(i);
			}
		}
	}
	m_targetIter = m_targetChoices.begin();
}

void AgentHuman::IncrementTargetChoice()
{
	++m_targetIter;
	if (m_targetIter >= m_targetChoices.end()) {
		--m_targetIter;
	}
}

void AgentHuman::DecrementTargetChoice()
{
	if (m_targetIter > m_targetChoices.begin()) {
		--m_targetIter;
	}
}
