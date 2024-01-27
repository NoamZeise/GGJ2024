#ifndef FISH_H
#define FISH_H

#include "drawable.h"

class Fish : public Drawable  {
public:
    Fish(){}
    Fish(Resource::Texture tex, float scale, glm::vec2 mid);
    void Update(Timer &timer);
    void Draw(Render* render) override;
    void Collide(glm::vec4 otherRect, glm::vec2 otherVel);
    void panUpdate(glm::vec4 pan, glm::vec2 panVel);
    bool fishAbove = false;
    void addVel(glm::vec2 v) { vel += v;}
private:
    glm::vec2 vel = glm::vec2(0, 0);
    glm::vec2 accel = glm::vec2(0, 1);
    glm::vec2 change;

    glm::vec2 mid;
    glm::vec4 collider;

    Drawable fishIndicator;
    bool fishOffscreen = false;
};

#endif /* FISH_H */
