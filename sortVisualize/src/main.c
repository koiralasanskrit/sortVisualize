#include <string.h>
#include <stdio.h>
#include "ksacgl/ksaIndexBuffer.h"
#include "ksacgl/ksaVertexBuffer.h"
#include "ksacgl/ksaVertexArray.h"
#include "ksacgl/ksaShader.h"
#include <cglm.h>

#include "sortVis/sortview.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "main.h"
#include "test.h"
#include "vendor/nuklear/nuklear_glfw_gl3.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define INCLUDE_OVERVIEW

struct nk_glfw maini_glfw = { 0 };
GLFWwindow* main_window;
int window_width;
int window_height;
struct nk_context* main_ctx;
struct nk_colorf bg;
double previous_time;
struct nk_font *big_font;
struct nk_font *droid;
char name_of_sort[50];

void GLAPIENTRY debugCall(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    printf("\nOpenGl Error: \n");
    printf("%s (%s)", message, __FILE__);
    __debugbreak();
}

int main(void)
{
    /* Initialize the library */
        main_init();
        /* Test */
        glfwSetWindowAspectRatio(main_window, 16, 9);

		glfwGetWindowSize(main_window, &window_width, &window_height);
        glfwSwapInterval(1);
        sortview_init(window_width, window_height);

        main_ctx = nk_glfw3_init(&maini_glfw, main_window, NK_GLFW3_INSTALL_CALLBACKS);

        struct nk_font_atlas* atlas;
        nk_glfw3_font_stash_begin(&maini_glfw, &atlas);
	    droid = nk_font_atlas_add_from_file(atlas, "res/fonts/sans.ttf", 20, 0);
		big_font = nk_font_atlas_add_from_file(atlas, "res/fonts/sans.ttf", 50, 0);
        nk_glfw3_font_stash_end(&maini_glfw);

        nk_style_set_font(main_ctx, &droid->handle);
        bg.r = 0.10f; bg.g = 0.10f; bg.b = 0.10f; bg.a = 1.0f;

        previous_time = glfwGetTime();
        while (!glfwWindowShouldClose(main_window))
        {
            glfwGetWindowSize(main_window, &window_width, &window_height);
            glViewport(0, 0, window_width, window_height);
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

            main_draw(main_ctx);
            glfwSwapBuffers(main_window);

            glfwPollEvents();
        }

        nk_glfw3_shutdown(&maini_glfw);
        glfwTerminate();
        return 0;
}

void main_draw()
{
    if (glfwGetTime() - previous_time > 0.5f * (1/sortview_animation_speed)) {
        sortview_onUpdate(0.0f);
        previous_time = glfwGetTime();
    }
	sortview_onRender(0.03, main_window, window_height, window_width);
	nk_glfw3_new_frame(&maini_glfw);
	if (nk_begin(main_ctx, "Debug", nk_rect(0.8*window_width, 0, 0.2 * window_width, window_height),
		NK_WINDOW_BORDER | NK_WINDOW_TITLE))
	{
        nk_style_set_font(main_ctx, &droid->handle);
        sortview_onGui(main_ctx);
        nk_style_set_font(main_ctx, &big_font->handle);
        nk_label(main_ctx, " ", NK_TEXT_CENTERED);
        nk_label(main_ctx, " ", NK_TEXT_CENTERED);
        nk_label(main_ctx, name_of_sort, NK_TEXT_CENTERED);
        nk_style_set_font(main_ctx, &droid->handle);
	}
	nk_end(main_ctx);

	nk_glfw3_render(&maini_glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void main_init()
{
    if (!glfwInit())
        return ;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    main_window = glfwCreateWindow(960, 540, "Sort Visualization", NULL, NULL);
    if (!main_window)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(main_window);

    if (glewInit() != GLEW_OK)
        printf("Error:\n");

    glDebugMessageCallback(debugCall, NULL);

}