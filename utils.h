#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <random>
#include "bahamut.h"

static inline
i32 random_int(i32 min, i32 max) {
	static std::random_device rd;
	static std::mt19937 mt(rd());

	std::uniform_int_distribution<> dist(0, max - min);
	return dist(mt) + min;
}

struct Animation {
	std::string identifier;
	Texture img;
	u16 frames;
	u16 current;
	u16 width;
	u16 height;
	u32 delay;
};

typedef std::vector<Animation> AnimationList;

static inline
Animation create_animation(std::string name, Texture sheet, u16 frames, u16 width, u16 height, u32 delay) {
	Animation anim;
	anim.current = 0;
	anim.identifier = name;
	anim.img = sheet;
	anim.frames = frames;
	anim.width = width;
	anim.height = height;
	anim.delay = delay;
	return anim;
}

static inline
void update_animation(Animation* anim, i32 timer) {
	if (timer % anim->delay == 0)
		anim->current++;
	if (anim->current >= anim->frames)
		anim->current = 0;
}

static inline
void draw_animation(RenderBatch* batch, Animation anim, f32 x, f32 y) {
	Rect src = { anim.current * anim.width, 0, anim.width, anim.height };
	Rect dst = { x, y, anim.width, anim.height };
	draw_texture_EX(batch, anim.img, src, dst);
}

static inline
Texture get_sub_image(unsigned char* pixels, int pixels_width, int x, int y, int width, int height, int texparam) {
	Texture subimage;
	glPixelStorei(GL_UNPACK_ROW_LENGTH, pixels_width);
	unsigned char* subimage_pixels = pixels + (x + y * pixels_width) * 4;
	subimage = load_texture(subimage_pixels, width, height, texparam);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	return subimage;
}

static inline
void load_ninepatch(const char* filename, Texture ninepatch[9]) {
	i32 w;
	i32 h;
	u16 i = 0;
	unsigned char* image = SOIL_load_image(filename, &w, &h, 0, SOIL_LOAD_RGBA);
	for (u8 x = 0; x < 3; ++x)
		for (u8 y = 0; y < 3; ++y)
			ninepatch[i++] = get_sub_image(image, w, x * (w / 3), y * (h / 3), w / 3, h / 3, GL_LINEAR);
	free(image);
}

struct Panel {
	void* active;
	void* hot;
};

static inline
void begin_gui(Panel* panel) {
	if (is_button_released(MOUSE_BUTTON_LEFT)) {
		panel->active = NULL;
		panel->hot = NULL;
	}
}

static inline
void end_gui(Panel* panel) {

}

static inline
void draw_panel(RenderBatch* batch, Texture ninepatch[9], i32 x, i32 y, u16 width, u16 height) {
	draw_texture(batch, ninepatch[0], x, y);
	draw_texture(batch, ninepatch[2], x, y + (height * ninepatch[0].height));
	draw_texture(batch, ninepatch[6], x + (width * ninepatch[0].width), y);
	draw_texture(batch, ninepatch[8], x + (width * ninepatch[0].width), y + (height * ninepatch[0].height));

	for (int i = 1; i < width; ++i) {
		draw_texture(batch, ninepatch[3], (i * ninepatch[0].width) + x, y);
		draw_texture(batch, ninepatch[5], (i * ninepatch[0].width) + x, y + (height * ninepatch[0].height));
	}
	for (int i = 1; i < height; ++i) {
		draw_texture(batch, ninepatch[1], x, (i * ninepatch[0].height) + y);
		draw_texture(batch, ninepatch[7], x + (width * ninepatch[0].width), (i * ninepatch[0].height) + y);
	}
	for (int i = 1; i < width; ++i) {
		for (int j = 1; j < height; ++j) {
			draw_texture(batch, ninepatch[4], (i * ninepatch[0].width) + x, (j * ninepatch[0].height) + y);
		}
	}
}

static inline
void tooltip(RenderBatch* batch, Font* font, Texture ninepatch[9], const char* text, Rect rect, vec2 mouse) {
	u32 numTokens = 0;
	char** tokens = split_string(text, "\n", &numTokens);

	if (colliding(rect, mouse.x, mouse.y)) {
		draw_panel(batch, ninepatch, rect.x + 60, rect.y, 5, 5);
		for (u32 i = 0; i < numTokens; ++i)
			draw_text(batch, font, tokens[i], rect.x + 70, rect.y + 10 + (20 * i), { 59, 34, 8 });
	}

	for (u8 i = 0; i < numTokens; ++i)
		if (tokens[i] != NULL)
			free(tokens[i]);
	if (tokens != NULL)
		free(tokens);
}

static inline
bool button(RenderBatch* batch, Texture btn, Texture btnDown, i32 x, i32 y, vec2 mouse) {
	Rect button = { x, y, btn.width, btn.height };
	bool collided = colliding(button, mouse.x, mouse.y);
	bool buttonReleased = is_button_released(MOUSE_BUTTON_LEFT);

	if (collided && is_button_down(MOUSE_BUTTON_LEFT)) {
		draw_texture(batch, btnDown, x, y + 5);
	}
	else {
		draw_texture(batch, btn, x, y);
	}

	return collided & buttonReleased;
}

static inline
f32 get_angle(vec2 a, vec2 b) {
	return atan2(b.y - a.y, b.x - a.x) * 180 / 3.14156;
}

static inline
void clamp(i32& input, i32 min, i32 max) {
	if (input > max) input = max;
	if (input < min) input = min;
}

#endif