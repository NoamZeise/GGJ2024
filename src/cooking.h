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

class Cooking {
public:
    Cooking(ResourcePool* pool);
    void Update(Timer &timer, Input &input);
    void Draw(Render* render);
    void addFish();
 private:
    Pan pan;
    Resource::Texture fishTex;
    Resource::Font font;
    std::vector<Fish> fishes;
    float timeElapsed = 0;
    int score = 0;
    
};


#endif /* COOKING_H */
