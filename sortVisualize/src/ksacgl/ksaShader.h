#pragma once
#include <gl/glew.h>

typedef struct ksaShaderFiles
{
	char *vertex;
	char *fragment;
	char* geometry;
} ksaShaderFiles;

typedef struct ksaShader
{
	unsigned int programId;
	unsigned int vshaderId;
	unsigned int fshaderId;
	unsigned int gshaderId;
	const char* vshaderPath;
	const char* fshaderPath;
	const char* gshaderPath;
} ksaShader;

void ksaShaderGet(ksaShader *shader, ksaShaderFiles *files);
unsigned int ksaCreateShader(ksaShaderFiles *files);
void ksaShaderDestroy(ksaShader *shader, ksaShaderFiles *files);
void ksaShaderInit(ksaShader *shader, ksaShaderFiles *files);
void ksaShaderDestroy(ksaShader *shader, ksaShaderFiles *files);
void ksaShaderUse(ksaShader* shader);