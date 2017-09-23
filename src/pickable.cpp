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
#include "main.hpp"

#include "main.hpp"

TargetSelector::TargetSelector(SelectorType type, float range)
        :type(type), range(range)
{
}

void TargetSelector::selectTargets(Actor* wearer, TCODList<Actor*>& list)
{
    switch (type) {
    case SelectorType::CLOSEST_MONSTER : {
        Actor* closestMonster = engine.getClosestMonster(wearer->x, wearer->y, range);
        if (closestMonster) {
            list.push(closestMonster);
        }
    }
        break;
    case SelectorType::SELECTED_MONSTER : {
        int x, y;
        engine.gui->message(TCODColor::cyan, "Left-click to select an enemy,\nor right-click to cancel.");
        if (engine.pickATile(&x, &y, range)) {
            Actor* actor = engine.getActor(x, y);
            if (actor) {
                list.push(actor);
            }
        }
    }
        break;
    case SelectorType::WEARER_RANGE :
        for (Actor** iterator = engine.actors.begin();
             iterator!=engine.actors.end(); iterator++) {
            Actor* actor = *iterator;
            if (actor->destructible && !actor->destructible->isDead()
                    && actor->getDistance(wearer->x, wearer->y)<=range) {
                list.push(actor);
            }
        }
        break;
    case SelectorType::SELECTED_RANGE :int x, y;
        engine.gui->message(TCODColor::cyan, "Left-click to select a tile,\nor right-click to cancel.");
        if (engine.pickATile(&x, &y)) {
            for (Actor** iterator = engine.actors.begin();
                 iterator!=engine.actors.end(); iterator++) {
                Actor* actor = *iterator;
                if (actor->destructible && !actor->destructible->isDead()
                        && actor->getDistance(x, y)<=range) {
                    list.push(actor);
                }
            }
        }
        break;
    }
    if (list.isEmpty()) {
        engine.gui->message(TCODColor::lightGrey, "No enemy is close enough");
    }
}

HealthEffect::HealthEffect(float amount, const char* message)
        :amount(amount), message(message)
{
}

bool HealthEffect::applyTo(Actor* actor)
{
    if (!actor->destructible) return false;
    if (amount>0) {
        float pointsHealed = actor->destructible->heal(amount);
        if (pointsHealed>0) {
            if (message) {
                engine.gui->message(TCODColor::lightGrey, message, actor->name, pointsHealed);
            }
            return true;
        }
    }
    else {
        if (message && -amount-actor->destructible->defense>0) {
            engine.gui->message(TCODColor::lightGrey, message, actor->name,
                    -amount-actor->destructible->defense);
        }
        if (actor->destructible->takeDamage(actor, -amount)>0) {
            return true;
        }
    }
    return false;
}

AiChangeEffect::AiChangeEffect(TemporaryAi* newAi, const char* message)
        :newAi(newAi), message(message)
{
}

bool AiChangeEffect::applyTo(Actor* actor)
{
    newAi->applyTo(actor);
    if (message) {
        engine.gui->message(TCODColor::lightGrey, message, actor->name);
    }
    return true;
}

Pickable::Pickable(TargetSelector* selector, Effect* effect)
        :
        selector(selector), effect(effect)
{
}

Pickable::~Pickable()
{
    if (selector) delete selector;
    if (effect) delete effect;
}

bool Pickable::pick(Actor* owner, Actor* wearer)
{
    if (wearer->container && wearer->container->add(owner)) {
        engine.actors.remove(owner);
        return true;
    }
    return false;
}

void Pickable::drop(Actor* owner, Actor* wearer)
{
    if (wearer->container) {
        wearer->container->remove(owner);
        engine.actors.push(owner);
        owner->x = wearer->x;
        owner->y = wearer->y;
        engine.gui->message(TCODColor::lightGrey, "%s drops a %s.",
                wearer->name, owner->name);
    }
}

bool Pickable::use(Actor* owner, Actor* wearer)
{
    TCODList<Actor*> list;
    if (selector) {
        selector->selectTargets(wearer, list);
    }
    else {
        list.push(wearer);
    }
    bool succeed = false;
    for (Actor** it = list.begin(); it!=list.end(); it++) {
        if (effect->applyTo(*it)) {
            succeed = true;
        }
    }
    if (succeed) {
        if (wearer->container) {
            wearer->container->remove(owner);
            delete owner;
        }
    }
    return succeed;
}
