#include "house.h"
#include "vec_math.h"

House::House(ResourcePool* pool) {
    bg = Drawable(pool->tex()->load("textures/house/house.png"), 0.5f);
    bathroom = Drawable(pool->tex()->load("textures/house/bathroom.png"), 0.5f);
    bedroom = Drawable(pool->tex()->load("textures/house/bedroom.png"), 0.5f);
    bedroom_quilt = Drawable(pool->tex()->load("textures/house/quilt.png"), 0.5f);
    kitchen = Drawable(pool->tex()->load("textures/house/kitchen.png"), 0.5f);
    livingroom = Drawable(pool->tex()->load("textures/house/livingroom.png"), 0.5f);
    cat = Drawable(pool->tex()->load("textures/cat.png"), 0.5f);
    pixel = pool->tex()->load("textures/pixel.png");
    bg.depth = DEPTH_AVG - DEPTH_STEP*5;
    kitchen.sizeFromMid = true;
    kitchen.depth = bg.depth + DEPTH_STEP;
    kitchen.rect.x = WINDOW_WIDTH * 0.481;
    kitchen.rect.y = WINDOW_HEIGHT * 0.606;
    bedroom.sizeFromMid = true;
    bedroom.depth = bg.depth + DEPTH_STEP;
    bedroom.rect.x = WINDOW_WIDTH * 0.572;
    bedroom.rect.y = WINDOW_HEIGHT * 0.144;
    bedroom_quilt.depth = bg.depth + DEPTH_STEP + DEPTH_STEP*1.2f;
    bedroom_quilt.rect = bedroom.rect;
    bedroom_quilt.sizeFromMid = true;
    bathroom.sizeFromMid = true;
    bathroom.depth = bg.depth + DEPTH_STEP + DEPTH_STEP*1.2f;
    bathroom.rect.x = WINDOW_WIDTH * 0.041;
    bathroom.rect.y = WINDOW_HEIGHT * 0.167;
    livingroom.sizeFromMid = true;
    livingroom.depth = bg.depth + DEPTH_STEP;
    livingroom.rect.x = WINDOW_WIDTH * 0.038;
    livingroom.rect.y = WINDOW_HEIGHT * 0.561;
    
    cat.depth = bg.depth + DEPTH_STEP + DEPTH_STEP*1.1f;
}

glm::mat4 calcRot(float mov) {
    return glm::translate(glm::scale(
				  glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0)),
				  glm::vec3(1 - (mov*-1), 1 - (mov), 1)),
			  glm::vec3(-0.5, -0.5, 0.0));
}

void House::Update(Timer &timer, Input &input, glm::vec2 cursor) {
    if(!active)
	cursor = glm::vec2(-100, -100);
    elapsed += timer.dt() * 0.0005f;
    pressed = Room::None;
    if(active)
	hovering = Room::None;
    float movKit  = sin(elapsed + 1)*0.03;
    float movBed  = sin(elapsed + 2)*0.03;
    float movBath = sin(elapsed + 3)*0.03;
    float movLiv  = sin(elapsed + 4)*0.03;

    kitchen.offsetMat = calcRot(movKit);
    bathroom.offsetMat = calcRot(movBath);
    bedroom.offsetMat = calcRot(movBed);
    bedroom_quilt.offsetMat = bedroom.offsetMat;
    livingroom.offsetMat = calcRot(movLiv);


    if(gh::contains(cursor, kitchen.rect)) {
	kitchen.drawable_size = 1.1f;
	hovering = Room::Kitc;
	if(input.m.press(GLFW_MOUSE_BUTTON_LEFT))
	    pressed = Room::Kitc;
    } else {
	kitchen.drawable_size = 1.0f;
    }

    if(gh::contains(cursor, bedroom.rect)) {
	bedroom.drawable_size = 1.1f;
	bedroom_quilt.drawable_size = 1.1f;
	hovering = Room::Bed;
	if(input.m.press(GLFW_MOUSE_BUTTON_LEFT))
	    pressed = Room::Bed;
    } else {
	bedroom.drawable_size = 1.0f;
	bedroom_quilt.drawable_size = 1.0f;
    }

    if(gh::contains(cursor, bathroom.rect)) {
	bathroom.drawable_size = 1.1f;
	hovering = Room::Bath;
	if(input.m.press(GLFW_MOUSE_BUTTON_LEFT))
	    pressed = Room::Bath;
    } else {
	bathroom.drawable_size = 1.0f;
    }

    if(gh::contains(cursor, livingroom.rect)) {
	livingroom.drawable_size = 1.1f;
	hovering = Room::Live;
	if(input.m.press(GLFW_MOUSE_BUTTON_LEFT))
	    pressed = Room::Live;
    } else {
	livingroom.drawable_size = 1.0f;
    }
}

void House::Draw(Render *render) {
    bg.Draw(render);
    
    kitchen.Draw(render);
    bedroom.Draw(render);
    livingroom.Draw(render);
    
    switch(hovering) {
    case Room::Kitc:
	cat.rect.x = WINDOW_WIDTH * 0.7125;
	cat.rect.y = WINDOW_HEIGHT * 0.789;
	cat.Draw(render);
	break;
    case Room::Bed:
	cat.rect.x = WINDOW_WIDTH * 0.719;
	cat.rect.y = WINDOW_HEIGHT * 0.278;
	cat.Draw(render);
	break;
    case Room::Live:
	cat.rect.x = WINDOW_WIDTH * 0.216;
	cat.rect.y = WINDOW_HEIGHT * 0.798;
	cat.Draw(render);
	break;
    case Room::Bath:
	cat.rect.x = WINDOW_WIDTH * 0.215;
	cat.rect.y = WINDOW_HEIGHT * 0.258;
	cat.Draw(render);
	break;
    default:
	break;
    }
    if(!active) {
	cat.Draw(render);
    }
    bedroom_quilt.Draw(render);
    bathroom.Draw(render);

    if(!active) {
	Drawable bg = this->bg;
	bg.tex = pixel;
	bg.colour = glm::vec4(0, 0, 0, 0.5f);
	bg.depth = cat.depth + DEPTH_STEP/2;
	bg.Draw(render);
    }
}


