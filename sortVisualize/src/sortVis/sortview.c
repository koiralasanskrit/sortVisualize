#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm.h>
#include <string.h>
#include "../ksacgl/ksaIndexBuffer.h"
#include "../ksacgl/ksaVertexArray.h"
#include "../ksacgl/ksaShader.h"
#include "sortbase.h"
#include "sortview.h"
#include "../vendor/nuklear/nuklear.h"
#include "../main.h"
#include "soundsynth.h"
/*DEBUG*/
char sortview_debug[100][100];
int sortview_debug_count = 0;

/* Matrices and Projections */
mat4 sortview_projection_matrix;
mat4 sortview_swap_matrix;

/* Color Stuff */
sortview_color sortview_selection_first;
sortview_color sortview_selection_second;
sortview_color sortview_other_color;

/* ksacGL stuff */
ksaVBuffer sortview_swapvbo;
ksaVBufferLayout sortview_swapvlayout;
ksaVArray sortview_swapvarray;
ksaIBuffer sortview_swapibo;
ksaShader sortview_swapshader;

ksaVBuffer sortview_mainvbo;
ksaVBufferLayout sortview_mainvlayout;
ksaVArray sortview_mainvarray;
ksaIBuffer sortview_mainibo;
ksaShader sortview_mainshader;

/* Animation Stuff */
static unsigned int current_frame = 0;
sortview_vertex vertices[500*1024];
sortview_quad indices[500*1024];
sortview_vertex swap_vertices[8];
sortview_quad swap_indices[2];

float sortview_window_height = 0;
float sortview_window_width = 0;
int sortview_no_of_items = 0;
float sortview_animation_speed = 0;

void
sortview_init(float window_width, float window_height)
{
	soundsynth_audio_init();
	sortview_selection_first = (sortview_color) {.r = 0.0f, 1.0f, 1.0f};
	sortview_selection_second = (sortview_color) {.r = 1.0f, 0.0f, 0.0f};
	sortview_other_color = (sortview_color) {.r = 1.0f, 1.0f, 1.0f};
	sortbase_randomize();
	sortbase_bubblesort(sortbase_frame_stuff.array, sortbase_frame_stuff.array_size);
	glm_ortho(0.0f, window_width, 0, window_height, 1, 2, sortview_projection_matrix);

	glm_mat4_identity(sortview_swap_matrix);
	sortview_createRectBatch();

	//swap
	ksaVBufferInit(&sortview_swapvbo, NULL, 8 * sizeof(sortview_vertex), GL_DYNAMIC_DRAW);
	sortview_swapvlayout.index = 0;
	sortview_swapvlayout.stride = 0;
	ksaVBufferLayoutPush(&sortview_swapvlayout, 3);
	ksaVBufferLayoutPush(&sortview_swapvlayout, 3);
	ksaVArrayInit(&sortview_swapvarray);
	ksaVArrayBind(&sortview_swapvarray);
	ksaVArrayAddBuffer(&sortview_swapvarray, &sortview_swapvbo, &sortview_swapvlayout);
	ksaIBufferInit(&sortview_swapibo, NULL, 2 * sizeof(sortview_quad), GL_DYNAMIC_DRAW);
	ksaShaderFiles sortview_swapshader_files;
	sortview_swapshader = (ksaShader) { 
		.fshaderPath = "res/shaders/sortvis/swap_fragment.glsl", 
		.vshaderPath = "res/shaders/sortvis/swap_vertex.glsl",
		.gshaderPath = NULL,
	};
	ksaShaderInit(&sortview_swapshader, &sortview_swapshader_files);
	ksaShaderUse(&sortview_swapshader);

	// main
	ksaVBufferInit(&sortview_mainvbo, NULL, 500 * 1024 * sizeof(sortview_vertex), GL_DYNAMIC_DRAW);
	sortview_mainvlayout.index = 0;
	sortview_mainvlayout.stride = 0;
	ksaVBufferLayoutPush(&sortview_mainvlayout, 3);
	ksaVBufferLayoutPush(&sortview_mainvlayout, 3);
	ksaVArrayInit(&sortview_mainvarray);
	ksaVArrayAddBuffer(&sortview_mainvarray, &sortview_mainvbo, &sortview_mainvlayout);
	ksaVArrayBind(&sortview_mainvarray);
	ksaIBufferInit(&sortview_mainibo, NULL, 500 * 1024 * sizeof(sortview_quad), GL_DYNAMIC_DRAW);
	ksaShaderFiles sortview_mainshader_files;
	sortview_mainshader = (ksaShader) { 
		.fshaderPath = "res/shaders/sortvis/main_fragment.glsl", 
		.vshaderPath = "res/shaders/sortvis/main_vertex.glsl",
		.gshaderPath = NULL,
	};
	ksaShaderInit(&sortview_mainshader, &sortview_mainshader_files);
	ksaShaderUse(&sortview_mainshader);
}

