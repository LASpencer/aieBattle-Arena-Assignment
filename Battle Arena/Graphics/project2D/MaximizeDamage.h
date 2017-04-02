#pragma once
#include "Behaviour.h"
/* MaximizeDamage class
Behaviour that chooses attacks and target to maximize total damage done
*/
class MaximizeDamage :
	public Behaviour
{
public:
	MaximizeDamage();
	~MaximizeDamage();

	// Selects attack that will do the most damage, and sets m_target as target taking most damage
	Attack* selectAttack();
	// Returns target chosen by selectAttack as taking the most damage
	size_t selectTarget();
};

