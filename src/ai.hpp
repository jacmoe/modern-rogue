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
static const int SCENT_THRESHOLD=50;

class Ai: public Persistent {
public :
    virtual void update(Actor* owner)=0;

    static Ai* create(TCODZip& zip);

protected :
    enum AiType {
        MONSTER, CONFUSED_MONSTER, PLAYER
    };
};

class MonsterAi: public Ai {
public :
    MonsterAi();

    void update(Actor* owner) override;

    void load(TCODZip& zip) override;

    void save(TCODZip& zip) override;

protected :

    void moveOrAttack(Actor* owner, int targetx, int targety);
};

class ConfusedMonsterAi: public Ai {
public :
    ConfusedMonsterAi(int nbTurns, Ai* oldAi);

    void update(Actor* owner) override;

    void load(TCODZip& zip) override;

    void save(TCODZip& zip) override;

protected :
    int nbTurns;
    Ai* oldAi;
};

class PlayerAi: public Ai {
public :
    int xpLevel;

    PlayerAi();

    int getNextLevelXp();

    void update(Actor* owner) override;

    void load(TCODZip& zip) override;

    void save(TCODZip& zip) override;

protected :
    bool moveOrAttack(Actor* owner, int targetx, int targety);

    void handleActionKey(Actor* owner, int ascii);

    Actor* choseFromInventory(Actor* owner);
};
