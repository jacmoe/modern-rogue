#pragma once

class Actor;

class Ai {
public:
    virtual void update(Actor* owner) = 0;
    virtual ~Ai() {};
};

class MonsterAi : public Ai {
public:
    MonsterAi();
    void update(Actor* owner);
protected:
    int moveCount;

    void moveOrAttack(Actor* owner, int targetx, int targety);
};

class PlayerAi : public Ai {
public:
    void update(Actor* owner);
protected:
    bool moveOrAttack(Actor* owner, int targetx, int targety);
    void handleActionKey(Actor* owner, int ascii);
    Actor* chooseFromInventory(Actor* owner);
};
