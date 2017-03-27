#pragma once
#include <vector>

class Creature;
struct CreatureArray;
class Attack;
enum class EffectType;
enum class TargetType;
enum class Ability;

class Behaviour
{
public:
	Behaviour();
	~Behaviour();

	void SetFriends(CreatureArray* friends);
	void SetEnemies(CreatureArray* enemies);
	void SetPosition(size_t position);
	void SetCreature(Creature* creature);
	void SetPossibleAttacks(std::vector<Attack*>* possibleAttacks);

	virtual Attack* SelectAttack() = 0;
	virtual size_t SelectTarget() = 0;

	// Calculates damage dealt to creatures at each position on enemy team, copying damage to targeted creature to targetedDamage array and area effect damage to areaDamage array
	void CalculateAttackDamage(Attack * attack, int targetedDamage[], int areaDamage[]);
	void CalculateAttackValue(Attack* attack, EffectType effectType, TargetType targetGroup, Ability ability, int targetedValue[], int areaValue[]);

protected:
	Creature* m_creature;
	CreatureArray* m_enemies;
	CreatureArray* m_friends;
	std::vector<Attack*>* m_possibleAttacks;
	size_t m_position;
	Attack* m_chosenAttack;
	size_t m_target;
};

