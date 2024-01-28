#include <manager.h>
#include <game/camera.h>
#include <graphics/glm_helper.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <graphics/model_gen.h>
#include <graphics/logger.h>

#include <cstring>
#include "menu.h"
#include "cooking.h"
#include "consts.h"
#include "house.h"

int main(int argc, char** argv) {
    RenderFramework framework = RenderFramework::Vulkan;
    for(int i = 0; i < argc; i++)
    	if(strcmp(argv[i], "opengl") == 0)
    	    framework = RenderFramework::OpenGL;
    ManagerState state;
    state.cursor = cursorState::disabled;
    state.windowTitle = "Cat Flat";
    state.conf.multisampling = true;
    state.conf.mip_mapping = false;
    state.conf.texture_filter_nearest = false;
    state.conf.target_resolution[0] = WINDOW_WIDTH;
    state.conf.target_resolution[1] = WINDOW_HEIGHT;
    state.defaultRenderer = framework;
    state.windowWidth = WINDOW_WIDTH;
    state.windowHeight = WINDOW_HEIGHT;
    
    Manager manager(state);
    ResourcePool* pool = manager.render->pool();

    Menu menu(pool);
    House house(pool);
    Cooking cooking(pool);
    
    Resource::Font font = pool->font()->load("textures/Mali-Light.ttf");
    Drawable cursor = Drawable(pool->tex()->load("textures/menu/cursor.png"), 0.5f);
    cursor.rect.x = WINDOW_WIDTH / 2;
    cursor.rect.y = WINDOW_HEIGHT / 2;
    cursor.depth = DEPTH_AVG + DEPTH_STEP*2.0f;

    /*
    ModelInfo::Model sphereData = genSurface([](float a, float b){
	return glm::vec3(sin(a)*cos(b), sin(a)*sin(b), cos(a));
    }, true, 1.0, {0, 22.0/7, 0.1}, {0, 44.0/7, 0.1});
    sphereData.meshes[0].diffuseColour = glm::vec4(0.5f, 0.7f, 0.0f, 1.0f);
    sphereData.meshes[0].diffuseTextures.push_back("cat.png");
    Resource::Model sphere = pool->model()->load(sphereData);
    glm::mat4 sphereMat = glm::translate(glm::mat4(1.0f), glm::vec3(10, 0, 0));*/
    
    
    manager.render->LoadResourcesToGPU(pool);
    manager.render->UseLoadedResources();

    camera::FirstPerson cam;
    cam.setForward(glm::vec3(-1, 0, 0));
    cam.setPos(glm::vec3(0, 0, 0));
    cam.control(glm::vec2(0));

    float elapsed = 0;
    float warp = 0.04;

    enum class GameState {
	Menu,
	House,
	Cooking,	
    };
    GameState gamestate = GameState::Menu;
    GameState unpausedState = GameState::House;

    while(!glfwWindowShouldClose(manager.window)) {
	manager.update();
	
	if(manager.input.kb.press(GLFW_KEY_ESCAPE)) {
	    if(gamestate == GameState::Menu && menu.isPaused())
		gamestate = unpausedState;
	    else if(gamestate != GameState::Menu) {
		unpausedState = gamestate;
		gamestate = GameState::Menu;
		menu.setPaused(true);
	    }
	}
	
	if(manager.input.kb.press(GLFW_KEY_F))
	    manager.toggleFullscreen();       
	
	if(manager.input.kb.hold(GLFW_KEY_EQUAL))
	    warp += 0.1 * manager.timer.dt() * 0.001f;
	if(manager.input.kb.hold(GLFW_KEY_MINUS))
	    warp -= 0.1 * manager.timer.dt() * 0.001f;;

	if(gamestate == GameState::Menu || gamestate == GameState::House) {
	    float sensitivity = 1.0f;
	    cursor.rect.x += manager.input.m.dx() * sensitivity;
	    cursor.rect.y += manager.input.m.dy() * sensitivity;
	    cursor.rect.x = glm::clamp(cursor.rect.x, -cursor.rect.z, WINDOW_WIDTH);
	    cursor.rect.y = glm::clamp(cursor.rect.y, -cursor.rect.w, WINDOW_HEIGHT);
	}

	glm::vec2 cPos = glm::vec2(cursor.rect.x + cursor.rect.z/2,
				   cursor.rect.y + cursor.rect.w/2);

	if(manager.timer.dt() < 200) {
	    switch(gamestate) {
	    case GameState::Cooking:
		house.setActive(false);
		house.Update(manager.timer, manager.input, cPos);
		cooking.Update(manager.timer, manager.input);
		if(cooking.isOver()) {
		    gamestate = GameState::House;
		    cooking.reset();
		}
		break;
	    case GameState::House:
		house.setActive(true);
		house.Update(manager.timer, manager.input, cPos);
		switch(house.pressedRoom()) {
		case Room::Kitc:
		    gamestate = GameState::Cooking;
		    break;
		default:
		    break;
		}
		break;
	    case GameState::Menu:
		menu.Update(manager.timer, manager.input, cPos);
		if(menu.isExit())
		    glfwSetWindowShouldClose(manager.window, GLFW_TRUE);
		if(menu.isPlay())
		    gamestate = unpausedState;
		break;
	    }
	    
	}
	
	//sphereMat = glm::translate(sphereMat, dir * 0.01f * (float)manager.timer.dt());
	manager.render->set3DViewMat(cam.getView(), cam.getPos());
	elapsed += (float)manager.timer.dt() * 0.001f;
	manager.render->setShaderProps({elapsed, warp});
	if(manager.winActive()) {
	    //manager.render->DrawModel(sphere, sphereMat, glm::inverseTranspose(sphereMat));

	    house.Draw(manager.render);
		    
	    switch(gamestate) {
	    case GameState::Menu:
		menu.Draw(manager.render);
		cursor.Draw(manager.render);
		break;
	    case GameState::Cooking:
		cooking.Draw(manager.render);
		break;
	    case GameState::House:
		cursor.Draw(manager.render);
		break;
	    }	  		
	    manager.render->EndDraw();
	}
    }
}
