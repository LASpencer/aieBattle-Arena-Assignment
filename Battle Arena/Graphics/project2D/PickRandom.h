#pragma once
#include "Behaviour.h"
/*PickRandom class

Selects random option from possible attacks
*/
class PickRandom :
	public Behaviour
{
public:
	PickRandom();
	~PickRandom();

	// Returns randomly chosen attack from m_possibleAttacks
	Attack* selectAttack();

	// Returns randomly chosen valid target
	size_t selectTarget();
};

