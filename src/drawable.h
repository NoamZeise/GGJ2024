#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "consts.h"
#include <manager.h>

class Drawable {
public:
    Drawable() {}
    Drawable(Resource::Texture tex, float scale);
    virtual void Draw(Render* render);
    glm::vec4 rect;
    float rotate = 0;
    float depth = DEPTH_AVG;
    void UpdateMat();
 private:
    Resource::Texture tex;
    glm::mat4 mat;
};

#endif /* DRAWABLE_H */