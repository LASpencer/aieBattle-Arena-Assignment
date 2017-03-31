#include "Attack.h"
#include "Creature.h"
#include "MessageBar.h"
#include <iostream> //HACK note all uses, only use this for testing purposes

Attack::Attack()
{
	m_minPos = 0;
	m_maxPos = MAX_RANGE;
	m_minTgt = 0;
	m_maxTgt = MAX_RANGE;
	m_cooldown = 0;
	m_mainTarget = TargetType::SELF;
	m_Animation = Animation::NONE;
}

Attack::Attack(AttackData data, std::map<int, Effect> &effectMap)
{
	setAttackData(data, effectMap);
}


Attack::~Attack()
{
}

void Attack::setAttackData(AttackData data, std::map<int, Effect> &effectMap)
{
	m_name = data.name;
	m_description = data.description;
	m_minPos = data.minPos;			
	m_maxPos = data.maxPos;
	m_minTgt = 0;					// HACK figure out if I should base on AttackData
	m_maxTgt = MAX_RANGE;
	m_cooldown = data.cooldown;
	m_mainTarget = data.mainTarget;
	m_Animation = data.animation;
	for (std::vector<int>::iterator i = data.effectID.begin(); i != data.effectID.end(); ++i) {
		addEffect(effectMap[*i]);
	}
}

bool Attack::addEffect(const Effect &effect)
{
	bool validEffect = true;
	bool effectLimitsTarget = false;
	// check effect can be legally added to the attack
	if (!effect.areaEffect && effect.target != TargetType::SELF) {							// If the effect needs to have a target selected
		if (m_mainTarget == TargetType::SELF || m_mainTarget == effect.target) {		// If the effect's target type doesn't clash with the attack's
			effectLimitsTarget = true;
			if (effect.maxTgt< m_minTgt || effect.minTgt > m_maxTgt) {					// If the effect's target positions don't intersect with attack's, invalid effect
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
		if (m_mainTarget == TargetType::SELF) {
			m_mainTarget = effect.target;
		}
		m_effects.push_back(effect);
	}
	return validEffect;
}

std::vector<Effect>* Attack::GetEffects()
{
	return &m_effects;
}

bool Attack::isAreaEffect()
{
	bool nonSelfExists = false;
	bool areaEffect = true;
	for (std::vector<Effect>::iterator it = m_effects.begin(); it != m_effects.end(); ++it) {
		if (it->target != TargetType::SELF) {
			nonSelfExists = true;
			if (!it->areaEffect) {
				areaEffect = false;
			}
		}
	}
	return nonSelfExists && areaEffect;
}

bool Attack::getAreaEffectRange(size_t range[2])
{
	bool hasAreaEffect = false;
	size_t minTgt = MAX_RANGE;
	size_t maxTgt = 0;
	for (std::vector<Effect>::iterator it = m_effects.begin(); it != m_effects.end(); ++it) {
		// If area effect on main 
		if (it->areaEffect && it->target == m_mainTarget) {
			hasAreaEffect = true;
			// Expand range to include this effect
			if (it->minTgt < minTgt) {
				minTgt = it->minTgt;
			}
			if (it->maxTgt > maxTgt) {
				maxTgt = it->maxTgt;
			}
		}
	}
	if (hasAreaEffect) {
		range[0] = minTgt;
		range[1] = maxTgt;
	}
	return hasAreaEffect;
}

std::vector<Effect>::iterator Attack::useAttack(Creature * user, MessageBar* message)
{

	// Copy attack description to message bar
	std::string attackMessage = user->GetName() + m_description;
	std::cout << attackMessage << std::endl;				//HACK message sent to console for debugging
	message->SetMessage(attackMessage);
	// Start user animation
	user->StartAnimation(m_Animation);
	// Set attack cooldown
	if (m_cooldown > 0) {
		user->SetAttackCD(this);
	}
	return m_effects.begin();
}

bool Attack::applyEffect(std::vector<Effect>::iterator effect, Creature * user, size_t target, CreatureArray * friends, CreatureArray * enemies, bool dodgeArray[], MessageBar* message)
{
	if (effect != m_effects.end()) {
		std::cout << effect->useDescription << std::endl;			//HACK message sent to console for debugging
		CreatureArray* targetGroup;		// Team affected by effect
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
		for (std::map<Ability, float>::iterator it = effect->abilityOffModifier.begin(); it != effect->abilityOffModifier.end(); ++it) {	//Iterate over ability, 
			modifiedInfo.value += (int)(user->getAbility(it->first) * it->second);
		}
		// Apply effect to its targets
		if (effect->target == TargetType::SELF) {	// If effect targets (only) self, apply effect to self
			user->applyEffect(modifiedInfo);
			message->SetMessage(effect->useDescription);	// Copy effect description to message bar
		}
		else if (effect->areaEffect) {				// If area effect, iterate over all possible creatures in targetGroup
			bool attackHit = false;
			for (size_t i = effect->minTgt; i <= effect->maxTgt&&i < targetGroup->size; i++) {
				if (targetGroup->creature[i]->isAlive()) {
					if (effect->target == TargetType::FRIEND || !dodgeArray[i]) {
						EffectInfo defModifiedInfo = modifiedInfo;
						// modify value by target's abilities
						for (std::map<Ability, float>::iterator it = effect->abilityDefModifier.begin(); it != effect->abilityDefModifier.end(); ++it) {
							defModifiedInfo.value -= (int)(targetGroup->creature[i]->getAbility(it->first) * it->second);
						}
						// Make sure modified value is not negative
						if (modifiedInfo.value < 0) {
							modifiedInfo.value = 0;
						}
						targetGroup->creature[i]->applyEffect(modifiedInfo);
						attackHit = true;
					}
					else {
						targetGroup->creature[i]->StartAnimation(Animation::DUCK);
					}
				}
			}
			if (attackHit) {
				message->SetMessage(effect->useDescription);	// Copy effect description to message bar
			}
			else {
				// Display dodge message
				message->SetMessage(effect->useDescription + " but nobody was affected");
			}
		}
		else {										// If single target, apply effect to creature in targetGroup at target position
													// modify value by target's abilities
			if (effect->target == TargetType::FRIEND || !dodgeArray[target]) {
				for (std::map<Ability, float>::iterator it = effect->abilityDefModifier.begin(); it != effect->abilityDefModifier.end(); ++it) {
					modifiedInfo.value -= (int)(targetGroup->creature[target]->getAbility(it->first) * it->second);
				}
				targetGroup->creature[target]->applyEffect(modifiedInfo);
				message->SetMessage(effect->useDescription);	// Copy effect description to message bar
			}
			else {
				targetGroup->creature[target]->StartAnimation(Animation::DUCK);
				message->SetMessage(targetGroup->creature[target]->GetName() + " avoided the attack");
			}
		}
		return true;
	}
	else {
		return false;
	}
}
