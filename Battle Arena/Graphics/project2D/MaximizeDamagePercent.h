#pragma once
#include "Behaviour.h"

/*MaximizeDamagePercent class

Behaviour that chooses attacks and target to do the greatest possible ratio of damage to remaining health
*/
class MaximizeDamagePercent :
	public Behaviour
{
public:
	MaximizeDamagePercent();
	~MaximizeDamagePercent();

	// Selects attack and target to maximize portion of remaining health taken from creature
	Attack* selectAttack();
	// Returns target chosen by selectAttack as taking the most damage in proportion to health
	size_t selectTarget();
};

