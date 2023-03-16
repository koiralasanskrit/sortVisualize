#pragma once
#include <GLFW/glfw3.h>

extern double previous_time;
extern struct nk_font *big_font;
extern char name_of_sort[50];
void main_draw();

void main_init();
