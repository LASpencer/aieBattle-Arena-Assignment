#pragma once
#include "Behaviour.h"
class MaximizeDamage :
	public Behaviour
{
public:
	MaximizeDamage();
	~MaximizeDamage();

	Attack* selectAttack();
	size_t selectTarget();
};

