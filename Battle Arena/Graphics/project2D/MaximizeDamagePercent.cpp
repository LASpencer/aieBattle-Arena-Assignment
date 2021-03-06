#include "MaximizeDamagePercent.h"
#include "Attack.h"
#include "Creature.h"


MaximizeDamagePercent::MaximizeDamagePercent()
{
	
}


MaximizeDamagePercent::~MaximizeDamagePercent()
{
}

Attack * MaximizeDamagePercent::selectAttack()
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
	if (m_chosenAttack->getMainTarget() != TargetType::SELF)		// if target isn't self, need to find first valid target
	{
		for (size_t i = m_chosenAttack->getMinTgt(); i <= m_chosenAttack->getMaxTgt() && i < tgtArray->size; ++i) {
			if (tgtArray->creature[i]->isTargetable(m_chosenAttack->getMainTarget())) {
				// If valid target found, set as target and stop looking
				m_target = i;
				break;
			}
		}
	}
	// Iterate over all possible attack to find best attack
	for (std::vector<Attack*>::iterator it = m_possibleAttacks->begin(); it != m_possibleAttacks->end(); ++it) {
		int targetedDamage[Attack::MAX_RANGE + 1];
		int areaDamage[Attack::MAX_RANGE + 1];
		float attackMaxDamagePercent = 0.0f;		// Ratio of damage to creature health
		size_t target = 0;
		calculateAttackDamage(*it, targetedDamage, areaDamage);
		// Set attackMaxDamagePercent to highest damage to health ratio among enemies
		for (size_t i = 0; i < Attack::MAX_RANGE + 1; ++i) {
			// Just add together targeted and area damage, as this doesn't care about total damage output
			// but reducing one creature's health as much as possible
			float totalDamagePercent = (float)(targetedDamage[i] + areaDamage[i]) / (float)(m_enemies->creature[i]->getHealth());
			if (totalDamagePercent > attackMaxDamagePercent) {
				attackMaxDamagePercent = totalDamagePercent;
			}
		}
		// If new best found, set new m_chosenAttack, m_target, and maxDamagePercent
		if (attackMaxDamagePercent > maxDamagePercent) {
			maxDamagePercent = attackMaxDamagePercent;
			m_chosenAttack = *it;
			float maxTargetedDamagePercent = -1.0f;		// Start with negative value so first valid target found will be picked
			for (size_t i = m_chosenAttack->getMinTgt(); i <= m_chosenAttack->getMaxTgt(); ++i) {
				float targetedDamagePercent = (float)(targetedDamage[i]) / (float)(m_enemies->creature[i]->getHealth());
				if (targetedDamagePercent > maxTargetedDamagePercent&&m_enemies->creature[i]->isTargetable(TargetType::ENEMY)) {
					m_target = i;
					maxTargetedDamagePercent = targetedDamagePercent;
				}
			}
		}
	}
	return m_chosenAttack;
}

size_t MaximizeDamagePercent::selectTarget()
{
	return m_target;
}
