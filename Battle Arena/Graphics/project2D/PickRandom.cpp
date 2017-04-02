#include "PickRandom.h"
#include "Attack.h"
#include "Creature.h"

PickRandom::PickRandom()
{
}


PickRandom::~PickRandom()
{
}

Attack * PickRandom::selectAttack()
{
	// Pick random attack
	m_chosenAttack = (*m_possibleAttacks)[rand() % m_possibleAttacks->size()];
	return m_chosenAttack;
}

size_t PickRandom::selectTarget()
{
	CreatureArray* tgtArray;
	size_t possibleTargets[Attack::MAX_RANGE + 1];
	size_t possibleTargetsArraySize = 0;
	// Set tgtArray based on attack target
	switch (m_chosenAttack->getMainTarget()) {
	case TargetType::SELF:
		m_target = m_position;
		return m_target;
		break;
	case TargetType::FRIEND:
		tgtArray = m_friends;
		break;
	case TargetType::ENEMY:
		tgtArray = m_enemies;
		break;
	}
	// Fill possibleTargets with all possible targets
	for (size_t i = m_chosenAttack->getMinTgt(); i <= m_chosenAttack->getMaxTgt() && i<tgtArray->size; ++i) {
		if (tgtArray->creature[i]->isTargetable(m_chosenAttack->getMainTarget())) {
			possibleTargets[possibleTargetsArraySize] = i;
			++possibleTargetsArraySize;
			break;
		}
	}
	// Pick random target index from possibleTargets
	m_target = possibleTargets[rand() % possibleTargetsArraySize];
	return m_target;
}
