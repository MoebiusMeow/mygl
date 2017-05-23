#define GLFW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include "shaders/shaderManager.h"
using namespace std;

namespace utils
{
	void jam();
	void fps(int cnt=1);
	GLint rnd(GLint m)
	{
		return random()%m;
	}
	GLfloat randf()
	{
		return random()/(GLfloat)RAND_MAX;
	}
	GLfloat randsf()
	{
		return ((random()-RAND_MAX/2)/(GLfloat)(RAND_MAX/2));
	}
}

namespace top
{
	GLFWwindow *win;
	GLsizei win_width, win_height;
	int init();
	int main();
	void draw();
}

namespace gameControl
{
	void key_callback(GLFWwindow *window,int key,int scancode,int action,int mode);
}

namespace ui
{
	namespace randot
	{
		const int N = 100000*3;
		GLfloat dat[N],v[N],m[N];
		GLuint ind[N];
		GLuint VAO,VBO,EBO;
		const GLint cnt = 10000;
		double att = 1;
		void gen()
		{
			using namespace utils;
			using shaderManager::programs;
			for(int i=0;i<cnt;i++)
			{
				GLfloat mxv = 1e-2;
				dat[i*3] = randsf(), dat[i*3+1] = randsf(), dat[i*3+2] = randsf();
				ind[i*3] = rnd(cnt), ind[i*3+1] = rnd(cnt), ind[i*3+2] = rnd(cnt);
				v[i*3] = randsf()*mxv, v[i*3+1] = randsf()*mxv, v[i*3+2] = randsf()*mxv;
				m[i*3] = m[i*3+1] = m[i*3+2] = randsf();
			}
			glGenVertexArrays(1,&VAO);
			glGenBuffers(1,&VBO);
			glGenBuffers(1,&EBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER,VBO);
			glBufferData(GL_ARRAY_BUFFER,cnt*3*sizeof(GLfloat),dat,GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,cnt*3*sizeof(GLint),ind,GL_STATIC_DRAW);
			GLint posLoc = glGetAttribLocation(programs[1],"pos");
			glVertexAttribPointer(posLoc,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
			glEnableVertexAttribArray(posLoc);
			glBindVertexArray(0);
		}
		void regen()
		{
			using namespace utils;
			using shaderManager::programs;
			for(int i=0;i<100;i++) swap(ind[rnd(cnt*3)],ind[rnd(cnt*3)]);
			for(int i=0;i<10;i++) v[rnd(cnt*3)] *= 1.1+glfwGetTime()*1e-3;
			for(int i=0;i<cnt*3;i++)
			{
				double tatt = att;
				//tatt /= max(1,dat[i]*1e10*dat[i]);
				tatt *= min(dat[i]*dat[i]*1e2/m[i],1.0);
				dat[i] += v[i], dat[i] *= tatt;
			}
			att -= 1e-5;
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER,VBO);
			//glBufferData(GL_ARRAY_BUFFER,cnt*3*sizeof(GLfloat),dat,GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER,cnt*3*sizeof(GLfloat),dat,GL_STREAM_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
			//glBufferData(GL_ELEMENT_ARRAY_BUFFER,cnt*3*sizeof(GLint),ind,GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,cnt*3*sizeof(GLint),ind,GL_STREAM_DRAW);
			glBindVertexArray(0);
		}
	}
}

void utils::jam()
{
	double s = 0;
	for(int i=1;i<=1000;i++) for(int j=1;j<=10000;j++)
		s += pow(i,j);
	cerr << s << endl;
}

void utils::fps(int cnt)
{
	static double last = 0;
	double cur = glfwGetTime();
	fprintf(stderr,"Current FPS: %.4lf\n",cnt/(cur-last));
	last = cur;
}

int top::init()
{
	if(!glfwInit())
	{
		cerr << "Fail to init GLFW" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,0);
	glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);
	win = glfwCreateWindow(600,600,"Project 1",nullptr,nullptr);
	if(win==nullptr)
	{
		cerr << "Failed to create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);
	glewExperimental = GL_TRUE;
	if(glewInit()!=GLEW_OK)
	{
		cerr << "Failed to init GLEW" << endl;
		glfwTerminate();
		return -1;
	}
	glfwGetFramebufferSize(win,&win_width,&win_height);
	glViewport(0,0,win_width,win_height);
	return 0;
}

int top::main()
{
	using shaderManager::programs;
	shaderManager::init();
	programs[1] = shaderManager::createProgram("defaultVer.sdr","defaultFra.sdr");
	ui::randot::gen();
	glEnable(GL_DEPTH_TEST);
	// glClearColor(0.01f,0.13f,0.22f,1.0f);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glfwSetKeyCallback(win,gameControl::key_callback);
	int cnt = 0;
	while(!glfwWindowShouldClose(win))
	{
		glfwPollEvents();
		if(++cnt==60)
		{
			utils::fps(60);
			cnt = 0;
		}
		draw();
		glfwSwapBuffers(win);
	}
	glfwTerminate();
	return 0;
}

void top::draw()
{
	using shaderManager::programs;
	glUseProgram(programs[1]);
	ui::randot::regen();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(ui::randot::VAO);
	glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	glDrawArrays(GL_TRIANGLES,0,ui::randot::cnt);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glDrawElements(GL_TRIANGLES,ui::randot::cnt*3,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}

void gameControl::key_callback(GLFWwindow *window,int key,int scancode,int action,int mode)
{
	cerr << "key_callback: " << key << " " << action << endl;
	if(key==GLFW_KEY_ESCAPE&&action==GLFW_PRESS)
		glfwSetWindowShouldClose(top::win,GL_TRUE);
}

int main()
{
	srand(time(0));
	if(top::init()!=0) return -1;
	return top::main();
}
