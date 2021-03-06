#include "DefaultShader.h"
#include <Windows.h>


extern const char* svs;
extern const char* sfs;

extern const char* vs2;
extern const char* fs2;

extern const char* dvs2;
extern const char* dfs2;

extern const char* wvs;
extern const char* wfs;


struct{
	int M,
		VP,
		DepthMVP,
		textureSampler,
		shadowMap,
		ldir,
		camPos,
		ambient;
}ShaderVarLocation;


float wave = 0;
void DefaultShader::draw(std::vector<Object*>& objects){
	/*char buf[32];
	_itoa_s(glGetUniformLocation(program, "camdir"), buf, 10);
	MessageBoxA(0, buf, 0, 0);*/

	/*===INDECIES OF SHADER VARIABLES===
	vertexPosition_modelspace	0
	vertexNormal_modelspace		1
	vertexUV					2

	M							5
	VP							6
	DepthBiasMVP[]				0-4
	lightDir					7-11
	myTextureSampler			12
	shadowMap					13-15*/

	/*
	M				1
	VP				2
	DepthMVP		0
	textureSampler	6
	shadowMap		5
	ldir			4
	camdir			3
	*/

	glm::mat4 projShadow = glm::ortho(-80.f, 80.f, -80.f, 80.f, -80.f, 80.f);

	//=========================Generate shadow map=========================
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glUseProgram(shadowProgram);

	for (auto& l : lights){
		glBindFramebuffer(GL_FRAMEBUFFER, l.fb);
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(1, 1, FBO_W - 2, FBO_H - 2);
		for (auto& o : objects){
			if (!(o->options & ObjectOptions::NoShadow)){
				l.mat = projShadow *
					glm::lookAt(eye + l.dir*10.f,
					eye - l.dir*10.f, glm::vec3(0, 1, 0));
				glUniformMatrix4fv(0, 1, 0, &(l.mat*o->mat)[0][0]);
				glBindBuffer(GL_ARRAY_BUFFER, o->model->VBOs[1]);
				glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->model->VBOs[0]);
				glDrawElements(GL_TRIANGLES, o->model->vcount, GL_UNSIGNED_INT, 0);
				glDisableVertexAttribArray(0);
			}
		}
	}

	//=========================Draw true scene=========================
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport.x, viewport.y, viewport.z, viewport.w);

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	glm::mat4 projView =
		glm::perspective(40.f, (float)viewport.z / (float)viewport.w, 0.001f, 200.f)*
		glm::lookAt(eye, pos, glm::vec3(0, 1, 0));

	for (auto& o : objects){
		if (o->options & ObjectOptions::NoDepthTest)
			glDisable(GL_DEPTH_TEST);
		else
			glEnable(GL_DEPTH_TEST);

		if (o->options & ObjectOptions::NoCullFacing)
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);

		if (o->options & ObjectOptions::CullFacing)
			glCullFace(GL_FRONT);
		else
			glCullFace(GL_BACK);

		//Simple Shader
		if (o->options & ObjectOptions::SimpleShader){
			glUseProgram(simpleProgram);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, o->texture->id);
			glUniform1i(1, 0);

			glm::mat4 mat = projView;
			if (o->options & ObjectOptions::StickToCamera)
				mat *= glm::translate(eye)*o->mat;
			else
				mat *= o->mat;
			glUniformMatrix4fv(0, 1, 0, &(mat[0][0]));

			glBindBuffer(GL_ARRAY_BUFFER, o->model->VBOs[1]);
			glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, o->model->VBOs[3]);
			glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->model->VBOs[0]);
			glDrawElements(GL_TRIANGLES, o->model->vcount, GL_UNSIGNED_INT, 0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);
		}
		else if (!(o->options & ObjectOptions::NoVisible)){
			if (o->options & ObjectOptions::Water){

				char buf[32];
				sprintf_s(buf, "%d", glGetUniformLocation(waterProgram, "textureSampler"));
				//MessageBoxA(0, buf, 0, 0);

				glUseProgram(waterProgram);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, o->texture->id);
				glUniform1i(1, 0);
				glUniform1f(2, wave += 0.1f);

				glm::mat4 mat = projView;
				if (o->options & ObjectOptions::StickToCamera)
					mat *= glm::translate(eye)*o->mat;
				else
					mat *= o->mat;
				glUniformMatrix4fv(0, 1, 0, &(mat[0][0]));

				glBindBuffer(GL_ARRAY_BUFFER, o->model->VBOs[1]);
				glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
				glBindBuffer(GL_ARRAY_BUFFER, o->model->VBOs[3]);
				glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->model->VBOs[0]);
				glDrawElements(GL_TRIANGLES, o->model->vcount, GL_UNSIGNED_INT, 0);
				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(0);

			}
			else{
				glUseProgram(program);

				/*char buf[32];
				sprintf_s(buf, "%f", o->mat[2][1]);
				MessageBoxA(0, buf, 0, 0);*/

				//setup texture
				glActiveTexture(GL_TEXTURE0);
				if (o->options & ObjectOptions::ShadowToTexture)//for debug
					glBindTexture(GL_TEXTURE_2D, lights[0].dtext);
				else if (o->texture)
					glBindTexture(GL_TEXTURE_2D, o->texture->id);
				glUniform1i(ShaderVarLocation.textureSampler, 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, lights[0].dtext);
				glUniform1i(ShaderVarLocation.shadowMap, 1);

				//setup matrix
				glUniformMatrix4fv(ShaderVarLocation.VP, 1, 0, &(projView[0][0]));
				if (o->options & ObjectOptions::StickToCamera)
					glUniformMatrix4fv(ShaderVarLocation.M, 1, 0, &(glm::translate(eye)*o->mat)[0][0]);
				else
					glUniformMatrix4fv(ShaderVarLocation.M, 1, 0, &(o->mat[0][0]));
				glUniformMatrix4fv(ShaderVarLocation.DepthMVP, 1, 0, &((biasMatrix *
					lights[0].mat * o->mat)[0][0]));

				//setup light dir
				glUniform3fv(ShaderVarLocation.ldir, 1, &lights[0].dir[0]);
				glUniform3fv(ShaderVarLocation.ambient, 1, &ambient[0]);

				//setup cam pos
				glUniform3fv(ShaderVarLocation.camPos, 1, &pos[0]);
				//draw
				glBindBuffer(GL_ARRAY_BUFFER, o->model->VBOs[1]);
				glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
				glBindBuffer(GL_ARRAY_BUFFER, o->model->VBOs[2]);
				glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
				glBindBuffer(GL_ARRAY_BUFFER, o->model->VBOs[3]);
				glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, 0);

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->model->VBOs[0]);
				glDrawElements(GL_TRIANGLES, o->model->vcount, GL_UNSIGNED_INT, 0);

				glDisableVertexAttribArray(2);
				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(0);
			}
		}
	}
}

