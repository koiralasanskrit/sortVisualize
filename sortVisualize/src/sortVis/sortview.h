#pragma once
#include <GLFW/glfw3.h>
/* Vertices, Quads and Color Structs */
struct sortview_position { float x; float y; float z; };
struct sortview_color { float r; float g; float b; };
struct sortview_vertex { struct sortview_position position; struct sortview_color color; };
struct sortview_quad { unsigned int indices[6]; };

typedef struct sortview_color sortview_color;
typedef struct sortview_position sortview_position;
typedef struct sortview_vertex sortview_vertex;
typedef struct sortview_quad sortview_quad;

/* Matrices and Projections */
extern mat4 sortview_projection_matrix;
extern mat4 sortview_swap_matrix;

/* Color Stuff */
extern sortview_color sortview_selection_first;
extern sortview_color sortview_selection_second;
extern sortview_color sortview_other_color;

/* ksacGL stuff */
extern ksaVBuffer sortview_swapvbo;
extern ksaVBufferLayout sortview_swapvlayout;
extern ksaVArray sortview_swapvarray;
extern ksaIBuffer sortview_swapibo;
extern ksaShader sortview_swapshader;

extern ksaVBuffer sortview_mainvbo;
extern ksaVBufferLayout sortview_mainvlayout;
extern ksaVArray sortview_mainvarray;
extern ksaIBuffer sortview_mainibo;
extern ksaShader sortview_mainshader;

/* Animation Stuff */
extern unsigned int current_frame;
extern sortview_vertex vertices[500 * 1024];
extern sortview_quad indices[500 * 1024];
extern sortview_vertex swap_vertices[8];
extern sortview_quad swap_indices[2];

void sortview_init(float window_width, float window_height);

void sortview_onUpdate(float deltaTime);

void sortview_onRender(float deltaTime, GLFWwindow * main_window, float window_Height, float window_Width);

void sortview_onGui(struct nk_context* ctx);

void sortview_makeRect(float x, float y, float width, float height, int n, sortview_color _color);

void sortview_makeSwapRect(float x, float y, float width, float height, int n, sortview_color _color);

void sortview_makeSwapIndices();

void sortview_createRectBatch();

void sortview_clearRectBatch();
