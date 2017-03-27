#pragma once
#include "Behaviour.h"
class PickRandom :
	public Behaviour
{
public:
	PickRandom();
	~PickRandom();

	Attack* SelectAttack();
	size_t SelectTarget();
};

