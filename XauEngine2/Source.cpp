#include <Windows.h>
#include "Scene.h"
#include "DefaultShader.h"
#include "DungeonGenerator.h"
#include "GL\glfw3.h"
#include <ctime>
#include <thread>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GL/glew32.lib")
#pragma comment(lib, "GL/glfw3.lib")

#define M_PI 3.14159265358979323846

GLFWwindow* window;
DungeonGenerator dg;
float aa = 0, bb = 0, zoom = 0, slow_zoom = 5;

glm::vec4 camPos(0, 6, 0, 1);
glm::vec4 camTarget(0, 0, 0, 1);

glm::vec3 dirPos(0, 0, 0);

bool generatre = false;

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
	else if (k == GLFW_KEY_F && !generatre)
		if (action == GLFW_PRESS)
			generatre = true;
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

CRITICAL_SECTION section;

DWORD WINAPI dungeonProc(void* s){
	EnterCriticalSection(&section);
	dg.clear();
	LeaveCriticalSection(&section);

	dg.generate(20000, 1000);

	EnterCriticalSection(&section);
	dg.applyObjects((Scene*)s);
	LeaveCriticalSection(&section);
	return 0;
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT){

	InitializeCriticalSection(&section);

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
		shader->addLight(glm::normalize(glm::vec3(0.5f, 1.f, 0.5f)));
		//->setAmbient(glm::vec3(-0.02, -0.02, -0.035));

		Scene scene(shader);
		scene.add(new Object(loadModel("Models/skybox2.ply"),
			loadTexture("Textures/skybox.bmp"), SkyBox));

		scene.add(new Object(loadModel("Models/cube.ply"),
			loadTexture("Textures/brick.bmp")));

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
			glm::vec3(0, 0, -1),
		};
		glm::vec3 dirs42[] = {
			glm::vec3(1, 0, 0),
		};
		glm::vec3 dirs43[] = {
			glm::vec3(0, 0, 1),
		};
		/*glm::vec3 dirs401[] = {
			glm::vec3(-1, -1, 0),
			};
			glm::vec3 dirs411[] = {
			glm::vec3(0, -1, -1),
			};
			glm::vec3 dirs421[] = {
			glm::vec3(1, -1, 0),
			};
			glm::vec3 dirs431[] = {
			glm::vec3(0, -1, 1),
			};
			glm::vec3 dirs402[] = {
			glm::vec3(-1, 1, 0),
			};
			glm::vec3 dirs412[] = {
			glm::vec3(0, 1, -1),
			};
			glm::vec3 dirs422[] = {
			glm::vec3(1, 1, 0),
			};
			glm::vec3 dirs432[] = {
			glm::vec3(0, 1, 1),
			};*/
		glm::vec3 dirs5[] = {
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec3(0, 0, 1),
		};
		glm::vec3 dirs51[] = {
			glm::vec3(-1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, -1),
		};
		glm::vec3 dirs52[] = {
			glm::vec3(1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec3(0, 0, -1),
		};
		glm::vec3 dirs53[] = {
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, -1),
		};

		glm::vec3 dirs6[] = {
			glm::vec3(1.0, 1.0, 0),
			glm::vec3(-1, 0, 0),
		};
		glm::vec3 dirs61[] = {
			glm::vec3(0, 1.0, 1.0),
			glm::vec3(0, 0, -1),
		};
		glm::vec3 dirs62[] = {
			glm::vec3(-1.0, 1.0, 0),
			glm::vec3(1, 0, 0),
		};
		glm::vec3 dirs63[] = {
			glm::vec3(0, 1.0, -1.0),
			glm::vec3(0, 0, 1),
		};

		glm::vec3 dirs7[] = {
			glm::vec3(-1.0, -1.0, 0),
			glm::vec3(1, 0, 0),
		};
		glm::vec3 dirs71[] = {
			glm::vec3(0, -1.0, -1.0),
			glm::vec3(0, 0, 1),
		};
		glm::vec3 dirs72[] = {
			glm::vec3(1.0, -1.0, 0),
			glm::vec3(-1, 0, 0),
		};
		glm::vec3 dirs73[] = {
			glm::vec3(0, -1.0, 1.0),
			glm::vec3(0, 0, -1),
		};

		Texture* dungText = loadTexture("Textures/dung_tex.bmp");

		dg.defineObject(new Object(loadModel("Models/dung1.ply"),
			dungText), dirs, 4);
		dg.defineObject(new Object(loadModel("Models/dung2.ply"),
			dungText), dirs2, 2);
		dg.defineObject(new Object(loadModel("Models/dung21.ply"),
			dungText), dirs21, 2);


		dg.defineObject(new Object(loadModel("Models/dung3.ply"),
			dungText), dirs3, 2);
		dg.defineObject(new Object(loadModel("Models/dung31.ply"),
			dungText), dirs31, 2);
		dg.defineObject(new Object(loadModel("Models/dung32.ply"),
			dungText), dirs32, 2);
		dg.defineObject(new Object(loadModel("Models/dung33.ply"),
			dungText), dirs33, 2);

		dg.defineObject(new Object(loadModel("Models/dung4.ply"),
			dungText), dirs4, 1, End);
		dg.defineObject(new Object(loadModel("Models/dung41.ply"),
			dungText), dirs41, 1, End);
		dg.defineObject(new Object(loadModel("Models/dung42.ply"),
			dungText), dirs42, 1, End);
		dg.defineObject(new Object(loadModel("Models/dung43.ply"),
			dungText), dirs43, 1, End);

		dg.defineObject(new Object(loadModel("Models/dung5.ply"),
			dungText), dirs5, 3);
		dg.defineObject(new Object(loadModel("Models/dung51.ply"),
			dungText), dirs51, 3);
		dg.defineObject(new Object(loadModel("Models/dung52.ply"),
			dungText), dirs52, 3);
		dg.defineObject(new Object(loadModel("Models/dung53.ply"),
			dungText), dirs53, 3);

		dg.defineObject(new Object(loadModel("Models/dung6.ply"),
			dungText), dirs6, 2);
		dg.defineObject(new Object(loadModel("Models/dung61.ply"),
			dungText), dirs61, 2);
		dg.defineObject(new Object(loadModel("Models/dung62.ply"),
			dungText), dirs62, 2);
		dg.defineObject(new Object(loadModel("Models/dung63.ply"),
			dungText), dirs63, 2);

		dg.defineObject(new Object(loadModel("Models/dung7.ply"),
			dungText), dirs7, 2);
		dg.defineObject(new Object(loadModel("Models/dung71.ply"),
			dungText), dirs71, 2);
		dg.defineObject(new Object(loadModel("Models/dung72.ply"),
			dungText), dirs72, 2);
		dg.defineObject(new Object(loadModel("Models/dung73.ply"),
			dungText), dirs73, 2);

		int pos = scene.add(new Object(createPlane(32, 32, 0.5f), loadTexture("Textures/blueText.bmp"), Water | NoCullFacing | NoShadow));
		scene[pos]->move(glm::vec3(0, -28, 0));

		//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		//glClearColor(1, 1, 1, 1);

		clock_t fpsCounter = clock();

		float counter = (float)M_PI;
		int fps = 0;
		while (!glfwWindowShouldClose(window)){
			counter += 0.1f;
			++fps;

			scene[1]->move(glm::vec3(0, sin(counter) / 2.f - 25, 0));
			if (clock() > fpsCounter + 2000){
				char buf[32];
				sprintf_s(buf, "FPS: %d", fps / 2);
				glfwSetWindowTitle(window, buf);

				fps = 0;
				fpsCounter = clock();
			}

			scene[0]->rotateNext(glm::vec4(0, 1, 0, 0.01f));

			if (generatre){
				CreateThread(0, 0, dungeonProc, &scene, 0, 0);
				generatre = false;
			}

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
		dg.clear();
	}
	catch (const char* e){
		MessageBoxA(0, e, 0, 0);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}