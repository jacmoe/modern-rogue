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

void Map::load(TCODZip& zip)
{
    seed = zip.getInt();
    init(false);
    for (int i = 0; i<width*height; i++) {
        tiles[i].explored = static_cast<bool>(zip.getInt());
    }
}

void Map::save(TCODZip& zip)
{
    zip.putInt(static_cast<int>(seed));
    for (int i = 0; i<width*height; i++) {
        zip.putInt(tiles[i].explored);
    }
}

void Actor::load(TCODZip& zip)
{
    x = zip.getInt();
    y = zip.getInt();
    ch = zip.getInt();
    col = zip.getColor();
    name = strdup(zip.getString());
    blocks = static_cast<bool>(zip.getInt());
    fovOnly = static_cast<bool>(zip.getInt());
    auto hasAttacker = static_cast<bool>(zip.getInt());
    auto hasDestructible = static_cast<bool>(zip.getInt());
    auto hasAi = static_cast<bool>(zip.getInt());
    auto hasPickable = static_cast<bool>(zip.getInt());
    auto hasContainer = static_cast<bool>(zip.getInt());
    if (hasAttacker) {
        attacker = new Attacker(0.0f);
        attacker->load(zip);
    }
    if (hasDestructible) {
        destructible = Destructible::create(zip);
    }
    if (hasAi) {
        ai = Ai::create(zip);
    }
    if (hasPickable) {
        pickable = Pickable::create(zip);
    }
    if (hasContainer) {
        container = new Container(0);
        container->load(zip);
    }
}

void Actor::save(TCODZip& zip)
{
    zip.putInt(x);
    zip.putInt(y);
    zip.putInt(ch);
    zip.putColor(&col);
    zip.putString(name);
    zip.putInt(blocks);
    zip.putInt(fovOnly);
    zip.putInt(attacker!=nullptr);
    zip.putInt(destructible!=nullptr);
    zip.putInt(ai!=nullptr);
    zip.putInt(pickable!=nullptr);
    zip.putInt(container!=nullptr);
    if (attacker) attacker->save(zip);
    if (destructible) destructible->save(zip);
    if (ai) ai->save(zip);
    if (pickable) pickable->save(zip);
    if (container) container->save(zip);
}

void Container::load(TCODZip& zip)
{
    size = zip.getInt();
    int nbActors = zip.getInt();
    while (nbActors>0) {
        auto* actor = new Actor(0, 0, 0, nullptr, TCODColor::white);
        actor->load(zip);
        inventory.push(actor);
        nbActors--;
    }
}

void Container::save(TCODZip& zip)
{
    zip.putInt(size);
    zip.putInt(inventory.size());
    for (auto& it : inventory) {
        it->save(zip);
    }
}

void Destructible::load(TCODZip& zip)
{
    maxHp = zip.getFloat();
    hp = zip.getFloat();
    defense = zip.getFloat();
    corpseName = strdup(zip.getString());
    xp = zip.getInt();
}

void Destructible::save(TCODZip& zip)
{
    zip.putFloat(maxHp);
    zip.putFloat(hp);
    zip.putFloat(defense);
    zip.putString(corpseName);
    zip.putInt(xp);
}

void PlayerDestructible::save(TCODZip& zip)
{
    zip.putInt(PLAYER);
    Destructible::save(zip);
}

void MonsterDestructible::save(TCODZip& zip)
{
    zip.putInt(MONSTER);
    Destructible::save(zip);
}

Destructible* Destructible::create(TCODZip& zip)
{
    auto type = (DestructibleType) zip.getInt();
    Destructible* destructible = nullptr;
    switch (type) {
    case MONSTER :destructible = new MonsterDestructible(0, 0, nullptr, 0);
        break;
    case PLAYER :destructible = new PlayerDestructible(0, 0, nullptr);
        break;
    }
    destructible->load(zip);
    return destructible;
}

void Attacker::load(TCODZip& zip)
{
    power = zip.getFloat();
}

void Attacker::save(TCODZip& zip)
{
    zip.putFloat(power);
}

void MonsterAi::load(TCODZip& zip)
{
    moveCount = zip.getInt();
}

void MonsterAi::save(TCODZip& zip)
{
    zip.putInt(MONSTER);
    zip.putInt(moveCount);
}

void ConfusedMonsterAi::load(TCODZip& zip)
{
    nbTurns = zip.getInt();
    oldAi = Ai::create(zip);
}

void ConfusedMonsterAi::save(TCODZip& zip)
{
    zip.putInt(CONFUSED_MONSTER);
    zip.putInt(nbTurns);
    oldAi->save(zip);
}

void PlayerAi::load(TCODZip& zip)
{
    xpLevel = zip.getInt();
}

void PlayerAi::save(TCODZip& zip)
{
    zip.putInt(PLAYER);
    zip.putInt(xpLevel);
}

