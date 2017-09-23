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

enum class SelectorType {
    CLOSEST_MONSTER,
    SELECTED_MONSTER,
    WEARER_RANGE,
    SELECTED_RANGE
};

class TargetSelector {
public :
    TargetSelector(SelectorType type, float range);
    void selectTargets(Actor* wearer, TCODList<Actor*>& list);
protected :
    SelectorType type;
    float range;
};

class Effect {
public :
    virtual bool applyTo(Actor* actor) = 0;
};

class HealthEffect : public Effect {
public :
    float amount;
    const char *message;

    HealthEffect(float amount, const char *message);
    bool applyTo(Actor *actor);
};

class AiChangeEffect : public Effect {
public :
    TemporaryAi *newAi;
    const char *message;

    AiChangeEffect(TemporaryAi *newAi, const char *message);
    bool applyTo(Actor *actor);
};

class Pickable {
public :
    Pickable(TargetSelector *selector, Effect *effect);
    virtual ~Pickable();
    bool pick(Actor *owner, Actor *wearer);
    void drop(Actor *owner, Actor *wearer);
    bool use(Actor *owner, Actor *wearer);
protected :
    TargetSelector *selector;
    Effect *effect;
};

