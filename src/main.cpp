#include <libtcod/libtcod.hpp>
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"

Engine engine;

int main() {
    int playerx {40};
    int playery {25};
    TCODConsole::initRoot(80,50,"libtcod C++ tutorial",false);
    while ( !TCODConsole::isWindowClosed() ) {
        engine.update();
        engine.render();
        TCODConsole::flush();
    }
    return 0;
}
