#include "Creature.h"
#include "Attack.h"
#include <iostream> //HACK note all uses, only use this for testing purposes
#include <cmath>
#include "AgentAI.h"

#include "Texture.h"	//HACK for testing only

Creature::Creature() : m_xSlidePos(0), m_ySlidePos(0)
{
	//HACK
	m_ongoingEffect.clear();
	m_health = 0;
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_animation = Animation::NONE;
}

Creature::Creature(std::string name,aie::Texture* sprite, aie::Texture* dead,  std::map<Ability, int>& baseAbilities, std::vector<int> attackID, std::map<int, Attack>& attackMap) : m_xSlidePos(0), m_ySlidePos(0)
{
	initialize(name, sprite, dead, baseAbilities, attackID, attackMap);
}


Creature::~Creature()
{
	delete m_agent;
}

void Creature::initialize(std::string name, aie::Texture* sprite, aie::Texture* dead, std::map<Ability, int>& baseAbilities, std::vector<int> attackID, std::map<int, Attack>& attackMap)
{
	//TODO convert as much as possible to initializaiton list
	m_name = name;
	m_sprite = sprite;
	m_dead = dead;
	m_ongoingEffect.clear();
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_animation = Animation::NONE;
	for (std::map<Ability, int>::iterator it = baseAbilities.begin(); it != baseAbilities.end(); ++it) {
		m_ability[it->first] = { it->second, it->second };
	}
	m_health = m_ability[Ability::HEALTH].base;
	for (std::vector<int>::iterator it = attackID.begin(); it != attackID.end(); ++it) {
		addAttack(&attackMap[*it]);				// HACK only works if map never changes during program
	}
}

bool Creature::isAlive()
{
	return m_health > 0;
}

bool Creature::isTargetable(TargetType effectType)
{
	return isAlive();
}

int Creature::getHealth()
{
	return m_health;
}

int Creature::getAbility(Ability ability)
{
	return m_ability[ability].modified;
}

int Creature::getBaseAbility(Ability ability)
{
	return m_ability[ability].base;
}

std::string Creature::getName()
{
	return m_name;
}

void Creature::applyEffect(const EffectInfo &effectInfo)
{
	activateEffect(effectInfo);
	//If effect duration >0, push effect onto ongoing effects
	if (effectInfo.duration > 0) {
		m_ongoingEffect.push_back(effectInfo);
	}
}

void Creature::activateEffect(const EffectInfo &effectInfo)
{
	std::cout << m_name << effectInfo.activateDescription<<std::endl; //HACK messages should be sent to a messageBar
	switch (effectInfo.type) {
	case EffectType::DAMAGE:
			applyDamage(effectInfo.value);
			break;
	case EffectType::HEAL:
				heal(effectInfo.value);
			break;
	case EffectType::BUFF:
		m_ability[effectInfo.ability].modified += effectInfo.value;
		if (effectInfo.ability == Ability::HEALTH) {
			heal(effectInfo.value);
		}
		break;
	case EffectType::DEBUFF:
		m_ability[effectInfo.ability].modified -= effectInfo.value;
		if (effectInfo.ability == Ability::HEALTH || effectInfo.ability == Ability::EVASION) {	// Limit Health and Evasion to minimum of 0
			m_ability[effectInfo.ability].modified = 0;
		}
		break;
	}

	// Start effect animation
	startAnimation(effectInfo.animation);
}

int Creature::applyDamage(int damage)
{
	int damageTaken = m_health;
	m_health -= damage;
	if (m_health < 0) {
		m_health = 0;
		
	} else{
		damageTaken = damage;
	}
	return damageTaken;
}

void Creature::heal(int damage)
{
	int maxHealth = m_ability[Ability::HEALTH].modified;
	m_health += damage;
	if (m_health > maxHealth) {
		m_health = maxHealth;
	}
}

std::vector<EffectInfo>::iterator Creature::getFirstOngoingEffect()
{
	return m_ongoingEffect.begin();
}

