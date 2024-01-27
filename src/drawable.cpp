#include "drawable.h"

#include <graphics/glm_helper.h>

Drawable::Drawable(Resource::Texture tex, float scale) {
    this->tex = tex;
    rect = glm::vec4(0, 0, tex.dim.x * scale, tex.dim.y * scale);
}

void Drawable::UpdateMat() {
    this->mat = glmhelper::calcMatFromRect(rect, rotate, depth);
}

void Drawable::Draw(Render *render) { UpdateMat(); render->DrawQuad(tex, mat); }
