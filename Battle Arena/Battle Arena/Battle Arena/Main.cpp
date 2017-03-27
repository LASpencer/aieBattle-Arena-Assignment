#include <iostream>
#include "Attack.h"
#include "Enemy.h"

//TODO comment everything

//TODO write actual main game loop
int main() {


	std::map<int, Effect>EffectMap = {
		{1,
			{ "The blade swings down",
				{ " was injured by the blow",
				EffectType::DAMAGE,
				0,
				10,
				Ability::HEALTH },
			10,
			0,0,
			false,
			TargetType::ENEMY,
				{
					{Ability::STRENGTH, 1.0f}
				},
				{
					{Ability::TOUGHNESS,1.0f}
				} 
			}},
		{2,{"The arrow arcs through the air",{" was hit by the arrow", EffectType::DAMAGE,0,5,Ability::HEALTH},5,0,4,false, TargetType::ENEMY,{{Ability::ACCURACY,1.0f}},{{Ability::TOUGHNESS,1.0f}} }},
		{3,{"Attacks rain down on their opponents", {" was struck", EffectType::DAMAGE, 0,10,Ability::HEALTH},10,0,1,true,TargetType::ENEMY,{ { Ability::STRENGTH, 0.5f } },{ {Ability::TOUGHNESS,1.0f} }} }
	};

	std::map<int, Attack>AttackMap = {
		{1,Attack({ "Strike", " strikes with their blade", 0,0,0,0,0,TargetType::ENEMY,{ 1 } }, EffectMap) },
		{2, Attack({ "Shoot", " looses an arrow",1,4,0,4,0,TargetType::ENEMY,{ 2 } }, EffectMap) },
		{3,Attack({ "Charge", " charges into the enemy",0,0,0,0,2,TargetType::ENEMY,{ 1,3 } },EffectMap) }
	};

	std::map<Ability, int>SwordsmanAbility = {
		{Ability::STRENGTH, 1},
		{Ability::ACCURACY,0},
		{Ability::TOUGHNESS,1},
		{Ability::EVASION,0},
		{Ability::HEALTH,50}
	};
	
	std::map<Ability, int>GoblinAbility = {
		{ Ability::STRENGTH, 0 },
		{ Ability::ACCURACY,1 },
		{ Ability::TOUGHNESS,0 },
		{ Ability::EVASION,0 },
		{ Ability::HEALTH,50 }
	};

	Creature testCreature[2] = { Creature("Swordsman 1",SwordsmanAbility,{3,1},AttackMap), Creature("Swordsman 2",SwordsmanAbility,{ 3,1 },AttackMap) };
	Creature testEnemy[3] = { Creature("Angry Goblin",GoblinAbility,{1,2},AttackMap),Creature("Cunning Goblin",GoblinAbility,{1,2},AttackMap),Creature("Sneaky Goblin",GoblinAbility,{ 1,2 },AttackMap) };

	CreatureArray friends = { testCreature,2 };
	CreatureArray enemies = { testEnemy,3 };
	while (enemies.creature[0].isAlive() &&friends.creature[0].isAlive()) {
		for (int i = 0; i < friends.size && i < Attack::MAX_RANGE; i++) {
			testCreature[i].Update(friends, enemies, i);
			std::cout << std::endl;
		}
		for (int i = 0; i < enemies.size && i< Attack::MAX_RANGE; i++) {
			testEnemy[i].Update(enemies, friends, i);
			std::cout << std::endl;
		}
		std::cout << std::endl;
		// sort both arrays
		//HACK this needs to be a function
		for (int i = 1; i < friends.size; ++i) {
			Creature temp = friends.creature[i];	//HACK no copy constructor defined	
			int j = i;
			while (j > 0 && temp.getHealth() > friends.creature[j - 1].getHealth()) {
				friends.creature[j] = friends.creature[j - 1];
				j--;
			}
			friends.creature[j] = temp;
		}
		for (int i = 1; i < enemies.size; ++i) {
			Creature temp = enemies.creature[i];	//HACK no copy constructor defined	
			int j = i;
			while (j > 0 && temp.getHealth() > enemies.creature[j - 1].getHealth()) {
				enemies.creature[j] = enemies.creature[j - 1];
				j--;
			}
				enemies.creature[j] = temp;
		}
	}
	system("pause");

}