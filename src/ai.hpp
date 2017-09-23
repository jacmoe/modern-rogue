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
*
* project : https://github.com/jacmoe/modern-rogue
*
* Copyright 2017 Jacob Moen
*
**********************************************************************************************************/
#pragma once

#include "fwd.hpp"
#include "persistent.hpp"

// after 20 turns, the monster cannot smell the scent anymore
static const int SCENT_THRESHOLD = 50;

class Ai {
public :
    virtual void update(Actor* owner)=0;
};

class TemporaryAi: public Ai {
public :
    TemporaryAi(int nbTurns);
    void update(Actor* owner);
    void applyTo(Actor* actor);
protected :
    int nbTurns;
    Ai* oldAi;
};

class MonsterAi: public Ai {
public :
    MonsterAi();
    void update(Actor* owner);
protected :
    int moveCount;

    void moveOrAttack(Actor* owner, int targetx, int targety);
};

class ConfusedMonsterAi: public TemporaryAi {
public :
    ConfusedMonsterAi(int nbTurns);
    void update(Actor* owner);
};

class PlayerAi: public Ai {
public :
    void update(Actor* owner);

protected :
    bool moveOrAttack(Actor* owner, int targetx, int targety);
    void handleActionKey(Actor* owner, int ascii);
    Actor* choseFromInventory(Actor* owner);
};
