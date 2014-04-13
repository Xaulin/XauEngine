#include <Windows.h>
#include "Scene.h"
#include "DefaultShader.h"
#include "DungeonGenerator.h"
#include "GL\glfw3.h"
#include <ctime>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GL/glew32.lib")
#pragma comment(lib, "GL/glfw3.lib")

#define M_PI 3.14159265358979323846

GLFWwindow* window;
double cx = 0, cy = 0;
double dx = 0, dy = 0;
float l = 0;

bool keysState[2];
char scroll_state = 0;
float aa = 3.14f, b = 0, zoom = 5, slow_zoom = 5;

glm::vec4 camPos(0, 8, 0, 1);
glm::vec4 camTarget(0, 0, 0, 1);


void mouse_callback(GLFWwindow*, int k, int action, int){
	if (k == GLFW_MOUSE_BUTTON_LEFT){
		if (action == GLFW_PRESS || action == GLFW_REPEAT){
			glfwGetCursorPos(window, &cx, &cy);
			keysState[0] = true;
		}
		else
			keysState[0] = false;
	}
	else if (k == GLFW_MOUSE_BUTTON_RIGHT){
		if (action == GLFW_PRESS || action == GLFW_REPEAT){
			glfwGetCursorPos(window, &dx, &dy);
			keysState[1] = true;
		}
		else
			keysState[1] = false;
	}
}


void scroll_callback(GLFWwindow*, double x, double y){
	zoom -= (float)y;
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT){
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(1024, 768, "XauEngine", 0, 0);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwMakeContextCurrent(window);
	glewInit();

	try{
		auto shader = new DefaultShader(glm::ivec4(0, 0, 1024, 768));
		shader->addLight(glm::normalize(glm::vec3(1, 2, 2)));

		Scene scene(shader);
		scene.add(new Object(loadModel("Models/skybox2.ply"),
			loadTexture("Textures/skybox.bmp"), SkyBox));

		DungeonGenerator dg;
		glm::vec3 dirs[] = {
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec3(-1, 0, 0),
			glm::vec3(0, 0, -1),
		};
		glm::vec3 dirs2[] = {
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
		};
		glm::vec3 dirs3[] = {
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
		};
		dg.defineObject(new Object(loadModel("Models/dung1.ply"),
			loadTexture("Textures/dung_tex2.bmp")), dirs, 4);
		dg.defineObject(new Object(loadModel("Models/dung2.ply"),
			loadTexture("Textures/dung_tex2.bmp")), dirs, 2);
		dg.defineObject(new Object(loadModel("Models/dung3.ply"),
			loadTexture("Textures/dung_tex2.bmp")), dirs, 2);
		dg.generate(5000);
		dg.applyObjects(&scene);

		while (!glfwWindowShouldClose(window)){
			if (keysState[0]){
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				camPos += glm::vec4((float)(cx - x) / 50.f, 0, (float)(cy - y) / 50.f, 0)*
					glm::rotate(aa, glm::vec3(0, 1, 0));
				cx = x;
				cy = y;
			}
			if (keysState[1]){
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				aa += (float)(dx - x) / 4.f;
				dx = x;
				dy = y;
			}
			//l += 0.05f;
			//shader->setLight(0, glm::normalize(glm::vec3(sin(l), 1.5, cos(l))));
			slow_zoom += (zoom - slow_zoom) / 5;

			camTarget = glm::vec4(0, 0, 4, 1)* glm::rotate(aa, glm::vec3(0, 1, 0));
			shader->setCamera(glm::vec3(camPos) + glm::vec3(0, slow_zoom, 0) + glm::vec3(camTarget),
				glm::vec3(camPos));

			scene.draw();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	catch (const char* e){
		MessageBoxA(0, e, 0, 0);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}