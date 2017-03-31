#pragma once
#include "Behaviour.h"
class MaximizeDamagePercent :
	public Behaviour
{
public:
	MaximizeDamagePercent();
	~MaximizeDamagePercent();

	Attack* selectAttack();
	size_t selectTarget();
};

