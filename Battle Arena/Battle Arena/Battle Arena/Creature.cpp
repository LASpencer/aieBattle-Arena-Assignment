#include "Creature.h"
#include "Attack.h"
#include <iostream> //HACK note all uses, only use this for testing purposes


Creature::Creature()
{
	//HACK
	m_ongoingEffect.clear();
	m_health = 0;
}

Creature::Creature(std::string name, std::map<Ability, int>& baseAbilities, std::vector<int> attackID, std::map<int, Attack>& attackMap)
{
	Initialize(name, baseAbilities, attackID, attackMap);
}


Creature::~Creature()
{
}

void Creature::Initialize(std::string name, std::map<Ability, int>& baseAbilities, std::vector<int> attackID, std::map<int, Attack>& attackMap)
{
	m_name = name;
	m_ongoingEffect.clear();
	for (std::map<Ability, int>::iterator it = baseAbilities.begin(); it != baseAbilities.end(); ++it) {
		m_ability[it->first] = { it->second, it->second };
	}
	m_health = m_ability[Ability::HEALTH].base;
	for (std::vector<int>::iterator it = attackID.begin(); it != attackID.end(); ++it) {
		AddAttack(&attackMap[*it]);				// HACK only works if map never changes during program
	}
}

bool Creature::isAlive()
{
	return m_health > 0;
}

bool Creature::IsTargetable(TargetType effectType)
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

void Creature::ApplyEffect(const EffectInfo &effectInfo)
{
	ActivateEffect(effectInfo);
	//TODO if effect duration >0, push effect onto ongoing effects
}

void Creature::ActivateEffect(const EffectInfo &effectInfo)
{
	std::cout << m_name << effectInfo.activateDesctiption<<std::endl; //HACK messages should be sent to a buffer
	if (effectInfo.type == EffectType::DAMAGE) {
		int damageTaken = ApplyDamage(effectInfo.value);
		std::cout << m_name + " took " <<damageTaken<< " damage" << std::endl; 
		if (!isAlive()) {
			std::cout << m_name + " was killed" << std::endl;
		}
	}
	else if (effectInfo.type == EffectType::HEAL) {
		Heal(effectInfo.value);
	}
}

int Creature::ApplyDamage(int damage)
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

void Creature::Heal(int damage)
{
	int maxHealth = m_ability[Ability::HEALTH].modified;
	m_health += damage;
	if (m_health > maxHealth) {
		m_health = maxHealth;
	}
}

void Creature::Update(CreatureArray friends, CreatureArray enemies, size_t position)
{
	//TODO
	//TODO reset abilities to base value
	//TODO apply ongoing effects
	if (isAlive()) {
		//Pick attack and target
		std::vector<Attack*> possibleAttacks = GetPossibleAttacks(friends, enemies, position);
		if (!possibleAttacks.empty()) {			// If attack can be made
			Attack* chosenAttack = SelectAttack(possibleAttacks, friends, enemies, position);
			size_t target;
			switch (chosenAttack->GetMainTarget()) {
			case TargetType::SELF:
				target = position;
				break;
			case TargetType::FRIEND:
				target = SelectTarget(*chosenAttack, friends, position);
				break;
			case TargetType::ENEMY:
				target = SelectTarget(*chosenAttack, enemies, position);
				break;
			}
			// Use attack on target
			useAttack(*chosenAttack, target, friends, enemies);
			//Set cooldown timer on attack used
			for (std::vector<AttackInfo>::iterator it = m_attacks.begin(); it != m_attacks.end(); ++it) {
				if (it->attack == chosenAttack) {
					it->cdTimer = chosenAttack->GetCooldown();
					break;
				}
			}
		}
		// decrease cdTimer on attacks
		for (std::vector<AttackInfo>::iterator it = m_attacks.begin(); it != m_attacks.end(); ++it) {
			//HACK as written cooldown of 1 == cooldown of 0, decide what exactly cooldown means and rewrite to match
			if (it->cdTimer > 0) {
				it->cdTimer--;
			}
		}
	}
}

void Creature::useAttack(Attack &attack, size_t target, CreatureArray friends, CreatureArray enemies)
{
	//TODO
	std::cout << m_name + attack.GetDescription() << std::endl; //HACK messages should be sent to a buffer instead of just displayed
	attack.UseAttack(this, target, friends, enemies);
	
}


bool Creature::AddAttack(Attack *attack)
{
	if (m_attacks.size() < NUM_ATTACKS) {
		AttackInfo attackInfo = { attack, 0 };
		m_attacks.push_back(attackInfo);
		return true;
	} else{
		return false;
	}
}

bool Creature::RemoveAttack(size_t index)
{
	//TODO
	return false;
}

bool Creature::IsAttackAllowed(AttackInfo attackInfo, CreatureArray friends, CreatureArray enemies, size_t position)
{
	bool attackAllowed;
	Attack *attack = attackInfo.attack;
	if (attackInfo.cdTimer > 0) {
		attackAllowed = false;
	}
	// Check if attack can be made from this position
	else if (attack->GetMinPos() > position || attack->GetMaxPos() < position) {	
		attackAllowed = false;
	}else {
	// Check if valid target exists
		switch (attack->GetMainTarget()) {
		case TargetType::SELF:
			attackAllowed = true;
			break;
		case TargetType::FRIEND:
			attackAllowed = false;
			for (size_t i = attack->GetMinTgt(); i < friends.size && i <= attack->GetMaxTgt(); i++) {
				if (friends.creature[i].IsTargetable(TargetType::FRIEND)) {
					attackAllowed = true;
					break;
				}
			}
			break;
		case TargetType::ENEMY:
			attackAllowed = false;
			for (size_t i = attack->GetMinTgt(); i < enemies.size && i <= attack->GetMaxTgt(); i++) {
				if (enemies.creature[i].IsTargetable(TargetType::ENEMY)) {
					attackAllowed = true;
					break;
				}
			}
			break;
		}
	}
	return attackAllowed;
}

std::vector<Attack*> Creature::GetPossibleAttacks(CreatureArray friends, CreatureArray enemies, size_t position)
{
	std::vector<Attack*> possibleAttacks;
	for (std::vector<AttackInfo>::iterator it = m_attacks.begin(); it != m_attacks.end(); ++it) {
		if(IsAttackAllowed(*it, friends, enemies, position)){
			possibleAttacks.push_back(it->attack);
		}
	}
	return possibleAttacks;
}

Attack* Creature::SelectAttack(std::vector<Attack*> possibleAttacks, CreatureArray friends, CreatureArray enemies, size_t position)
{
	//TODO
	return possibleAttacks[0]; //HACK for testing only 
}

size_t Creature::SelectTarget(Attack & chosenAttack, CreatureArray targetArray, size_t position)
{
	//TODO 
	// HACK for testing only
	size_t target;
	for (size_t i = chosenAttack.GetMinTgt(); i <= chosenAttack.GetMaxTgt(); i++) {
		if (targetArray.creature[i].IsTargetable(chosenAttack.GetMainTarget())) {
			target = i;
			break;
		}
	}
	return target;
}

