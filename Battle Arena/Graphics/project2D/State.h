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

	virtual void Init() =0;
	virtual void Exit() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(aie::Renderer2D &renderer) =0;
	virtual void Add(StateMachine *stateMachine);

protected:
	StateMachine* m_stateMachine;
};