DefaultShader::DefaultShader(){
	init();
}
DefaultShader::DefaultShader(glm::ivec4& viewport){
	this->viewport = viewport;
	init();
}
DefaultShader::~DefaultShader(){
	glDeleteProgram(program);
	glDeleteProgram(shadowProgram);
	glDeleteProgram(simpleProgram);
}

void DefaultShader::init(){
	//opengl settings
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//========================Compile shaders========================

	//===============Main shader===============

	GLuint vsid[8];
	vsid[0] = glCreateShader(GL_VERTEX_SHADER);
	vsid[1] = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsid[0], 1, &vs2, 0);
	glCompileShader(vsid[0]);
	glShaderSource(vsid[1], 1, &fs2, 0);
	glCompileShader(vsid[1]);

	program = glCreateProgram();

	glAttachShader(program, vsid[0]);
	glAttachShader(program, vsid[1]);

	glLinkProgram(program);

#ifndef NDEBUG
	{
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		char* log = new char[len];
		glGetProgramInfoLog(program, len, 0, log);

		if (log[0])
			throw log;

		delete[] log;
	}
#endif

	//===============Shadow shader===============

	vsid[2] = glCreateShader(GL_VERTEX_SHADER);
	vsid[3] = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsid[2], 1, &dvs2, 0);
	glCompileShader(vsid[2]);
	glShaderSource(vsid[3], 1, &dfs2, 0);
	glCompileShader(vsid[3]);

	shadowProgram = glCreateProgram();

	glAttachShader(shadowProgram, vsid[2]);
	glAttachShader(shadowProgram, vsid[3]);

	glLinkProgram(shadowProgram);

