#include "PickFirstOption.h"
#include "Attack.h"
#include "Creature.h"

PickFirstOption::PickFirstOption()
{
}


PickFirstOption::~PickFirstOption()
{
}

Attack * PickFirstOption::SelectAttack()
{
	m_chosenAttack = (*m_possibleAttacks)[0];
	return m_chosenAttack;
}

size_t PickFirstOption::SelectTarget()
{
	CreatureArray* tgtArray;
	// Set array of possible targets based on attack target
	switch (m_chosenAttack->GetMainTarget()) {
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
	// Select first valid target in tgtArray
	for (size_t i = m_chosenAttack->GetMinTgt(); i <= m_chosenAttack->GetMaxTgt() && i<tgtArray->size; ++i) {
		if (tgtArray->creature[i]->IsTargetable(m_chosenAttack->GetMainTarget())) {
			m_target = i;
			break;
		}
	}
	return m_target;
}
