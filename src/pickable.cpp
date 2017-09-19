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

bool Pickable::pick(Actor* owner, Actor* wearer) {
    if ( wearer->container && wearer->container->add(owner) ) {
        engine.actors.remove(owner);
        return true;
    }
    return false;
}

bool Pickable::use(Actor* owner, Actor* wearer) {
    if ( wearer->container ) {
        wearer->container->remove(owner);
        delete owner;
        return true;
    }
    return false;
}

Healer::Healer(float amount) : amount(amount) {
}

bool Healer::use(Actor* owner, Actor* wearer) {
    if ( wearer->destructible ) {
        float amountHealed = wearer->destructible->heal(amount);
        if ( amountHealed > 0 ) {
            return Pickable::use(owner,wearer);
        }
    }
    return false;
}

LightningBolt::LightningBolt(float range, float damage)
: range(range),damage(damage) {
}

bool LightningBolt::use(Actor* owner, Actor* wearer) {
    Actor* closestMonster = engine.getClosestMonster(wearer->x, wearer->y, range);
    if (!closestMonster) {
        engine.gui->message(TCODColor::lightGrey, "No enemy is close enough to strike.");
        return false;
    }
    // hit closest monster for <damage> hit points
    engine.gui->message(TCODColor::lightBlue,
        "A lighting bolt strikes the %s with a loud thunder!\n"
        "The damage is %g hit points.",
        closestMonster->name,damage);
        closestMonster->destructible->takeDamage(closestMonster,damage);
    return Pickable::use(owner,wearer);
}
