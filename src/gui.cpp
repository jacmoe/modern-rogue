#include "gui.hpp"
#include "main.hpp"

static const int PANEL_HEIGHT=7;
static const int BAR_WIDTH=20;

Gui::Gui() {
    con = new TCODConsole(engine.screenWidth,PANEL_HEIGHT);
}
 
Gui::~Gui() {
    delete con;
}

void Gui::render() {
    con->setDefaultBackground(TCODColor::black);
    con->clear();
    // draw the health bar
    renderBar(1, 1, BAR_WIDTH, "HP", engine.player->destructible->hp,
        engine.player->destructible->maxHp, TCODColor::lightRed, TCODColor::darkerRed);
    // blit the GUI console to the root console
    TCODConsole::blit(con, 0, 0, engine.screenWidth, PANEL_HEIGHT,
        TCODConsole::root, 0, engine.screenHeight - PANEL_HEIGHT);
}

void Gui::renderBar(int x, int y, int width, const char* name, float value, float maxValue, const TCODColor& barColor, const TCODColor& backColor) {
    con->setDefaultBackground(backColor);
    con->rect(x, y, width, 1, false, TCOD_BKGND_SET);
    int barWidth = (int)(value / maxValue * width);
    if (barWidth > 0) {
        con->setDefaultBackground(barColor);
        con->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
    }
    con->setDefaultForeground(TCODColor::white);
    con->printEx(x + width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g/%g", name, value, maxValue);
}