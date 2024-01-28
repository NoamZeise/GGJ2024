#include "menu.h"
#include "consts.h"
#include "vec_math.h"

Menu::Menu(ResourcePool* pool) {
    bg = Drawable(pool->tex()->load("textures/menu/bg-shadowless.png"), 0.5f);
    title = Drawable(pool->tex()->load("textures/menu/title.png"), 0.5f);
    cat = Drawable(pool->tex()->load("textures/menu/cat.png"), 0.5f);
    play = Drawable(pool->tex()->load("textures/menu/play.png"), 0.5f);    
    exit = Drawable(pool->tex()->load("textures/menu/exit.png"), 0.5f);
    bg.depth = DEPTH_AVG;
    title.depth = bg.depth + DEPTH_STEP;
    cat.depth = bg.depth + DEPTH_STEP;
    play.depth = bg.depth + DEPTH_STEP;
    exit.depth = bg.depth + DEPTH_STEP;
    play.rect.x = bg.rect.z * 0.5625f;
    play.rect.y = bg.rect.w / 2;
    play.sizeFromMid = true;
    exit.rect.x = bg.rect.z * 0.75f;
    exit.rect.y = play.rect.y;
    exit.sizeFromMid = true;
    cat.rect.x = WINDOW_WIDTH * 0.058;
    cat.rect.y = WINDOW_HEIGHT * 0.383;
    title.rect.x = WINDOW_WIDTH * 0.219;
    title.rect.y = WINDOW_HEIGHT * 0.07;
    pixel = pool->tex()->load("textures/pixel.png");
}

void Menu::Update(Timer &timer, Input &input, glm::vec2 cursor) {
    elapsed += timer.dt() * 0.001f;
    float movement = sin(elapsed)*0.1;
    float catmovement = sin(elapsed + 22.0f/28.0f)*0.03;

    title.rotate = movement;
    cat.rotate = catmovement*-1;

    cat.offsetMat = glm::translate(glm::scale(
	    glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0)),
	    glm::vec3(1 - (catmovement*-1), 1 - (catmovement), 1)),
				   glm::vec3(-0.5, -0.5, 0.0));
    
    play.rotate = movement;
    exit.rotate = movement;
    
    
    playPress = false;
    exitPress = false;
    if(gh::contains(cursor, play.rect)) {
	play.colour = glm::vec4(0.5, 0.5, 0.5, 1.0);
	play.drawable_size = 1.1f;
	if(input.m.press(GLFW_MOUSE_BUTTON_LEFT))
	    playPress = true;
    } else {
	play.colour = glm::vec4(1.0f);
	play.drawable_size = 1.0f;
    }
    if(gh::contains(cursor, exit.rect)) {
	exit.colour = glm::vec4(0.5, 0.5, 0.5, 1);
	exit.drawable_size = 1.1f;
	if(input.m.press(GLFW_MOUSE_BUTTON_LEFT))
	    exitPress = true;
    } else {
	exit.colour = glm::vec4(1.0f);
	exit.drawable_size = 1.0f;
    }
}

void Menu::Draw(Render* render) {
    if(paused) {
	Drawable bg = this->bg;
	bg.tex = pixel;
	bg.colour = glm::vec4(0, 0, 0, 0.5f);
	bg.Draw(render);
    } else {
	bg.Draw(render);
    }
    title.Draw(render);
    cat.Draw(render);
    play.Draw(render);
    exit.Draw(render);
}

bool Menu::isExit() {
    return exitPress;
}

bool Menu::isPlay() {
    return playPress;
}