void
sortview_onUpdate(float deltaTime)
{
	if ((current_frame < sortbase_frame_stuff.frame_index)) {
		for(int i = 0; i < 1000000; i++)
			soundsynth_pause();
		memcpy(sortbase_frame_stuff.array,
			sortbase_frame_stuff.array_of_arrays[current_frame],
			sizeof(int) * sortbase_frame_stuff.array_size);
		int played = sortbase_frame_stuff.array_of_selection[current_frame].second;
		int another_played = sortbase_frame_stuff.array[played] + sortbase_frame_stuff.array_of_selection[current_frame].first;
		soundsynth_playsvar(another_played, another_played);
		soundsynth_play();
		current_frame++;
		previous_time = glfwGetTime();
	}
	else {
		soundsynth_pause();
		current_frame = 0;
	}
}

void
sortview_onRender(float deltaTime, GLFWwindow *main_window, float window_Height, float window_Width)
{
	sortview_window_height = window_Height;
	sortview_window_width = window_Width;
	glm_ortho(0.0f, sortview_window_width, 0, sortview_window_height, 1, 2, sortview_projection_matrix);
	sortview_createRectBatch();
	// main
	ksaVArrayBind(&sortview_mainvarray);
	ksaIBufferBind(&sortview_mainibo);
	ksaVBufferBind(&sortview_mainvbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4* sortbase_frame_stuff.array_size * sizeof(sortview_vertex), vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sortbase_frame_stuff.array_size * sizeof(sortview_quad), &indices);
	ksaShaderUse(&sortview_mainshader);
	glUniformMatrix4fv(glGetUniformLocation(sortview_mainshader.programId, "u_MVP"), 1, GL_FALSE, sortview_projection_matrix[0]);
	ksaShaderUse(&sortview_mainshader);
	ksaVArrayBind(&sortview_mainvarray);
	glDrawElements(GL_TRIANGLES, 6 * sortbase_frame_stuff.array_size, GL_UNSIGNED_INT, NULL);
	/* HERE AS WELL */

	// swap;
	ksaVArrayBind(&sortview_swapvarray);
	ksaIBufferBind(&sortview_swapibo);
	ksaVBufferBind(&sortview_swapvbo);
	glBufferSubData(GL_ARRAY_BUFFER,
		0, 8 * sizeof(sortview_vertex),
		swap_vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 
		2 * sizeof(sortview_quad),
		&swap_indices);
	ksaShaderUse(&sortview_swapshader);
	glUniformMatrix4fv(glGetUniformLocation(sortview_swapshader.programId, "u_MVP"), 1, GL_FALSE, sortview_projection_matrix[0]);
	glUniform1f(glGetUniformLocation(sortview_swapshader.programId, "u_Time"), 1, sin(deltaTime * 16 * glfwGetTime()));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	/* THING TO BE LOOKED */
	sortview_clearRectBatch();
}

