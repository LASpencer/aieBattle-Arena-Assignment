#include "Behaviour.h"
#include "Creature.h"
#include "Attack.h"

const float Behaviour::EFFECT_TIME_DISCOUNT = 0.9f;

Behaviour::Behaviour()
{
}


Behaviour::~Behaviour()
{
}

void Behaviour::setFriends(CreatureArray * friends)
{
	m_friends = friends;
}

void Behaviour::setEnemies(CreatureArray * enemies)
{
	m_enemies = enemies;
}

void Behaviour::setPosition(size_t position)
{
	m_position = position;
}

void Behaviour::setCreature(Creature * creature)
{
	m_creature = creature;
}

void Behaviour::setPossibleAttacks(std::vector<Attack*>* possibleAttacks)
{
	m_possibleAttacks = possibleAttacks;
}

void Behaviour::calculateAttackDamage(Attack * attack, int targetedDamage[], int areaDamage[])
{
	// Call calculateAttackValue for damage to enemies
	calculateAttackValue(attack, EffectType::DAMAGE, TargetType::ENEMY,Ability::HEALTH, targetedDamage, areaDamage);
}

void Behaviour::calculateAttackValue(Attack * attack, EffectType effectType, TargetType targetGroup, Ability ability, int targetedValue[], int areaValue[])
{
	std::vector<Effect>* effects = attack->GetEffects();
	//Set damage arrays to 0
	for (int i = 0; i < Attack::MAX_RANGE + 1; ++i) {
		targetedValue[i] = 0;
		areaValue[i] = 0;
	}
	// Get pointer to relevant team
	CreatureArray* targetedTeam;
	switch (targetGroup) {
	case TargetType::SELF:
	case TargetType::FRIEND:
		targetedTeam = m_friends;
		break;
	case TargetType::ENEMY:
		targetedTeam = m_enemies;
		break;
	}
	//Iterate over each effect in attack
	for (std::vector<Effect>::iterator it = effects->begin(); it != effects->end(); ++it) {
		bool suitableTarget = false;
		switch (targetGroup) {
		case TargetType::SELF:
		case TargetType::FRIEND:
			suitableTarget = (it->target == TargetType::SELF || it->target == TargetType::FRIEND);		//SELF is part of FRIEND team, so effects for both must be checked
			break;
		case TargetType::ENEMY:
			suitableTarget = (it->target == TargetType::ENEMY);
			break;
		}
		if (suitableTarget && it->info.type == effectType && it->info.ability == ability) {		// If effect does specified effect to chosen group
			int attackerModValue= it->baseValue;
			// Modify value by attacker's abilities
			for (std::map<Ability, float>::iterator modIt = it->abilityOffModifier.begin(); modIt != it->abilityOffModifier.end(); ++modIt) {
				attackerModValue += (int)(m_creature->getAbility(modIt->first) * modIt->second);					// Modify value by modifying ability score * modifier coefficient
			}
			/*	Calculate multiplier for ongoing effect duration
				This is calculated with a geometric series, so that effects are considered less valuable the further in the future they occur
			*/
			float ongoingEffectMultiplier = (1.0f - powf(EFFECT_TIME_DISCOUNT, (it->info.duration + 1))) / (1.0f - EFFECT_TIME_DISCOUNT);
			// Iterate over targetGroup
			for (int i = it->minTgt; i <= it->maxTgt && i<targetedTeam->size; ++i) {
				if (it->target != TargetType::SELF || i == m_position) {	//If effect targets other creatures, or checking own position
					int totalModValue = attackerModValue;
					// Modify value by defender's abilities
					for (std::map<Ability, float>::iterator modIt = it->abilityDefModifier.begin(); modIt != it->abilityDefModifier.end(); ++modIt) {
						totalModValue -= (int)(targetedTeam->creature[i]->getAbility(modIt->first) * modIt->second);		// Modify value by modifying ability score * modifier coefficient
					}
					// Make sure modified value is not negative
					if (totalModValue < 0) {
						totalModValue = 0;
					}
					//Calculate hitChance based on defender's evasion (if they're enemies)
					float hitChance;
					if (targetGroup == TargetType::ENEMY) {
						int hitPercent = 100 - targetedTeam->creature[i]->getAbility(Ability::EVASION);		// Percentage chance of evasion not stopping attack
						if (hitPercent >= 100) {
							hitChance = 1.0f;						// max value of 1
						}
						else if (hitPercent <= 0) {
							hitChance = 0.0f;						// min value of 0
						}
						else {
							hitChance = (float)hitPercent / 100.0f;
						}
					}
					else {
						hitChance = 1.0f;							// Friends and self don't evade attacks
					}
					// Multiply value by ongoingEffectMultiplier
					totalModValue *= ongoingEffectMultiplier;
					// Multiply value by hitChance
					totalModValue *= hitChance;
					if (it->areaEffect) {
						// If area effect, add to areaDamage array
						areaValue[i] += totalModValue;
					}
					else {
						// Otherwise add to targetedDamage array
						targetedValue[i] += totalModValue;
					}
				}
			}
		}
	}
}
