#include "Battle.h"
#include "Texture.h"
#include "Input.h"
#include "StateMachine.h"
#include "Creature.h"
#include "Application2D.h"
#include "MessageBar.h"
#include "Attack.h"
#include "Agent.h"

const float Battle::PLAYER_TEAM_STARTPOS = 525.0f;
const float Battle::ENEMY_TEAM_STARTPOS = 755.0f;
const float Battle::POSITION_WIDTH = 150.0f;
const float Battle::GROUND_POS = 200.0f;

Battle::Battle(Application2D* application) : m_message("",640,550)
{
	m_application = application;

	m_turnStateMachine = new StateMachine();

	m_turnStateMachine->add("Start", new Start(this));
	m_turnStateMachine->add("TakeTurn", new TakeTurn(this));
	m_turnStateMachine->add("OngoingEffects", new OngoingEffects(this));
	m_turnStateMachine->add("SelectAction", new SelectAction(this));
	m_turnStateMachine->add("PerformAttack", new PerformAttack(this));
	m_turnStateMachine->add("SortTeams", new SortTeams(this));
	m_turnStateMachine->add("BattleWin", new BattleWin(this));
	m_turnStateMachine->add("BattleLoss", new BattleLoss(this));

	m_background = new aie::Texture("./textures/Background.png");

}


Battle::~Battle()
{
	delete m_background;

	// delete turn states


	delete m_turnStateMachine;
}

void Battle::init()
{
	m_timer = 0.0f;					//Reset timer
	m_activeTeam = m_playerTeam;	//Player takes first turn
	m_passiveTeam = m_enemyTeam;
	m_turnPosition = 0;				//Start with first creature
	m_activeCreaturePosition = 0;
	m_turnStateMachine->Change("Start");
}

void Battle::init(CreatureArray* playerTeam, CreatureArray* enemyTeam) {
	m_playerTeam = playerTeam;
	m_enemyTeam = enemyTeam;
	init();
}

void Battle::exit()
{
	
}

void Battle::update(float deltaTime)
{
	m_timer += deltaTime;
	aie::Input* input = aie::Input::getInstance();
	// Update current state
	m_turnStateMachine->GetState()->update(deltaTime);
	//Update creatures
	for (size_t i = 0; i < m_playerTeam->size && i <= Attack::MAX_RANGE; ++i) {
		m_playerTeam->creature[i]->update(deltaTime);
	}
	for (size_t i = 0; i < m_enemyTeam->size && i <= Attack::MAX_RANGE; ++i) {
		m_enemyTeam->creature[i]->update(deltaTime);
	}
}

void Battle::draw(aie::Renderer2D & renderer)
{

	// Draw background
	renderer.drawSprite(m_background, 640.0f, 360.0f, 1280.0f, 720.0f);

	// Draw creatures
	for (size_t i = 0; i < m_playerTeam->size && i <= Attack::MAX_RANGE; ++i) {
		m_playerTeam->creature[i]->draw(renderer, PLAYER_TEAM_STARTPOS - (POSITION_WIDTH * i), GROUND_POS+100);
	}
	for (size_t i = 0; i < m_enemyTeam->size && i <= Attack::MAX_RANGE; ++i) {
		m_enemyTeam->creature[i]->draw(renderer, ENEMY_TEAM_STARTPOS + (POSITION_WIDTH * i), GROUND_POS+100);
	}


	// Draw current state
	m_turnStateMachine->GetState()->draw(renderer);

}

Battle::Start::Start(Battle * battle) : m_battle(battle), m_startMessage(&battle->m_message), m_timer(0)
{
}

Battle::Start::~Start()
{
}

void Battle::Start::init()
{
	std::string startMessage = m_battle->m_enemyTeam->creature[0]->GetName() + "'s band attacks the party!";
	m_startMessage->SetMessage(startMessage);
	m_timer = 0;
}

void Battle::Start::exit()
{
}

void Battle::Start::update(float deltaTime)
{
	// Exit to TakeTurn after 1 second
	m_timer += deltaTime;
	if (m_timer > 1) {
			m_stateMachine->Change("TakeTurn");
	}
}

void Battle::Start::draw(aie::Renderer2D &renderer)
{
	// Print battle start message
	
	m_startMessage->draw(renderer);
}

Battle::TakeTurn::TakeTurn(Battle * battle) : m_battle(battle), m_turnMessage(&battle->m_message)
{
}

Battle::TakeTurn::~TakeTurn()
{
}

