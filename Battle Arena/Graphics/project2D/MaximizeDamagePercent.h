#pragma once
#include "Behaviour.h"
class MaximizeDamagePercent :
	public Behaviour
{
public:
	MaximizeDamagePercent();
	~MaximizeDamagePercent();

	Attack* SelectAttack();
	size_t SelectTarget();
};

