#include "../ksacgl/vBatching/vBatching.h"
#include "../ksacgl/ksaIndexBuffer.h"
#include "../ksacgl/ksaFrameBuffer.h"
#include "../ksacgl/ksaRenderBuffer.h"
#include "draw.h"
#include "../vendor/nuklear/nuklear.h"
#include <stdlib.h>
#include <string.h>
#include <cglm.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stbi/stbi_image_write.h"

#define EASYTAB_IMPLEMENTATION
#include "../vendor/apoorvaJ/easy_tab.h"


/* NON STATICS */
ksaVBuffer draw_buffer;
ksaIBuffer index_buffer;
ksaFBuffer draw_frame_buffer;
ksaRBuffer draw_render_buffer;
ksaVBufferLayout draw_layout;
ksaVArray draw_array;
ksaShader draw_shader;
ksaShaderFiles test_files;
struct nk_colorf draw_textcolor;
unsigned int draw_geometryShader;
unsigned int draw_textureColorbuffer;

ksaVertex draw_vertices[1024 * 640];
ksaLine draw_lines[2048 * 480];

int draw_window_width = 0;
int draw_window_height = 0;

/*STATICS*/
static unsigned int draw_vertices_index = 0;
static unsigned int draw_lines_index = 0;
static float draw_tol = 0.01f;
static float draw_thickness_uniform = 0.005f;
static float draw_pressure = 0.001f;
static int draw_continuous = 1;
static int draw_mode = 1;
static char draw_file_name[50];

/*
DEBUG STUFFS
*/
float draw_debug__nx; // x
float draw_debug__ny; // y

static void
draw_vertices_optimize()
{
    /* to be made */
}

static void
draw_batch_vertices(GLFWwindow *window)
{
    int _winWidth;
    int _winHeight;
    double _x;
    double _y;
    if (!draw_continuous) {
        if (draw_lines_index > 1) {
            draw_lines[--draw_lines_index].indices[0] = NULL;
            draw_lines[draw_lines_index++].indices[1] = NULL;
        }
        draw_lines[draw_lines_index].indices[0] = NULL;
        draw_lines[draw_lines_index++].indices[1] = NULL;
        draw_vertices_index++;
        draw_vertices_index++;
    }
    draw_lines_index++;
	draw_lines[draw_lines_index].indices[0] = draw_vertices_index;
    glfwGetWindowSize(window, &_winWidth, &_winHeight);
    glfwGetCursorPos(window, &_x, &_y);
    draw_vertices[draw_vertices_index].pos[0] = (float) (_x / _winWidth) * 2 - 1; // x
    draw_vertices[draw_vertices_index].pos[1] = (float) - ((_y / _winHeight) * 2 - 1); // y
    /* debug */
	draw_debug__nx = draw_vertices[draw_vertices_index].pos[0]; // x
	draw_debug__ny = draw_vertices[draw_vertices_index].pos[0]; // x


    draw_vertices[draw_vertices_index].pos[2] = 0.0; // z
    draw_vertices[draw_vertices_index].sortview_color[0] = draw_textcolor.r; // x
    draw_vertices[draw_vertices_index].sortview_color[1] = draw_textcolor.g; // y
    draw_vertices[draw_vertices_index].sortview_color[2] = draw_textcolor.b; // z
    draw_vertices[draw_vertices_index].thickness = draw_thickness_uniform * draw_pressure;
    draw_vertices_index++;
	draw_lines[draw_lines_index].indices[1] = draw_vertices_index;
}

static void 
draw_erase_vertices(GLFWwindow* window)
{
    int _winWidth;
    int _winHeight;
    double _x;
    double _y;
    glfwGetWindowSize(window, &_winWidth, &_winHeight);
    glfwGetCursorPos(window, &_x, &_y);
    for (int i = 0; i < draw_vertices_index; i++) 
    {
        /* DRAW TOLERANCE */
		float _nx = (float) (_x / _winWidth) * 2 - 1; // x
		float _ny = (float) - ((_y / _winHeight) * 2 - 1); // y
		draw_debug__nx = _nx; // x
		draw_debug__ny = _ny; // x
        if ((draw_vertices[i].pos[0] > (_nx - draw_tol) && draw_vertices[i].pos[0] < (_nx + draw_tol))
            && ((draw_vertices[i].pos[1] > (_ny - draw_tol)) && (draw_vertices[i].pos[1] < (_ny + draw_tol))))
        {
                draw_lines[i].indices[0] = 0;
                draw_lines[i].indices[1] = 0;
        }
    }
}

static void
draw_save_imageFile()
{
    strcpy(draw_file_name, "new.png");
    stbi_flip_vertically_on_write(1);
    char *data;
    data = (char*)malloc(sizeof(char) * 3 * draw_window_width * 0.8 * draw_window_height);
    memset(data, 0, 3 * draw_window_width * 0.8 * draw_window_height);
    ksaFBufferBind(&draw_frame_buffer);
    glReadPixels(draw_window_width * 0.2, 0, draw_window_width * 0.8, draw_window_height, GL_RGB, GL_UNSIGNED_BYTE, data);

    if (stbi_write_png(draw_file_name, draw_window_width * 0.8, draw_window_height, 3, data, 0.8 * draw_window_width * 3))
    {
        printf("\nFile Saved\n");
    }
    free(data);
}

