#ifndef COOKING_H
#define COOKING_H

#include <manager.h>
#include "consts.h"
#include "fish.h"

class Pan : public Drawable {
public:
    Pan(){}
    Pan(Resource::Texture tex, float scale);
    void Update(Timer &timer, Input &input);
    glm::vec2 vel() { return mouseDiff; }
private:
    glm::vec2 mouseDiff;
};

class Cooking {
public:
    Cooking(ResourcePool* pool);
    void Update(Timer &timer, Input &input);
    void Draw(Render* render);
    void addFish();
 private:
    Pan pan;
    Resource::Texture fishTex;
    std::vector<Fish> fishes;
    float timeElapsed;
};


#endif /* COOKING_H */