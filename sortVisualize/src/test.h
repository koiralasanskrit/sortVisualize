#pragma once
#include <stdio.h>
#include "ksacgl/ksaVertexBuffer.h"
#include "ksacgl/ksaVertexArray.h"
#include "ksacgl/ksaShader.h"
#include "vendor/nuklear/nuklear.h"

extern ksaVBuffer test_buffer;
extern ksaVBufferLayout test_layout;
extern ksaVArray test_array;
extern ksaShader test_shader;
extern ksaShaderFiles test_files;
extern float test_arr[];
extern unsigned int test_indics[];

void test_onGUI(struct nk_context* ctx, struct nk_colorf bg);

void test_init();

void test_onDraw();
