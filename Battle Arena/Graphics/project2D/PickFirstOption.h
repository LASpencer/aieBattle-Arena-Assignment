#pragma once
#include "Behaviour.h"
/*PickFirstOption class
Behaviour that picks first possible attack and valid target
Provides a simple way to control behaviour: set attacks in order
of preference and the first one possible will be done.
*/
class PickFirstOption :
	public Behaviour
{
public:
	PickFirstOption();
	~PickFirstOption();

	// Returns first attack in m_possibleAttacks
	Attack* selectAttack();

	// Returns lowest index that points to valid target
	size_t selectTarget();
};

