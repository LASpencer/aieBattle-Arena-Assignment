#pragma once
#include "Behaviour.h"
class PickFirstOption :
	public Behaviour
{
public:
	PickFirstOption();
	~PickFirstOption();

	Attack* SelectAttack();
	size_t SelectTarget();
};