Ai* Ai::create(TCODZip& zip)
{
    auto type = (AiType) zip.getInt();
    Ai* ai = nullptr;
    switch (type) {
    case PLAYER :ai = new PlayerAi();
        break;
    case MONSTER :ai = new MonsterAi();
        break;
    case CONFUSED_MONSTER :ai = new ConfusedMonsterAi(0, nullptr);
        break;
    }
    ai->load(zip);
    return ai;
}

void Healer::load(TCODZip& zip)
{
    amount = zip.getFloat();
}

void Healer::save(TCODZip& zip)
{
    zip.putInt(HEALER);
    zip.putFloat(amount);
}

void LightningBolt::load(TCODZip& zip)
{
    range = zip.getFloat();
    damage = zip.getFloat();
}

void LightningBolt::save(TCODZip& zip)
{
    zip.putInt(LIGHTNING_BOLT);
    zip.putFloat(range);
    zip.putFloat(damage);
}

void Confuser::load(TCODZip& zip)
{
    nbTurns = zip.getInt();
    range = zip.getFloat();
}

void Confuser::save(TCODZip& zip)
{
    zip.putInt(CONFUSER);
    zip.putInt(nbTurns);
    zip.putFloat(range);
}

void Fireball::save(TCODZip& zip)
{
    zip.putInt(FIREBALL);
    zip.putFloat(range);
    zip.putFloat(damage);
}

Pickable* Pickable::create(TCODZip& zip)
{
    auto type = (PickableType) zip.getInt();
    Pickable* pickable = nullptr;
    switch (type) {
    case HEALER :pickable = new Healer(0);
        break;
    case LIGHTNING_BOLT :pickable = new LightningBolt(0, 0);
        break;
    case CONFUSER :pickable = new Confuser(0, 0);
        break;
    case FIREBALL :pickable = new Fireball(0, 0);
        break;
    }
    pickable->load(zip);
    return pickable;
}

void Gui::load(TCODZip& zip)
{
    int nbMessages = zip.getInt();
    while (nbMessages>0) {
        const char* text = zip.getString();
        TCODColor col = zip.getColor();
        message(col, text);
        nbMessages--;
    }
}

void Gui::save(TCODZip& zip)
{
    zip.putInt(log.size());
    for (auto& it : log) {
        zip.putString(it->text);
        zip.putColor(&it->col);
    }
}

const int SAVEGAME_VERSION = 0x1100;

void Engine::load(bool pause)
{
    TCODZip zip;
    engine.gui->menu.clear();
    engine.gui->menu.addItem(Menu::NEW_GAME, "New game");
    if (TCODSystem::fileExists("game.sav")) {
        zip.loadFromFile("game.sav");
        int version = zip.getInt();
        if (version==SAVEGAME_VERSION) {
            engine.gui->menu.addItem(Menu::CONTINUE, "Continue");
        }
    }
    engine.gui->menu.addItem(Menu::EXIT, "Exit");

    Menu::MenuItemCode menuItem = engine.gui->menu.pick(
            pause ? Menu::PAUSE : Menu::MAIN);
    if (menuItem==Menu::EXIT || menuItem==Menu::NONE) {
        // Exit or window closed
        exit(0);
    }
    else if (menuItem==Menu::NEW_GAME) {
        // New game
        engine.term();
        engine.init();
    }
    else {
        // continue a saved game
        engine.term();
        // load the map
        level = zip.getInt();
        int width = zip.getInt();
        int height = zip.getInt();
        map = new Map(width, height);
        map->load(zip);
        // then the player
        player = new Actor(0, 0, 0, nullptr, TCODColor::white);
        actors.push(player);
        player->load(zip);
        // the stairs
        stairs = new Actor(0, 0, 0, nullptr, TCODColor::white);
        stairs->load(zip);
        actors.push(stairs);
        // then all other actors
        int nbActors = zip.getInt();
        while (nbActors>0) {
            auto* actor = new Actor(0, 0, 0, nullptr, TCODColor::white);
            actor->load(zip);
            actors.push(actor);
            nbActors--;
        }
        // finally the message log
        gui->load(zip);
        // to force FOV recomputation
        gameStatus = STARTUP;
    }
}

void Engine::save()
{
    if (player->destructible->isDead()) {
        TCODSystem::deleteFile("game.sav");
    }
    else {
        TCODZip zip;
        zip.putInt(SAVEGAME_VERSION);
        zip.putInt(level);
        // save the map first
        zip.putInt(map->width);
        zip.putInt(map->height);
        map->save(zip);
        // then the player
        player->save(zip);
        // then the stairs
        stairs->save(zip);
        // then all the other actors
        zip.putInt(actors.size()-2);
        for (auto& actor : actors) {
            if (actor!=player && actor!=stairs) {
                actor->save(zip);
            }
        }
        // finally the message log
        gui->save(zip);
        zip.saveToFile("game.sav");
    }
}
