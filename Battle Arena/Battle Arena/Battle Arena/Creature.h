#pragma once
#include <map>
#include <string>
#include <vector>

const size_t NUM_ATTACKS = 4; //HACK maybe a vector is better?
class Creature;

enum class TargetType;
struct EffectInfo;
class Attack;

enum class Ability {
	STRENGTH,
	ACCURACY,
	TOUGHNESS,
	EVASION,
	HEALTH
};

struct AbilityScore {
	int base;
	int modified;
};

struct CreatureArray {			//HACK can "array" in assignment be array of pointers? if so, could just use nullptr at end instead of saving size
	Creature* creature;
	size_t size;
};

class Creature
{
public:
	Creature();
	Creature(std::string name, std::map<Ability, int> &baseAbilities, std::vector<int> attackID, std::map<int, Attack> &attackMap);
	//TODO value constructor
	//TODO move and copy
	~Creature();

	//TODO initialize creature from loaded values
	void Initialize(std::string name, std::map<Ability, int> &baseAbilities, std::vector<int> attackID, std::map<int, Attack> &attackMap);

	struct AttackInfo {
		Attack* attack;
		int cdTimer;
	};

	bool isAlive();
	bool IsTargetable(TargetType effectType);
	int getHealth();
	int getAbility(Ability ability);
	int getBaseAbility(Ability ability);
	std::string getName();

	virtual void ApplyEffect(const EffectInfo &effectInfo);
	virtual void ActivateEffect(const EffectInfo &effectInfo);
	virtual int ApplyDamage(int damage);
	virtual void Heal(int damage);

	virtual void Update(CreatureArray friends, CreatureArray enemies, size_t position);
	void useAttack(Attack &attack, size_t target, CreatureArray friends, CreatureArray enemies);

	virtual bool AddAttack(Attack *attack);
	virtual bool RemoveAttack(size_t index);

protected:
	std::string m_name;
	int m_health;
	std::vector<AttackInfo> m_attacks;
	std::map<Ability, AbilityScore>m_ability;
	std::vector<EffectInfo> m_ongoingEffect;

	bool IsAttackAllowed(AttackInfo attackInfo, CreatureArray friends, CreatureArray enemies, size_t position);
	std::vector<Attack*> GetPossibleAttacks(CreatureArray friends, CreatureArray enemies, size_t position);
	virtual Attack *SelectAttack(std::vector<Attack*> possibleAttacks, CreatureArray friends, CreatureArray enemies, size_t position);
	virtual size_t SelectTarget(Attack &chosenAttack, CreatureArray targetArray, size_t position);
};

