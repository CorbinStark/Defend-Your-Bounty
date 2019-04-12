#include "bahamut.h"
#include "map.h"

int main() {
	init_window(1400, 800, "Defend Your Bounty", false, true, true);
	set_fps_cap(60);
	set_clear_color(SKYBLUE);
	set_mouse_state(MOUSE_HIDDEN);

	RenderBatch* batch = &create_batch();

	Game g = { GAME_IDLE };
	g.nextWaveTime = 3000;
	g.map = load_map("data/map.txt");
	g.money = 1000;
	MapScene scene = load_scene();

	Editor edit = { 0 };
	edit.selectedTile = 72;
	edit.map = create_map(30, 30);

	Texture cursor = load_texture("data/art/cursorGauntlet.png", GL_LINEAR);
	Shader basic = load_default_shader_2D();

	while (window_open()) {
		set_viewport(0, 0, get_window_width(), get_window_height());
		vec2 mouse = get_mouse_pos();

		begin_drawing();
		begin2D(batch, basic);
			upload_mat4(basic, "projection", orthographic_projection(0, 0, get_window_width(), get_window_height(), -1, 1));
			//editor(batch, &edit, &scene, mouse);
			game(batch, &g, &scene, mouse);
			draw_texture(batch, cursor, mouse.x, mouse.y);
		end2D(batch);
		end_drawing();
	}

	dispose_texture(cursor);
	dispose_batch(batch);
	dispose_window();
	return 0;
}