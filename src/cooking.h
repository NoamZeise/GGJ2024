#ifndef COOKING_H
#define COOKING_H

#include <manager.h>
#include "consts.h"
#include "fish.h"

class Pan : public Drawable {
public:
    Pan(){}
    Pan(Resource::Texture tex, Resource::Texture front, float scale);
    void Update(Timer &timer, Input &input);
    void Draw(Render* render) override;
    glm::vec2 vel() { return mouseDiff; }
private:
    glm::vec2 mouseDiff;
    Resource::Texture front;
};

const float GAME_TIME = 120 + 1;

class Cooking {
public:
    Cooking(ResourcePool* pool);
    void Update(Timer &timer, Input &input);
    void Draw(Render* render);
    void addFish();
    void reset();
    bool isOver() { return timeRemaining < -0.5f && clickedOver; };
 private:
    Drawable scoreD, timeD;
    Pan pan;
    Resource::Texture fishTex;
    Resource::Font font;
    
    std::vector<Fish> fishes;
    float timeElapsed = 0;
    float fishDelay = 10000;
    float timeStart = 0;
    int score = 0;
    float timeRemaining = GAME_TIME;

    bool clickedOver = false;
};


#endif /* COOKING_H */
