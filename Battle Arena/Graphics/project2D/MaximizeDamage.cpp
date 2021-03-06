#include "MaximizeDamage.h"
#include "Attack.h"
#include "Creature.h"

MaximizeDamage::MaximizeDamage()
{
}


MaximizeDamage::~MaximizeDamage()
{
}

Attack * MaximizeDamage::selectAttack()
{
	int maxDamage = 0;
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
	if(m_chosenAttack->getMainTarget() != TargetType::SELF)	// if target isn't self, need to find first valid target
	{
		for (size_t i = m_chosenAttack->getMinTgt(); i <= m_chosenAttack->getMaxTgt() && i < tgtArray->size; ++i) {
			if (tgtArray->creature[i]->isTargetable(m_chosenAttack->getMainTarget())) {	
				// If valid target found, set as target and stop looking
				m_target = i;
				break;
			}
		}
	}
	// Iterate over all possible attacks to find maximum damage
	for (std::vector<Attack*>::iterator it = m_possibleAttacks->begin(); it != m_possibleAttacks->end(); ++it) {
		int targetedDamage[Attack::MAX_RANGE + 1];
		int areaDamage[Attack::MAX_RANGE + 1];
		int totalDamage = 0;
		calculateAttackDamage(*it, targetedDamage, areaDamage);
		// Set totalDamage to highest value in targetedDamage
		for (size_t i = 0; i < Attack::MAX_RANGE + 1; ++i) {
			if (targetedDamage[i] > totalDamage) {
				totalDamage = targetedDamage[i];
			}
		}
		// Add all areaDamage to totalDamage
		for (size_t i = 0; i < Attack::MAX_RANGE + 1; ++i) {
			totalDamage += areaDamage[i];
		}
		// If totalDamage > maxDamage, set new m_chosenAttack, m_target, and targetMaxDamage
		if (totalDamage > maxDamage) {
			maxDamage = totalDamage;
			m_chosenAttack = *it;
			int maxTargetedDamage = -1;		// Start with negative value so first valid target found will be picked
			for (size_t i = m_chosenAttack->getMinTgt(); i <= m_chosenAttack->getMaxTgt(); ++i) {
				if (targetedDamage[i] > maxTargetedDamage && m_enemies->creature[i]->isTargetable(TargetType::ENEMY)) {
					m_target = i;
					maxTargetedDamage = targetedDamage[i];
				}
			}
		}
	}
	return m_chosenAttack;
}

size_t MaximizeDamage::selectTarget()
{
	return m_target;
}
