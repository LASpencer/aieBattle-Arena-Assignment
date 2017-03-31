#pragma once
#include "Behaviour.h"
class PickRandom :
	public Behaviour
{
public:
	PickRandom();
	~PickRandom();

	Attack* selectAttack();
	size_t selectTarget();
};