bool Creature::applyOngoingEffect(std::vector<EffectInfo>::iterator effect, MessageBar * message)
{
	if (effect != m_ongoingEffect.end()) {
		// Copy effect description to message bar
		std::string msgText = m_name + effect->activateDescription;
		message->setMessage(msgText);
		// Activate effect
		activateEffect(*effect);
		return true;
	}
	else {
		return false;
	}
}



void Creature::update(float deltaTime)
{
	//HACK put magic numbers in constants
	switch (m_animation) {
	case Animation::JUMP:
		m_animationTimer += deltaTime;
		if (m_animationTimer <= 0.6) {
			m_yPos = 0;
		}
		else if (m_animationTimer <= 0.9) {	//Upswing
			m_yPos = 50.0f * (m_animationTimer-0.6f) / 0.3f;
		}
		else if (m_animationTimer <= 1) {	//Downswing
			m_yPos = 50.0f*(1-((m_animationTimer-0.9f) / 0.1f));
		}
		else {								//Ended
			m_animation = Animation::NONE;
		}
		break;
	case Animation::SHAKE:
		m_animationTimer += deltaTime;
		if (m_animationTimer <= 1) {
			m_xPos = 2*sinf(4*(3.1415f) * m_animationTimer);
		}
		else {
			m_animation = Animation::NONE;
			}
		break;
	case Animation::SLIDE:
		m_animationTimer += deltaTime;
		if (m_animationTimer > 1) {
			m_animation = Animation::NONE;
		}
		else {
			m_xPos = m_xSlidePos * (1.0f - m_animationTimer);
			m_yPos = m_ySlidePos * (1.0f - m_animationTimer);
		}
		break;
	case Animation::HOVER:
		m_animationTimer += deltaTime;
		if (m_animationTimer > 1) {
			m_animation = Animation::NONE;
		}
		else {
			if (m_animationTimer < 0.1f) {
				m_yPos = (m_animationTimer / 0.1f) * 25.0f;
			}
			else if(m_animationTimer < 0.9f){
				m_yPos = (4 * sinf(3 * (3.1415f)*((m_animationTimer - 0.1f) / 0.8f)))+25.0f;
			}
			else {
				m_yPos = ((1 - m_animationTimer) / 0.1f)*25.0f;
			}
		}
		break;
	case Animation::BOUNCE:
		m_animationTimer += deltaTime;
		if (m_animationTimer > 1) {
			m_animation = Animation::NONE;
		}
		else {
			m_yPos = 5.0f * (1 - cosf(6 * (3.1415f)*m_animationTimer));
		}
		break;
		//TODO duck animation
	case Animation::DUCK:
		m_animationTimer += deltaTime;
		if (m_animationTimer > 1) {
			m_animation = Animation::NONE;
		}
		else {
			m_yPos = -25.0f;
		}
		break;
	case Animation::NONE:
		m_xPos = 0;
		m_yPos = 0;
		break;
	}
}

void Creature::draw(aie::Renderer2D & renderer, float xPos, float yPos)
{
	if (isAlive()) {
		renderer.drawSprite(m_sprite, xPos+m_xPos, yPos+m_yPos);
		// Draw healthbar
		//Move healthbar with sprite if sliding
		if (m_animation == Animation::SLIDE) {
			xPos += m_xPos;
			yPos += m_yPos;
		}
		renderer.setRenderColour(0xFF0000FF);
		renderer.drawLine(xPos - 50, yPos - 100, xPos + 50, yPos - 100, 5);
		renderer.setRenderColour(0x00FF00FF);
		renderer.drawLine(xPos - 50, yPos - 100, xPos - 50 + (100 * m_health/ m_ability[Ability::HEALTH].modified), yPos - 100, 5);
		renderer.setRenderColour(0xFFFFFFFF);
	}
	else {
		renderer.drawSprite(m_dead, xPos + m_xPos, yPos + m_yPos);
	}
}

