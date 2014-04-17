#include <Windows.h>
#include "Scene.h"
#include "DefaultShader.h"
#include "DungeonGenerator.h"
#include "GL\glfw3.h"
#include <ctime>
#include <process.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GL/glew32.lib")
#pragma comment(lib, "GL/glfw3.lib")

#define M_PI 3.14159265358979323846

GLFWwindow* window;
float aa = 0, bb = 0, zoom = 0, slow_zoom = 5;

glm::vec4 camPos(0, 6, 0, 1);
glm::vec4 camTarget(0, 0, 0, 1);

glm::vec3 dirPos(0, 0, 0);

void keyboard_callback(GLFWwindow*, int k, int, int action, int){
	if (k == GLFW_KEY_W){
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			dirPos.z = -1.f;
		else
			dirPos.z = 0.f;
	}
	else if (k == GLFW_KEY_S){
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			dirPos.z = 1.f;
		else
			dirPos.z = 0.f;
	}
	else if (k == GLFW_KEY_A){
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			dirPos.x = -1.f;
		else
			dirPos.x = 0.f;
	}
	else if (k == GLFW_KEY_D){
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			dirPos.x = 1.f;
		else
			dirPos.x = 0.f;
	}
}

void scroll_callback(GLFWwindow*, double x, double y){
	zoom -= (float)y;
}

void cursor_callback(GLFWwindow*, double x, double y){
	aa += (float)(1024.f / 2.f - x) / 6.f;
	if (abs(bb + (float)(768.f / 2.f - y) / 6.f) < 60.f)
		bb += (float)(768.f / 2.f - y) / 6.f;
	glfwSetCursorPos(window, 1024.f / 2.f, 768.f / 2.f);
}

void dungeonProc(void* s){

	Scene* scene = (Scene*)s;
	


	_endthread();
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT){
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 8);
	window = glfwCreateWindow(1024, 768, "XauEngine", 0, 0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwMakeContextCurrent(window);
	glewInit();

	try{
		auto shader = new DefaultShader(glm::ivec4(0, 0, 1024, 768));
		shader->addLight(glm::normalize(glm::vec3(1.5, 2, 0.8)));

		Scene scene(shader);
		scene.add(new Object(loadModel("Models/skybox2.ply"),
			loadTexture("Textures/skybox.bmp"), SkyBox));
		scene.add(new Object(loadModel("Models/cube.ply"),
			loadTexture("Textures/brick.bmp")));

		scene[1]->move(glm::vec3(0, 1, 0));

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
	glm::vec3 dirs21[] = {
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, -1),
	};

	glm::vec3 dirs3[] = {
		glm::vec3(1, 0, 0),
		glm::vec3(0, 0, -1),
	};
	glm::vec3 dirs31[] = {
		glm::vec3(1, 0, 0),
		glm::vec3(0, 0, 1),
	};
	glm::vec3 dirs32[] = {
		glm::vec3(-1, 0, 0),
		glm::vec3(0, 0, 1),
	};
	glm::vec3 dirs33[] = {
		glm::vec3(-1, 0, 0),
		glm::vec3(0, 0, -1),
	};
	glm::vec3 dirs4[] = {
		glm::vec3(-1, 0, 0),
	};
	glm::vec3 dirs41[] = {
		glm::vec3(0, 1, 0),
	};
	glm::vec3 dirs42[] = {
		glm::vec3(1, 0, 0),
	};
	glm::vec3 dirs43[] = {
		glm::vec3(0, 0, -1),
	};


	dg.defineObject(new Object(loadModel("Models/dung1.ply"),
		loadTexture("Textures/dung_tex2.bmp")), dirs, 4);

	dg.defineObject(new Object(loadModel("Models/dung2.ply"),
		loadTexture("Textures/dung_tex2.bmp")), dirs2, 2);

	dg.defineObject(new Object(loadModel("Models/dung21.ply"),
		loadTexture("Textures/dung_tex2.bmp")), dirs21, 2);


	dg.defineObject(new Object(loadModel("Models/dung3.ply"),
		loadTexture("Textures/tex30.bmp")), dirs3, 2);

	dg.defineObject(new Object(loadModel("Models/dung31.ply"),
		loadTexture("Textures/tex31.bmp")), dirs31, 2);

	dg.defineObject(new Object(loadModel("Models/dung32.ply"),
		loadTexture("Textures/tex32.bmp")), dirs32, 2);

	dg.defineObject(new Object(loadModel("Models/dung33.ply"),
		loadTexture("Textures/tex33.bmp")), dirs33, 2);

	dg.defineObject(new Object(loadModel("Models/dung4.ply"),
		loadTexture("Textures/tex30.bmp")), dirs4, 2);

	dg.defineObject(new Object(loadModel("Models/dung41.ply"),
		loadTexture("Textures/tex31.bmp")), dirs41, 2);

	dg.defineObject(new Object(loadModel("Models/dung42.ply"),
		loadTexture("Textures/tex32.bmp")), dirs42, 2);

	dg.defineObject(new Object(loadModel("Models/dung43.ply"),
		loadTexture("Textures/tex33.bmp")), dirs43, 2);

	dg.generate(20000);
	dg.applyObjects(&scene);
	

		while (!glfwWindowShouldClose(window)){
			camPos += glm::rotate(aa, glm::vec3(0, 1, 0))*
				glm::normalize(glm::vec4(dirPos, 1.f));
			slow_zoom += (zoom - slow_zoom) / 4;

			camTarget = glm::rotate(aa, glm::vec3(0, 1, 0)) *
				glm::rotate(bb, glm::vec3(1, 0, 0)) *
				glm::vec4(0, 0, 1, 1);
			shader->setCamera(glm::vec3(camPos) + glm::vec3(camTarget) +
				glm::vec3(0, slow_zoom, 0), glm::vec3(camPos) + glm::vec3(0, slow_zoom, 0));

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