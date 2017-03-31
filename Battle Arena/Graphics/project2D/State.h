#pragma once
#include <string>
#include <map>
#include "Renderer2D.h"

class StateMachine;

class State
{
public:
	State();
	~State();

	virtual void init() =0;
	virtual void exit() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw(aie::Renderer2D &renderer) =0;
	virtual void add(StateMachine *stateMachine);

protected:
	StateMachine* m_stateMachine;
};