void Creature::startAnimation(Animation animation)
{
	m_animation = animation;
	m_animationTimer = 0;
}

void Creature::setSlidePos(float xPos, float yPos)
{
	m_xSlidePos = xPos;
	m_ySlidePos = yPos;
}

void Creature::startTurn()
{
	// Reset abilities to base value
	for (std::map<Ability, AbilityScore>::iterator it = m_ability.begin(); it != m_ability.end(); ++it) {
		it->second.modified = it->second.base;
	}
}

void Creature::endTurn()
{
	// decrease cdTimer on attacks
	for (std::vector<AttackInfo>::iterator it = m_attacks.begin(); it != m_attacks.end(); ++it) {
		//HACK as written cooldown of 1 == cooldown of 0, decide what exactly cooldown means and rewrite to match
		if (it->cdTimer > 0) {
			it->cdTimer--;
		}
	}
	//Decrease ongoing effect durations and remove if <=0
	std::vector<EffectInfo>::iterator effectIter = m_ongoingEffect.begin();		//TODO test this code works
	while (effectIter != m_ongoingEffect.end()) {
		--(effectIter->duration);
		if (effectIter->duration <=0) {
			effectIter = m_ongoingEffect.erase(effectIter);
		}
		else {
			++effectIter;
		}
	}
	//Limit health to modified maximum
	if (m_health > m_ability[Ability::HEALTH].modified) {
		m_health = m_ability[Ability::HEALTH].modified;
	}
}

void Creature::setAttackCD(Attack * attack)
{
	for (std::vector<AttackInfo>::iterator it = m_attacks.begin(); it != m_attacks.end(); ++it) {
		if (it->attack == attack) {
			it->cdTimer = attack->getCooldown();
			break;
		}
	}
}


bool Creature::addAttack(Attack *attack)
{
	if (m_attacks.size() < NUM_ATTACKS) {
		AttackInfo attackInfo = { attack, 0 };
		m_attacks.push_back(attackInfo);
		return true;
	} else{
		return false;
	}
}



bool Creature::isAttackAllowed(AttackInfo attackInfo, CreatureArray friends, CreatureArray enemies, size_t position)
{
	bool attackAllowed;
	Attack *attack = attackInfo.attack;
	if (attackInfo.cdTimer > 0) {
		attackAllowed = false;
	}
	// Check if attack can be made from this position
	else if (attack->getMinPos() > position || attack->getMaxPos() < position) {	
		attackAllowed = false;
	}else {
	// Check if valid target exists
		switch (attack->getMainTarget()) {
		case TargetType::SELF:
			attackAllowed = true;
			break;
		case TargetType::FRIEND:
			attackAllowed = false;
			for (size_t i = attack->getMinTgt(); i < friends.size && i <= attack->getMaxTgt(); i++) {
				if (friends.creature[i]->isTargetable(TargetType::FRIEND)) {
					attackAllowed = true;
					break;
				}
			}
			break;
		case TargetType::ENEMY:
			attackAllowed = false;
			for (size_t i = attack->getMinTgt(); i < enemies.size && i <= attack->getMaxTgt(); i++) {
				if (enemies.creature[i]->isTargetable(TargetType::ENEMY)) {
					attackAllowed = true;
					break;
				}
			}
			break;
		}
	}
	return attackAllowed;
}

std::vector<Attack*> Creature::getPossibleAttacks(CreatureArray friends, CreatureArray enemies, size_t position)
{
	std::vector<Attack*> possibleAttacks;
	for (std::vector<AttackInfo>::iterator it = m_attacks.begin(); it != m_attacks.end(); ++it) {
		if(isAttackAllowed(*it, friends, enemies, position)){
			possibleAttacks.push_back(it->attack);
		}
	}
	return possibleAttacks;
}

void Creature::setAgent(Agent * agent)
{
	m_agent = agent;
	m_agent->setCreature(this);
}


