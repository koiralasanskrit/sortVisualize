#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../ksacgl/ksaVertexBuffer.h"
#include "../ksacgl/ksaVertexArray.h"
#include "../ksacgl/ksaShader.h"
#include "../vendor/nuklear/nuklear.h"

extern ksaVBuffer draw_buffer;
extern ksaVBufferLayout draw_layout;
extern ksaVArray draw_array;
extern ksaShader draw_shader;
extern ksaShaderFiles test_files;
extern int draw_continuous;
extern int draw_window_width;
extern int draw_window_height;

void draw_onGUI(struct nk_context* ctx);

void draw_init(GLFWwindow *window);

void draw_onDraw(GLFWwindow *window);

void draw_poll_events(GLFWwindow* main_window);
