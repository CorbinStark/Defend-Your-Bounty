#include "bahamut.h"
#include "map.h"

#ifdef _DEBUG
#else
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

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
void choose_map(RenderBatch* batch, Game* selectedMap, Game* demo, MainState* state, MapScene* scene, BitmapFont* font, vec2 mouse) {
	i32 yInitial = (get_window_height() / 2) - (((16 * 3) + 15) * 3) + 50;

	game(batch, demo, scene, mouse, state, true);

	draw_text(batch, font, "  Defend Your Bounty!", (get_window_width() / 2) - (get_string_width(font, "  Defend Your Bounty!") / 2), 50);
	draw_text(batch, font, "  Choose Map", (get_window_width() / 2) - (get_string_width(font, "  Choose Map") / 2), yInitial-50);

	if (text_button(batch, font, "  Crankshockz Bay (EASY)", &yInitial, mouse)) {
		*selectedMap = { GAME_MENU };
		selectedMap->map = load_map("data/crankshockz.txt");
		selectedMap->map.walls[5 + 6 * selectedMap->map.width] = { true, false, 100, 6 };
		selectedMap->map.turrets.push_back({ 5, 6, 0, 0, 150, TURRET_CANNON });
		add_wave(selectedMap, format_text("%d n", UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d s %d s %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d s %d e %d e %d", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_ELITE_SHIP));
		add_wave(selectedMap, format_text("%d s %d s %d e %d e %d s", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_ELITE_SHIP, UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d s %d s %d s %d e %d e %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_ELITE_SHIP, UNIT_DINGHY, UNIT_ELITE_SHIP));
		add_wave(selectedMap, format_text("%d e %d e %d s %d e %d e", UNIT_GOLIATH_SHIP, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d e %d s %d e %d s %d s %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_ELITE_SHIP, UNIT_DINGHY, UNIT_ELITE_SHIP));
		add_wave(selectedMap, format_text("%d s %d e %d s %d e %d e %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP));
		add_wave(selectedMap, format_text("%d e %d s %d e %d e %d s %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d e %d e %d s %d e %d s %d e", UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP));
		add_wave(selectedMap, format_text("%d e %d s %d e %d s %d e %d e", UNIT_ULTIMATE_BOSS_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP));
		add_wave(selectedMap, format_text("%d s %d e %d s %d e %d s %d e %d s", UNIT_GOLIATH_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP));
		selectedMap->currentWave = 0;
		selectedMap->nextWaveTime = 3200;
		selectedMap->money = 3000;
		selectedMap->map.goldpiles.push_back({5, 4, 10});
		*state = MAIN_GAME;
	}

	if (text_button(batch, font, "  Pegleg John's Bluff (NORMAL)", &yInitial, mouse)) {
		*selectedMap = { GAME_MENU };
		selectedMap->map = load_map("data/PeglegJohnBluff.txt");
		selectedMap->map.walls[15 + 7 * selectedMap->map.width] = { true, false, 100, 6 };
		selectedMap->map.turrets.push_back({ 15, 7, 0, 0, 150, TURRET_CANNON });
		selectedMap->map.walls[15 + 19 * selectedMap->map.width] = { true, false, 100, 6 };
		selectedMap->map.turrets.push_back({ 15, 19, 0, 0, 150, TURRET_CANNON });
		add_wave(selectedMap, format_text("%d n", UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d n %d e %d s %d n %d e", UNIT_DINGHY, UNIT_STONETHROWER_SHIP, UNIT_DINGHY, UNIT_MAGE_SHIP, UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d n %d n %d n %d s %d e %d e %d e", UNIT_ELITE_SHIP, UNIT_DINGHY, UNIT_DINGHY, UNIT_STONETHROWER_SHIP, UNIT_DINGHY, UNIT_DINGHY, UNIT_ELITE_SHIP));
		add_wave(selectedMap, format_text("%d n %d s %d n %d n", UNIT_MAGE_SHIP, UNIT_RUSH_SHIP, UNIT_DINGHY, UNIT_STONETHROWER_SHIP));
		add_wave(selectedMap, format_text("%d e %d e %d n %d s %d n %d n", UNIT_DINGHY, UNIT_DINGHY, UNIT_STONETHROWER_SHIP, UNIT_ELITE_SHIP, UNIT_STONETHROWER_SHIP, UNIT_ELITE_SHIP));
		add_wave(selectedMap, format_text("%d e %d s %d s %d n %d n", UNIT_MAGE_SHIP, UNIT_ELITE_SHIP, UNIT_RUSH_SHIP, UNIT_RUSH_SHIP, UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d e %d s %d s %d n %d n", UNIT_GOLIATH_SHIP, UNIT_STONETHROWER_SHIP, UNIT_STONETHROWER_SHIP, UNIT_STONETHROWER_SHIP, UNIT_STONETHROWER_SHIP));
		add_wave(selectedMap, format_text("%d e %d e %d e %d s %d e", UNIT_EDRIC_SHIP, UNIT_DINGHY, UNIT_MAGE_SHIP, UNIT_DINGHY, UNIT_MAGE_SHIP));
		selectedMap->currentWave = 0;
		selectedMap->nextWaveTime = 2800;
		selectedMap->money = 2500;
		selectedMap->map.goldpiles.push_back({ 15, 8, 10 });
		selectedMap->map.goldpiles.push_back({ 15, 18, 10 });
		*state = MAIN_GAME;
	}

	if (text_button(batch, font, "  Bluebeard's Torment (HARD)", &yInitial, mouse)) {
		*selectedMap = { GAME_MENU };
		selectedMap->map = load_map("data/BluebeardsTorment.txt");
		selectedMap->map.walls[25 + 19 * selectedMap->map.width] = { true, false, 100, 6 };
		selectedMap->map.turrets.push_back({ 25, 19, 0, 0, 150, TURRET_CANNON });
		selectedMap->map.walls[20 + 9 * selectedMap->map.width] = { true, false, 100, 6 };
		selectedMap->map.turrets.push_back({ 20, 9, 0, 0, 150, TURRET_CANNON });
		selectedMap->map.walls[14 + 19 * selectedMap->map.width] = { true, false, 100, 6 };
		selectedMap->map.turrets.push_back({ 14, 19, 0, 0, 150, TURRET_CANNON });
		add_wave(selectedMap, format_text("%d n", UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d n %d s %d e %d w", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d n %d s %d e %d w", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d n %d s %d e %d w %d e", UNIT_MAGE_SHIP, UNIT_STONETHROWER_SHIP, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d n %d s %d e %d w %d s", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP));
		add_wave(selectedMap, format_text("%d n %d s %d e %d w %d s", UNIT_DINGHY, UNIT_DINGHY, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP));
		add_wave(selectedMap, format_text("%d n %d s %d e %d w %d n %d s", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_ELITE_SHIP, UNIT_DINGHY, UNIT_ELITE_SHIP));
		add_wave(selectedMap, format_text("%d n %d s %d e %d w %d s", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
		add_wave(selectedMap, format_text("%d n %d s %d e %d w %d s", UNIT_GOLIATH_SHIP, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
		selectedMap->currentWave = 0;
		selectedMap->nextWaveTime = 2650;
		selectedMap->money = 2500;
		selectedMap->map.goldpiles.push_back({ 25, 18, 10 });
		selectedMap->map.goldpiles.push_back({ 20, 10, 10 });
		selectedMap->map.goldpiles.push_back({ 14, 18, 10 });
		*state = MAIN_GAME;
	}

	if (text_button(batch, font, "  I am NOT enjoying my life (IMPOSSIBLE)", &yInitial, mouse)) {
		*state = MAIN_GAME;
	}

	if (text_button(batch, font, "  Back", &yInitial, mouse)) {
		*state = MAIN_TITLE;
	}

	if (demo->timer % 100 > 50) {
		draw_text(batch, font, "Demonstration Gameplay", (get_window_width() / 2) - (get_string_width(font, "Demonstration  Gameplay") / 2), get_window_height() - 50);
	}
}

static inline
void title_screen(RenderBatch* batch, Game* demo, MainState* state, MapScene* scene, BitmapFont* font, vec2 mouse, f32* creditsScroll) {
	i32 yInitial = (get_window_height() / 2) - (((16 * 3) + 15) * 3);

	game(batch, demo, scene, mouse, state, true);

	draw_text(batch, font, "  Defend Your Bounty!", (get_window_width() / 2) - (get_string_width(font, "  Defend Your Bounty!") / 2), 50);

	if (text_button(batch, font, "  Play Game", &yInitial, mouse)) {
		*state = MAIN_CHOOSE_MAP;
	}
	if (text_button(batch, font, "  Options", &yInitial, mouse)) {
		*state = MAIN_OPTIONS;
	}
	if (text_button(batch, font, "  Credits", &yInitial, mouse)) {
		(*creditsScroll) = get_window_height();
		*state = MAIN_CREDITS;
	}
	if (text_button(batch, font, "  Exit", &yInitial, mouse)) {
		*state = MAIN_EXIT;
	}

#ifdef _DEBUG
	yInitial += 50;
	if (text_button(batch, font, "  Editor", &yInitial, mouse)) {
		*state = MAIN_EDITOR;
	}
#endif

	if (demo->timer % 100 > 50) {
		draw_text(batch, font, "Demonstration Gameplay", (get_window_width() / 2) - (get_string_width(font, "Demonstration  Gameplay") / 2), get_window_height() - 50);
	}
}

static inline
void credits(RenderBatch* batch, MainState* state, BitmapFont* font, f32* creditsScroll) {
	(*creditsScroll) -= 0.5f;
	f32 yOffset = (*creditsScroll);

	draw_text(batch, font, "The following works were used to create this game:", 50, yOffset);
	yOffset += 32;
	draw_text(batch, font, "Kenney UI Pack RPG Extension   (This work has been modified.)", 50, yOffset += 32);
	draw_text(batch, font, "Author: kenney.nl", 50, yOffset += 32);
	draw_text(batch, font, "URL: https://opengameart.org/content/ui-pack-rpg-extension", 50, yOffset += 32);
	draw_text(batch, font, "License: * CC0 (http://creativecommons.org/publicdomain/zero/1.0/legalcode)", 50, yOffset += 32);
	yOffset += 32;
	draw_text(batch, font, "Kenney UI Expansion   (This work has been modified.)", 50, yOffset += 32);
	draw_text(batch, font, "Author: Photoshopwizard", 50, yOffset += 32);
	draw_text(batch, font, "URL: https://opengameart.org/content/ui-pack-expansion", 50, yOffset += 32);
	draw_text(batch, font, "License: * CC0 (http://creativecommons.org/publicdomain/zero/1.0/legalcode)", 50, yOffset += 32);
	yOffset += 32;
	draw_text(batch, font, "Good Neighbors Pixel Font", 50, yOffset += 32);
	draw_text(batch, font, "Author: Clint Bellanger", 50, yOffset += 32);
	draw_text(batch, font, "URL: https://opengameart.org/content/good-neighbors-pixel-font", 50, yOffset += 32);
	draw_text(batch, font, "License: * CC0 (http://creativecommons.org/publicdomain/zero/1.0/legalcode)", 50, yOffset += 32);
	yOffset += 32;
	draw_text(batch, font, "Kenney Medieval RTS (120+)", 50, yOffset += 32);
	draw_text(batch, font, "Author: kenney.nl", 50, yOffset += 32);
	draw_text(batch, font, "URL: https://opengameart.org/content/medieval-rts-120", 50, yOffset += 32);
	draw_text(batch, font, "License: * CC0 (http://creativecommons.org/publicdomain/zero/1.0/legalcode)", 50, yOffset += 32);
	yOffset += 32;
	draw_text(batch, font, "Kenney Pirate Pack (190+)   (This work has been modified.)", 50, yOffset += 32);
	draw_text(batch, font, "Author: kenney.nl", 50, yOffset += 32);
	draw_text(batch, font, "URL: https://opengameart.org/content/pirate-pack-190", 50, yOffset += 32);
	draw_text(batch, font, "License: * CC0 (http://creativecommons.org/publicdomain/zero/1.0/legalcode)", 50, yOffset += 32);
	yOffset += 32;
	draw_text(batch, font, "Blackmoor Tides (Epic Pirate Battle Theme)", 50, yOffset += 32);
	draw_text(batch, font, "Author: Matthew Pablo", 50, yOffset += 32);
	draw_text(batch, font, "URL: https://opengameart.org/content/blackmoor-tides-epic-pirate-battle-theme", 50, yOffset += 32);
	draw_text(batch, font, "License: * CC-BY 3.0 (https://creativecommons.org/licenses/by/3.0/)", 50, yOffset += 32);
	yOffset += 32;
	draw_text(batch, font, "RPG Sound Pack", 50, yOffset += 32);
	draw_text(batch, font, "Author: artisticdude", 50, yOffset += 32);
	draw_text(batch, font, "URL: https://opengameart.org/content/rpg-sound-pack", 50, yOffset += 32);
	draw_text(batch, font, "License: * CC0 (http://creativecommons.org/publicdomain/zero/1.0/legalcode)", 50, yOffset += 32);
	yOffset += 32;
	draw_text(batch, font, "Fire And Evil Spell", 50, yOffset += 32);
	draw_text(batch, font, "Author: artisticdude", 50, yOffset += 32);
	draw_text(batch, font, "URL: https://opengameart.org/content/fire-evil-spell", 50, yOffset += 32);
	draw_text(batch, font, "License: * CC0 (http://creativecommons.org/publicdomain/zero/1.0/legalcode)", 50, yOffset += 32);
	yOffset += 32;
	draw_text(batch, font, "9 Explosion Sounds", 50, yOffset += 32);
	draw_text(batch, font, "Author: Unnamed", 50, yOffset += 32);
	draw_text(batch, font, "URL: https://opengameart.org/content/9-explosion-sounds", 50, yOffset += 32);
	draw_text(batch, font, "License: * CC-BY-SA 3.0 (https://creativecommons.org/licenses/by-sa/3.0/)", 50, yOffset += 32);

	if (yOffset > 950)
		draw_text(batch, font, "Press any key to go back to title screen.", 50, 50);

	if (get_key_pressed())
		*state = MAIN_TITLE;

	if (yOffset < -100)
		*state = MAIN_TITLE;
}

static inline
Game initialize_demo_map() {
	Game g = { GAME_IDLE };
	g.map = load_map("data/map.txt");
	g.nextWaveTime = 2000;
	g.currentWave = 0;
	g.money = 1000;
	g.timer = 1999;
	add_wave(&g, format_text("%d n %d s %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
	add_wave(&g, format_text("%d n %d s %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
	add_wave(&g, format_text("%d n %d s %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
	add_wave(&g, format_text("%d n %d s %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
	add_wave(&g, format_text("%d n %d s %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
	add_wave(&g, format_text("%d n %d s %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
	add_wave(&g, format_text("%d n %d s %d s", UNIT_ELITE_SHIP, UNIT_ELITE_SHIP, UNIT_ELITE_SHIP));
	add_wave(&g, format_text("%d n %d s %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
	add_wave(&g, format_text("%d n %d s %d e", UNIT_DINGHY, UNIT_DINGHY, UNIT_DINGHY));
	add_wave(&g, format_text("%d s %d s %d s", UNIT_GOLIATH_SHIP, UNIT_DINGHY, UNIT_DINGHY));
	for (u32 x = 10; x < 13; ++x) {
		for (u32 y = 7; y < 10; ++y) {
			if (x == 11 && y == 8) continue;
			g.map.walls[x + y * g.map.width].active = true;
			g.map.walls[x + y * g.map.width].hp = 100;

			Turret cannon = { 0 };
			cannon.x = x;
			cannon.y = y;
			cannon.shotDelay = 170;
			g.map.turrets.push_back(cannon);
		}
	}
	g.map.walls[11 + 6 * g.map.width].active = true;
	g.map.walls[11 + 6 * g.map.width].hp = 100;
	orient_walls(&g.map);
	Turret cannon = { 0 };
	cannon.x = 11;
	cannon.y = 6;
	cannon.type = TURRET_MAGE;
	cannon.shotDelay = 300;
	g.map.turrets.push_back(cannon);
	g.map.goldpiles.push_back({ 11, 8, 10 });
	return g;
}

int main() {
	init_window(1400, 800, "Defend Your Bounty", false, true, true);
	init_audio();
	set_fps_cap(60);
	set_clear_color(SKYBLUE);
	set_mouse_state(MOUSE_HIDDEN);
	set_master_volume(100);
	set_vsync(true);

	RenderBatch* batch = &create_batch();
	MainState state = MAIN_TITLE;

	MapScene scene = load_scene();

	Editor edit = { 0 };
	edit.selectedTile = 72;
	edit.map = create_map(40, 30);

	BitmapFont big = load_neighbors_font(3);
	BitmapFont small = load_neighbors_font(2);

	Texture cursor = load_texture("data/art/cursorGauntlet.png", GL_LINEAR);
	Shader basic = load_default_shader_2D();

	f32 creditsScroll = 0;

	Game demo = initialize_demo_map();
	Game g;

	Sound blackmoorTides = load_sound("data/sounds/Blackmoor Tides Loop.wav");
	set_sound_looping(blackmoorTides, true);
	set_sound_volume(blackmoorTides, 100);
	play_sound(blackmoorTides);

	while (window_open()) {
		set_viewport(0, 0, get_window_width(), get_window_height());
		vec2 mouse = get_mouse_pos();

		begin_drawing();
		begin2D(batch, basic);
		upload_mat4(basic, "projection", orthographic_projection(0, 0, get_window_width(), get_window_height(), -1, 1));

		if (state == MAIN_EDITOR)
			editor(batch, &edit, &scene, mouse);
		if (state == MAIN_GAME)
			game(batch, &g, &scene, mouse, &state);
		if (state == MAIN_TITLE) {
			title_screen(batch, &demo, &state, &scene, &big, mouse, &creditsScroll);
			if (demo.map.turrets.size() == 0)
				demo = initialize_demo_map();
		}
		if (state == MAIN_CHOOSE_MAP) {
			choose_map(batch, &g, &demo, &state, &scene, &big, mouse);
			if (goldpile_depleted(&demo.map))
				demo = initialize_demo_map();
		}
		if (state == MAIN_CREDITS)
			credits(batch, &state, &small, &creditsScroll);
		if (state == MAIN_EXIT)
			exit(0);

		draw_texture(batch, cursor, mouse.x, mouse.y);
		end2D(batch);
		end_drawing();
	}

	dispose_sound(blackmoorTides);
	dispose_texture(cursor);
	dispose_batch(batch);
	dispose_window();
	return 0;
}