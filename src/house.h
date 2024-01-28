#ifndef HOUSE_H
#define HOUSE_H

#include <graphics/render.h>
#include <game/timer.h>
#include <game/input.h>

#include "drawable.h"

enum class Room {
    None,
    Live,
    Bed,
    Bath,
    Kitc,
};

class House {
 public:
    House(ResourcePool* pool);
    void Update(Timer &timer, Input &input, glm::vec2 cursor);
    void Draw(Render* render);
    void setActive(bool b) { active = b; }
    Room pressedRoom() { return pressed; }
 private:
    Drawable bg;
    Drawable kitchen;
    Drawable bathroom;
    Drawable bedroom;
    Drawable bedroom_quilt;
    Drawable livingroom;
    Drawable cat;
    Resource::Texture pixel;
    bool active = false;
    float elapsed = 0;
    Room pressed = Room::None;
    Room hovering = Room::None;
};


#endif /* HOUSE_H */
