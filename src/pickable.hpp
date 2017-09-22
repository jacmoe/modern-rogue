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

class Pickable: public Persistent {
public :
    bool pick(Actor* owner, Actor* wearer);

    void drop(Actor* owner, Actor* wearer);

    virtual bool use(Actor* owner, Actor* wearer);

    static Pickable* create(TCODZip& zip);

protected :
    enum PickableType {
        HEALER, LIGHTNING_BOLT, CONFUSER, FIREBALL
    };
};

class Healer: public Pickable {
public :
    float amount; // how many hp

    explicit Healer(float amount);

    bool use(Actor* owner, Actor* wearer) override;

    void load(TCODZip& zip) override;

    void save(TCODZip& zip) override;
};

class LightningBolt: public Pickable {
public :
    float range, damage;

    LightningBolt(float range, float damage);

    bool use(Actor* owner, Actor* wearer) override;

    void load(TCODZip& zip) override;

    void save(TCODZip& zip) override;
};

class Confuser: public Pickable {
public :
    int nbTurns;
    float range;

    Confuser(int nbTurns, float range);

    bool use(Actor* owner, Actor* wearer) override;

    void load(TCODZip& zip) override;

    void save(TCODZip& zip) override;
};

class Fireball: public LightningBolt {
public :
    Fireball(float range, float damage);

    bool use(Actor* owner, Actor* wearer) override;

    void save(TCODZip& zip) override;
};

