#include "cooking.h"
#include <graphics/glm_helper.h>
#include "consts.h"
#include "vec_math.h"
#include <graphics/logger.h>
#include <game/random.h>


Pan::Pan(Resource::Texture tex, Resource::Texture front, float scale) : Drawable(tex, scale) {
    this->front = front;
}


void Pan::Update(Timer &timer, Input &input) {
    const float PAN_SPEED = 0.04f;
    
    mouseDiff = glm::vec2(input.m.dx(), input.m.dy());
    mouseDiff *= timer.dt() * PAN_SPEED;
    rect.x += mouseDiff.x;
    rect.y += mouseDiff.y;

    rotate += mouseDiff.x*0.0003f*timer.dt();
    rotate -= rotate*0.01f*timer.dt();

    if(rect.x + rect.z < 0)
	rect.x = -rect.z;
    if(rect.y + rect.w < 0)
	rect.y = -rect.w;
    if(rect.x > WINDOW_WIDTH)
	rect.x = WINDOW_WIDTH;
    if(rect.y > WINDOW_HEIGHT)
	rect.y = WINDOW_HEIGHT;
}

void Pan::Draw(Render* render) {
    Drawable::Draw(render);
    glm::mat4 m = glm::translate(mat, glm::vec3(0, 0, DEPTH_STEP*2.0f));    
    render->DrawQuad(front, m);
}

Cooking::Cooking(ResourcePool* pool) {
    this->pan = Pan(pool->tex()->load("textures/cooking/pan.png"),
		    pool->tex()->load("textures/cooking/pan-front.png"), 0.5f);
    this->font = pool->font()->load("textures/Mali-Light.ttf");
    fishTex = pool->tex()->load("textures/cooking/fish.png");
    timeD = Drawable(pool->tex()->load("textures/cooking/timer.png"), 0.5f);
    timeD.rect.x = WINDOW_WIDTH - 150;
    timeD.rect.y = 5;
    timeD.sizeFromMid = true;
    timeD.depth = DEPTH_AVG + DEPTH_STEP*2;
    scoreD = Drawable(pool->tex()->load("textures/cooking/score.png"), 0.5f);
    scoreD.rect.x = 10;
    scoreD.rect.y = 7;
    scoreD.sizeFromMid = true;
    scoreD.depth = DEPTH_AVG + DEPTH_STEP*2;
    reset();
}

void Cooking::addFish() {
    float x =  game::random::posReal() * 100 + (pan.rect.x + pan.rect.z/2) + pan.vel().x;
    if(x < 0) x = 0;
    if(x > WINDOW_WIDTH - fishTex.dim.y*0.2f)
	x = WINDOW_WIDTH - fishTex.dim.y*0.2f;
    Fish fish = Fish(fishTex, 0.2f, glm::vec2(x,-150));
    fish.depth = DEPTH_AVG + DEPTH_STEP;
    fish.addVel(glm::vec2(game::random::real() * 10, game::random::real() * 4));
    fishes.push_back(fish);
}

void Cooking::Update(Timer &timer, Input &input) {
    if(timeRemaining < -0.5f) {
	if(input.m.press(GLFW_MOUSE_BUTTON_LEFT)) {
	    clickedOver = true;
	}
	return;
    }
    timeStart += timer.dt() * 0.0025f;
    timeElapsed += timer.dt();
    timeRemaining -= timer.dt()*0.001f;
    if(timeElapsed > fishDelay || fishes.size() == 0) {
	fishDelay -= 25*log(fishDelay);
	timeElapsed = 0;
	addFish();
    }    
    pan.Update(timer, input);    
    glm::vec2 off(pan.rect.x + pan.rect.z/2, pan.rect.y + pan.rect.w/1.5);
    glm::vec2 p1 = gh::rot(glm::vec2(-pan.rect.z/2.2, 0), pan.rotate) + off;
    glm::vec2 p2 = gh::rot(glm::vec2(pan.rect.z/16, 0), pan.rotate) + off;

    float rot = sin(timeStart)*0.1f;
    timeD.rotate = rot;
    scoreD.rotate = rot;
    
    for(int i = 0; i < fishes.size(); i++) {
	Fish* fish = &fishes[i];
	fish->Update(timer);
	if(fish->isDone()) {
	    float factor = 1;
	    if(fish->getDoneness() > FISH_IS_DONE) {
		const float ADD_GAME_TIME = 5;		
		factor = 2;
		timeRemaining += ADD_GAME_TIME;
	    }
	    score += fish->getDoneness() * factor;
	    fishes.erase(fishes.begin() + i--);
	} else {
	    fish->panUpdate(p1, p2, pan.vel());
	}
    }
}

void Cooking::reset() {
    timeElapsed = 0;
    score = 0;
    timeRemaining = GAME_TIME;
    pan.rect.x = (WINDOW_WIDTH / 2) - (pan.rect.z/3);
    pan.rect.y = (WINDOW_HEIGHT / 2) - (pan.rect.w/2);
    pan.depth = DEPTH_AVG - DEPTH_STEP/10;
    pan.rotate = 0;
    pan.UpdateMat();
    fishes.clear();
    clickedOver = false;
    addFish();
}

void Cooking::Draw(Render* render) {
    if(timeRemaining < -0.5f) {
	render->DrawString(font, "Time's Up!",
			   glm::vec2(230, 250), 70, DEPTH_AVG + DEPTH_STEP*2, glm::vec4(1.0f));
    } else {
	pan.Draw(render);
	int i = 0;
	for(auto& fish: fishes) {
	    fish.depth = DEPTH_AVG + (i/100.0)*DEPTH_STEP;
	    fish.Draw(render);
	    i++;
	}
    }
    timeD.Draw(render);
    scoreD.Draw(render);
    render->DrawString(font, std::to_string(score),
		       glm::vec2(100, 65), 30, DEPTH_AVG + DEPTH_STEP*2, glm::vec4(1.0f));
    render->DrawString(font, std::to_string((int)timeRemaining),
		       glm::vec2(WINDOW_WIDTH - 65, 65), 30, DEPTH_AVG + DEPTH_STEP*2,
		       glm::vec4(1.0f));

}
