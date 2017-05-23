#ifndef SHADER_SRC_H
#define SHADER_SRC_H

#include <bits/stdc++.h>
using namespace std;

namespace shaderSrc
{
	map<string,string> lis;
	void init();
}

void shaderSrc::init()
{
	// WARNING: Code between <Begin> and <End> will be automatically edited
	// <Begin>
	// <defaultFra.sdr>
	lis[string("defaultFra.sdr")] = string
	(
		"#version 110\n"
		"\n"
		"varying vec3 col;\n"
		"// uniform sampler2D tex;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = vec4(vec3(col),1.0f);\n"
		"}\n"
	);
	// <defaultVer.sdr>
	lis[string("defaultVer.sdr")] = string
	(
		"#version 110\n"
		"\n"
		"attribute vec3 pos;\n"
		"// attribute vec2 texCood;\n"
		"varying vec3 col;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(pos,1.0);\n"
		"	col = normalize(abs(vec3(dot(pos.xyz,pos.xzy),dot(pos.yxz,pos.yzx)+1.0,dot(pos.zxy,pos.zyx)+1.0))*(-pos.z+1.0)*0.5);\n"
		"}\n"
	);
	// <End>
}

#endif
