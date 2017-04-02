#pragma once
#include <string>
#include <vector>
#include <map>

// Types of effect
enum class EffectType {
	DAMAGE,			// Effect does damage
	HEAL,			// Effect heals damage up to MaxHealth
	BUFF,			// Effect temporarily increases ability until start of creature's next turn
	DEBUFF			// Effect temporarily decreases ability until start of creature's next turn
};

enum class Ability;

enum class Animation;

// Target for effect or attack
enum class TargetType {
	ENEMY,			// Targets enemy team
	FRIEND,			// Targets friendly team
	SELF			// Targets only self, or no target needs to be chosen
};

// Information necessary to apply effect to a creature
struct EffectInfo
{
	std::string activateDescription;	// Text to display when ongoing effect activates
	EffectType type;					// Type of effect
	int duration;						// Effect duration, if >1 will be applied for that many turns
	int value;							// Amount of damage/ability points 
	Ability ability;					// Ability the buff or debuff will modify
	Animation animation;				// Animation to perform when effect is applied
};


// An effect of an attack
struct Effect
{
	std::string useDescription;			// Text to display when effect is used
	EffectInfo info;					// Information necessary to apply effect to a creature
	int baseValue;						// Value before modifiers are applied
	size_t minTgt;						// Lowest index in CreatureArray this effect can target
	size_t maxTgt;						// Highest index in CreatureArray this effect can target
	bool areaEffect;					// If true, effect applied to all creatures in range
	TargetType target;					// Type of target effect is applied to
	std::map<Ability, float> abilityOffModifier;		// Modifies value by user's abilities
	std::map<Ability, float> abilityDefModifier;		// Modifies value by target's abilities
};

// Information needed to construct an Attack
struct AttackData
{
	std::string name;				// Name of attack
	std::string description;		// Text to display when attack is used
	size_t minPos;					// Lowest index for user of attack
	size_t maxPos;					// Highest index for user of attack
	size_t minTgt;					// Lowest index the attack can target
	size_t maxTgt;					// Highest index the attack can target
	int cooldown;					// How many turns to wait until attack can be used again
	TargetType mainTarget;			// What team has to have a target selected from to use this attack
	std::vector<int> effectID;		// Keys to effects used by attack
	Animation animation;			// Animation to perform when performing attack
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

	/*
	data = information about attack
	effectMap = map of effect including keys from data.effectID
	*/
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

	/* Try to add a new effect to the attack
	returns true if successful, false if invalid effect
	*/
	bool addEffect(const Effect &effect);

	std::vector<Effect>* GetEffects();

	// Returns true if all non-self effects are area effects
	bool isAreaEffect();
	// Copies area effect minTgt and maxTgt to array
	bool getAreaEffectRange(size_t range[2]);

	/* Returns iterator of first effect and sets cooldown timer for user
	user = creature performing attack
	message = pointer to messagebar where description text can be displayed
	*/
	std::vector<Effect>::iterator useAttack(Creature* user, MessageBar* message);

	/* Applies effect to a creature, or returns false if iterator is at end of m_effects vector
	effect = iterator of m_effects
	user = creature performing attack
	target = position of attack's target
	friends = pointer to CreatureArray of user's team
	enemies = pointer to CreatureArray of user's enemies
	dodgeArray = array indicating whether creature at same index in enemies successfully dodged the attack
	message = pointer to messagebar where effect description text can be displayed
	*/
	bool applyEffect(std::vector<Effect>::iterator effect, Creature *user, size_t target, CreatureArray* friends, CreatureArray* enemies, bool dodgeArray[], MessageBar* message);

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

