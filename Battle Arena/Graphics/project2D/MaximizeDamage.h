#pragma once
#include "Behaviour.h"
class MaximizeDamage :
	public Behaviour
{
public:
	MaximizeDamage();
	~MaximizeDamage();

	Attack* SelectAttack();
	size_t SelectTarget();
};

