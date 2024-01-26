
#include <manager.h>
#include <game/camera.h>
#include <graphics/glm_helper.h>
#include <glm/gtc/matrix_inverse.hpp>

int main(int argc, char** argv) {
    ManagerState state;
    state.cursor = cursorState::disabled;
    state.windowTitle = "minimum example";
    state.conf.multisampling = true;
    state.conf.sample_shading = true;
    
    Manager manager(state);
    ResourcePool* pool = manager.render->pool();

    Resource::Model monkey = pool->model()->load("models/monkey.obj");
    glm::mat4 monkeyMat = glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(270.0f),
						     glm::vec3(-1.0f, 0.0f, 0.0f)),
					 glm::vec3(0.0f, -8.0f, -10.0f));    
    Resource::Font font = pool->font()->load("textures/Roboto-Black.ttf");

    manager.render->LoadResourcesToGPU(pool);
    manager.render->UseLoadedResources();

    camera::FirstPerson cam;
    cam.setPos(glm::vec3(0, 0, 0));
    manager.audio.Play("audio/test.wav", false, 1.0f);
    while(!glfwWindowShouldClose(manager.window)) {
	manager.update();
	
	if(manager.input.kb.press(GLFW_KEY_ESCAPE))
	    glfwSetWindowShouldClose(manager.window, GLFW_TRUE);
	cam.flycamUpdate(manager.input, manager.timer);
	manager.render->set3DViewMat(cam.getView(), cam.getPos());
	
	if(manager.winActive()) {
	    manager.render->DrawModel(monkey, monkeyMat, glm::inverseTranspose(monkeyMat));
	    manager.render->DrawString(font, "Minimum Example", glm::vec2(220.0f, 50.0f),
				       50.0f, 1.0f, glm::vec4(1.0f, 0.5f, 1.0f, 1.0f));
	    manager.render->EndDraw();
	}
    }
}
