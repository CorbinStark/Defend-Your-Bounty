#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <random>
#include "bahamut.h"

enum MainState {
	MAIN_TITLE,
	MAIN_OPTIONS,
	MAIN_CREDITS,
	MAIN_GAME,
	MAIN_EDITOR,
	MAIN_CHOOSE_MAP,
	MAIN_RETURN_TO_TITLE,
	MAIN_GO_TO_OPTIONS,
	MAIN_CONFIRM,
	MAIN_EXIT
};

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
	f32 scale;
};

struct BitmapFont {
	Texture chars[SCHAR_MAX];
};

struct Config {
	bool vsync;
	bool fullscreen;
	bool lastFsState;
	u8 volume;
};

static inline
Config load_config() {
	Config con = { 0 };

	FILE *file = fopen("data/config.txt", "r");
	if (file == NULL)
		BMT_LOG(FATAL_ERROR, "Error opening file");

	i32 fullscreen;
	i32 vsync;
	i32 volume;
	fscanf(file, "%d", &fullscreen);
	fscanf(file, "%d", &vsync);
	fscanf(file, "%d", &volume);

	con.fullscreen = fullscreen == 0 ? false : true;
	con.lastFsState = fullscreen;
	con.vsync = vsync == 0 ? false : true;
	con.volume = volume;

	fclose(file);

	return con;
}

static inline
void save_config(Config con) {
	FILE *file = fopen("data/config.txt", "w");
	if (file == NULL)
		BMT_LOG(FATAL_ERROR, "Error opening file");

	fprintf(file, "%d\n", con.fullscreen);
	fprintf(file, "%d\n", con.vsync);
	fprintf(file, "%d\n", con.volume);

	fclose(file);
}

typedef std::vector<Animation> AnimationList;

