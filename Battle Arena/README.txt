BattleArena by L. Andrew Spencer 2017

This program plays a JRPG style combat. Two teams of creatures battle each other, doing damage to
the members of the other team until all creatures on one team are killed, resulting in the other 
team winning.

I. Use
-------

This program uses the following keyboard controls
UP/DOWN		: Select through menu options
LEFT/RIGHT	: Select target creatures in battle
SPACEBAR	: Choose currently selected option
BACKSPACE	: Undo attack choice
ESCAPE		: Exit the game

The program begins in the main menu screen, with the options to play the game or quit. If you choose
to play, a battle starts. 

In a battle, the player's team is shown on the left side facing the enemy team on the right. The two 
teams are sorted by health, with the ones with most health at the front of their team. 
Each turn, you select the attack made by each member of your team, and if necessary the target of that 
attack. The creatures on the enemy team then make their attacks. For more details about the characters, 
monsters, attacks and combat system, see the player manual in the Docs folder.
After both teams have taken their turns, both teams are sorted by health, so the ones with most health 
are again at the front of their team. If the player's team is killed, they lose the game, and if the 
enemy team is killed, the player wins the game.


II. Design
----------

To be consistent with Bootstrap, methods are written in lower camel case where possible.

The program is made up of several classes, as shown in the Class Diagram image in the Docs folder. Not 
shown are the various classes implementing the State and GameModeState abstract classes.

Sections:
1. State Machines
2. Application2D
3. Attack
4. Creature
5. Agent
6. Battle
7. MessageBar and MenuBox

1. State Machines

In this program state machines (StateMachine and GameModeStateMachine objects) are used to control 
gameplay. State machines contain maps of pointers to their states, and have methods to get the current 
state and change to a new state.
Each state is a class derived from the State abstract class, with its own update and draw method to be 
called while it is the current state.
By using  a state machine the program can remember its place in a series of instuctions or a loop while 
waiting for user input or displaying a message. As the Battle Substate Statechart Diagram shows, TakeTurn
selects the next creature and starts its turn (with the OngoingEffects state) until all creatures have 
had their turns, then changes state to SortTeams, which after sorting both teams changes state back to 
TakeTurn if both teams have live creatures. This is like a for loop inside a while loop, but taking place
over several game updates rather than all at once.

2. Application2D

The Application2D class is a Bootstrap Application. On startup it creates the objects needed for the 
game: a map of attack objects and two arrays of creatures.
The data for effects, attacks, creatures, and creature ability scores is in the Application2D.cpp source 
file. However, it should be possible to write some methods to instead read the data from an external file, 
allowing designers to add, remove, or alter attacks and creatures without recompiling the application. Time 
constraints prevented this from being fully implemented.

Application2D contains a GameModeStateMachine determining the screen to be displayed. Every update and draw
it calls the update and draw methods of the current state, respectively. The GameMode Statechart diagram 
shows its behaviour.
The fairly linear GameModeStateMachine could be modified. For example, a GetNextBattle state could be used 
after a battle is won to start a battle with a new enemy team until all teams are defeated, and only then 
going to the VictoryScreen. 

3. Attack

When designing Attacks, the goal was to allow attacks with various different effects to be created using only
data, so it would be possible to load them from an external file. This would allow game designers to try 
out ideas or balance the game without having to rebuild the program. 
An attack object contains a vector of effects, which are all applied when the attack is made. Effects contain 
information about what creatures they are applied to, what EffectType they are (damage, healing, buff, or 
debuff), their default value, and how the user and target's abilities modify that value. When an effect is 
applied to a creature, this information is passed to it, and depending on the EffectType something is done 
to the creature.
So, while code has to be written for completely new game mechanics, old mechanics can be reused for new 
effects and attacks.

Some foreseen limitations in actually loading attack data from a file are the use of enumerations, and 
ensuring that data entered by designers is valid. If enumeration classes are altered in the source code, the 
values referring to them in external files will be invalid. This could be solved by giving them explicit 
integer values, or by the class responsible for loading attack data containing maps of integers to enumerated values.
To ensure the data entered to the files is valid, an application could be made for viewing and editing game 
data, rather than editing the file directly.

4. Creature

As with Attacks, Creatures were designed with the goal of being defined by data, allowing them to be modified 
more easily.


5. Agent
The Agent class is used to determine what attack a Creature should make, and if necessary its target. Two 
classes are derived from it: AgentAI which uses the methods of a Behaviour object to make its choice, and 
AgentHuman, which asks the player to choose their attack and target.

6. Battle

7. MessageBar and MenuBox