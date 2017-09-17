#pragma once

class Actor {
public:
    int x, y;   // position on the map
    int ch;     // ascii code
    TCODColor col;

    Actor(int x, int y, int ch, const TCODColor &col);
    void render() const;
};