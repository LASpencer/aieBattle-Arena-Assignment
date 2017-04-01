#pragma once
#include "GameModeState.h"
#include "MessageBar.h"
#include <vector>
#include "Attack.h"

struct CreatureArray;
class Creature;
class Attack;
class Agent;
struct Effect;
struct EffectInfo;

/*
Battle state
Battle fought between player and enemy teams
Exits when one team has no living members
*/
class Battle :
	public GameModeState
{
public:
	Battle(Application2D* application);
	~Battle();

	virtual void init();
	void init(CreatureArray * playerTeam, CreatureArray * enemyTeam);
	virtual void exit();
	virtual void update(float deltaTime);
	virtual void draw(aie::Renderer2D &renderer);


	// Substates 

	/*
	Start
	Displays message for start of battle
	Exits to TakeTurn
	*/
	class Start : public State {
	public:
		Start(Battle* battle);
		~Start();
		void init();
		void exit();
		void update(float deltaTime);
		void draw(aie::Renderer2D &renderer);

	protected:
		Battle* m_battle;
		MessageBar* m_startMessage;
		float m_timer;
	};
	friend class Start;
	/*
	TakeTurn
	Selects creature to take turn and exits to OngoingEffects, or SortTeams if no creatures left
	*/
	class TakeTurn : public State {
	public:
		TakeTurn(Battle* battle);
		~TakeTurn();
		void init();
		void exit();
		void update(float deltaTime);
		void draw(aie::Renderer2D &renderer);

	protected:
		Battle* m_battle;
		float m_msgTimer;
		MessageBar* m_turnMessage;
	};
	friend class TakeTurn;
	/*
	OngoingEffects
	Applies ongoing effects on current creature and displays messages and animations
	Exits to SelectAction if they're still alive or TakeTurn if not
	*/
	class OngoingEffects :public State {
	public:
		OngoingEffects(Battle* battle);
		void init();
		void exit();
		void update(float deltaTime);
		void draw(aie::Renderer2D &renderer);
	protected:
		Battle* m_battle;
		Creature* m_activeCreature;
		std::vector<EffectInfo>::iterator m_effectIter;
		MessageBar* m_message;
		float m_timer;
	};
	friend class OngoingEffects;
	/*
	SelectAction
	Asks current creature's agent to choose an attack and target
	Exits to PerformAttack when attack chosen, or TakeTurn if no attacks are possible
	*/
	class SelectAction : public State {
	public:
		SelectAction(Battle* battle);
		void init();
		void exit();
		void update(float deltaTime);
		void draw(aie::Renderer2D &renderer);


	protected:
		Battle* m_battle;
		Agent* m_agent;
	};
	friend class SelectAction;

	/*
	PerformAttack
	Display message and perform animation for chosen attack and its effects, and apply those effects to its targets
	Exits to TakeTurn when all animations complete
	*/
	class PerformAttack :public State {
	public:
		PerformAttack(Battle* battle);
		~PerformAttack();
		void init();
		void exit();
		void update(float deltaTime);
		void draw(aie::Renderer2D &renderer);

	protected:
		Battle* m_battle;
		Creature* m_activeCreature;
		std::vector<Effect>::iterator m_effectIter;
		MessageBar* m_attackMsg;
		bool m_dodgeArray[Attack::MAX_RANGE + 1];
		float m_msgTimer;
	};
	friend class PerformAttack;

	/*
	SortTeams
	Sort both teams by health
	Exits to TakeTurn if both sides are alive, BattleWin if player team alive and enemy team killed, BattleLoss if player team killed
	*/
	class SortTeams :public State {
	public:
		SortTeams(Battle* battle);
		void init();
		void exit();
		void update(float deltaTime);
		void draw(aie::Renderer2D &renderer);
		void SortCreatureArray(CreatureArray* team);

	protected:
		Battle* m_battle;
		float m_timer;
		MessageBar *m_message;
	};
	friend class SortTeams;

	/*
	BattleWin
	Displays victory message
	Causes Battle to exit to VictoryScreen
	*/
	class BattleWin :public State {
	public:
		BattleWin(Battle* battle);
		~BattleWin();
		void init();
		void exit();
		void update(float deltaTime);
		void draw(aie::Renderer2D &renderer);

	protected:
		Battle* m_battle;
		MessageBar* m_winMessage;
		float m_timer;
	};
	friend class BattleWin;

	/*
	BattleLoss
	Displays defeat message
	Causes Battle to exit to GameOver
	*/
	class BattleLoss :public State {
	public:
		BattleLoss(Battle* battle);
		~BattleLoss();
		void init();
		void exit();
		void update(float deltaTime);
		void draw(aie::Renderer2D &renderer);

	protected:
		Battle* m_battle;
		MessageBar* m_loseMessage;
		float m_timer;
	};
	friend class BattleLoss;

	static const float PLAYER_TEAM_STARTPOS;	// X position for player team creature at index 0
	static const float ENEMY_TEAM_STARTPOS;		// X position for enemy team creature at index 0
	static const float POSITION_WIDTH;			// Distance between each creature in a team
	static const float GROUND_POS;				// Y position for bottom of creature sprites

private:
	aie::Texture* m_background;
	float m_timer;
	CreatureArray* m_playerTeam;
	CreatureArray* m_enemyTeam;
	CreatureArray* m_activeTeam;
	CreatureArray* m_passiveTeam;
	size_t m_turnPosition;
	size_t m_activeCreaturePosition;
	Attack* m_attack;
	size_t m_target;
	MessageBar m_message;

	// State Machine containing each part of a battle turn
	StateMachine* m_turnStateMachine;
};

