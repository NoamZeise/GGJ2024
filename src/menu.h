#ifndef MENU_H
#define MENU_H

#include <graphics/render.h>
#include <game/input.h>
#include <game/timer.h>
#include "drawable.h"

class Menu {
 public:
    Menu(ResourcePool* pool);
    void Update(Timer &timer, Input &input, glm::vec2 cursor);
    void Draw(Render *render);
    bool isExit();
    bool isPlay();
    void setPaused(bool paused) { this->paused = paused;}
    bool isPaused() { return paused; }
 private:
    Drawable bg, title, cat, play, exit;
    Resource::Texture pixel;
    bool exitPress = false;
    bool playPress = false;
    float elapsed = 0;
    bool paused = false;
};

#endif /* MENU_H */
