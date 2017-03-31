#include <time.h>
#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Battle.h"
#include "MainMenu.h"
#include "GameOver.h"
#include "VictoryScreen.h"

#include "Attack.h"
#include "Creature.h"
#include "Agent.h"
#include "AgentAI.h"
#include "AgentHuman.h"

#include "PickFirstOption.h"
#include "MaximizeDamage.h"
#include "MaximizeDamagePercent.h"
#include "PickRandom.h"

Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);

	srand((unsigned int)time(NULL));

	m_effectMap = new std::map<int, Effect>();
	//{key,{useDescription,{activateDescription,type,duration,value,ability,animation},baseValue,minTgt,maxTgt,areaEffect,target,{abilityOffModifier},{abilityDefModifier}

	//TODO extract out raw data into another file
	*m_effectMap = {
				{1,{"Their blade swings down",{ " was injured by the blow",EffectType::DAMAGE,0,10,Ability::HEALTH,Animation::SHAKE},10,0,0,false,TargetType::ENEMY, { {Ability::STRENGTH, 1.0f} }, {{Ability::TOUGHNESS,1.0f} } }},
				{2,{"It strikes its target",{" was hit by the arrow", EffectType::DAMAGE,0,5,Ability::HEALTH, Animation::SHAKE},5,0,3,false, TargetType::ENEMY,{{Ability::ACCURACY,1.0f}},{{Ability::TOUGHNESS,1.0f}} }},
				{3,{"Attacks rain down on their opponents", {" was struck", EffectType::DAMAGE, 0,15,Ability::HEALTH, Animation::SHAKE},10,0,1,true,TargetType::ENEMY,{ { Ability::STRENGTH, 0.5f } },{ {Ability::TOUGHNESS,1.0f} }} },
				{4,{"A ball of flame bursts among their enemies", {" was burned", EffectType::DAMAGE, 0, 10, Ability::HEALTH, Animation::SHAKE},10,0,3,true,TargetType::ENEMY,{ {Ability::MYSTIC, 1.0f} }, { {Ability::MAGIC_RESIST, 2.0f} } } },
				{5,{"Wounds close and heal", {" was healed", EffectType::HEAL, 0, 0, Ability::HEALTH, Animation::BOUNCE}, 0, 0,3,false, TargetType::FRIEND,{{Ability::HOLY, 1.0f}},{{Ability::HEALTH, -0.1f}} }},
				{6,{"They recover some health", {" recovered health", EffectType::HEAL, 0, 5, Ability::HEALTH, Animation::BOUNCE}, 5, 0,3,false,TargetType::SELF, { {Ability::TOUGHNESS, 1.0f}},{} }},
				{7,{"It was poisoned", {" was hurt by poison", EffectType::DAMAGE, 4, 2, Ability::HEALTH, Animation::SHAKE}, 2,0,3,false,TargetType::ENEMY, {},{{Ability::TOUGHNESS,0.5f}}}},
				{8,{"A holy light deflects enemy attacks", {" was protected by the gods", EffectType::BUFF, 1, 40, Ability::EVASION, Animation::HOVER}, 40,0,3,false, TargetType::FRIEND,{{Ability::HOLY,5.0f}},{}}},
				{9,{"A baleful light shines down", {" was weakened by the curse", EffectType::DEBUFF, 1, 3, Ability::TOUGHNESS, Animation::SHAKE}, 3,0,3,false, TargetType::ENEMY,{{Ability::MYSTIC,1.0f}},{{Ability::MAGIC_RESIST, 1.0f}}}},
				{10,{"Their teeth sink in", {" was bitten", EffectType::DAMAGE, 0,5, Ability::HEALTH, Animation::SHAKE}, 5, 0,0,false,TargetType::ENEMY,{{Ability::STRENGTH,2.0f}},{{Ability::TOUGHNESS,1.0f}} }},
				{11,{"A cold chill runs through the target", {" feels weak from the bite", EffectType::DEBUFF, 2, 1, Ability::STRENGTH, Animation::SHAKE}, 1, 0,0,false,TargetType::ENEMY,{},{{Ability::MAGIC_RESIST, 1.0f}}}},
				{12,{"Rays of energy blast the target",{" was hit by the rays", EffectType::DAMAGE,0,5,Ability::HEALTH,Animation::SHAKE},5,0,3,false, TargetType::ENEMY,{{Ability::MYSTIC,1.0f}},{{Ability::TOUGHNESS,0.5f},{Ability::MAGIC_RESIST,1.0f}} }},
				{13,{"The party was inspired!",{" feels inspired", EffectType::BUFF,1,2,Ability::STRENGTH,Animation::BOUNCE},2,0,1,true, TargetType::FRIEND,{},{}}},
				{14,{"The powers of evil are driven off",{" was protected from evil magic", EffectType::BUFF,1,2,Ability::MAGIC_RESIST,Animation::HOVER},2,0,3,true,TargetType::FRIEND,{{Ability::HOLY, 1.0f}},{} }}
			};
		
	m_attackMap = new std::map<int, Attack>();

	*m_attackMap = {
				{1,Attack({ "Strike", " strikes with their blade", 0,0,0,0,0,TargetType::ENEMY,{ 1 },Animation::JUMP }, *m_effectMap) },
				{2, Attack({ "Shoot", " looses an arrow",1,3,0,3,0,TargetType::ENEMY,{ 2 },Animation::JUMP },  *m_effectMap)},
				{3,Attack({ "Charge", " charges into the enemy",0,1,0,0,2,TargetType::ENEMY,{ 3 },Animation::JUMP }, *m_effectMap)},
				{4,Attack({"Fireball", " casts Fireball",1,3,0,3,4,TargetType::ENEMY,{4},Animation::JUMP}, *m_effectMap)},
				{5,Attack({"Cure", " casts a healing spell",1,3,0,3,0,TargetType::FRIEND,{5},Animation::HOVER},*m_effectMap)},
				{6,Attack({"Rest", " catches their breath",0,3,0,3,0,TargetType::SELF,{6},Animation::NONE}, *m_effectMap)},
				{7,Attack({"Assassin's Blade", " lashes out with their dagger",0,0,0,0,0,TargetType::ENEMY,{1,7},Animation::JUMP}, *m_effectMap)},
				{8,Attack({"Poison Dart", " throws a small dart",1,3,0,3,0,TargetType::ENEMY,{7}, Animation::JUMP},*m_effectMap)},
				{9,Attack({"Divine Shield", " begs for divine protection", 0,3,0,3,4,TargetType::FRIEND,{8}, Animation::HOVER},*m_effectMap)},
				{10,Attack({"Curse", " calls on evil spirits", 0,3,0,3,2,TargetType::ENEMY,{9},Animation::HOVER}, *m_effectMap)},
				{11,Attack({"Zombie Bite", " bites its opponent",0,0,0,0,2,TargetType::ENEMY,{10,11},Animation::JUMP},*m_effectMap)},
				{12,Attack({"Groan", " groans and lurches around",0,3,0,3,0,TargetType::SELF,{},Animation::NONE},*m_effectMap)},
				{13,Attack({"Poison Bite", " bites their opponent",0,0,0,0,0,TargetType::ENEMY,{10,7},Animation::JUMP},*m_effectMap)},
				{14,Attack({"Magic Blast", " casts an attack spell",1,3,0,3,0,TargetType::ENEMY,{12},Animation::JUMP},*m_effectMap)},
				{15,Attack({"Battlecry"," shouts words of encouragement",0,3,0,1,0, TargetType::FRIEND,{13},Animation::BOUNCE}, *m_effectMap)},
				{16,Attack({"Evil Eye"," locks eyes with their enemy",0,3,0,3,2,TargetType::ENEMY,{12,12,12,12},Animation::HOVER}, *m_effectMap)},//TODO test repeatedly using same effect works
				{17,Attack({"Holy Ward"," chants a protective spell",2,3,0,3,0,TargetType::FRIEND,{14},Animation::HOVER}, *m_effectMap)},
				{18,Attack({"Bite", " bites its opponent",0,0,0,0,0,TargetType::ENEMY,{10},Animation::JUMP}, *m_effectMap)}
	};

		
			std::map<Ability, int>SwordsmanAbility = {	//TODO move these to other file
				{Ability::STRENGTH, 2},
				{Ability::ACCURACY,0},
				{Ability::MYSTIC,0},
				{Ability::HOLY,0},
				{Ability::TOUGHNESS,1},
				{Ability::MAGIC_RESIST,0},
				{Ability::EVASION,0},
				{Ability::HEALTH,100}
			};

			std::map<Ability, int>AssassinAbility = {
				{ Ability::STRENGTH, 0 },
				{ Ability::ACCURACY,2 },
				{ Ability::MYSTIC,0 },
				{ Ability::HOLY,0 },
				{ Ability::TOUGHNESS,0 },
				{ Ability::MAGIC_RESIST,0 },
				{ Ability::EVASION,25 },
				{ Ability::HEALTH,70 }
			};
			
			std::map<Ability, int>MendicantAbility = {
				{ Ability::STRENGTH, 0 },
				{ Ability::ACCURACY,0 },
				{ Ability::MYSTIC,0 },
				{ Ability::HOLY,2 },
				{ Ability::TOUGHNESS,1 },
				{ Ability::MAGIC_RESIST,0 },
				{ Ability::EVASION,0 },
				{ Ability::HEALTH,50 }
			};

			std::map<Ability, int>WitchAbility = {
				{ Ability::STRENGTH, 0 },
				{ Ability::ACCURACY,1 },
				{ Ability::MYSTIC,2 },
				{ Ability::HOLY,0 },
				{ Ability::TOUGHNESS,0 },
				{ Ability::MAGIC_RESIST,1 },
				{ Ability::EVASION,0 },
				{ Ability::HEALTH,50 }
			};

			std::map<Ability, int>GoblinAbility = {
				{ Ability::STRENGTH, 0 },
				{ Ability::ACCURACY,1 },
				{ Ability::MYSTIC,0 },
				{ Ability::HOLY,0 },
				{ Ability::TOUGHNESS,0 },
				{ Ability::MAGIC_RESIST,0 },
				{ Ability::EVASION,0 },
				{ Ability::HEALTH,50 }
			};

			std::map<Ability, int>ZombieAbility = {
				{ Ability::STRENGTH, 2 },
				{ Ability::ACCURACY,0 },
				{ Ability::MYSTIC,0 },
				{ Ability::HOLY,0 },
				{ Ability::TOUGHNESS,2 },
				{ Ability::MAGIC_RESIST,0 },
				{ Ability::EVASION,0 },
				{ Ability::HEALTH,80 }
			};

			std::map<Ability, int>MedusaAbility = {
				{ Ability::STRENGTH, 2 },
				{ Ability::ACCURACY,0 },
				{ Ability::MYSTIC,2 },
				{ Ability::HOLY,0 },
				{ Ability::TOUGHNESS,1 },
				{ Ability::MAGIC_RESIST,1 },
				{ Ability::EVASION,0 },
				{ Ability::HEALTH,100 }
			};

			std::map<Ability, int>ImpAbility = {
				{ Ability::STRENGTH, -1 },
				{ Ability::ACCURACY,0 },
				{ Ability::MYSTIC,1 },
				{ Ability::HOLY,0 },
				{ Ability::TOUGHNESS,-1 },
				{ Ability::MAGIC_RESIST,2 },
				{ Ability::EVASION,60 },
				{ Ability::HEALTH,20 }
			};

			//TODO medusa abilities (poison bite, gaze attack

				m_playerTeam = new CreatureArray();
				m_playerTeam->creature = new Creature*[4];
				m_playerTeam->size = 0;
				m_enemyTeam = new CreatureArray();
				m_enemyTeam->creature = new Creature*[4];
				m_enemyTeam->size = 0;

				m_sword = new aie::Texture("./textures/Swordsman.png");
				m_assassin = new aie::Texture("./textures/Assassin.png");
				m_monk = new aie::Texture("./textures/Mendicant.png");
				m_witch = new aie::Texture("./textures/Witch.png");
				m_goblin = new aie::Texture("./textures/Goblin.png");
				m_zombie = new aie::Texture("./textures/Zombie.png");
				m_imp = new aie::Texture("./textures/Imp.png");
				m_medusa = new aie::Texture("./textures/Medusa.png");
				m_dead = new aie::Texture("./textures/Grave.png");
				m_targetArrow = new aie::Texture("./textures/Arrow.png");


				//HACK creatures should be initialized elsewhere
				m_playerTeam->creature[0] = new Creature("Swordsman", m_sword, m_dead, SwordsmanAbility, { 1,3,6,15 }, *m_attackMap); 
				m_playerTeam->creature[1] = new Creature("Assassin", m_assassin, m_dead, AssassinAbility, { 7,8 }, *m_attackMap);
				m_playerTeam->creature[2] = new Creature("Mendicant", m_monk, m_dead, MendicantAbility, { 1,5,9,17 }, *m_attackMap);
				m_playerTeam->creature[3] = new Creature("Witch", m_witch, m_dead, WitchAbility, {14,4,10 }, *m_attackMap);
				m_playerTeam->size = 4;
				m_enemyTeam->creature[0] = new Creature("Medusa", m_medusa, m_dead, MedusaAbility, { 13,16,10 }, *m_attackMap);
				m_enemyTeam->creature[1] = new Creature("Zombie", m_zombie, m_dead, ZombieAbility, { 11,12 }, *m_attackMap);
				m_enemyTeam->creature[2] = new Creature("Goblin", m_goblin, m_dead, GoblinAbility, { 1,2,3 }, *m_attackMap);
				m_enemyTeam->creature[3] = new Creature("Imp", m_imp, m_dead, ImpAbility, { 18,4,10 }, *m_attackMap);	
				m_enemyTeam->size = 4;

				m_playerTeam->creature[0]->setAgent(new AgentHuman(m_targetArrow));
				m_playerTeam->creature[1]->setAgent(new AgentHuman(m_targetArrow));
				m_playerTeam->creature[2]->setAgent(new AgentHuman(m_targetArrow));
				m_playerTeam->creature[3]->setAgent(new AgentHuman(m_targetArrow));
				m_enemyTeam->creature[0]->setAgent(new AgentAI(new MaximizeDamagePercent()));
				m_enemyTeam->creature[1]->setAgent(new AgentAI(new PickFirstOption()));
				m_enemyTeam->creature[2]->setAgent(new AgentAI(new MaximizeDamage()));
				m_enemyTeam->creature[3]->setAgent(new AgentAI(new MaximizeDamage()));


	m_modeStateMachine = new GameModeStateMachine;
	m_modeStateMachine->add("MainMenu", new MainMenu(this));
	m_modeStateMachine->add("Battle", new Battle(this));
	m_modeStateMachine->add("GameOver", new GameOver(this));
	m_modeStateMachine->add("VictoryScreen", new VictoryScreen(this));

	m_modeStateMachine->change("MainMenu", m_playerTeam, m_enemyTeam);

	m_cameraX = 0;
	m_cameraY = 0;
	m_timer = 0;

	return true;
}

void Application2D::shutdown() {
	
	delete m_modeStateMachine;
	delete m_sword;
	delete m_assassin;
	delete m_monk;
	delete m_witch;
	delete m_goblin;
	delete m_zombie;
	delete m_imp;
	delete m_medusa;
	delete m_dead;
	delete m_targetArrow;
	delete[] m_enemyTeam->creature;
	delete m_enemyTeam;
	delete[] m_playerTeam->creature;
	delete m_playerTeam;
	delete m_attackMap;
	delete m_effectMap;
	delete m_font;
	delete m_2dRenderer;
	
	
}

void Application2D::update(float deltaTime) {

	m_timer += deltaTime;

	// Get user input
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
	 m_modeStateMachine->getState()->update(deltaTime);
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	m_modeStateMachine->getState()->draw(*m_2dRenderer);
	
	// show fps
	
	char fps[32];
	m_2dRenderer->setRenderColour(1, 1, 0, 1);
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->end();
}