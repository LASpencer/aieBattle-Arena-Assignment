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
	STRENGTH,
	ACCURACY,
	MYSTIC,
	HOLY,
	TOUGHNESS,
	MAGIC_RESIST,
	EVASION,
	HEALTH
};

enum class Animation {
	NONE,
	JUMP,
	SHAKE,
	SLIDE,
	HOVER,
	BOUNCE,
	DUCK
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
	int cdTimer;
};

class Creature
{
public:
	Creature();
	Creature(std::string name, aie::Texture* sprite, aie::Texture* dead, std::map<Ability, int> &baseAbilities, std::vector<int> attackID, std::map<int, Attack> &attackMap);
	~Creature();

	//TODO initialize creature from loaded values
	void initialize(std::string name, aie::Texture* sprite, aie::Texture* dead, std::map<Ability, int> &baseAbilities, std::vector<int> attackID, std::map<int, Attack> &attackMap);


	bool isAlive();
	bool isTargetable(TargetType effectType);
	int getHealth();
	int getAbility(Ability ability);
	int getBaseAbility(Ability ability);
	std::string getName();

	void applyEffect(const EffectInfo &effectInfo);
	void activateEffect(const EffectInfo &effectInfo);
	int applyDamage(int damage);
	void heal(int damage);

	std::vector<EffectInfo>::iterator getFirstOngoingEffect();
	bool applyOngoingEffect(std::vector<EffectInfo>::iterator effect, MessageBar* message);

	void update(float deltaTime);
	void draw(aie::Renderer2D &renderer, float xPos, float yPos);

	void startAnimation(Animation animation);

	// Sets start position for slide animation
	void setSlidePos(float xPos, float yPos);

	void startTurn();
	void endTurn();
	void setAttackCD(Attack* attack);
	bool isAttackAllowed(AttackInfo attackInfo, CreatureArray friends, CreatureArray enemies, size_t position);
	std::vector<Attack*> getPossibleAttacks(CreatureArray friends, CreatureArray enemies, size_t position);
	void setAgent(Agent* agent); //HACK setting agent in constructor leads to losing it
	Agent* getAgent() {
		return m_agent;
	}

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

