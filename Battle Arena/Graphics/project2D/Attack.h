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

enum class Animation;

enum class TargetType {
	ENEMY,
	FRIEND,
	SELF
};

struct EffectInfo
{
	std::string activateDescription;
	EffectType type;
	int duration;
	int value;
	Ability ability;
	Animation animation;
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
	Animation animation;
};

class Creature;
class MessageBar;
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

	std::vector<Effect>* GetEffects();

	// Returns true if all non-self effects are area effects
	bool IsAreaEffect();
	// Copies area effect minTgt and maxTgt to array
	bool GetAreaEffectRange(size_t range[2]);

	/* Returns iterator of first effect and sets cooldown timer for user
	*/
	std::vector<Effect>::iterator UseAttack(Creature* user, MessageBar* message);

	bool ApplyEffect(std::vector<Effect>::iterator effect, Creature *user, size_t target, CreatureArray* friends, CreatureArray* enemies, bool dodgeArray[], MessageBar* message);

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
		Animation m_Animation;
};