void Battle::TakeTurn::init()
{
	// Reset msgTimer
	m_msgTimer = 0;
	// If active team has more members than current turn position
	if (m_battle->m_turnPosition < m_battle->m_activeTeam->size) {
		// Set active creature position
		m_battle->m_activeCreaturePosition = m_battle->m_turnPosition;
		// Increment turn position
		++m_battle->m_turnPosition;
		// Check creature is alive
		if (m_battle->m_activeTeam->creature[m_battle->m_activeCreaturePosition]->isAlive()) {
			std::string turnMessage = m_battle->m_activeTeam->creature[m_battle->m_activeCreaturePosition]->GetName() + "'s turn";
			m_turnMessage->SetMessage(turnMessage);
			// Start creature's turn
			m_battle->m_activeTeam->creature[m_battle->m_activeCreaturePosition]->startTurn();
		}
		else {
			// If not re-initialize TakeTurn for next creature
			init();
		}
		
	}
	else {
		// Reset turn position
		m_battle->m_turnPosition = 0;
		// Change active team
		CreatureArray* temp = m_battle->m_activeTeam;
		m_battle->m_activeTeam = m_battle->m_passiveTeam;
		m_battle->m_passiveTeam = temp;
		// If enemy team just played, turn is over so exit to SortTeams
		if (m_battle->m_activeTeam == m_battle->m_playerTeam) {
			m_stateMachine->Change("SortTeams");
		}
		// Else re-initialize TakeTurn state with new team
		else {
			init();
		}
	}
}

void Battle::TakeTurn::exit()
{
}

void Battle::TakeTurn::update(float deltaTime)
{
	m_msgTimer += deltaTime;
	// After displaying message for 1 second, exit to OngoingEffects
	if (m_msgTimer > 1) {
		m_stateMachine->Change("OngoingEffects");
	}
}

void Battle::TakeTurn::draw(aie::Renderer2D &renderer)
{
	m_turnMessage->draw(renderer);
}

Battle::SelectAction::SelectAction(Battle * battle) : m_battle(battle)
{
}

void Battle::SelectAction::init()
{
	m_battle->m_attack = nullptr;
	// Pass information to active creature's agent
	m_agent = m_battle->m_activeTeam->creature[m_battle->m_activeCreaturePosition]->GetAgent();
	m_agent->startTurn(m_battle->m_activeTeam, m_battle->m_passiveTeam, m_battle->m_activeCreaturePosition);
	// Check if agent has possible moves and end turn if not
	if (!m_agent->canTakeTurn()) {
		m_stateMachine->Change("TakeTurn");
	}
}

void Battle::SelectAction::exit()
{
	//End creature's turn
	m_battle->m_activeTeam->creature[m_battle->m_activeCreaturePosition]->endTurn();
}

void Battle::SelectAction::update(float deltaTime)
{
	m_agent->update(deltaTime);
	if (m_agent->hasDecided()) {
		m_battle->m_attack = m_agent->getChosenAttack();
		m_battle->m_target = m_agent->getTarget();
		m_stateMachine->Change("PerformAttack");
	}
}

void Battle::SelectAction::draw(aie::Renderer2D &renderer)
{
	// Draw agent
	m_agent->draw(renderer);
}

Battle::PerformAttack::PerformAttack(Battle * battle) : m_battle(battle), m_attackMsg(&battle->m_message)
{
}

Battle::PerformAttack::~PerformAttack()
{
}

void Battle::PerformAttack::init()
{
	m_activeCreature = m_battle->m_activeTeam->creature[m_battle->m_activeCreaturePosition];
	// Check which enemies dodged attack
	for (size_t i = 0; i <= Attack::MAX_RANGE; ++i) {
		int evasion = m_battle->m_passiveTeam->creature[i]->getAbility(Ability::EVASION);
		m_dodgeArray[i] = (rand() % 100 < evasion);
	}
	// Get first effect
	m_effectIter = m_battle->m_attack->UseAttack(m_activeCreature, m_attackMsg);
	// Reset timer to 0
	m_msgTimer = 0;
}

void Battle::PerformAttack::exit()
{
}

void Battle::PerformAttack::update(float deltaTime)
{
	// Wait for animations to end
	m_msgTimer += deltaTime;
	if (m_msgTimer > 1) {
		// Apply current effect(if it's not end of vector)
		if (m_battle->m_attack->ApplyEffect(m_effectIter, m_activeCreature, m_battle->m_target, m_battle->m_activeTeam, m_battle->m_passiveTeam, m_dodgeArray, m_attackMsg))
		{
			// Reset timer and go to next effect
			m_msgTimer = 0;
			++m_effectIter;
		}
		else {
			// Exit to next character's turn
			m_stateMachine->Change("TakeTurn");
		}
	}
}

void Battle::PerformAttack::draw(aie::Renderer2D &renderer)
{
	m_attackMsg->draw(renderer);
}

Battle::SortTeams::SortTeams(Battle* battle) : m_timer(0), m_battle(battle), m_message(&battle->m_message)
{
}

