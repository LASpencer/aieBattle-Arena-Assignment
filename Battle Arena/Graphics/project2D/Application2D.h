#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Audio.h"
#include "GameModeStateMachine.h"

struct Effect;
class Attack;
class GameModeState;
class AgentAI;

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	aie::Font* GetFont() {
		return m_font;
	}

protected:

	aie::Renderer2D*	m_2dRenderer;

	aie::Texture*		m_sword;
	aie::Texture*		m_assassin;
	aie::Texture*		m_monk;
	aie::Texture*		m_witch;
	aie::Texture*		m_goblin;
	aie::Texture*		m_zombie;
	aie::Texture*		m_imp;
	aie::Texture*		m_medusa;
	aie::Texture*		m_dead;
	aie::Texture*		m_targetArrow;

	aie::Font*			m_font;
	std::map<int, Effect>* m_effectMap;
	std::map<int, Attack>* m_attackMap;
	AgentAI*			m_aiAgent;
	CreatureArray*		m_playerTeam;
	CreatureArray*		m_enemyTeam;
	GameModeStateMachine* m_modeStateMachine;

	float m_cameraX, m_cameraY;
	float m_timer;
};