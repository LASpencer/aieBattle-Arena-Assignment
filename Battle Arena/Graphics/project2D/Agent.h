#pragma once
#include "Renderer2D.h"
#include <vector>

class Creature;
struct CreatureArray;
class Attack;

class Agent
{
public:
	Agent();
	~Agent();

	virtual void startTurn(CreatureArray* friends, CreatureArray* enemies, size_t position);

	virtual void update(float deltaTime)=0;
	virtual void draw(aie::Renderer2D &renderer)=0;
	
	void setCreature(Creature* creature) {
		m_creature = creature;
	}

	bool hasDecided() {
		return m_decided;
	}

	bool canTakeTurn();

	size_t getTarget() {
		return m_target;
	}
	Attack* getChosenAttack() {
		return m_chosenAttack;
	}

protected:
	Creature* m_creature;
	CreatureArray* m_enemies;
	CreatureArray* m_friends;
	std::vector<Attack*> m_possibleAttacks;
	size_t m_position;
	bool m_decided;
	size_t m_target;
	Attack* m_chosenAttack;
};

