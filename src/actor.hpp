#pragma once
/*********************************************************************************************************
* This file is part of the
*
* ███╗   ███╗ ██████╗ ██████╗ ███████╗██████╗ ███╗   ██╗      ██████╗  ██████╗ ██╗   ██╗ ██████╗ ███████╗
* ████╗ ████║██╔═══██╗██╔══██╗██╔════╝██╔══██╗████╗  ██║      ██╔══██╗██╔═══██╗██║   ██║██╔════╝ ██╔════╝
* ██╔████╔██║██║   ██║██║  ██║█████╗  ██████╔╝██╔██╗ ██║█████╗██████╔╝██║   ██║██║   ██║██║  ███╗█████╗  
* ██║╚██╔╝██║██║   ██║██║  ██║██╔══╝  ██╔══██╗██║╚██╗██║╚════╝██╔══██╗██║   ██║██║   ██║██║   ██║██╔══╝  
* ██║ ╚═╝ ██║╚██████╔╝██████╔╝███████╗██║  ██║██║ ╚████║      ██║  ██║╚██████╔╝╚██████╔╝╚██████╔╝███████╗
* ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝      ╚═╝  ╚═╝ ╚═════╝  ╚═════╝  ╚═════╝ ╚══════╝
*
* project : https://github.com/jacmoe/modern-rogue
*
* Copyright 2017 Jacob Moen
*
**********************************************************************************************************/

class Attacker;
class Destructible;
class Ai;

class Actor {
public:
    int x, y;   // position on the map
    int ch;     // ascii code
    TCODColor col;
    const char* name;   // the actor's name
    bool blocks;    // can we walk on this actor?
    Attacker* attacker; // something that deals damage
    Destructible* destructible; // something that can be damaged
    Ai* ai; // something self-updating
    
    Actor(int x, int y, int ch, const char* name, const TCODColor &col);
    void update();
    void render() const;
};