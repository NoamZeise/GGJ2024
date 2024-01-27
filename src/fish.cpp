#include "fish.h"

#include "vec_math.h"

Fish::Fish(Resource::Texture tex, float scale, glm::vec2 mid) : Drawable(tex, scale) {
    this->mid = mid;
    this->collider = glm::vec4(-tex.dim.x/4, -tex.dim.y/4, tex.dim.x/2, tex.dim.y/2);
    this->fishIndicator = Drawable(tex, 0.1f);
    fishIndicator.rect.y = 5;
    fishIndicator.UpdateMat();
}

void Fish::Update(Timer &timer) {
    const float ACCEL = 0.04f;
    const float SPEED = 0.0125f;
    vel += accel * (float)timer.dt() * ACCEL;
    change = vel * (float)timer.dt() * SPEED;
    mid += change;


     if(mid.y < -4) {
	fishOffscreen = true;
	fishIndicator.rect.x = mid.x - fishIndicator.rect.z/2;
	fishIndicator.UpdateMat();
    } else {
	fishOffscreen = false;
	}

    if(mid.y > WINDOW_HEIGHT)
	mid.y = -collider.w;
    if(mid.x < -4 || mid.x + collider.z > WINDOW_WIDTH + 4) {
	mid.x -= change.x;
	vel.x *= -0.8;
    }
}

void Fish::panUpdate(glm::vec4 pan, glm::vec2 panVel) {
    bool isAbove = mid.y < pan.y;
    if(!isAbove && fishAbove)
	if(gh::colliding(glm::vec2(mid.x - collider.x, collider.z), glm::vec2(pan.x, pan.z)))
	    mid.y = pan.y - collider.z*0.9f;
    fishAbove = isAbove;

    if(gh::colliding(gh::addVec2ToRect(mid, collider), pan)) {
	Collide(pan, panVel);
    }
}

void Fish::Collide(glm::vec4 orect, glm::vec2 otherVel) {
    const float FRICT = 0.7f;
    mid.y -= change.y;
    if(orect.y > mid.y) {
	const float BOUNCE = 0.6f;
	const float MAX = -40.0f;
	if(otherVel.y < 0) {
	    mid.y += otherVel.y;    
	}
	if(otherVel.y < vel.y) {
	    vel.y = glm::clamp((float)(otherVel.y - fabs(vel.y*BOUNCE)), MAX, 0.0f);
	}
    }
    if(orect.y < mid.y) {
	if(otherVel.y > 0) {
	    mid.y += otherVel.y;
	}
	if(otherVel.y > vel.y) {
	    vel.y = otherVel.y;
	}
    }
    vel.x = (1 - FRICT)*vel.x + (FRICT)*otherVel.x;
}

void Fish::Draw(Render* render) {
    rect.x = mid.x - rect.z/2;
    rect.y = mid.y - rect.w/2;
    Drawable::Draw(render);
    if(fishOffscreen)
	fishIndicator.Draw(render);
}