void
draw_onGUI(struct nk_context* ctx) 
{
	nk_layout_row_static(ctx, 30, 80, 1);
    if (nk_button_label(ctx, "Save"))
    {
        draw_save_imageFile("Hi");
    }

	nk_layout_row_dynamic(ctx, 30, 2);
	if (nk_option_label(ctx, "draw", draw_mode == 1)) draw_mode = 1;
	if (nk_option_label(ctx, "erase", draw_mode == 0)) draw_mode = 0;

	/* comment this  nk_layout_row_dynamic(ctx, 25, 1); */
	/* comment this  nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1); */

	nk_layout_row_dynamic(ctx, 20, 1);
	nk_label(ctx, "Stroke Color:", NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 25, 1);
	if (nk_combo_begin_color(ctx, nk_rgb_cf(draw_textcolor), nk_vec2(nk_widget_width(ctx), 400))) {
		nk_layout_row_dynamic(ctx, 120, 1);
		draw_textcolor = nk_color_picker(ctx, draw_textcolor, NK_RGBA);
		nk_layout_row_dynamic(ctx, 25, 1);
		draw_textcolor.r = nk_propertyf(ctx, "#R:", 0, draw_textcolor.r, 1.0f, 0.01f, 0.005f);
		draw_textcolor.g = nk_propertyf(ctx, "#G:", 0, draw_textcolor.g, 1.0f, 0.01f, 0.005f);
		draw_textcolor.b = nk_propertyf(ctx, "#B:", 0, draw_textcolor.b, 1.0f, 0.01f, 0.005f);
		draw_textcolor.a = nk_propertyf(ctx, "#A:", 0, draw_textcolor.a, 1.0f, 0.01f, 0.005f);
		nk_combo_end(ctx);
	}

    nk_label(ctx, "ERASER RADIUS", NK_TEXT_LEFT);
    nk_slider_float(ctx, 0.00, &draw_tol, 1, 0.01f);
    nk_label(ctx, "BRUSH THICKNESS", NK_TEXT_LEFT);
    nk_slider_float(ctx, 0.0001f, &draw_thickness_uniform, 0.01, 0.00001f);
}

void
draw_init(GLFWwindow* main_window) 
{
    glfwGetWindowSize(main_window, &draw_window_width, &draw_window_height);
    draw_textcolor.r = 1.0f;
    draw_textcolor.g = 1.0f;
    draw_textcolor.b = 1.0f;
    draw_textcolor.a = 1.0f;
    ksaVBufferInit(&draw_buffer, NULL, sizeof(draw_vertices), GL_STATIC_DRAW);
    ksaFBufferInit(&draw_frame_buffer, NULL, NULL, NULL);
    ksaRBufferInit(&draw_render_buffer, draw_window_width * 0.8, draw_window_height);
    draw_layout.index = 0;
    draw_layout.stride = 0;
    ksaVBufferLayoutPush(&draw_layout, 3);
    ksaVBufferLayoutPush(&draw_layout, 3);
    ksaVBufferLayoutPush(&draw_layout, 1);
    ksaVArrayInit(&draw_array);
    ksaVArrayAddBuffer(&draw_array, &draw_buffer, &draw_layout);
    draw_shader = (ksaShader) {
        .fshaderPath = "res/shaders/fragmentShader.glsl",
        .vshaderPath = "res/shaders/vertexShader.glsl",
        .gshaderPath = "res/shaders/geometryShader.glsl"
    };
    ksaVArrayBind(&draw_array);
    ksaIBufferInit(&index_buffer,  NULL, sizeof(draw_lines), GL_STATIC_DRAW);

    ksaShaderGet(&draw_shader, &test_files);
    ksaShaderInit(&draw_shader, &test_files);


    glEnable(GL_MULTISAMPLE);

    /* RENDER TO TEXTURE */
    ksaFBufferBind(&draw_frame_buffer);

    glGenTextures(1, &draw_textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, draw_textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0.8 * draw_window_width, draw_window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, draw_textureColorbuffer, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, draw_render_buffer.rendererId);
    ksaFBufferUnBind(&draw_frame_buffer);
}

void
draw_onDraw(GLFWwindow *window)
{
    mat4 orthoProj;
    glm_ortho(0, draw_window_width, 0, draw_window_height, 0, 1, orthoProj);
    glfwGetWindowSize(window, &draw_window_width, &draw_window_height);

	ksaVArrayBind(&draw_array);
    ksaVBufferBind(&draw_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, draw_vertices_index *  sizeof(ksaVertex), draw_vertices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, draw_lines_index * sizeof(ksaLine), draw_lines);

    ksaFBufferBind(&draw_frame_buffer);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT); // we're not using the stencil buffer now
	ksaShaderUse(&draw_shader);
    glUniformMatrix4fv(glGetUniformLocation(draw_shader.programId, "u_MVP"), sizeof(orthoProj), GL_FALSE, orthoProj);
    ksaVArrayBind(&draw_array);
    glDrawElements(GL_LINES, draw_lines_index * sizeof(ksaLine), GL_UNSIGNED_INT, NULL);

    // second pass
    ksaFBufferUnBind(&draw_frame_buffer);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT);

    ksaVArrayBind(&draw_array);
	ksaShaderUse(&draw_shader);
    glDrawElements(GL_LINES, draw_lines_index * sizeof(ksaLine), GL_UNSIGNED_INT, NULL);
}

void
draw_poll_events(GLFWwindow* main_window) {
	if (glfwGetMouseButton(main_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (draw_mode) 
        {
            draw_batch_vertices(main_window);
        }
        else 
        {
            draw_erase_vertices(main_window);
        }
		draw_continuous = 1;
	}
    if (glfwGetMouseButton(main_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        draw_continuous = 0;
        if (draw_lines_index > 0) {
            draw_lines[--draw_lines_index].indices[0] = 0;
            draw_lines[draw_lines_index++].indices[1] = 0;
        }
    }
}

void 
draw_onDestroy()
{
    ksaVBufferDestroy(&draw_buffer);
    ksaIBufferDestroy(&index_buffer);
    ksaShaderDestroy(&draw_shader, &test_files);
}

