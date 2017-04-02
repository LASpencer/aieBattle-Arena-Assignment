#pragma once
#include <map>
#include <string>
#include <vector>
#include "Renderer2D.h"
#include "Agent.h"
#include "MessageBar.h"

class Creature;

enum class TargetType;
struct EffectInfo;
class Attack;

enum class Ability {
	STRENGTH,			// Often used to increase damage of close combat attacks
	ACCURACY,			// Often used to increase damage of ranged attacks
	MYSTIC,				// Often used to increase damage or value of magic effects
	HOLY,				// Often used to increase value of healing and defensive magic effects
	TOUGHNESS,			// Often used to decrease damage of close combat attacks
	MAGIC_RESIST,		// Often used to decrease damage or value of magic effects targeting this creature
	EVASION,			// Percentage chance an enemy attack can be dodged
	HEALTH				// Maximum health of creature
};

// Possible animations a creature can perform, each takes 1 second
enum class Animation {
	NONE,
	JUMP,				// Jumps once
	SHAKE,				// Wobbles side to side repeatedly
	SLIDE,				// Smoothly move from set slide position to actual position 
	HOVER,				// Rise in the air and bob up and down, then return to ground
	BOUNCE,				// Repeated jumps
	DUCK				// Instantly move down for duration of animation
};

struct AbilityScore {
	int base;
	int modified;
};

struct CreatureArray {
	Creature** creature;
	size_t size;
};

struct AttackInfo {
	Attack* attack;
	int cdTimer;			//Turns until attack can be used
};

/*Creature class
Represents something fighting in the battle.
*/
class Creature
{
public:
	Creature();
	Creature(std::string name, aie::Texture* sprite, aie::Texture* dead, std::map<Ability, int> &baseAbilities, std::vector<int> attackID, std::map<int, Attack> &attackMap);
	~Creature();

	void initialize(std::string name, aie::Texture* sprite, aie::Texture* dead, std::map<Ability, int> &baseAbilities, std::vector<int> attackID, std::map<int, Attack> &attackMap);

	// Returns true if creature has positive health
	bool isAlive();
	// Returns true if creature can be targeted
	bool isTargetable(TargetType effectType);
	int getHealth();
	// Returns modified value of ability
	int getAbility(Ability ability);
	// Returns base value of ability before buffs or debuffs were applied
	int getBaseAbility(Ability ability);
	std::string getName();

	// Applies effect to creature, activating it and adding ongoing effects to m_ongoingEffect
	void applyEffect(const EffectInfo &effectInfo);

	// Modifies creature based on effect
	void activateEffect(const EffectInfo &effectInfo);

	// Reduces creature's health to a minimum of 0
	int applyDamage(int damage);

	// Increases creature's health, up to its maximum value
	void heal(int damage);

	// Returns iterator for first effect in m_ongoingEffect
	std::vector<EffectInfo>::iterator getFirstOngoingEffect();

	/* Applies effect to creature, or returns false if end of m_ongoingEffect reached
	effect = iterator of m_ongoingEffect
	message = pointer to MessageBar where effect description text can be displayed
	*/
	bool applyOngoingEffect(std::vector<EffectInfo>::iterator effect, MessageBar* message);

	void update(float deltaTime);
	void draw(aie::Renderer2D &renderer, float xPos, float yPos);

	// Set creature's animation and reset animation timer
	void startAnimation(Animation animation);

	// Sets start position for slide animation
	void setSlidePos(float xPos, float yPos);

	void startTurn();
	void endTurn();
	// Set cooldown timer on attack passed to attack's cooldown value
	void setAttackCD(Attack* attack);
	/* Check if attack could be performed from current position
	attackInfo = AttackInfo containing attack to be performed
	friends = creature's team
	enemies = creature's enemies
	position = creature's current position
	*/
	bool isAttackAllowed(AttackInfo attackInfo, CreatureArray friends, CreatureArray enemies, size_t position);
	/*Returns vector of attacks that can be made from this position
	friends = creature's team
	enemies = creature's enemies
	position = creature's current position
	*/
	std::vector<Attack*> getPossibleAttacks(CreatureArray friends, CreatureArray enemies, size_t position);

	void setAgent(Agent* agent);
	Agent* getAgent() {
		return m_agent;
	}

	// Add attack to m_attacks, or returns false if too many attack in vector
	bool addAttack(Attack *attack);

	static const size_t NUM_ATTACKS = 4;

protected:
	std::string m_name;
	int m_health;
	std::vector<AttackInfo> m_attacks;
	std::map<Ability, AbilityScore>m_ability;
	std::vector<EffectInfo> m_ongoingEffect;
	aie::Texture* m_sprite;
	aie::Texture* m_dead;
	Agent* m_agent;
	Animation m_animation;
	float m_animationTimer;
	float m_xPos;
	float m_yPos;
	float m_xSlidePos;
	float m_ySlidePos;
};

