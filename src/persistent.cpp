#include "main.hpp"

void Map::load(TCODZip &zip) {
	seed=zip.getInt();
    init(false);
	for (int i=0; i < width*height; i++) {
		tiles[i].explored=zip.getInt();
	}
}

void Map::save(TCODZip &zip) {
	zip.putInt(seed);
	for (int i=0; i < width*height; i++) {
		zip.putInt(tiles[i].explored);
	}
}

void Actor::load(TCODZip &zip) {
	x=zip.getInt();
	y=zip.getInt();
	ch=zip.getInt();
	col=zip.getColor();
	name=strdup(zip.getString());
	blocks=zip.getInt();
	bool hasAttacker=zip.getInt();
	bool hasDestructible=zip.getInt();
	bool hasAi=zip.getInt();
	bool hasPickable=zip.getInt();
	bool hasContainer=zip.getInt();
	if ( hasAttacker ) {
		attacker = new Attacker(0.0f);
		attacker->load(zip);
	}
	if ( hasDestructible ) {
		destructible = Destructible::create(zip);
	}
	if ( hasAi ) {
		ai = Ai::create(zip);
	}
	if ( hasPickable ) {
		pickable = Pickable::create(zip);
	}
	if ( hasContainer ) {
		container = new Container(0);
		container->load(zip);
	}
}

void Actor::save(TCODZip &zip) {
	zip.putInt(x);
	zip.putInt(y);
	zip.putInt(ch);
	zip.putColor(&col);
	zip.putString(name);
	zip.putInt(blocks);
	zip.putInt(attacker != NULL);
	zip.putInt(destructible != NULL);
	zip.putInt(ai != NULL);
	zip.putInt(pickable != NULL);
	zip.putInt(container != NULL);
	if ( attacker ) attacker->save(zip);
	if ( destructible ) destructible->save(zip);
	if ( ai ) ai->save(zip);
	if ( pickable ) pickable->save(zip);
	if ( container ) container->save(zip);
}

void Container::load(TCODZip &zip) {
	size=zip.getInt();
	int nbActors=zip.getInt();
	while ( nbActors > 0 ) {
		Actor *actor=new Actor(0,0,0,NULL,TCODColor::white);
		actor->load(zip);
		inventory.push(actor);
		nbActors--;
	}
}

void Container::save(TCODZip &zip) {
	zip.putInt(size);
	zip.putInt(inventory.size());
	for (Actor **it=inventory.begin(); it != inventory.end(); it++) {
		(*it)->save(zip);
	}
}

void Destructible::load(TCODZip &zip) {
	maxHp=zip.getFloat();
	hp=zip.getFloat();
	defense=zip.getFloat();
	corpseName=strdup(zip.getString());
}

void Destructible::save(TCODZip &zip) {
	zip.putFloat(maxHp);
	zip.putFloat(hp);
	zip.putFloat(defense);
	zip.putString(corpseName);
}

void PlayerDestructible::save(TCODZip &zip) {
	zip.putInt(PLAYER);
	Destructible::save(zip);
}

void MonsterDestructible::save(TCODZip &zip) {
	zip.putInt(MONSTER);
	Destructible::save(zip);
}

Destructible *Destructible::create(TCODZip &zip) {
	DestructibleType type=(DestructibleType)zip.getInt();
	Destructible *destructible=NULL;
	switch(type) {
		case MONSTER : destructible=new MonsterDestructible(0,0,NULL); break;
		case PLAYER : destructible=new PlayerDestructible(0,0,NULL); break;
	}
	destructible->load(zip);
	return destructible;
}

void Attacker::load(TCODZip &zip) {
	power=zip.getFloat();
}

void Attacker::save(TCODZip &zip) {
	zip.putFloat(power);
}

void MonsterAi::load(TCODZip &zip) {
	moveCount=zip.getInt();
}

void MonsterAi::save(TCODZip &zip) {
	zip.putInt(MONSTER);
	zip.putInt(moveCount);
}

void ConfusedMonsterAi::load(TCODZip &zip) {
	nbTurns=zip.getInt();
	oldAi=Ai::create(zip);
}

void ConfusedMonsterAi::save(TCODZip &zip) {
	zip.putInt(CONFUSED_MONSTER);
	zip.putInt(nbTurns);
	oldAi->save(zip);
}

void PlayerAi::load(TCODZip &zip) {
}

void PlayerAi::save(TCODZip &zip) {
	zip.putInt(PLAYER);
}