void
sortview_onGui(struct nk_context *ctx)
{
	sprintf(sortview_debug[sortview_debug_count++], "W:%f", sortview_window_width);
	sprintf(sortview_debug[sortview_debug_count++], "H:%f", sortview_window_height);
	nk_layout_row_static(ctx, 20, sortview_window_width * 0.2 * 0.9, 1);
	for (int i = 0; i < sortview_debug_count; i++)
	{
		nk_label(ctx, sortview_debug[i], NK_TEXT_LEFT);
	}
	sortview_debug_count = 0;
	nk_label(ctx, "No of Items:", NK_TEXT_LEFT);
	nk_slider_int(ctx, 10, &sortbase_no_of_items, 300, 1);
	nk_label(ctx, "Speed:", NK_TEXT_LEFT);
	nk_slider_float(ctx, 0.1, &sortview_animation_speed, 100, 0.1);
	nk_label(ctx, "", NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 20, 1);

	/* Colors */
	nk_label(ctx, "Color All:", NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 25, 1);
	if (nk_combo_begin_color(ctx, nk_rgb_cf(*((struct nk_colorf*) &sortview_other_color)), nk_vec2(nk_widget_width(ctx), 400))) {
		nk_layout_row_dynamic(ctx, 120, 1);
		*((struct nk_colorf*) &sortview_other_color) = nk_color_picker(ctx, *((struct nk_colorf*) &sortview_other_color), NK_RGBA);
		nk_layout_row_dynamic(ctx, 25, 1);
		sortview_other_color.r = nk_propertyf(ctx, "#R:", 0, sortview_other_color.r, 1.0f, 0.01f, 0.005f);
		sortview_other_color.g = nk_propertyf(ctx, "#G:", 0, sortview_other_color.g, 1.0f, 0.01f, 0.005f);
		sortview_other_color.b = nk_propertyf(ctx, "#B:", 0, sortview_other_color.b, 1.0f, 0.01f, 0.005f);
		nk_combo_end(ctx);
	}

	nk_label(ctx, "Color First:", NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 25, 1);
	if (nk_combo_begin_color(ctx, nk_rgb_cf(*((struct nk_colorf*) &sortview_selection_first)), nk_vec2(nk_widget_width(ctx), 400))) {
		nk_layout_row_dynamic(ctx, 120, 1);
		*((struct nk_colorf*) &sortview_selection_first) = nk_color_picker(ctx, *((struct nk_colorf*) &sortview_selection_first), NK_RGBA);
		nk_layout_row_dynamic(ctx, 25, 1);
		sortview_selection_first.r = nk_propertyf(ctx, "#R:", 0, sortview_selection_first.r, 1.0f, 0.01f, 0.005f);
		sortview_selection_first.g = nk_propertyf(ctx, "#G:", 0, sortview_selection_first.g, 1.0f, 0.01f, 0.005f);
		sortview_selection_first.b = nk_propertyf(ctx, "#B:", 0, sortview_selection_first.b, 1.0f, 0.01f, 0.005f);
		nk_combo_end(ctx);
	}


	nk_label(ctx, "Color Second:", NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 25, 1);
	if (nk_combo_begin_color(ctx, nk_rgb_cf(*((struct nk_colorf*) &sortview_selection_second)), nk_vec2(nk_widget_width(ctx), 400))) {
		nk_layout_row_dynamic(ctx, 120, 1);
		*((struct nk_colorf*) &sortview_selection_second) = nk_color_picker(ctx, *((struct nk_colorf*) &sortview_selection_second), NK_RGBA);
		nk_layout_row_dynamic(ctx, 25, 1);
		sortview_selection_second.r = nk_propertyf(ctx, "#R:", 0, sortview_selection_second.r, 1.0f, 0.01f, 0.005f);
		sortview_selection_second.g = nk_propertyf(ctx, "#G:", 0, sortview_selection_second.g, 1.0f, 0.01f, 0.005f);
		sortview_selection_second.b = nk_propertyf(ctx, "#B:", 0, sortview_selection_second.b, 1.0f, 0.01f, 0.005f);
		nk_combo_end(ctx);
	}
	
	if (nk_button_label(ctx, "Bubble Sort"))
	{
		sortbase_destroy();
		sortbase_randomize();
		sortbase_bubblesort(sortbase_frame_stuff.array, sortbase_frame_stuff.array_size);
		current_frame = 0;
	}

	if (nk_button_label(ctx, "Selection Sort"))
	{
		sortbase_destroy();
		sortbase_randomize();
		sortbase_selectionsort(sortbase_frame_stuff.array, sortbase_frame_stuff.array_size);
		current_frame = 0;
	}

	if (nk_button_label(ctx, "Insertion Sort"))
	{
		sortbase_destroy();
		sortbase_randomize();
		sortbase_insertionsort(sortbase_frame_stuff.array, sortbase_frame_stuff.array_size);
		current_frame = 0;
	}

	if (nk_button_label(ctx, "Heap Sort"))
	{
		sortbase_destroy();
		sortbase_randomize();
		sortbase_heapsort(sortbase_frame_stuff.array, sortbase_frame_stuff.array_size);
		current_frame = 0;
	}

	if (nk_button_label(ctx, "Merge Sort"))
	{
		sortbase_destroy();
		sortbase_randomize();
		sortbase_mergesort(sortbase_frame_stuff.array, 0, sortbase_frame_stuff.array_size - 1, sortbase_frame_stuff.array_size);
		current_frame = 0;
	}

	if (nk_button_label(ctx, "Quick Sort"))
	{
		sortbase_destroy();
		sortbase_randomize();
		sortbase_quicksort(sortbase_frame_stuff.array, 0, sortbase_frame_stuff.array_size - 1, sortbase_frame_stuff.array_size);
		current_frame = 0;
	}

	if (nk_button_label(ctx, "Radix Sort"))
	{
		sortbase_destroy();
		sortbase_randomize();
		sortbase_radixsort(sortbase_frame_stuff.array, sortbase_frame_stuff.array_size);
		current_frame = 0;
	}
}