#ifndef NDEBUG
	{
		GLsizei len;
		glGetProgramiv(shadowProgram, GL_INFO_LOG_LENGTH, &len);
		char* log = new char[len];
		glGetProgramInfoLog(shadowProgram, len, 0, log);

		if (log[0])
			throw log;

		delete[] log;
	}
#endif

	//===============Simple shader===============

	vsid[4] = glCreateShader(GL_VERTEX_SHADER);
	vsid[5] = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsid[4], 1, &svs, 0);
	glCompileShader(vsid[4]);
	glShaderSource(vsid[5], 1, &sfs, 0);
	glCompileShader(vsid[5]);

	simpleProgram = glCreateProgram();

	glAttachShader(simpleProgram, vsid[4]);
	glAttachShader(simpleProgram, vsid[5]);

	glLinkProgram(simpleProgram);

#ifndef NDEBUG
	{
		GLsizei len;
		glGetProgramiv(simpleProgram, GL_INFO_LOG_LENGTH, &len);
		char* log = new char[len];
		glGetProgramInfoLog(simpleProgram, len, 0, log);

		if (log[0])
			throw log;

		delete[] log;
	}
#endif

	//===============Water shader===============

	vsid[6] = glCreateShader(GL_VERTEX_SHADER);
	vsid[7] = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsid[6], 1, &wvs, 0);
	glCompileShader(vsid[6]);
	glShaderSource(vsid[7], 1, &wfs, 0);
	glCompileShader(vsid[7]);

	waterProgram = glCreateProgram();

	glAttachShader(waterProgram, vsid[6]);
	glAttachShader(waterProgram, vsid[7]);

	glLinkProgram(waterProgram);

#ifndef NDEBUG
	{
		GLsizei len;
		glGetProgramiv(waterProgram, GL_INFO_LOG_LENGTH, &len);
		char* log = new char[len];
		glGetProgramInfoLog(waterProgram, len, 0, log);

		if (log[0])
			throw log;

		delete[] log;
	}
#endif

	for each(GLuint a in vsid)
		glDeleteShader(a);

	//=======================Get uniforms location=======================

	ShaderVarLocation.M = glGetUniformLocation(program, "M");
	ShaderVarLocation.VP = glGetUniformLocation(program, "VP");
	ShaderVarLocation.DepthMVP = glGetUniformLocation(program, "DepthMVP");
	ShaderVarLocation.textureSampler = glGetUniformLocation(program, "textureSampler");
	ShaderVarLocation.shadowMap = glGetUniformLocation(program, "shadowMap");
	ShaderVarLocation.ldir = glGetUniformLocation(program, "ldir");
	ShaderVarLocation.camPos = glGetUniformLocation(program, "camPos");
	ShaderVarLocation.ambient = glGetUniformLocation(program, "ambient");
}

void DefaultShader::setViewport(glm::ivec4& viewport){
	this->viewport = viewport;
}
void DefaultShader::setCamera(glm::vec3& eye, glm::vec3& pos){
	this->eye = eye;
	this->pos = pos;
}
void DefaultShader::addLight(glm::vec3& vec){
	if (lights.size() >= MAX_LIGHTS)
		return;

	LightData ld;
	//========================Build Framebuffer========================
	glGenFramebuffers(1, &ld.fb);
	glBindFramebuffer(GL_FRAMEBUFFER, ld.fb);
	glGenTextures(1, &ld.dtext);
	glBindTexture(GL_TEXTURE_2D, ld.dtext);

	glTexImage2D(GL_TEXTURE_2D, 0, FBO_T, FBO_W, FBO_H, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ld.dtext, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

#ifndef NDEBUG
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::exception("Shaodw map framebuffer error", 9);
#endif

	ld.dir = glm::normalize(vec);
	lights.push_back(ld);
}
void DefaultShader::setLight(unsigned slot, glm::vec3& vec){
	lights[slot].dir = vec;
}

void DefaultShader::setAmbient(glm::vec3& _ambient){
	ambient = _ambient;
}