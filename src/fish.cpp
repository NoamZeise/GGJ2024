#include "fish.h"

#include "vec_math.h"
#include <graphics/glm_helper.h>
#include <graphics/logger.h>
#include <game/random.h>

Fish::Fish(Resource::Texture tex, float scale, glm::vec2 mid) : Drawable(tex, scale) {
    this->mid = mid;
    glm::vec2 dim = tex.dim * scale;
    this->collider = glm::vec4(-dim.x/2, -dim.y/2, dim.x, 0);
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

    rotate += thetaVel * timer.dt() * SPEED * 0.001f;
    thetaVel -= thetaVel*0.001f*timer.dt();

    if(!collided) {
	timeOff += timer.dt();
    }
    
    p1 = glm::vec2(collider.x, 0);
    p2 = glm::vec2(p1.x + collider.z, 0);
    r1 = gh::rot(p1, rotate) + mid;
    r2 = gh::rot(p2, rotate) + mid;
    
    if(mid.y < 0) {
	fishOffscreen = true;
	fishIndicator.rect.x = mid.x - fishIndicator.rect.z/2;
	fishIndicator.drawable_size = 1/(glm::clamp((float)log((-mid.y*0.2f))*0.5f, 1.0f, 10.0f));
	fishIndicator.UpdateMat();
    } else {
	fishOffscreen = false;
    }
    
    if((r1.y > r2.y ? r2.y : r1.y) > WINDOW_HEIGHT)
	done = true;
	//mid.y = -rect.w;
    if(mid.x < -4 || (r1.x > r2.x ? r2.x : r1.x) > WINDOW_WIDTH - 10) {
	mid.x -= change.x;
	vel.x *= -0.8;
    }
}

void Fish::panUpdate(glm::vec2 pan1, glm::vec2 pan2, glm::vec2 panVel) {
    const float MAX = 40.0f;
    const float MAX_DT = 400.0f;
    const float FRICT = 0.9f;
    const float BOUNCE = 0.8f;
    const float BOUNCE_X = 0.01f;
    const float PAN_POW = 400.0f;

    glm::vec2 midPan = (pan1 + pan2) / 2.0f;
    
    float dirPrev = glm::sign(prevPan.y - prevMid.y); // pos => below last frame
    float dir = glm::sign(midPan.y - mid.y);
    
    bool wentThrough = (dirPrev != dir  && dir == -1 && prevPan != glm::vec2(0)
			&& prevMid != glm::vec2(0));
    if(wentThrough &&
       (gh::colliding(midPan + panVel, prevPan - panVel, mid + vel, prevMid - vel) == glm::vec2(0) &&
	!gh::colliding(
		glm::vec2(r1.x > r2.x ? r2.x : r1.x, r1.x > r2.x ? r1.x : r2.x),
		glm::vec2(pan1.x, pan2.x)))){
	wentThrough = false;
    }
    dir = dirPrev;
    glm::vec2 c = gh::colliding(r1, r2, pan1, pan2);
    if(c != glm::vec2(0) || wentThrough) {
	if(!collided) {
	    collided = true;
	    doneness+=timeOff*0.001f;
	    timeOff = 0;
	    colour = glm::vec4((float)pow(1.0f/(float)doneness, 0.4));
	    colour.a = 1.0f;
	}
	float pivotD = c.x - mid.x;
	if(c == glm::vec2(0))
	    pivotD = game::random::real() * 10;
	thetaVel -= pivotD * vel.y;
	if(fabs(thetaVel) > MAX_DT)
	    thetaVel = MAX_DT * glm::sign(thetaVel);

	float miny = r1.y > r2.y ? r1.y : r2.y;
	
	mid.y -= dir * fabs(miny - midPan.y)*1.1f;
	vel.x = (1 - FRICT)*vel.x + (FRICT)*panVel.x - pivotD*BOUNCE_X*fabs(vel.y);
	vel.y = (-dir * vel.y * BOUNCE)
	    + ((glm::sign(panVel.y) == glm::sign(mid.y - midPan.y)) ? panVel.y*PAN_POW : 0);
	
    } else {
	collided = false;
    }

    if(fabs(vel.y) > MAX)
	vel.y = MAX * glm::sign(vel.y);

    prevPan = midPan;
    prevMid = mid;
}

void Fish::Draw(Render* render) {
    rect.x = mid.x - rect.z/2;
    rect.y = mid.y - rect.w/2;
    Drawable::Draw(render);
    if(fishOffscreen)
	fishIndicator.Draw(render);
}
