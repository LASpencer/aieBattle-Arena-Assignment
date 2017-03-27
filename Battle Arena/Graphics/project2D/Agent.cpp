#include "Agent.h"
#include "Creature.h"


Agent::Agent()
{
}


Agent::~Agent()
{
}

void Agent::StartTurn(CreatureArray * friends, CreatureArray * enemies, size_t position)
{
	m_friends = friends;
	m_enemies = enemies;
	m_position = position;
	m_possibleAttacks = m_creature->GetPossibleAttacks(*friends, *enemies, position);
	m_decided = false;
}

bool Agent::CanTakeTurn()
{
	return !m_possibleAttacks.empty();
}

