#include <manager.h>
#include <game/camera.h>
#include <graphics/glm_helper.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <graphics/model_gen.h>
#include <graphics/logger.h>

#include "cooking.h"
#include "consts.h"

int main(int argc, char** argv) {
    ManagerState state;
    state.cursor = cursorState::disabled;
    state.windowTitle = "GGJ2024";
    state.conf.multisampling = true;
    state.conf.sample_shading = true;
    state.conf.target_resolution[0] = WINDOW_WIDTH;
    state.conf.target_resolution[1] = WINDOW_HEIGHT;
    
    Manager manager(state);
    ResourcePool* pool = manager.render->pool();

    Cooking cooking(pool);
    
    Resource::Texture bg = pool->tex()->load("textures/house.png");
    Resource::Font font = pool->font()->load("textures/Mali-Light.ttf");
    Resource::Texture cat = pool->tex()->load("textures/cat.png");
    ModelInfo::Model sphereData = genSurface([](float a, float b){
	return glm::vec3(sin(a)*cos(b), sin(a)*sin(b), cos(a));
    }, true, 1.0, {0, 22.0/7, 0.1}, {0, 44.0/7, 0.1});
    sphereData.meshes[0].diffuseColour = glm::vec4(0.5f, 0.7f, 0.0f, 1.0f);
    //sphereData.meshes[0].diffuseTextures.push_back("testscreen.png");
    Resource::Model sphere = pool->model()->load(sphereData);
    glm::mat4 sphereMat = glm::translate(glm::mat4(1.0f), glm::vec3(10, 0, 0));

    manager.render->LoadResourcesToGPU(pool);
    manager.render->UseLoadedResources();

    camera::FirstPerson cam;
    cam.setForward(glm::vec3(-1, 0, 0));
    cam.setPos(glm::vec3(0, 0, 0));
    cam.control(glm::vec2(0));

    float elapsed = 0;
    float warp = 0.04;

    while(!glfwWindowShouldClose(manager.window)) {
	manager.update();
	if(manager.input.kb.press(GLFW_KEY_ESCAPE))
	    glfwSetWindowShouldClose(manager.window, GLFW_TRUE);
	if(manager.input.kb.press(GLFW_KEY_F))
	    manager.toggleFullscreen();
	glm::vec3 dir(0);
	if(manager.input.kb.hold(GLFW_KEY_Q))
	    dir.x += 1;
	if(manager.input.kb.hold(GLFW_KEY_E))
	    dir.x -= 1;
	if(manager.input.kb.hold(GLFW_KEY_A))
	    dir.y += 1;
	if(manager.input.kb.hold(GLFW_KEY_D))
	    dir.y -= 1;
	if(manager.input.kb.hold(GLFW_KEY_W))
	    dir.z += 1;
	if(manager.input.kb.hold(GLFW_KEY_S))
	    dir.z -= 1;
	
	if(manager.input.kb.hold(GLFW_KEY_EQUAL))
	    warp += 0.1 * manager.timer.dt() * 0.001f;
	if(manager.input.kb.hold(GLFW_KEY_MINUS))
	    warp -= 0.1 * manager.timer.dt() * 0.001f;;

	if(manager.timer.dt() < 100) {
	    cooking.Update(manager.timer, manager.input);
	}
	
	sphereMat = glm::translate(sphereMat, dir * 0.01f * (float)manager.timer.dt());
	manager.render->set3DViewMat(cam.getView(), cam.getPos());
	elapsed += (float)manager.timer.dt() * 0.001f;
	manager.render->setShaderProps({elapsed, warp});
	if(manager.winActive()) {
	    manager.render->DrawQuad(
	    	    bg, glmhelper::calcMatFromRect(glm::vec4(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), 0, DEPTH_AVG - DEPTH_STEP*2));
	    manager.render->DrawQuad(cat, glmhelper::calcMatFromRect(
					     glm::vec4(100, 200, cat.dim.x, cat.dim.y),
					     0, DEPTH_AVG - DEPTH_STEP));
	    //manager.render->DrawModel(sphere, sphereMat, glm::inverseTranspose(sphereMat));
	    cooking.Draw(manager.render);

	    /*	    manager.render->DrawString(font,
		    "fps: " + std::to_string(1.0/(0.001 * manager.timer.dt())),
		    glm::vec2(20, 30), 30.0f, DEPTH_AVG, glm::vec4(1.0f)
				       );*/
		
	    manager.render->EndDraw();
	}
    }
}
