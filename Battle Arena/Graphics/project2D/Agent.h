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

	/*Tells Agent its creature's turn has started and passes it necessary information to pick an attack
	friends = Pointer to agent's creature's team
	enemies = Pointer to enemy team
	position = Position of agent's creature in team
	*/
	virtual void startTurn(CreatureArray* friends, CreatureArray* enemies, size_t position);

	virtual void update(float deltaTime)=0;
	virtual void draw(aie::Renderer2D &renderer)=0;
	
	void setCreature(Creature* creature) {
		m_creature = creature;
	}

	// Check if agent has picked an attack and target yet
	bool hasDecided() {
		return m_decided;
	}

	// Check if creature has any possible attacks to make
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

