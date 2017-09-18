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

Attacker::Attacker(float power) : power(power) {
}

void Attacker::attack(Actor* owner, Actor* target) {
    if (target->destructible && ! target->destructible->isDead()) {
        if (power - target->destructible->defense > 0) {
            engine.gui->message(owner == engine.player ? TCODColor::red : TCODColor::lightGrey,
                "%s attacks %s for %g hit points.", owner->name, target->name, power - target->destructible->defense);
        } else {
            engine.gui->message(TCODColor::lightGrey, "%s attacks %s but it has no effect!",
                owner->name, target->name);
        }
        target->destructible->takeDamage(target, power);
    } else {
        engine.gui->message(TCODColor::lightGrey, "%s attacks %s in vain", owner->name, target->name);
    }
}