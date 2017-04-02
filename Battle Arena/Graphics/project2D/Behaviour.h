#pragma once
#include <vector>

class Creature;
struct CreatureArray;
class Attack;
enum class EffectType;
enum class TargetType;
enum class Ability;

/*Behaviour class
Used to determine what attack and target a creature should choose
*/
class Behaviour
{
public:
	Behaviour();
	~Behaviour();

	void setFriends(CreatureArray* friends);
	void setEnemies(CreatureArray* enemies);
	void setPosition(size_t position);
	void setCreature(Creature* creature);
	void setPossibleAttacks(std::vector<Attack*>* possibleAttacks);

	/* Returns pointer to attack chosen from m_possibleAttacks
		This value is then assigned to m_chosenAttack. Some implementations may also assign m_target
	*/
	virtual Attack* selectAttack() = 0;
	/* Returns index of valid target for m_chosenAttack
	 This may be selected and then assigned to m_target, or just read from m_target
	*/
	virtual size_t selectTarget() = 0;

	/* Calculates damage done by an attack to enemies at each position on enemy team
	attack = pointer to attack to calculate damage for
	targetedDamage = array to copy damage dealt by targeting enemy at this position
	areaDamage = array to copy damage dealt to enemy at this position by area effects
	*/
	void calculateAttackDamage(Attack * attack, int targetedDamage[], int areaDamage[]);

	/* Calculates modified value of attack for a specified effect on an ability to creatures at each position on targeted team
		attack = pointer to attack to calculate value for
		effectType = EffectType to checked
		targetGroup = TargetType to be checked
		ability = Ability for effectType to affect
		targetedValue = array to copy total value of attack's relevant effects by targeting creature at this position
		areaValue = array to copy total value of attack's relevant effects from area effects
	*/
	void calculateAttackValue(Attack* attack, EffectType effectType, TargetType targetGroup, Ability ability, int targetedValue[], int areaValue[]);

	static const float EFFECT_TIME_DISCOUNT;		// Base for geometric series to calculate value of effect in future turn

protected:
	Creature* m_creature;
	CreatureArray* m_enemies;
	CreatureArray* m_friends;
	std::vector<Attack*>* m_possibleAttacks;
	size_t m_position;
	Attack* m_chosenAttack;			// Last attack chosen by selectAttack
	size_t m_target;				// Last target chosen by selectTarget
};

