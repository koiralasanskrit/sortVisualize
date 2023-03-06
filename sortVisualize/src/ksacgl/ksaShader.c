#include "ksaShader.h"
#include <stdio.h>
#include <stdlib.h>

void 
ksaShaderGet(ksaShader *shader, ksaShaderFiles* files)
{
	FILE *_vshaderFile = NULL;
	FILE *_fshaderFile = NULL;
	FILE *_gshaderFile = NULL;
	size_t _vshaderSize = 0;
	size_t _fshaderSize = 0;
	size_t _gshaderSize = 0;

	/*VShader*/
	fopen_s(&_vshaderFile, shader->vshaderPath, "rb");
	if (_vshaderFile != NULL)
	{
		fseek(_vshaderFile, 0, SEEK_END);
		_vshaderSize = ftell(_vshaderFile);
		fseek(_vshaderFile, 0, SEEK_SET);
		files->vertex = (char*)malloc(_vshaderSize + 1);
		fread(files->vertex, 1, _vshaderSize, _vshaderFile);
		files->vertex[_vshaderSize] = 0;
		fclose(_vshaderFile);
	}
	else
	{
		printf("No Vertex Shader:\n Using Default\n");
	}

	/*FShader*/
	fopen_s(&_fshaderFile, shader->fshaderPath, "rb");
	if (_fshaderFile != NULL) {
		fseek(_fshaderFile, 0, SEEK_END);
		_fshaderSize = ftell(_fshaderFile);
		fseek(_fshaderFile, 0, SEEK_SET);
		files->fragment = (char*)malloc(_fshaderSize + 1);
		fread(files->fragment, 1, _fshaderSize, _fshaderFile);
		files->fragment[_fshaderSize] = 0;
		fclose(_fshaderFile);
	}
	else
	{
		printf("No Pixel Shader:\n Using Default\n");
	}

	/*GShader*/
	if (shader->gshaderPath != NULL) {
		fopen_s(&_gshaderFile, shader->gshaderPath, "rb");
		if (_gshaderFile != NULL) {
			fseek(_gshaderFile, 0, SEEK_END);
			_gshaderSize = ftell(_gshaderFile);
			fseek(_gshaderFile, 0, SEEK_SET);
			files->geometry = (char*)malloc(_gshaderSize + 1);
			fread(files->geometry, 1, _gshaderSize, _gshaderFile);
			files->geometry[_gshaderSize] = 0;
			fclose(_gshaderFile);
		}
		else
		{
			printf("No Geometry Shader:\n Using Default\n");
		}
	}
	printf("%s\n", files->vertex);
	printf("%s\n", files->fragment);
	if (shader->gshaderPath != NULL)
		printf("%s\n", files->geometry);

}

void ksaShaderInit(ksaShader *shader, ksaShaderFiles *files)
{
	// compileShader
	ksaShaderGet(shader, files);
	shader->programId = glCreateProgram();

	shader->vshaderId = glCreateShader(GL_VERTEX_SHADER);
	shader->fshaderId = glCreateShader(GL_FRAGMENT_SHADER);
	if(shader->gshaderPath != NULL)
		shader->gshaderId = glCreateShader(GL_GEOMETRY_SHADER);

	glShaderSource(shader->vshaderId, 1, &files->vertex, NULL);
	glShaderSource(shader->fshaderId, 1, &files->fragment, NULL);
	if (shader->gshaderPath != NULL) {
		glShaderSource(shader->gshaderId, 1, &files->geometry, NULL);
	}

	glCompileShader(shader->vshaderId);
	glCompileShader(shader->fshaderId);
	if (shader->gshaderPath != NULL) {
		glCompileShader(shader->gshaderId);
	}

	glAttachShader(shader->programId, shader->vshaderId);
	glAttachShader(shader->programId, shader->fshaderId);
	if (shader->gshaderPath != NULL) {
		glAttachShader(shader->programId, shader->gshaderId);
	}

	glLinkProgram(shader->programId);
	glValidateProgram(shader->programId);

	glDeleteShader(shader->vshaderId);
	glDeleteShader(shader->fshaderId);
	if (shader->gshaderPath != NULL) {
		glDeleteShader(shader->gshaderId);
	}
}

unsigned int ksaCreateShader(ksaShaderFiles *files)
{
	return 0;
}

void ksaShaderDestroy(ksaShader *shader, ksaShaderFiles *files)
{
	free(shader);
	free(files);
}

void ksaShaderUse(ksaShader* shader)
{
	glUseProgram(shader->programId);
}


