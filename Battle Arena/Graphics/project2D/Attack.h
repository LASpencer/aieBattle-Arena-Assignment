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

	void setAttackData(AttackData data, std::map<int, Effect> &effectMap);

	// Accessors
	std::string getName() {
		return m_name;
	}
	std::string getDescription() {
		return m_description;
	}
	size_t getMinPos() {
		return m_minPos;
	}
	size_t getMaxPos() {
		return m_maxPos;
	}

	size_t getMinTgt() {
		return m_minTgt;
	}

	size_t getMaxTgt() {
		return m_maxTgt;
	}

	int getCooldown() {
		return m_cooldown;
	}

	TargetType getMainTarget() {
		return m_mainTarget;
	}

	bool addEffect(const Effect &effect);

	std::vector<Effect>* GetEffects();

	// Returns true if all non-self effects are area effects
	bool isAreaEffect();
	// Copies area effect minTgt and maxTgt to array
	bool getAreaEffectRange(size_t range[2]);

	/* Returns iterator of first effect and sets cooldown timer for user
	*/
	std::vector<Effect>::iterator useAttack(Creature* user, MessageBar* message);

	bool applyEffect(std::vector<Effect>::iterator effect, Creature *user, size_t target, CreatureArray* friends, CreatureArray* enemies, bool dodgeArray[], MessageBar* message);

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