void Battle::SortTeams::init()
{
	// map creature* to original position
	std::map<Creature*, size_t> startPosition;
	for (size_t i = 0; i < m_battle->m_playerTeam->size; ++i) {
		startPosition[m_battle->m_playerTeam->creature[i]] = i;
	}
	for (size_t i = 0; i < m_battle->m_enemyTeam->size; ++i) {
		startPosition[m_battle->m_enemyTeam->creature[i]] = i;
	}
	// Sort player team
	SortCreatureArray(m_battle->m_playerTeam);
	// Sort enemy team
	SortCreatureArray(m_battle->m_enemyTeam);
	// If playerTeam is dead, exit to BattleLoss
	if (!m_battle->m_playerTeam->creature[0]->isAlive()) {
		m_stateMachine->Change("BattleLoss");
	// If enemyTeam is dead, exit to BattleWin
	}
	else if (!m_battle->m_enemyTeam->creature[0]->isAlive()) {
		m_stateMachine->Change("BattleWin");
	}
	else {
	// Else make creatures slide to new positions
		for (size_t i = 0; i < m_battle->m_playerTeam->size; ++i) {
			int difference = (int)i - (int)startPosition[m_battle->m_playerTeam->creature[i]];
			m_battle->m_playerTeam->creature[i]->SetSlidePos(difference * POSITION_WIDTH, 0);
			m_battle->m_playerTeam->creature[i]->StartAnimation(Animation::SLIDE);
		}
		for (size_t i = 0; i < m_battle->m_enemyTeam->size; ++i) {
			int difference = (int)i - (int)startPosition[m_battle->m_enemyTeam->creature[i]];
			m_battle->m_enemyTeam->creature[i]->SetSlidePos(difference * (-POSITION_WIDTH), 0);
			m_battle->m_enemyTeam->creature[i]->StartAnimation(Animation::SLIDE);
		}
		// Set message text
		std::string msgText  = m_battle->m_playerTeam->creature[0]->GetName() + " faces off against " + m_battle->m_enemyTeam->creature[0]->GetName();
		m_message->SetMessage(msgText);
		//Reset timer
		m_timer = 0;
	}
}

void Battle::SortTeams::exit()
{

}

void Battle::SortTeams::update(float deltaTime)
{
	// Wait for creatures to slide into position
	m_timer += deltaTime;
	if (m_timer > 1) {
		m_stateMachine->Change("TakeTurn");
	}
}

void Battle::SortTeams::draw(aie::Renderer2D &renderer)
{
	m_message->draw(renderer);
}

void Battle::SortTeams::SortCreatureArray(CreatureArray * team)
{
	for (size_t i = 1; i < team->size; ++i) {
		Creature* temp = team->creature[i]; //HACK need proper copy constructor
		size_t j = i;
		while (j > 0 && temp->getHealth() > team->creature[j - 1]->getHealth()) {
			team->creature[j] = team->creature[j - 1];
			--j;
		}
		team->creature[j] = temp;
	}
}

Battle::BattleWin::BattleWin(Battle * battle): m_battle(battle), m_winMessage(&battle->m_message), m_timer(0)
{
}

Battle::BattleWin::~BattleWin() {
}

void Battle::BattleWin::init()
{
	m_timer = 0;
	m_winMessage->SetMessage("You have triumphed!");
}

void Battle::BattleWin::exit()
{
}

void Battle::BattleWin::update(float deltaTime)
{

	// Battle state exits to VictoryScreen
	m_timer += deltaTime;
	if (m_timer > 1) {
		m_battle->m_stateMachine->Change("VictoryScreen");
	}
}

void Battle::BattleWin::draw(aie::Renderer2D &renderer)
{
	m_winMessage->draw(renderer);
}

Battle::BattleLoss::BattleLoss(Battle * battle) : m_battle(battle), m_loseMessage(&battle->m_message), m_timer(0)
{
}

Battle::BattleLoss::~BattleLoss() {

}

void Battle::BattleLoss::init()
{
	m_timer = 0;
	m_loseMessage->SetMessage("You were defeated");
}

void Battle::BattleLoss::exit()
{
}

void Battle::BattleLoss::update(float deltaTime)
{
	// Battle state exits to Game Over
	m_timer += deltaTime;
	if (m_timer > 1) {
		m_battle->m_stateMachine->Change("GameOver");
	}
}

void Battle::BattleLoss::draw(aie::Renderer2D &renderer)
{
	m_loseMessage->draw(renderer);
}

Battle::OngoingEffects::OngoingEffects(Battle * battle) : m_battle(battle), m_message(&battle->m_message)
{
}

void Battle::OngoingEffects::init()
{
	m_activeCreature = m_battle->m_activeTeam->creature[m_battle->m_activeCreaturePosition];
	// Get first effect
	m_effectIter = m_activeCreature->GetFirstOngoingEffect();
	// Set timer to 1, so first effect is checked
	m_timer = 1;
}

void Battle::OngoingEffects::exit()
{
}

void Battle::OngoingEffects::update(float deltaTime)
{
	// Wait for animations to end
	m_timer += deltaTime;
	if (m_timer > 1) {
		// Apply current effect (if not at end of vector)
		if (m_activeCreature->isAlive() && m_activeCreature->ApplyOngoingEffect(m_effectIter, m_message)) {
			// Reset timer and go to next effect
			m_timer = 0;
			++m_effectIter;
		}
		else {
			if (m_activeCreature->isAlive()) {
				// If still alive, exit to SelectAction
				m_stateMachine->Change("SelectAction");
			}
			else {
				// Else, exit to next creature's turn
				m_stateMachine->Change("TakeTurn");
			}
		}
	}
}

void Battle::OngoingEffects::draw(aie::Renderer2D & renderer)
{
	m_message->draw(renderer);
}
