#ifndef FISH_H
#define FISH_H

#include "drawable.h"

class Fish : public Drawable  {
public:
    Fish(){}
    Fish(Resource::Texture tex, float scale, glm::vec2 mid);
    void Update(Timer &timer);
    void Draw(Render* render) override;
    void panUpdate(glm::vec2 pan1, glm::vec2 p2, glm::vec2 panVel);
    bool fishAbove = false;
    void addVel(glm::vec2 v) { vel += v;}
    bool isDone() { return done; }
    int getDoneness() { return doneness; }
private:
    glm::vec2 vel = glm::vec2(0, 0);
    glm::vec2 accel = glm::vec2(0, 1);
    float thetaVel = 0;//100;
    glm::vec2 change;

    glm::vec2 mid;
    glm::vec4 collider;
    
    glm::vec2 prevPan;
    glm::vec2 prevMid;
    glm::vec2 r1;
    glm::vec2 r2;
    glm::vec2 p1;
    glm::vec2 p2;
    
    Drawable fishIndicator;
    bool fishOffscreen = false;

    bool collided = false;
    float timeOff = 0;
    bool done = false;
    float doneness = 1;
};

#endif /* FISH_H */
