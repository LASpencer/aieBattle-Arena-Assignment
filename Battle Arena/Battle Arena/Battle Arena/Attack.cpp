#include "Attack.h"
#include "Creature.h"
#include <iostream> //HACK note all uses, only use this for testing purposes

Attack::Attack()
{
	m_minPos = 0;
	m_maxPos = MAX_RANGE;
	m_minTgt = 0;
	m_maxTgt = MAX_RANGE;
	m_cooldown = 0;
	m_mainTarget = TargetType::SELF;
}

Attack::Attack(AttackData data, std::map<int, Effect> &effectMap)
{
	SetAttackData(data, effectMap);
}


Attack::~Attack()
{
}

void Attack::SetAttackData(AttackData data, std::map<int, Effect> &effectMap)
{
	//TODO
	m_name = data.name;
	m_description = data.description;
	m_minPos = data.minPos;			
	m_maxPos = data.maxPos;
	m_minTgt = 0;					// HACK figure out if I should base on AttackData
	m_maxTgt = MAX_RANGE;
	m_cooldown = data.cooldown;
	m_mainTarget = data.mainTarget;
	for (std::vector<int>::iterator i = data.effectID.begin(); i != data.effectID.end(); ++i) {
		AddEffect(effectMap[*i]);
	}
}

bool Attack::AddEffect(const Effect &effect)
{
	bool validEffect = true;
	bool effectLimitsTarget = false;
	// check effect can be legally added to the attack
	if (!effect.areaEffect && effect.target != TargetType::SELF) {							// If the effect needs to have a target selected
		if (m_mainTarget == TargetType::SELF || m_mainTarget == effect.target) {		// If the effect's target type doesn't clash with the attack's
			effectLimitsTarget = true;
			if (effect.maxTgt< m_minTgt || effect.minTgt > m_maxTgt) {					// If the effect's target position's don't intersect with attack's, invalid effect
				validEffect = false;
			}
		}
		else {																					// Else effect and attack target types clash, invalid effect
			validEffect = false;
		}
	}
	if (validEffect) {
		if (effectLimitsTarget) {
			if (m_minTgt < effect.minTgt) {			// Increase attack minTgt to effect minTgt
				m_minTgt = effect.minTgt;
			}
			if (m_maxTgt > effect.maxTgt) {			// Decrease attack maxTgt to effect maxTgt
				m_maxTgt = effect.maxTgt;
			}
		}
		m_effects.push_back(effect);
	}
	return validEffect;
}

void Attack::UseAttack(Creature * user, size_t target, CreatureArray friends, CreatureArray enemies)
{
														   // TODO code for missing/dodging attack (bool array[enemies.size] holding whether enemy was hit, don't apply effect if false)
														   // TODO or char array holding codes for hit, missed, not checked.
	for (std::vector<Effect>::iterator effect = m_effects.begin(); effect != m_effects.end(); effect++) {		// Iterate over attack's effects
		std::cout << effect->useDescription << std::endl; //HACK messag should be sent to a buffer
		CreatureArray targetGroup;		// Team affected by effect
		switch (effect->target) {		// Set targetGroup based on effect's target type
		case TargetType::SELF:
		case TargetType::FRIEND:
			targetGroup = friends;
			break;
		case TargetType::ENEMY:
			targetGroup = enemies;
			break;
		}
		// modify info by user's abilites
		EffectInfo modifiedInfo = effect->info;
		for (std::map<Ability, float>::iterator it = effect->abilityOffModifier.begin(); it != effect->abilityOffModifier.end(); ++it) {
			modifiedInfo.value += (int)(user->getAbility(it->first) * it->second);
		}
		// Apply effect to its targets
		if (effect->target == TargetType::SELF) {	// If effect targets (only) self, apply effect to self
			user->ApplyEffect(modifiedInfo);
		}
		else if (effect->areaEffect) {				// If area effect, iterate over all possible creatures in targetGroup
			for (size_t i = effect->minTgt; i <= effect->maxTgt&&i < targetGroup.size; i++) {
				EffectInfo defModifiedInfo = modifiedInfo;
				// modify value by target's abilities
				for (std::map<Ability, float>::iterator it = effect->abilityDefModifier.begin(); it != effect->abilityDefModifier.end(); ++it) {
					defModifiedInfo.value -= (int)(targetGroup.creature[i].getAbility(it->first) * it->second);
				}
				targetGroup.creature[i].ApplyEffect(modifiedInfo);
			}
		}
		else {										// If single target, apply effect to creature in targetGroup at target position
			// modify value by target's abilities
			for (std::map<Ability, float>::iterator it = effect->abilityDefModifier.begin(); it != effect->abilityDefModifier.end(); ++it) {
				modifiedInfo.value -= (int)(targetGroup.creature[target].getAbility(it->first) * it->second);
			}
			targetGroup.creature[target].ApplyEffect(modifiedInfo);
		}
	}
}
