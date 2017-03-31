#pragma once
#include "Behaviour.h"
class PickFirstOption :
	public Behaviour
{
public:
	PickFirstOption();
	~PickFirstOption();

	Attack* selectAttack();
	size_t selectTarget();
};

