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
#include <cstdio>
#include <cstdarg>
#include "main.hpp"

static const int PANEL_HEIGHT = 7;
static const int BAR_WIDTH = 20;
static const int MSG_X = BAR_WIDTH+2;
static const int MSG_HEIGHT = PANEL_HEIGHT-1;

Gui::Gui()
{
    con = new TCODConsole(engine.screenWidth, PANEL_HEIGHT);
}

Gui::~Gui()
{
    delete con;
    clear();
}

void Gui::clear()
{
    log.clearAndDelete();
}

void Gui::render()
{
    // clear the GUI console
    con->setDefaultBackground(TCODColor::black);
    con->clear();

    // draw the health bar
    renderBar(1, 1, BAR_WIDTH, "HP", engine.player->destructible->hp,
            engine.player->destructible->maxHp,
            TCODColor::lightRed, TCODColor::darkerRed);

    // draw the XP bar
    /*
    auto* ai = (PlayerAi*) engine.player->ai;
    char xpTxt[128];
    sprintf(xpTxt, "XP(%d)", ai->xpLevel);
    renderBar(1, 5, BAR_WIDTH, xpTxt, engine.player->destructible->xp,
            ai->getNextLevelXp(),
            TCODColor::lightViolet, TCODColor::darkerViolet);
    //*/
    // draw the message log
    int y = 1;
    float colorCoef = 0.4f;
    for (auto message : log) {
        con->setDefaultForeground(message->col*colorCoef);
        con->print(MSG_X, y, message->text);
        y++;
        if (colorCoef<1.0f) {
            colorCoef += 0.3f;
        }
    }

    // mouse look
    renderMouseLook();

    // dungeon level
    con->setDefaultForeground(TCODColor::white);
    con->print(3, 3, "Dungeon level %d", engine.level);

    // blit the GUI console on the root console
    TCODConsole::blit(con, 0, 0, engine.screenWidth, PANEL_HEIGHT,
            TCODConsole::root, 0, engine.screenHeight-PANEL_HEIGHT);
}

void Gui::renderBar(int x, int y, int width, const char* name,
        float value, float maxValue, const TCODColor& barColor,
        const TCODColor& backColor)
{
    // fill the background
    con->setDefaultBackground(backColor);
    con->rect(x, y, width, 1, false, TCOD_BKGND_SET);

    auto barWidth = (int) (value/maxValue*width);
    if (barWidth>0) {
        // draw the bar
        con->setDefaultBackground(barColor);
        con->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
    }
    // print text on top of the bar
    con->setDefaultForeground(TCODColor::white);
    con->printEx(x+width/2, y, TCOD_BKGND_NONE, TCOD_CENTER,
            "%s : %g/%g", name, value, maxValue);
}

Gui::Message::Message(const char* text, const TCODColor& col)
        :
        text(strdup(text)), col(col)
{
}

Gui::Message::~Message()
{
    free(text);
}

void Gui::renderMouseLook()
{
    if (!engine.map->isInFov(engine.mouse.cx, engine.mouse.cy)) {
        // if mouse is out of fov, nothing to render
        return;
    }
    char buf[128] = "";
    bool first = true;
    for (Actor** it = engine.actors.begin(); it!=engine.actors.end(); it++) {
        Actor* actor = *it;
        // find actors under the mouse cursor
        if (actor->x==engine.mouse.cx && actor->y==engine.mouse.cy) {
            if (!first) {
                strcat(buf, ", ");
            }
            else {
                first = false;
            }
            strcat(buf, actor->name);
        }
    }
    // display the list of actors under the mouse cursor
    con->setDefaultForeground(TCODColor::lightGrey);
    con->print(1, 0, buf);
}

void Gui::message(const TCODColor& col, const char* text, ...)
{
    // build the text
    va_list ap;
    char buf[128];
    va_start(ap, text);
    vsprintf(buf, text, ap);
    va_end(ap);

    char* lineBegin = buf;
    char* lineEnd;
    do {
        // make room for the new message
        if (log.size()==MSG_HEIGHT) {
            Message* toRemove = log.get(0);
            log.remove(toRemove);
            delete toRemove;
        }

        // detect end of the line
        lineEnd = strchr(lineBegin, '\n');
        if (lineEnd) {
            *lineEnd = '\0';
        }

        // add a new message to the log
        auto* msg = new Message(lineBegin, col);
        log.push(msg);

        // go to next line
        lineBegin = lineEnd+1;
    }
    while (lineEnd);
}

Menu::~Menu()
{
    clear();
}

void Menu::clear()
{
    items.clearAndDelete();
}

void Menu::addItem(MenuItemCode code, const char* label)
{
    auto* item = new MenuItem();
    item->code = code;
    item->label = label;
    items.push(item);
}

const int PAUSE_MENU_WIDTH = 30;
const int PAUSE_MENU_HEIGHT = 15;

Menu::MenuItemCode Menu::pick(DisplayMode mode)
{
    int selectedItem = 0;
    int menux, menuy;
    if (mode==PAUSE) {
        menux = engine.screenWidth/2-PAUSE_MENU_WIDTH/2;
        menuy = engine.screenHeight/2-PAUSE_MENU_HEIGHT/2;
        TCODConsole::root->setDefaultForeground(TCODColor(200, 180, 50));
        TCODConsole::root->printFrame(menux, menuy, PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT, true,
                TCOD_BKGND_ALPHA(70), "menu");
        menux += 2;
        menuy += 3;
    }
    else {
        static TCODImage img("menu_background1.png");
        img.blit2x(TCODConsole::root, 0, 0);
        menux = 10;
        menuy = TCODConsole::root->getHeight()/3;
    }

    while (!TCODConsole::isWindowClosed()) {
        int currentItem = 0;
        for (auto& item : items) {
            if (currentItem==selectedItem) {
                TCODConsole::root->setDefaultForeground(TCODColor::lighterOrange);
            }
            else {
                TCODConsole::root->setDefaultForeground(TCODColor::lightGrey);
            }
            TCODConsole::root->print(menux, menuy+currentItem*3, item->label);
            currentItem++;
        }
        TCODConsole::flush();

        // check key presses
        TCOD_key_t key{};
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr);
        switch (key.vk) {
        case TCODK_UP :selectedItem--;
            if (selectedItem<0) {
                selectedItem = items.size()-1;
            }
            break;
        case TCODK_DOWN :selectedItem = (selectedItem+1)%items.size();
            break;
        case TCODK_ENTER :return items.get(selectedItem)->code;
        default :break;
        }
    }
    return NONE;
}
