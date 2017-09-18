/*********************************************************************************************************
* This file is part of the
*
* ███╗   ███╗ ██████╗ ██████╗ ███████╗██████╗ ███╗   ██╗      ██████╗  ██████╗  ██████╗ ██╗   ██╗███████╗
* ████╗ ████║██╔═══██╗██╔══██╗██╔════╝██╔══██╗████╗  ██║      ██╔══██╗██╔═══██╗██╔════╝ ██║   ██║██╔════╝
* ██╔████╔██║██║   ██║██║  ██║█████╗  ██████╔╝██╔██╗ ██║█████╗██████╔╝██║   ██║██║  ███╗██║   ██║█████╗  
* ██║╚██╔╝██║██║   ██║██║  ██║██╔══╝  ██╔══██╗██║╚██╗██║╚════╝██╔══██╗██║   ██║██║   ██║██║   ██║██╔══╝  
* ██║ ╚═╝ ██║╚██████╔╝██████╔╝███████╗██║  ██║██║ ╚████║      ██║  ██║╚██████╔╝╚██████╔╝╚██████╔╝███████╗
* ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝      ╚═╝  ╚═╝ ╚═════╝  ╚═════╝  ╚═════╝ ╚══════╝
*
* project : https://github.com/jacmoe/modern-rogue
*
* Copyright 2017 Jacob Moen
*
**********************************************************************************************************/
#include "main.hpp"

Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP),fovRadius(10),
	screenWidth(screenWidth),screenHeight(screenHeight) {
    TCODConsole::initRoot(screenWidth,screenHeight,"libtcod C++ tutorial",false);
    player = new Actor(40,25,'@',"player",TCODColor::white);
    player->destructible=new PlayerDestructible(30,2,"your cadaver");
    player->attacker=new Attacker(5);
	player->ai = new PlayerAi();
	player->container = new Container(26);
    actors.push(player);
	map = new Map(80,45);
	gui = new Gui();
    gui->message(TCODColor::red, "Welcome stranger!\nPrepare to perish in the Tombs of the Ancient Kings.");
}

Engine::~Engine() {
    actors.clearAndDelete();
	delete map;
	delete gui;
}

void Engine::update() {
	if ( gameStatus == STARTUP ) map->computeFov();
   	gameStatus=IDLE;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &lastKey, &mouse);
	if (lastKey.vk == TCODK_ESCAPE) gameStatus = EXIT;
    player->update();
    if ( gameStatus == NEW_TURN ) {
	    for (Actor **iterator=actors.begin(); iterator != actors.end();
	        iterator++) {
	        Actor *actor=*iterator;
	        if ( actor != player ) {
	            actor->update();
	        }
	    }
	}
}

void Engine::render() {
	TCODConsole::root->clear();
	// draw the map
	map->render();
	// draw the actors
	for (Actor **iterator=actors.begin();
	    iterator != actors.end(); iterator++) {
		Actor *actor=*iterator;
		if ( actor != player && map->isInFov(actor->x,actor->y) ) {
	        actor->render();
	    }
	}
	player->render();
	// show the player's stats
	gui->render();
	TCODConsole::root->print(1,screenHeight-2, "HP : %d/%d",
		(int)player->destructible->hp,(int)player->destructible->maxHp);
}

void Engine::sendToBack(Actor *actor) {
	actors.remove(actor);
	actors.insertBefore(actor,0);
}
