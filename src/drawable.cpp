#include "drawable.h"

#include <graphics/glm_helper.h>

Drawable::Drawable(Resource::Texture tex, float scale) {
    this->tex = tex;
    rect = glm::vec4(0, 0, tex.dim.x * scale, tex.dim.y * scale);
}

void Drawable::UpdateMat() {
    glm::vec4 rect = this->rect;
    rect.z *= drawable_size;
    rect.w *= drawable_size;
    if(sizeFromMid) {
	rect.x += (this->rect.z - rect.z) / 2;
	rect.y += (this->rect.w - rect.w) / 2;
    }
    this->mat = glmhelper::calcMatFromRect(rect, rotate, depth);
}

void Drawable::Draw(Render *render) { UpdateMat(); render->DrawQuad(tex, premat*mat*offsetMat, colour); }
