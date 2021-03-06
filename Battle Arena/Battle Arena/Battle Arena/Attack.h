#pragma once
#include <string>
#include <vector>
#include <map>

enum class EffectType {
	DAMAGE,
	HEAL,
	BUFF,
	DEBUFF
};

enum class Ability;

enum class TargetType {
	ENEMY,
	FRIEND,
	SELF
};

struct EffectInfo
{
	std::string activateDesctiption;
	EffectType type;
	int duration;
	int value;
	Ability abilty;
};



struct Effect
{
	std::string useDescription;
	EffectInfo info;
	int baseValue;
	size_t minTgt;
	size_t maxTgt;
	bool areaEffect;
	TargetType target;
	std::map<Ability, float> abilityOffModifier;
	std::map<Ability, float> abilityDefModifier;
};

// TODO function to put together attack from data provided, effects list, etc
struct AttackData
{
	std::string name;
	std::string description;
	size_t minPos;
	size_t maxPos;
	size_t minTgt;
	size_t maxTgt;
	int cooldown;
	TargetType mainTarget;
	std::vector<int> effectID;
};

class Creature;
struct CreatureArray;

class Attack
{
public:
	Attack();
	Attack(AttackData data, std::map<int, Effect> &effectMap);
	~Attack();

	void SetAttackData(AttackData data, std::map<int, Effect> &effectMap);

	// Accessors
	std::string GetName() {
		return m_name;
	}
	std::string GetDescription() {
		return m_description;
	}
	size_t GetMinPos() {
		return m_minPos;
	}
	size_t GetMaxPos() {
		return m_maxPos;
	}

	size_t GetMinTgt() {
		return m_minTgt;
	}

	size_t GetMaxTgt() {
		return m_maxTgt;
	}

	int GetCooldown() {
		return m_cooldown;
	}

	TargetType GetMainTarget() {
		return m_mainTarget;
	}

	bool AddEffect(const Effect &effect);

	void UseAttack(Creature *user, size_t target, CreatureArray friends, CreatureArray enemies);

	//HACK figure out where this should actually go
	static const size_t MAX_RANGE = 3;

private:
		std::string m_name;
		std::string m_description;
		size_t m_minPos;
		size_t m_maxPos;
		size_t m_minTgt;
		size_t m_maxTgt;
		int m_cooldown;
		TargetType m_mainTarget;
		std::vector<Effect> m_effects;
};

