#include "cooking.h"
#include <graphics/glm_helper.h>
#include "consts.h"
#include "vec_math.h"
#include <graphics/logger.h>
#include <game/random.h>


Pan::Pan(Resource::Texture tex, float scale) : Drawable(tex, scale) {}


void Pan::Update(Timer &timer, Input &input) {
    const float PAN_SPEED = 0.02f;
    
    //if(input.m.hold(GLFW_MOUSE_BUTTON_LEFT)) {
	mouseDiff = glm::vec2(input.m.dx(), input.m.dy());
	mouseDiff *= timer.dt() * PAN_SPEED;
	rect.x += mouseDiff.x;
	rect.y += mouseDiff.y;
	//}
}

Cooking::Cooking(ResourcePool* pool) {
    this->pan = Pan(pool->tex()->load("textures/cooking/pan.png"), 0.7f);
    fishTex = pool->tex()->load("textures/cooking/fish.png");
    pan.rect.x = (WINDOW_WIDTH / 2) - (pan.rect.z/3);
    pan.rect.y = (WINDOW_HEIGHT / 2) - (pan.rect.w/2);
    pan.UpdateMat();
    addFish();
}

void Cooking::addFish() {
    Fish fish = Fish(fishTex, 0.4f,
		     glm::vec2(game::random::posReal() * (WINDOW_WIDTH - fishTex.dim.y),
			       -10));
    fish.depth = DEPTH_AVG + DEPTH_STEP;
    fish.addVel(glm::vec2(game::random::real() * 10, game::random::real() * 4));
    fishes.push_back(fish);
}

void Cooking::Update(Timer &timer, Input &input) {
    timeElapsed += timer.dt();
    if(timeElapsed > 10000) {
	timeElapsed = 0;
	//	addFish();
    }
    pan.Update(timer, input);
    glm::vec4 panline(pan.rect.x + 8, pan.rect.y + pan.rect.w/2, pan.rect.z / 1.8, 2);

    for(auto& fish: fishes) {
	fish.Update(timer);
	fish.panUpdate(panline, pan.vel());
    }
}

void Cooking::Draw(Render* render) {
    pan.Draw(render);
    for(auto& fish: fishes) {
	fish.Draw(render);
    }
}