void
sortview_makeRect(float x, float y, float width, float height, int n, sortview_color _color) {
	vertices[n].position = (sortview_position)  { x, y, 10.0f };
	vertices[n].color = _color;
	n++;
	vertices[n].position = (sortview_position) { x + width, y, 10.0f };
	vertices[n].color = _color;
	n++;
	vertices[n].position = (sortview_position) { x + width, y + height, 10.0f };
	vertices[n].color = _color;
	n++;
	vertices[n].position = (sortview_position) { x, y + height, 10.0f };
	vertices[n].color = _color;
}

void
sortview_makeSwapRect(float x, float y, float width, float height, int n, sortview_color _color) {
	swap_vertices[n].position = (sortview_position) { x, y, 10.0f };
	swap_vertices[n].color = _color;
	n++;
	swap_vertices[n].position = (sortview_position) { x + width, y, 10.0f };
	swap_vertices[n].color = _color;
	n++;
	swap_vertices[n].position = (sortview_position) { x + width, y + height, 10.0f };
	swap_vertices[n].color = _color;
	n++;
	swap_vertices[n].position = (sortview_position) { x, y + height, 10.0f };
	swap_vertices[n].color = _color;
}

void
sortview_makeSwapIndices()
{
	swap_indices[0].indices[0] = 0 ;
	swap_indices[0].indices[1] = 1 ;
	swap_indices[0].indices[2] = 2 ;
	swap_indices[0].indices[3] = 0 ;
	swap_indices[0].indices[4] = 2 ;
	swap_indices[0].indices[5] = 3 ;
	swap_indices[1].indices[0] = 0 + 4;
	swap_indices[1].indices[1] = 1 + 4;
	swap_indices[1].indices[2] = 2 + 4;
	swap_indices[1].indices[3] = 0 + 4;
	swap_indices[1].indices[4] = 2 + 4;
	swap_indices[1].indices[5] = 3 + 4;
}

void
sortview_createRectBatch()
{
	sortview_makeSwapIndices();
	float spacingBarwidth = (sortview_window_width * 0.8f) / sortbase_frame_stuff.array_size;
	sprintf(sortview_debug[sortview_debug_count++], "widt:%f", sortview_window_width * 0.8);
	sprintf(sortview_debug[sortview_debug_count++], "space:%f", spacingBarwidth);
	sprintf(sortview_debug[sortview_debug_count++], "x:%f", (sortview_window_width * 0.8)/ spacingBarwidth);
	float height = sortview_window_height;
	float barwidth = 0.9 * spacingBarwidth;
	float spacing = 0.1 * spacingBarwidth;
	float i = spacing;
	float barheight;
	sortview_color sortview_color = sortview_other_color;

	int m = 0;
	for (int n = 0; n < sortbase_frame_stuff.array_size; n++)
	{
		float x = i + 0.0f;
		float y = 0.0f;
		barheight = sortbase_frame_stuff.array[n] * height / 10;

		if (current_frame < sortbase_frame_stuff.frame_array_size) 
		{
			if (n == sortbase_frame_stuff.array_of_selection[current_frame].first)
			{
				sortview_color = sortview_selection_first;
				sortview_makeSwapRect(x, y, barwidth, barheight, 0, sortview_color);
			}
			else if (n == sortbase_frame_stuff.array_of_selection[current_frame].second)
			{
				sortview_color = sortview_selection_second;
				sortview_makeSwapRect(x, y, barwidth, barheight, 4, sortview_color);
			}
			else
			{
				sortview_color = sortview_other_color;
			}
		}

		sortview_makeRect(x, y, barwidth, barheight, m, sortview_color);
		m += 4;


		indices[n].indices[0] = 0 + n * 4;
		indices[n].indices[1] = 1 + n * 4;
		indices[n].indices[2] = 2 + n * 4;
		indices[n].indices[3] = 0 + n * 4;
		indices[n].indices[4] = 2 + n * 4;
		indices[n].indices[5] = 3 + n * 4;

		i += spacingBarwidth;
	}
}

void
sortview_clearRectBatch()
{
	memset(vertices, 0, 1024 * sizeof(sortview_vertex));
	memset(indices, 0, 1024 * sizeof(sortview_quad));
}