Ai *Ai::create(TCODZip &zip) {
	AiType type=(AiType)zip.getInt();
	Ai *ai=NULL;
	switch(type) {
		case PLAYER : ai = new PlayerAi(); break;
		case MONSTER : ai = new MonsterAi(); break;
		case CONFUSED_MONSTER : ai = new ConfusedMonsterAi(0,NULL); break;
	}
	ai->load(zip);
	return ai;
}

void Healer::load(TCODZip &zip) {
	amount=zip.getFloat();
}

void Healer::save(TCODZip &zip) {
	zip.putInt(HEALER);
	zip.putFloat(amount);
}

void LightningBolt::load(TCODZip &zip) {
	range=zip.getFloat();
	damage=zip.getFloat();
}

void LightningBolt::save(TCODZip &zip) {
	zip.putInt(LIGHTNING_BOLT);
	zip.putFloat(range);
	zip.putFloat(damage);
}

void Confuser::load(TCODZip &zip) {
	nbTurns=zip.getInt();
	range=zip.getFloat();
}

void Confuser::save(TCODZip &zip) {
	zip.putInt(CONFUSER);
	zip.putInt(nbTurns);
	zip.putFloat(range);
}

void Fireball::save(TCODZip &zip) {
	zip.putInt(FIREBALL);
	zip.putFloat(range);
	zip.putFloat(damage);	
}

Pickable *Pickable::create(TCODZip &zip) {
	PickableType type=(PickableType)zip.getInt();
	Pickable *pickable=NULL;
	switch(type) {
		case HEALER : pickable=new Healer(0); break;
		case LIGHTNING_BOLT : pickable=new LightningBolt(0,0); break;
		case CONFUSER : pickable=new Confuser(0,0); break;
		case FIREBALL : pickable=new Fireball(0,0); break;
	}
	pickable->load(zip);
	return pickable;
}

void Gui::load(TCODZip &zip) {
	int nbMessages=zip.getInt();
	while (nbMessages > 0) {
		const char *text=zip.getString();
		TCODColor col=zip.getColor();
		message(col,text);
		nbMessages--;
	}
}

void Gui::save(TCODZip &zip) {
	zip.putInt(log.size());
	for (Message **it=log.begin(); it != log.end(); it++) {
		zip.putString((*it)->text);
		zip.putColor(&(*it)->col);
	}
}

void Engine::load() {
	engine.gui->menu.clear();
	engine.gui->menu.addItem(Menu::NEW_GAME,"New game");
	if ( TCODSystem::fileExists("game.sav")) {
		engine.gui->menu.addItem(Menu::CONTINUE,"Continue");
	}
	engine.gui->menu.addItem(Menu::EXIT,"Exit");

	Menu::MenuItemCode menuItem=engine.gui->menu.pick();
	if ( menuItem == Menu::EXIT || menuItem == Menu::NONE ) {
		// Exit or window closed
		exit(0);
	} else if ( menuItem == Menu::NEW_GAME ) {
		// New game
		engine.term();
		engine.init();
	} else {
		TCODZip zip;
		// continue a saved game
		engine.term();
		zip.loadFromFile("game.sav");
		// load the map
		int width=zip.getInt();
		int height=zip.getInt();
		map = new Map(width,height);
		map->load(zip);
		// then the player
		player=new Actor(0,0,0,NULL,TCODColor::white);
		actors.push(player);
		player->load(zip);
		// then all other actors
		int nbActors=zip.getInt();
		while ( nbActors > 0 ) {
			Actor *actor = new Actor(0,0,0,NULL,TCODColor::white);
			actor->load(zip);
			actors.push(actor);
			nbActors--;
		}
		// finally the message log
		gui->load(zip);
		// to force FOV recomputation
		gameStatus=STARTUP;
	}
}

void Engine::save() {
	if ( player->destructible->isDead() ) {
		TCODSystem::deleteFile("game.sav");
	} else {
		TCODZip zip;
		// save the map
		zip.putInt(map->width);
		zip.putInt(map->height);
		map->save(zip);
		// save the player
		player->save(zip);
		// then all the other actors
		zip.putInt(actors.size()-1);
		for (Actor **it=actors.begin(); it!=actors.end(); it++) {
			if ( *it != player ) {
				(*it)->save(zip);
			}
		}
		// finally the message log
		gui->save(zip);
		zip.saveToFile("game.sav");
	}
}