static inline
Animation create_animation(std::string name, Texture sheet, u16 frames, u16 width, u16 height, u32 delay, f32 scale = 1) {
	Animation anim;
	anim.current = 0;
	anim.identifier = name;
	anim.img = sheet;
	anim.frames = frames;
	anim.width = width;
	anim.height = height;
	anim.delay = delay;
	anim.scale = scale;
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
	Rect dst = { x, y, anim.width * anim.scale, anim.height * anim.scale };
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

static inline
BitmapFont load_neighbors_font(u8 scale = 1) {
	BitmapFont font = { 0 };
	i32 h;
	i32 w;
	unsigned char* image = SOIL_load_image("data/art/good_neighbors.png", &w, &h, 0, SOIL_LOAD_RGBA);
	font.chars['!'] = get_sub_image(image, w, 1, 0, 6, h, GL_NEAREST);
	font.chars['"'] = get_sub_image(image, w, 8, 0, 7, h, GL_NEAREST);
	font.chars['#'] = get_sub_image(image, w, 16, 0, 10, h, GL_NEAREST);
	font.chars['$'] = get_sub_image(image, w, 27, 0, 10, h, GL_NEAREST);
	font.chars['%'] = get_sub_image(image, w, 38, 0, 11, h, GL_NEAREST);
	font.chars['&'] = get_sub_image(image, w, 50, 0, 11, h, GL_NEAREST);
	font.chars['\''] = get_sub_image(image, w, 62, 0, 4, h, GL_NEAREST);
	font.chars['('] = get_sub_image(image, w, 67, 0, 6, h, GL_NEAREST);
	font.chars[')'] = get_sub_image(image, w, 74, 0, 6, h, GL_NEAREST);
	font.chars['*'] = get_sub_image(image, w, 81, 0, 10, h, GL_NEAREST);
	font.chars['+'] = get_sub_image(image, w, 92, 0, 8, h, GL_NEAREST);
	font.chars[','] = get_sub_image(image, w, 101, 0, 4, h, GL_NEAREST);
	font.chars['-'] = get_sub_image(image, w, 106, 0, 9, h, GL_NEAREST);
	font.chars['.'] = get_sub_image(image, w, 116, 0, 4, h, GL_NEAREST);
	font.chars['/'] = get_sub_image(image, w, 121, 0, 8, h, GL_NEAREST);
	font.chars['0'] = get_sub_image(image, w, 130, 0, 8, h, GL_NEAREST);
	font.chars['1'] = get_sub_image(image, w, 139, 0, 6, h, GL_NEAREST);
	font.chars['2'] = get_sub_image(image, w, 146, 0, 8, h, GL_NEAREST);
	font.chars['3'] = get_sub_image(image, w, 155, 0, 8, h, GL_NEAREST);
	font.chars['4'] = get_sub_image(image, w, 164, 0, 9, h, GL_NEAREST);
	font.chars['5'] = get_sub_image(image, w, 174, 0, 8, h, GL_NEAREST);
	font.chars['6'] = get_sub_image(image, w, 183, 0, 8, h, GL_NEAREST);
	font.chars['7'] = get_sub_image(image, w, 192, 0, 8, h, GL_NEAREST);
	font.chars['8'] = get_sub_image(image, w, 201, 0, 8, h, GL_NEAREST);
	font.chars['9'] = get_sub_image(image, w, 210, 0, 8, h, GL_NEAREST);
	font.chars[':'] = get_sub_image(image, w, 219, 0, 4, h, GL_NEAREST);
	font.chars[';'] = get_sub_image(image, w, 224, 0, 4, h, GL_NEAREST);
	font.chars['<'] = get_sub_image(image, w, 229, 0, 9, h, GL_NEAREST);
	font.chars['='] = get_sub_image(image, w, 239, 0, 7, h, GL_NEAREST);
	font.chars['>'] = get_sub_image(image, w, 247, 0, 9, h, GL_NEAREST);
	font.chars['?'] = get_sub_image(image, w, 257, 0, 8, h, GL_NEAREST);
	font.chars['@'] = get_sub_image(image, w, 266, 0, 10, h, GL_NEAREST);
	font.chars['A'] = get_sub_image(image, w, 277, 0, 8, h, GL_NEAREST);
	font.chars['B'] = get_sub_image(image, w, 286, 0, 8, h, GL_NEAREST);
	font.chars['C'] = get_sub_image(image, w, 295, 0, 8, h, GL_NEAREST);
	font.chars['D'] = get_sub_image(image, w, 304, 0, 9, h, GL_NEAREST);
	font.chars['E'] = get_sub_image(image, w, 314, 0, 8, h, GL_NEAREST);
	font.chars['F'] = get_sub_image(image, w, 323, 0, 8, h, GL_NEAREST);
	font.chars['G'] = get_sub_image(image, w, 332, 0, 8, h, GL_NEAREST);
	font.chars['H'] = get_sub_image(image, w, 341, 0, 8, h, GL_NEAREST);
	font.chars['I'] = get_sub_image(image, w, 350, 0, 6, h, GL_NEAREST);
	font.chars['J'] = get_sub_image(image, w, 357, 0, 9, h, GL_NEAREST);
	font.chars['K'] = get_sub_image(image, w, 367, 0, 8, h, GL_NEAREST);
	font.chars['L'] = get_sub_image(image, w, 376, 0, 8, h, GL_NEAREST);
	font.chars['M'] = get_sub_image(image, w, 385, 0, 10, h, GL_NEAREST);
	font.chars['N'] = get_sub_image(image, w, 396, 0, 9, h, GL_NEAREST);
	font.chars['O'] = get_sub_image(image, w, 406, 0, 8, h, GL_NEAREST);
	font.chars['P'] = get_sub_image(image, w, 415, 0, 8, h, GL_NEAREST);
	font.chars['Q'] = get_sub_image(image, w, 424, 0, 9, h, GL_NEAREST);
	font.chars['R'] = get_sub_image(image, w, 434, 0, 9, h, GL_NEAREST);
	font.chars['S'] = get_sub_image(image, w, 444, 0, 8, h, GL_NEAREST);
	font.chars['T'] = get_sub_image(image, w, 453, 0, 8, h, GL_NEAREST);
	font.chars['U'] = get_sub_image(image, w, 462, 0, 8, h, GL_NEAREST);
	font.chars['V'] = get_sub_image(image, w, 471, 0, 8, h, GL_NEAREST);
	font.chars['W'] = get_sub_image(image, w, 480, 0, 10, h, GL_NEAREST);
	font.chars['X'] = get_sub_image(image, w, 491, 0, 9, h, GL_NEAREST);
	font.chars['Y'] = get_sub_image(image, w, 501, 0, 8, h, GL_NEAREST);
	font.chars['Z'] = get_sub_image(image, w, 510, 0, 8, h, GL_NEAREST);
	font.chars['['] = get_sub_image(image, w, 519, 0, 6, h, GL_NEAREST);
	font.chars['\\'] = get_sub_image(image, w, 526, 0, 8, h, GL_NEAREST);
	font.chars[']'] = get_sub_image(image, w, 535, 0, 6, h, GL_NEAREST);
	font.chars['^'] = get_sub_image(image, w, 542, 0, 11, h, GL_NEAREST);
	font.chars['_'] = get_sub_image(image, w, 554, 0, 8, h, GL_NEAREST);
	font.chars['`'] = get_sub_image(image, w, 563, 0, 6, h, GL_NEAREST);
	font.chars['a'] = get_sub_image(image, w, 570, 0, 8, h, GL_NEAREST);
	font.chars['b'] = get_sub_image(image, w, 579, 0, 8, h, GL_NEAREST);
	font.chars['c'] = get_sub_image(image, w, 588, 0, 8, h, GL_NEAREST);
	font.chars['d'] = get_sub_image(image, w, 597, 0, 8, h, GL_NEAREST);
	font.chars['e'] = get_sub_image(image, w, 606, 0, 8, h, GL_NEAREST);
	font.chars['f'] = get_sub_image(image, w, 615, 0, 7, h, GL_NEAREST);
	font.chars['g'] = get_sub_image(image, w, 623, 0, 8, h, GL_NEAREST);
	font.chars['h'] = get_sub_image(image, w, 632, 0, 8, h, GL_NEAREST);
	font.chars['i'] = get_sub_image(image, w, 641, 0, 6, h, GL_NEAREST);
	font.chars['j'] = get_sub_image(image, w, 648, 0, 6, h, GL_NEAREST);
	font.chars['k'] = get_sub_image(image, w, 655, 0, 8, h, GL_NEAREST);
	font.chars['l'] = get_sub_image(image, w, 664, 0, 5, h, GL_NEAREST);
	font.chars['m'] = get_sub_image(image, w, 670, 0, 10, h, GL_NEAREST);
	font.chars['n'] = get_sub_image(image, w, 681, 0, 8, h, GL_NEAREST);
	font.chars['o'] = get_sub_image(image, w, 690, 0, 8, h, GL_NEAREST);
	font.chars['p'] = get_sub_image(image, w, 699, 0, 8, h, GL_NEAREST);
	font.chars['q'] = get_sub_image(image, w, 708, 0, 9, h, GL_NEAREST);
	font.chars['r'] = get_sub_image(image, w, 718, 0, 8, h, GL_NEAREST);
	font.chars['s'] = get_sub_image(image, w, 727, 0, 8, h, GL_NEAREST);
	font.chars['t'] = get_sub_image(image, w, 736, 0, 8, h, GL_NEAREST);
	font.chars['u'] = get_sub_image(image, w, 745, 0, 8, h, GL_NEAREST);
	font.chars['v'] = get_sub_image(image, w, 754, 0, 8, h, GL_NEAREST);
	font.chars['w'] = get_sub_image(image, w, 763, 0, 10, h, GL_NEAREST);
	font.chars['x'] = get_sub_image(image, w, 774, 0, 8, h, GL_NEAREST);	
	font.chars['y'] = get_sub_image(image, w, 783, 0, 8, h, GL_NEAREST);
	font.chars['z'] = get_sub_image(image, w, 792, 0, 8, h, GL_NEAREST);
	font.chars['{'] = get_sub_image(image, w, 801, 0, 7, h, GL_NEAREST);
	font.chars['|'] = get_sub_image(image, w, 809, 0, 4, h, GL_NEAREST);
	font.chars['}'] = get_sub_image(image, w, 814, 0, 7, h, GL_NEAREST);
	font.chars['~'] = get_sub_image(image, w, 822, 0, 9, h, GL_NEAREST);
	font.chars[' '] = get_sub_image(image, w, 831, 0, 6, h, GL_NEAREST);
	font.chars[1] = get_sub_image(image, w, 831, 0, 1, h, GL_NEAREST);

	for (u8 i = 0; i < SCHAR_MAX; ++i) {
		font.chars[i].width *= scale;
		font.chars[i].height *= scale;
	}
	free(image);

	return font;
}

static inline
void draw_text(RenderBatch* batch, BitmapFont* font, const char* str, f32 x, f32 y, f32 r=255, f32 g=255, f32 b=255, f32 a=255) {
	i32 len = strlen(str);
	for (u16 i = 0; i < len; ++i) {
		draw_texture(batch, font->chars[str[i]], x, y, { r/255,g/255,b/255,a/255 });
		x += font->chars[str[i]].width - font->chars[1].width;
	}
}

static inline
i32 get_string_width(BitmapFont* font, const char* str) {
	i32 width = 0;
	i32 len = strlen(str);
	for (u16 i = 0; i < len; ++i) {
		width += font->chars[str[i]].width - font->chars[1].width;
	}
	return width;
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
bool text_button(RenderBatch* batch, BitmapFont* font, std::string str, i32* yInitial, vec2 mouse) {
	f32 height = 16 * 3;
	f32 width = get_string_width(font, str.c_str());
	f32 xPos = (get_window_width() / 2) - (width / 2);
	f32 yPos = *yInitial += (16 * 3) + 15;
	bool collided = colliding({ xPos, yPos, width, height }, { mouse.x, mouse.y });
	if (collided) {
		str[0] = '>';
		str[1] = ' ';
		draw_text(batch, font, str.c_str(), xPos, yPos);
	}
	else {
		str[0] = ' ';
		str[1] = ' ';
		draw_text(batch, font, str.c_str(), xPos, yPos);
	}

	return collided & is_button_released(MOUSE_BUTTON_LEFT);
}

static inline
void tooltip(RenderBatch* batch, BitmapFont* font, Texture ninepatch[9], const char* text, u8 width, u8 height, Rect rect, vec2 mouse) {
	u32 numTokens = 0;
	char** tokens = split_string(text, "\n", &numTokens);

	if (colliding(rect, mouse.x, mouse.y)) {
		draw_panel(batch, ninepatch, rect.x + 60, rect.y, width, height);
		for (u32 i = 0; i < numTokens; ++i)
			draw_text(batch, font, tokens[i], rect.x + 70, rect.y + 10 + (font->chars['a'].height * i));
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

static inline
i32 roundUp(i32 numToRound, i32 multiple) {
	if (multiple == 0)
		return numToRound;

	i32 remainder = abs(numToRound) % multiple;
	if (remainder == 0)
		return numToRound;

	if (numToRound < 0)
		return -(abs(numToRound) - remainder);
	else
		return numToRound + multiple - remainder;
}

#endif