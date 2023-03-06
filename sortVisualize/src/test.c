#include "test.h"

ksaVBuffer test_buffer;
ksaVBufferLayout test_layout;
ksaVArray test_array;
ksaShader test_shader;
ksaShaderFiles test_files;

float test_arr[] = {
	0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,// top right
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
	0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f, // bottom right
	-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f // top left
};

unsigned int test_indics[] = {
	0, 1, 1, 2, 2, 3
};

void test_onGUI(struct nk_context* ctx, struct nk_colorf bg) 
{
	enum { EASY, HARD };
	static int op = EASY;
	static int property = 20;
	nk_layout_row_static(ctx, 30, 80, 1);
	if (nk_button_label(ctx, "button"))
		fprintf(stdout, "button pressed\n");

	nk_layout_row_dynamic(ctx, 30, 2);
	if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
	if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

	nk_layout_row_dynamic(ctx, 20, 1);
	nk_label(ctx, "background:", NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 25, 1);
	if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
		nk_layout_row_dynamic(ctx, 120, 1);
		bg = nk_color_picker(ctx, bg, NK_RGBA);
		nk_layout_row_dynamic(ctx, 25, 1);
		bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
		bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
		bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
		bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
		nk_combo_end(ctx);
	}
}

void test_init() 
{

    ksaVBufferInit(&test_buffer, test_arr, sizeof(test_arr), GL_STATIC_DRAW);
    test_layout.index = 0;
    test_layout.stride = 0;
    ksaVBufferLayoutPush(&test_layout, 3);
    ksaVBufferLayoutPush(&test_layout, 3);
    ksaVArrayInit(&test_array);
    ksaVArrayAddBuffer(&test_array, &test_buffer, &test_layout);
    test_shader = (ksaShader) {
		.fshaderPath = "res/shaders/fragmentShader.glsl",
        .vshaderPath = "res/shaders/vertexShader.glsl",
		.gshaderPath = NULL
	};

    ksaShaderGet(&test_shader, &test_files);
    ksaShaderInit(&test_shader, &test_files);
}

void test_onDraw()
{
	ksaShaderUse(&test_shader);
	ksaVArrayBind(&test_array);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, test_indics);
}