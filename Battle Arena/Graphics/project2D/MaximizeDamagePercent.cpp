#include "MaximizeDamagePercent.h"
#include "Attack.h"
#include "Creature.h"


MaximizeDamagePercent::MaximizeDamagePercent()
{
	
}


MaximizeDamagePercent::~MaximizeDamagePercent()
{
}

Attack * MaximizeDamagePercent::SelectAttack()
{
	float maxDamagePercent = 0.0f;
	// Set m_chosenAttack and m_target to first options in case no attacks do damage
	m_chosenAttack = *(m_possibleAttacks->begin());
	CreatureArray* tgtArray;
	switch (m_chosenAttack->getMainTarget()) {
	case TargetType::SELF:
		m_target = m_position;
		break;
	case TargetType::FRIEND:
		tgtArray = m_friends;
		break;
	case TargetType::ENEMY:
		tgtArray = m_enemies;
		break;
	}
	if (m_chosenAttack->getMainTarget() != TargetType::SELF)
	{
		for (size_t i = m_chosenAttack->getMinTgt(); i <= m_chosenAttack->getMaxTgt() && i < tgtArray->size; ++i) {
			if (tgtArray->creature[i]->IsTargetable(m_chosenAttack->getMainTarget())) {
				m_target = i;
				break;
			}
		}
	}
	for (std::vector<Attack*>::iterator it = m_possibleAttacks->begin(); it != m_possibleAttacks->end(); ++it) {
		int targetedDamage[Attack::MAX_RANGE + 1];
		int areaDamage[Attack::MAX_RANGE + 1];
		float attackMaxDamagePercent = 0.0f;
		size_t target = 0;
		CalculateAttackDamage(*it, targetedDamage, areaDamage);
		// Set attackMaxDamagePercent to highest damage to health ratio among enemies
		for (size_t i = 0; i < Attack::MAX_RANGE + 1; ++i) {
			float totalDamagePercent = (float)(targetedDamage[i] + areaDamage[i]) / (float)(m_enemies->creature[i]->getHealth());
			if (totalDamagePercent > attackMaxDamagePercent) {
				attackMaxDamagePercent = totalDamagePercent;
			}
		}
		if (attackMaxDamagePercent > maxDamagePercent) {
			maxDamagePercent = attackMaxDamagePercent;
			m_chosenAttack = *it;
			float maxTargetedDamagePercent = -1.0f;		// Start with negative value so first valid target found will be picked
			for (size_t i = m_chosenAttack->getMinTgt(); i <= m_chosenAttack->getMaxTgt(); ++i) {
				float targetedDamagePercent = (float)(targetedDamage[i]) / (float)(m_enemies->creature[i]->getHealth());
				if (targetedDamagePercent > maxTargetedDamagePercent&&m_enemies->creature[i]->IsTargetable(TargetType::ENEMY)) {
					m_target = i;
					maxTargetedDamagePercent = targetedDamagePercent;
				}
			}
		}
	}
	return m_chosenAttack;
}

size_t MaximizeDamagePercent::SelectTarget()
{
	return m_target;
}
