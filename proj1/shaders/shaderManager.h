#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>

#include "shaderSrc.h"
using namespace std;

namespace shaderManager
{
	GLuint programs[256];
	void init();
	int checkCompile(GLuint shader,const char *ex=0);
	int checkLink(GLuint prog,const char *ex=0);
	int createProgram(const char *fn_ver,const char *fn_fra);
}

void shaderManager::init()
{
	memset(programs,0xff,sizeof programs);
	shaderSrc::init();
}

int shaderManager::checkCompile(GLuint shader,const char *ex)
{
	GLint succ;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&succ);
	if(!succ)
	{
		GLchar log[1024];
		glGetShaderInfoLog(shader,1024,NULL,log);
		cerr << "Shader " << shader;
		if(ex) cerr << "(" << ex << ")";
		cerr << " Compile Failed" << endl;
		cerr << log << endl;
		return 0;
	}
	return 1;
}

int shaderManager::checkLink(GLuint prog,const char *ex)
{
	GLint succ;
	glGetProgramiv(prog,GL_LINK_STATUS,&succ);
	if(!succ)
	{
		GLchar log[1024];
		glGetProgramInfoLog(prog,1024,NULL,log);
		cerr << "Program " << prog;
		if(ex) cerr << "(" << ex << ")";
		cerr << " Link Failed" << endl;
		cerr << log << endl;
		return 0;
	}
	return 1;
}

int shaderManager::createProgram(const char *fn_ver,const char *fn_fra)
{
	GLuint ver = glCreateShader(GL_VERTEX_SHADER);
	GLuint fra = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar *s_ver = shaderSrc::lis[string(fn_ver)].c_str();
	const GLchar *s_fra = shaderSrc::lis[string(fn_fra)].c_str();
	glShaderSource(ver,1,&s_ver,NULL);
	glShaderSource(fra,1,&s_fra,NULL);
	glCompileShader(ver);
	glCompileShader(fra);
	if(!checkCompile(ver,fn_ver)) return -1;
	if(!checkCompile(fra,fn_fra)) return -1;
	GLuint prog = glCreateProgram();
	glAttachShader(prog,ver);
	glAttachShader(prog,fra);
	glLinkProgram(prog);
	glDeleteShader(ver);
	glDeleteShader(fra);
	if(!checkLink(prog)) return -1;
	return prog;
}

#endif
