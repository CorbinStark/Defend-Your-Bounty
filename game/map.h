#ifndef MAP_H
#define MAP_H

#include <algorithm>
#include "utils.h"

//TODO:
//
// KEY: # = not done, 
//		X = done
//
//# save/load walls correctly
//X cannons should rotate and shoot at invaders
//X invaders should attack walls. Perhaps there will be a wobble animation for the unit during the attack? for visual feedback purposes
//X path steering needs to include walls (and water tiles) in seperation vector calculation
//X destroy cannons when the wall tiles they sit on are destroyed
//X Add more wall tiles for 3-way and 4-way connections
//X have ships bring invaders to shore
//X timer display for next wave of invaders
//# spawn boats randomly instead of at a set spot, and find the nearest land tile to drop invaders off at
//# money system (and display of current money)
//# unit training
//# houses to increase maximum unit capacity

const i32 TILE_SIZE = 64;
const i32 NUM_LAYERS = 3;
const i32 SHOOT_DELAY = 170;
const i32 CANNON_RANGE = 700;
const i32 CANNON_DAMAGE = 2;
const i32 WALL_HP = 80;
const i32 WAVE_DELAY_START = 20000;
const i32 WAVE_DELAY_INCREMENT = 100;

const f32 MAX_SPEED = 2;
const f32 MAX_FORCE = 2.4;
const f32 MIN_SEPERATION = 25;
const f32 SCALING_FACTOR = (1 / 15.0f);
const f32 VELOCITY_MINIMUM = 0.20;

enum UnitType {
	UNIT_SHIP,
	UNIT_DINGHY,
	UNIT_FOOTSOLDIER,
	UNIT_ELITE,
	UNIT_WORKER,
	UNIT_CANNON
};

enum UnitState {
	UNIT_IDLE,
	UNIT_WALKING,
	UNIT_ATTACKING
};

enum BuildingType {
	BUILDING_WALL,
	BUILDING_MINE,
	BUILDING_BARRACKS,
	BUILDING_CANNON,
	BUILDING_GATE
};

enum Owner {
	OWNER_RED,
	OWNER_GREEN,
	OWNER_NEUTRAL
};

struct Wall;

struct Unit {
	vec2 pos;
	vec2 velocity;
	vec2 forceToApply;
	vec2 dest;
	f32 rotation;
	i16 hp;
	i16 maxHp;
	Wall* wtarget;
	Unit* utarget;
	Texture img;
	Owner owner;
	UnitType type;
	UnitState state;
};

struct Wall {
	bool active;
	bool gate;
	i32 hp;
	i32 adjacency;
};

struct Cannon {
	i16 x;
	i16 y;
	i16 timer;
	f32 rotation;
	i16 hp;
};

struct Cannonball {
	i16 x;
	i16 y;
	f32 rotation;
};

struct Explosion {
	i16 x;
	i16 y;
	Animation animation;
};

struct MapScene {
	Texture redship[3];
	Texture greenship[3];
	Texture cannon;
	Texture cannonBall;
	Texture crew[4];
	Texture dinghyLarge[3];
	Texture dinghySmall[3];
	Texture tilesheet;
	Texture walls;
	Texture attackers[2];
	Texture explosion;

	Texture gatebutton;
	Texture gatebutton_down;
	Texture cannonbutton;
	Texture cannonbutton_down;
	Texture minimizebutton;
	Texture minimizebutton_down;
	Texture maximizebutton;
	Texture maximizebutton_down;

	Texture wallbutton;
	Texture wallbuttonDown;
	Texture trainbutton;
	Texture trainbuttonDown;
	Texture buildbutton;
	Texture buildbuttonDown;
	Texture cancelbutton;
	Texture cancelbutton_down;

	Texture ninepatch[9];

	Font kenneySquare;
	Font kenneyNarrow;
};

struct Map {
	f32 x;
	f32 y;
	i16* grid[NUM_LAYERS];
	u16 width;
	u16 height;
	Wall* walls;
	std::vector<Cannon> cannons;
	std::vector<Unit> units;
	std::vector<Cannonball> cannonballs;
	std::vector<Explosion> explosions;
};

enum EditorState {
	EDIT_PLACE,
	EDIT_TILE,
	EDIT_UNIT
};

enum GameState {
	GAME_IDLE,
	GAME_MENU,
	GAME_BUILD,
	GAME_BUILD_MENU,
	GAME_TRAIN
};

struct Game {
	GameState state;
	Map map;
	BuildingType selectedBuilding;
	u32 currentWave;
	u32 nextWaveTime;
	u32 timer;
};

struct Editor {
	Map map;
	i32 selectedTile;
	i32 selectedShip;
	i32 selectedBuilding;
	u16 currLayer;
};

typedef std::vector<Unit> UnitList;

static inline
void truncate(vec2* v, f32 max) {
	f32 i = 0;
	i = max / length(*v);
	i = i < 1.0 ? i : 1.0;
	v->x *= i;
	v->y *= i;
}

static inline
vec2 calculate_seek(vec2 dest, Unit* unit) {
	vec2 desired = MAX_SPEED * normalize(dest - unit->pos);
	return desired - unit->velocity;
}

static inline
vec2 calculate_seperation(UnitList* list, Map* map, Unit* unit) {
	vec2 totalForce = { 0 };

	for (u32 i = 0; i < list->size(); ++i) {
		Unit* a = &(*list)[i];
		if (a != unit) {
			f32 distance = getDistanceE(unit->pos.x, unit->pos.y, a->pos.x, a->pos.y);
			if (distance < MIN_SEPERATION && distance >= 0) {
				vec2 pushForce = unit->pos - a->pos;
				totalForce = totalForce + (pushForce / 15.0f);
			}
		}
	}
	for (u32 x = 0; x < map->width; ++x) {
		for (u32 y = 0; y < map->height; ++y) {
			Wall* wall = &map->walls[x + y * map->width];

			if (wall->active) {
				f32 distance = getDistanceE(unit->pos.x, unit->pos.y, x * TILE_SIZE, y * TILE_SIZE);
				if (distance < 40 && distance >= 0) {
					vec2 pushForce = unit->pos - V2(x * TILE_SIZE, y * TILE_SIZE);
					totalForce = totalForce + (pushForce);
				}
			}
		}
	}
	return MAX_FORCE * totalForce;
}

static inline
MapScene load_scene() {
	MapScene scene = { 0 };

	scene.redship[0] = load_texture("data/art/shipred0.png", GL_LINEAR);
	scene.redship[1] = load_texture("data/art/shipred1.png", GL_LINEAR);
	scene.redship[2] = load_texture("data/art/shipred2.png", GL_LINEAR);
	scene.greenship[0] = load_texture("data/art/shipgreen0.png", GL_LINEAR);
	scene.greenship[1] = load_texture("data/art/shipgreen2.png", GL_LINEAR);
	scene.greenship[2] = load_texture("data/art/shipgreen3.png", GL_LINEAR);
	scene.cannon = load_texture("data/art/cannon.png", GL_LINEAR);
	scene.cannonBall = load_texture("data/art/cannonBall.png", GL_LINEAR);
	scene.crew[0] = load_texture("data/art/crew0.png", GL_LINEAR);
	scene.crew[1] = load_texture("data/art/crew1.png", GL_LINEAR);
	scene.crew[2] = load_texture("data/art/crew2.png", GL_LINEAR);
	scene.crew[3] = load_texture("data/art/crew3.png", GL_LINEAR);
	scene.dinghyLarge[0] = load_texture("data/art/dinghyLarge0.png", GL_LINEAR);
	scene.dinghyLarge[1] = load_texture("data/art/dinghyLarge1.png", GL_LINEAR);
	scene.dinghyLarge[2] = load_texture("data/art/dinghyLarge2.png", GL_LINEAR);
	scene.dinghySmall[0] = load_texture("data/art/dinghySmall0.png", GL_LINEAR);
	scene.dinghySmall[1] = load_texture("data/art/dinghySmall1.png", GL_LINEAR);
	scene.dinghySmall[2] = load_texture("data/art/dinghySmall2.png", GL_LINEAR);
	scene.tilesheet = load_texture("data/art/tilesheet.png", GL_LINEAR);
	scene.walls = load_texture("data/art/walls.png", GL_LINEAR);
	scene.attackers[0] = load_texture("data/art/attacker1.png", GL_LINEAR);
	scene.attackers[1] = load_texture("data/art/attacker2.png", GL_LINEAR);
	scene.explosion = load_texture("data/art/explosion.png", GL_LINEAR);

	scene.cannonbutton = load_texture("data/art/buttons/cannonbutton.png", GL_LINEAR);
	scene.cannonbutton_down = load_texture("data/art/buttons/cannonbutton_down.png", GL_LINEAR);

	scene.gatebutton = load_texture("data/art/buttons/gatebutton.png", GL_LINEAR);
	scene.gatebutton_down = load_texture("data/art/buttons/gatebutton_down.png", GL_LINEAR);
	scene.wallbutton = load_texture("data/art/buttons/wallbutton.png", GL_LINEAR);
	scene.wallbuttonDown = load_texture("data/art/buttons/wallbutton_down.png", GL_LINEAR);
	scene.minimizebutton = load_texture("data/art/buttons/minimizebutton.png", GL_LINEAR);
	scene.minimizebutton_down = load_texture("data/art/buttons/minimizebutton_down.png", GL_LINEAR);
	scene.maximizebutton = load_texture("data/art/buttons/maximizebutton.png", GL_LINEAR);
	scene.maximizebutton_down = load_texture("data/art/buttons/maximizebutton_down.png", GL_LINEAR);

	scene.trainbutton = load_texture("data/art/buttons/trainbutton.png", GL_LINEAR);
	scene.trainbuttonDown = load_texture("data/art/buttons/trainbutton_down.png", GL_LINEAR);
	scene.buildbutton = load_texture("data/art/buttons/buildbutton.png", GL_LINEAR);
	scene.buildbuttonDown = load_texture("data/art/buttons/buildbutton_down.png", GL_LINEAR);
	scene.cancelbutton = load_texture("data/art/buttons/cancelbutton.png", GL_LINEAR);
	scene.cancelbutton_down = load_texture("data/art/buttons/cancelbutton_down.png", GL_LINEAR);

	scene.kenneyNarrow = load_font("data/art/Kenney Future Narrow.ttf", 16, GL_LINEAR);
	scene.kenneySquare = load_font("data/art/Kenney Future.ttf", 16, GL_LINEAR);

	load_ninepatch("data/art/panel_brown.png", scene.ninepatch);

	return scene;
}

static inline
Map create_map(i32 width, i32 height) {
	Map map = { 0 };
	map.width = width;
	map.height = height;
	for (u16 i = 0; i < NUM_LAYERS; ++i) {
		map.grid[i] = (i16*)malloc(sizeof(i16)*(width*height));
		for (u16 j = 0; j < width*height; ++j) {
			map.grid[i][j] = 72;
		}
	}
	map.walls = (Wall*)malloc(sizeof(Wall) * (width*height));
	for (u16 i = 0; i < width*height; ++i) {
		map.walls[i] = { 0 };
	}
	return map;
}

static inline
void orient_walls(Map* map) {
	for (u32 x = 0; x < map->width; ++x) {
		for (u32 y = 0; y < map->height; ++y) {
			Wall* curr = &map->walls[x + y * map->width];
			if (map->walls[(x + 1) + y * map->width].active) {
				curr->adjacency = 10;
			}
			if (map->walls[(x - 1) + y * map->width].active) {
				curr->adjacency = 9;
			}
			if (map->walls[x + (y + 1) * map->width].active) {
				curr->adjacency = 7;
			}
			if (map->walls[x + (y - 1) * map->width].active) {
				curr->adjacency = 8;
			}
			if (map->walls[(x + 1) + y * map->width].active && map->walls[(x - 1) + y * map->width].active) {
				curr->adjacency = 1;
			}
			if (map->walls[x + (y + 1) * map->width].active && map->walls[x + (y - 1) * map->width].active) {
				curr->adjacency = 0;
			}
			if (map->walls[x + (y + 1) * map->width].active && map->walls[(x + 1) + y * map->width].active) {
				curr->adjacency = 11;
			}
			if (map->walls[x + (y + 1) * map->width].active && map->walls[(x - 1) + y * map->width].active) {
				curr->adjacency = 12;
			}
			if (map->walls[x + (y - 1) * map->width].active && map->walls[(x + 1) + y * map->width].active) {
				curr->adjacency = 13;
			}
			if (map->walls[x + (y - 1) * map->width].active && map->walls[(x - 1) + y * map->width].active) {
				curr->adjacency = 14;
			}
			if (map->walls[x + (y + 1) * map->width].active && map->walls[x + (y - 1) * map->width].active && map->walls[(x - 1) + y * map->width].active) {
				curr->adjacency = 4;
			}
			if (map->walls[x + (y + 1) * map->width].active && map->walls[x + (y - 1) * map->width].active && map->walls[(x + 1) + y * map->width].active) {
				curr->adjacency = 5;
			}
			if (map->walls[x + (y + 1) * map->width].active && map->walls[(x + 1) + y * map->width].active && map->walls[(x - 1) + y * map->width].active) {
				curr->adjacency = 2;
			}
			if (map->walls[x + (y - 1) * map->width].active && map->walls[(x + 1) + y * map->width].active && map->walls[(x - 1) + y * map->width].active) {
				curr->adjacency = 3;
			}
			if (map->walls[x + (y + 1) * map->width].active && map->walls[(x + 1) + y * map->width].active && map->walls[(x - 1) + y * map->width].active && map->walls[x + (y - 1) * map->width].active) {
				curr->adjacency = 17;
			}

			if (curr->gate) {
				if (map->walls[(x - 1) + y * map->width].active && map->walls[(x + 1) + y * map->width].active) {
					curr->adjacency = 16;
				}
				if (map->walls[x + (y + 1) * map->width].active && map->walls[x + (y - 1) * map->width].active) {
					curr->adjacency = 15;
				}
			}
		}
	}
}

static inline
void draw_map(RenderBatch* batch, Map* map, MapScene* scene) {
	//draw map tiles, making sure to cull tiles outside of the viewport
#define PADDING 3
	int x0 = (-map->x / (TILE_SIZE));
	int x1 = (-map->x / (TILE_SIZE)) + (get_window_width() / TILE_SIZE) + PADDING;
	int y0 = (-map->y / (TILE_SIZE));
	int y1 = (-map->y / (TILE_SIZE)) + (get_window_height() / TILE_SIZE) + PADDING;
	clamp(x0, 0, map->width);
	clamp(y0, 0, map->height);
	clamp(x1, 0, map->width);
	clamp(y1, 0, map->height);
#undef PADDING

	Rect dest;
	dest.width = TILE_SIZE;
	dest.height = TILE_SIZE;
	Rect src;
	src.width = TILE_SIZE;
	src.height = TILE_SIZE;
	i32 num_tiles_across = scene->tilesheet.width / TILE_SIZE;
	if (num_tiles_across == 0) num_tiles_across = 1;

	//convert to integer so it doesnt draw on a sub-pixel basis
	i32 mapx = (i32)map->x;
	i32 mapy = (i32)map->y;

	//draw water tiles
	for (u16 y = y0; y < y1; ++y) {
		for (u16 x = x0; x < x1; ++x) {
			dest.x = (x * (TILE_SIZE)) + mapx;
			dest.y = (y * (TILE_SIZE)) + mapy;
			src.x = 72 % num_tiles_across;
			src.y = 72 / num_tiles_across;
			src.x *= TILE_SIZE;
			src.y *= TILE_SIZE;
			draw_texture_EX(batch, scene->tilesheet, src, dest);
		}
	}

	//draw all other tiles above that, leave blank if water tile
	for (u8 i = 0; i < NUM_LAYERS; ++i) {
		for (u16 y = y0; y < y1; ++y) {
			for (u16 x = x0; x < x1; ++x) {
				i32 id = map->grid[i][x + y * map->width];

				if (id != 72) {
					dest.x = (x * (TILE_SIZE)) + mapx;
					dest.y = (y * (TILE_SIZE)) + mapy;
					src.x = id % num_tiles_across;
					src.y = id / num_tiles_across;
					src.x *= TILE_SIZE;
					src.y *= TILE_SIZE;
					draw_texture_EX(batch, scene->tilesheet, src, dest);
				}
			}
		}
	}

	//draw walls, if active (existing)
	for (u16 y = 0; y < map->height; ++y) {
		for (u16 x = 0; x < map->width; ++x) {
			Wall* curr = &map->walls[x + y * map->width];

			if (curr->active) {
				//remove wall if its hp went to 0
				if (curr->hp <= 0) {
					curr->active = false;
					Explosion explosion = { 0 };
					explosion.animation = create_animation("explode", scene->explosion, 5, 74, 75, 10);
					explosion.x = (x*TILE_SIZE) + (TILE_SIZE / 2) - (74 / 2);
					explosion.y = (y*TILE_SIZE) + (TILE_SIZE / 2) - (75 / 2);
					map->explosions.push_back(explosion);
					orient_walls(map);

					//remove cannon if there is one on top of the wall
					for (u16 i = 0; i < map->cannons.size(); ++i) {
						Cannon* cannon = &map->cannons[i];
						if (cannon->x == x && cannon->y == y) {
							map->cannons.erase(map->cannons.begin() + i);
							break;
						}
					}
				}

				if (x > x0 - 1 && x < x1 && y > y0 - 1 && y < y1) {
					dest.x = (x * (TILE_SIZE)) + mapx;
					dest.y = (y * (TILE_SIZE)) + mapy;
					src.x = curr->adjacency * TILE_SIZE;
					src.y = 0;
					draw_texture_EX(batch, scene->walls, src, dest);
				}
			}
		}
	}

	//draw units
	for (u16 i = 0; i < map->units.size(); ++i) {
		Unit* curr = &map->units[i];
		i32 xPos = curr->pos.x + mapx;
		i32 yPos = curr->pos.y + mapy;

		if (curr->type == UNIT_SHIP) {
			if (curr->owner == OWNER_RED) {
				if (curr->hp > 7)
					draw_texture_rotated(batch, scene->redship[0], xPos, yPos, curr->rotation);
				if (curr->hp <= 7 && curr->hp > 3)
					draw_texture_rotated(batch, scene->redship[1], xPos, yPos, curr->rotation);
				if (curr->hp <= 3)
					draw_texture_rotated(batch, scene->redship[2], xPos, yPos, curr->rotation);
			}
			if (curr->owner == OWNER_GREEN) {
				if (curr->hp > 7)
					draw_texture_rotated(batch, scene->greenship[0], xPos, yPos, curr->rotation);
				if (curr->hp <= 7 && curr->hp > 3)
					draw_texture_rotated(batch, scene->greenship[1], xPos, yPos, curr->rotation);
				if (curr->hp <= 3)
					draw_texture_rotated(batch, scene->greenship[2], xPos, yPos, curr->rotation);
			}
		}
		if (curr->type == UNIT_DINGHY) {
			draw_texture_rotated(batch, scene->dinghyLarge[0], xPos, yPos, curr->rotation);
			draw_texture_rotated(batch, scene->attackers[0], 
				xPos - (scene->attackers[0].width/4) , 
				yPos - (scene->attackers[0].height / 4) , 0);
		}
		if (curr->type == UNIT_FOOTSOLDIER) {
			draw_texture_rotated(batch, scene->attackers[0], xPos, yPos, sin(curr->rotation) * 16);
		}
		if (curr->type == UNIT_ELITE) {
			draw_texture_rotated(batch, scene->attackers[1], xPos, yPos, sin(curr->rotation) * 16);
		}
	}
}

static inline
Map load_map(const char* filename) {
	Map map = { 0 };

	FILE *file = fopen(filename, "r");
	if (file == NULL)
		BMT_LOG(FATAL_ERROR, "Error opening file");

	char buffer[255];
	fgets(buffer, 255, file);
	fgets(buffer, 255, file);

	i32 width;
	i32 height;
	fscanf(file, "%d", &width);
	fscanf(file, "%d", &height);
	map = create_map(width, height);
	for (u16 i = 0; i < width * height; ++i) {
		i32 id;
		fscanf(file, "%d", &id);
		map.grid[0][i] = id;
	}

	fgets(buffer, 255, file);
	fgets(buffer, 255, file);
	for (u16 i = 0; i < width * height; ++i) {
		i32 id;
		fscanf(file, "%d", &id);
		map.grid[1][i] = id;
	}

	fgets(buffer, 255, file);
	fgets(buffer, 255, file);
	for (u16 i = 0; i < width * height; ++i) {
		i32 id;
		fscanf(file, "%d", &id);
		map.grid[2][i] = id;
	}

	i32 size;
	fgets(buffer, 255, file);
	fgets(buffer, 255, file);
	fscanf(file, "%d", &size);
	for (u16 i = 0; i < size; ++i) {
		Unit unit = { 0 };
		fscanf(file, "%d %d %d %d %d %d %d %d %d", &unit.pos.x, &unit.pos.y, &unit.hp, &unit.maxHp, &unit.type, &unit.owner, &unit.img.ID, &unit.img.width, &unit.img.height);
		map.units.push_back(unit);
	}

	fgets(buffer, 255, file);
	fscanf(file, "%d", &size);
	for (u16 i = 0; i < size; ++i) {
		Wall building = { 0 };
		building.active = true;
		i32 x;
		i32 y;
		fscanf(file, "%d %d %d %d %d", &x, &y, &building.hp);
		map.walls[x + y * map.width] = building;
	}
	orient_walls(&map);

	fclose(file);

	//TODO: There's some SUPER weird bug going on where the boats wont spawn the correct amount of units or leave beach if this isnt here...
	//most baffling bug I've had in awhile.
	Unit unit = { 0 };
	for (u32 i = 0; i < 40; ++i) {
		map.units.push_back(unit);
	}
	map.units.clear();

	return map;
}

static inline
Wall* get_closest_wall(Map* map, Unit* unit, f32* distance, u16* xRef, u16* yRef) {
	Wall* result = NULL;
	f32 shortest = INT_MAX;

	for (u16 x = 0; x < map->width; ++x) {
		for (u16 y = 0; y < map->height; ++y) {
			f32 dist = getDistanceE(x * TILE_SIZE, y * TILE_SIZE, unit->pos.x, unit->pos.y);
			if (map->walls[x + y * map->width].active && shortest > dist) {
				shortest = dist;
				result = &map->walls[x + y * map->width];
				*xRef = x;
				*yRef = y;
			}
		}
	}

	*distance = shortest;
	return result;
}

static inline
Unit* get_closest_enemy(Map* map, vec2 origin, Owner targetFaction, f32* distance) {
	Unit* result = NULL;
	f32 shortest = INT_MAX;

	for (u16 i = 0; i < map->units.size(); ++i) {
		Unit* curr = &map->units[i];

		if (curr->owner == targetFaction) {
			f32 dist = getDistanceE(curr->pos.x, curr->pos.y, origin.x, origin.y);
			if (shortest > dist) {
				shortest = dist;
				result = curr;
			}
		}
	}

	if(distance != NULL)
		*distance = shortest;
	return result;
}

static inline
void game(RenderBatch* batch, Game* game, MapScene* scene, vec2 mouse) {
	game->timer++;
	draw_map(batch, &game->map, scene);

	//new wave spawns
	if (game->timer == game->nextWaveTime) {
		game->timer = 0;
		game->currentWave++;
		game->nextWaveTime -= WAVE_DELAY_INCREMENT;

		Unit boat = { 0 };
		if (game->currentWave > 5) {
			boat.hp = boat.maxHp = 100;
			boat.dest = { 1000, 730 };
			boat.state = UNIT_WALKING;
			boat.owner = OWNER_GREEN;
			boat.pos = { (f32)game->map.width * TILE_SIZE, (f32)game->map.height * TILE_SIZE };
			boat.type = UNIT_SHIP;
		}
		else {
			boat.hp = boat.maxHp = 100;
			boat.dest = { 1000, 730 };
			boat.state = UNIT_WALKING;
			boat.owner = OWNER_GREEN;
			boat.pos = { (f32)game->map.width * TILE_SIZE, (f32)game->map.height * TILE_SIZE };
			boat.type = UNIT_DINGHY;
		}

		game->map.units.push_back(boat);

		if(game->currentWave > 10)
			game->map.units.push_back(boat);
	}

	draw_text(batch, &scene->kenneyNarrow, format_text("Next Wave in: %d", (game->nextWaveTime - game->timer)/100), 70, 5, BLACK.xyz);

	//set all units destination upon right click
	if (is_button_released(MOUSE_BUTTON_RIGHT)) {
		for (u32 i = 0; i < game->map.units.size(); ++i) {
			if (game->map.units.at(i).owner == OWNER_GREEN)
				game->map.units.at(i).dest = V2(mouse.x - game->map.x, mouse.y - game->map.y);
		}
	}

	//health bars for walls
	for (u16 x = 0; x < game->map.width; ++x) {
		for (u16 y = 0; y < game->map.height; ++y) {
			Wall* curr = &game->map.walls[x + y * game->map.width];

			if (curr->active) {
				//draw_rectangle(batch, (x * TILE_SIZE) - 32, y * TILE_SIZE, 100, 5, BLACK);
				//draw_rectangle(batch, (x * TILE_SIZE) - 32, y * TILE_SIZE, curr->hp, 5, GREEN);
			}
		}
	}

	//calculate unit velocity and steering vectors
	for (u16 i = 0; i < game->map.units.size(); ++i) {
		Unit* unit = &game->map.units[i];

		if (unit->hp <= 0) {
			game->map.units.erase(game->map.units.begin() + i);
			continue;
		}

		if (unit->owner == OWNER_GREEN && unit->state == UNIT_IDLE) {
			f32 walldist = 0;
			f32 unitdist = 0;
			u16 wallx = 0;
			u16 wally = 0;
			//Unit* utarget = get_closest_enemy(&game->map, { unit->pos.x, unit->pos.y }, OWNER_RED, &unitdist);
			Wall* wtarget = get_closest_wall(&game->map, unit, &walldist, &wallx, &wally);

			if (wallx != 0 && wally != 0) {
				unit->wtarget = wtarget;
				//unit->utarget = utarget;
				unit->dest = { (f32)wallx * TILE_SIZE, (f32)wally * TILE_SIZE };
				unit->state = UNIT_WALKING;
			}
		}
		if (unit->owner == OWNER_GREEN && unit->state == UNIT_ATTACKING) {
			unit->rotation += 0.5;
			if ((i32)unit->rotation % 10 == 0) {
				unit->wtarget->hp -= 1;
				if (unit->wtarget->hp <= 0 || !unit->wtarget->active) {
					unit->state = UNIT_IDLE;
					unit->rotation = 0;
				}
			}
		}

		if (unit->dest.x >= 0 && unit->dest.y >= 0) {
			vec2 seek = calculate_seek(unit->dest, unit);
			vec2 seperation = calculate_seperation(&game->map.units, &game->map, unit);
			unit->forceToApply = seek + seperation;
		}
	}
	//apply vectors to unit position and check if it reached its destination
	for (u16 i = 0; i < game->map.units.size(); ++i) {
		Unit* unit = &game->map.units.at(i);

		unit->velocity = unit->velocity + (SCALING_FACTOR * unit->forceToApply);
		truncate(&unit->velocity, MAX_SPEED);

		if(unit->type == UNIT_SHIP || unit->type == UNIT_DINGHY)
			unit->rotation = -atan2(unit->velocity.y, unit->velocity.x) * (180 / 3.14159);
		unit->pos = unit->pos + unit->velocity;

		//if unit reached destination (roughly)
		if ((abs(unit->velocity.x) < VELOCITY_MINIMUM && abs(unit->velocity.y) < VELOCITY_MINIMUM) &&
			(getDistanceE(unit->pos.x, unit->pos.y, unit->dest.x, unit->dest.y) < 90))
		{
			unit->dest = { -1, -1 };
			unit->velocity = { 0, 0 };
			unit->forceToApply = { 0, 0 };

			if (unit->owner == OWNER_GREEN){
				if (unit->state == UNIT_WALKING && unit->type == UNIT_FOOTSOLDIER) {
					unit->state = UNIT_ATTACKING;
				}
				if (unit->type == UNIT_SHIP || unit->type == UNIT_DINGHY) {
					if (unit->pos.x > 2900 && unit->pos.y > 2900) {
						game->map.units.erase(game->map.units.begin() + i);
						break;
					}
					//spawn invaders
					i32 numInvaders = 10;
					if (unit->type == UNIT_DINGHY)
						numInvaders = (i32)(game->currentWave * 2.5);

					Unit invader = { 0 };
					invader.type = UNIT_FOOTSOLDIER;
					invader.state = UNIT_IDLE;
					invader.owner = OWNER_GREEN;
					invader.hp = 4;
					for (u32 j = 0; j < numInvaders; ++j) {
						invader.pos = { (f32)random_int(unit->pos.x - 5, unit->pos.x + 5), (f32)random_int(unit->pos.y - 5, unit->pos.y + 5) };
						game->map.units.push_back(invader);
					}
					if (unit->type == UNIT_SHIP) {
						invader.hp = 10;
						invader.pos = { (f32)random_int(unit->pos.x - 5, unit->pos.x + 5), (f32)random_int(unit->pos.y - 5, unit->pos.y + 5) };
						invader.type = UNIT_ELITE;
						game->map.units.push_back(invader);
					}

					unit->dest = { 3000, 3000 };
					unit->state = UNIT_WALKING;
				}
			}
			else {
				if (unit->state == UNIT_WALKING)
					unit->state = UNIT_IDLE;
			}
		}
	}

	//update cannonballs position then remove cannonball, deal damage, and push an explosion upon collision.
	for (u16 i = 0; i < game->map.cannonballs.size(); ++i) {
		Cannonball* ball = &game->map.cannonballs[i];
		draw_texture_rotated(batch, scene->cannonBall, ball->x + game->map.x, ball->y + game->map.y, ball->rotation);
		
		ball->x += cos(deg_to_rad(ball->rotation)) * 6;
		ball->y += sin(deg_to_rad(ball->rotation)) * 6;

		const i32 cboxOffset = 15;

		for (u16 j = 0; j < game->map.units.size(); ++j) {
			Unit* curr = &game->map.units[j];
			if (colliding(
				{ (f32)ball->x, (f32)ball->y, (f32)scene->cannonBall.width, (f32)scene->cannonBall.height },
				{ curr->pos.x + cboxOffset, curr->pos.y + cboxOffset, (f32)scene->attackers[0].width - (cboxOffset*2), (f32)scene->attackers[0].height - (cboxOffset*2) }
			)) {
				curr->hp -= CANNON_DAMAGE;
				Explosion explosion = { 0 };
				explosion.animation = create_animation("explode", scene->explosion, 5, 74, 75, 10);
				explosion.x = ball->x + (scene->cannonBall.width/2) - (74/2);
				explosion.y = ball->y + (scene->cannonBall.height/2) - (75/2);
				game->map.explosions.push_back(explosion);
				game->map.cannonballs.erase(game->map.cannonballs.begin() + i);
				break;
			}
		}
	}

	//update cannons target and shoot on interval
	for (u16 i = 0; i < game->map.cannons.size(); ++i) {
		f32 dist = 0;

		Cannon* cannon = &game->map.cannons.at(i);
		Unit* target = get_closest_enemy(&game->map, { (f32)cannon->x * TILE_SIZE, (f32)cannon->y * TILE_SIZE}, OWNER_GREEN, &dist);
		cannon->timer++;

		draw_texture_rotated(batch, scene->cannon, (cannon->x * TILE_SIZE) + game->map.x, (cannon->y * TILE_SIZE) + game->map.y, cannon->rotation);

		if (cannon->timer % SHOOT_DELAY == 0 && target != NULL && dist < CANNON_RANGE) {
			cannon->rotation = get_angle({ (f32)cannon->x * TILE_SIZE, (f32)cannon->y * TILE_SIZE }, { target->pos.x, target->pos.y });

			Cannonball ball = { 0 };
			ball.rotation = cannon->rotation;
			ball.x = (cannon->x * TILE_SIZE) + (scene->cannon.width / 2) - (scene->cannonBall.width / 2);
			ball.y = (cannon->y * TILE_SIZE) + (scene->cannon.height / 2) - (scene->cannonBall.height / 2);
			game->map.cannonballs.push_back(ball);
		}
	}

	//update explosion animation and remove when finished
	for (u32 i = 0; i < game->map.explosions.size(); ++i) {
		Explosion* curr = &game->map.explosions[i];
		update_animation(&curr->animation, game->timer);
		draw_animation(batch, curr->animation, curr->x + game->map.x, curr->y + game->map.y);
		if (curr->animation.current == curr->animation.frames-1)
			game->map.explosions.erase(game->map.explosions.begin() + i);
	}

	//sidebar menu
	if (game->state == GAME_IDLE) {
		draw_panel(batch, scene->ninepatch, 0, 0, 1, 1);
		tooltip(batch, &scene->kenneySquare, scene->ninepatch, "OPEN MENU\nOpens the side\nmenu.", { 10, 10, (f32)scene->maximizebutton.width, (f32)scene->maximizebutton_down.height }, mouse);
		if (button(batch, scene->maximizebutton, scene->maximizebutton_down, 10, 10, mouse)) {
			game->state = GAME_MENU;
			return;
		}
	}

	if (game->state == GAME_MENU) {
		draw_panel(batch, scene->ninepatch, 0, 0, 1, 10);
		tooltip(batch, &scene->kenneySquare, scene->ninepatch, "CLOSE MENU\nMinimizes the\nside menu.", { 10, 10, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->minimizebutton, scene->minimizebutton_down, 10, 10, mouse)) {
			game->state = GAME_IDLE;
			return;
		}
		tooltip(batch, &scene->kenneySquare, scene->ninepatch, "BUILD\nOpens the menu\nfor constructing\nbuildings.", { 10, 70, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->buildbutton, scene->buildbuttonDown, 10, 70, mouse)) {
			game->state = GAME_BUILD_MENU;
			return;
		}
		tooltip(batch, &scene->kenneySquare, scene->ninepatch, "TRAIN\nOpens the menu\nfor training\nunits.", { 10, 130, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->trainbutton, scene->trainbuttonDown, 10, 130, mouse)) {
			game->state = GAME_BUILD_MENU;
			return;
		}
	}

	if (game->state == GAME_BUILD_MENU) {
		draw_panel(batch, scene->ninepatch, 0, 0, 1, 12);
		i32 yPos = -50;

		tooltip(batch, &scene->kenneySquare, scene->ninepatch, "CANCEL\nCancels current\noption and\nreturns to the\nprevious menu.", { 10, (f32)yPos + 60, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->cancelbutton, scene->cancelbutton_down, 10, yPos += 60, mouse)) {
			game->state = GAME_MENU;
			return;
		}
		tooltip(batch, &scene->kenneySquare, scene->ninepatch, "WALL\nBuilds a wall\nthat prevents\ninvaders from\npassing through.", { 10, (f32)yPos + 60, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->wallbutton, scene->wallbuttonDown, 10, yPos += 60, mouse)) {
			game->selectedBuilding = BUILDING_WALL;
			game->state = GAME_BUILD;
		}
		tooltip(batch, &scene->kenneySquare, scene->ninepatch, "GATE\nBuilds a gate\nthat friendly\nunits can pass\nthrough.", { 10, (f32)yPos + 60, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->gatebutton, scene->gatebutton_down, 10, yPos += 60, mouse)) {
			game->selectedBuilding = BUILDING_GATE;
			game->state = GAME_BUILD;
		}
		tooltip(batch, &scene->kenneySquare, scene->ninepatch, "CANNON\nBuilds a cannon\nthat shoots at\noncoming\ninvaders. Can\nonly be built on\ntop of walls.", { 10, (f32)yPos + 60, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->cannonbutton, scene->cannonbutton_down, 10, yPos += 60, mouse)) {
			game->selectedBuilding = BUILDING_CANNON;
			game->state = GAME_BUILD;
		}
	}

	if (game->state == GAME_BUILD) {
		draw_panel(batch, scene->ninepatch, 0, 0, 1, 1);
		tooltip(batch, &scene->kenneySquare, scene->ninepatch, "CANCEL\nCancels current\noption and\nreturns to the\nprevious menu.", { 10, 30, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->cancelbutton, scene->cancelbutton_down, 10, 10, mouse)) {
			game->state = GAME_BUILD_MENU;
			return;
		}

		//place selected building down where mouse is, then orient walls according to adjacencies
		if (is_button_down(MOUSE_BUTTON_LEFT) && !(mouse.x < scene->ninepatch[0].width * 3 && mouse.y < scene->ninepatch[0].height * 3)) {
			i32 x = (mouse.x - game->map.x) / TILE_SIZE;
			i32 y = (mouse.y - game->map.y) / TILE_SIZE;
			if (game->selectedBuilding == BUILDING_WALL) {
				if (!game->map.walls[x + y * game->map.width].active || game->map.walls[x + y * game->map.width].gate) {
					Wall wall = { 0 };
					wall.hp = WALL_HP;
					wall.adjacency = 6;
					wall.gate = false;
					wall.active = true;
					game->map.walls[x + y * game->map.width] = wall;

					orient_walls(&game->map);
				}
			}
			if (game->selectedBuilding == BUILDING_GATE) {
				if (
					(game->map.walls[(x + 1) + y * game->map.width].active && game->map.walls[(x - 1) + y * game->map.width].active) ||
					(game->map.walls[x + (y + 1) * game->map.width].active && game->map.walls[x + (y - 1)*game->map.width].active)
					) {

					if (!game->map.walls[x + y * game->map.width].gate) {
						Wall gate = { 0 };
						gate.hp = WALL_HP;
						gate.adjacency = 6;
						gate.active = true;
						gate.gate = true;
						game->map.walls[x + y * game->map.width] = gate;

						orient_walls(&game->map);
					}
				}
			}
			if (game->selectedBuilding == BUILDING_CANNON) {
				bool cannonPresent = false;
				for (u16 i = 0; i < game->map.cannons.size(); ++i) {
					if (game->map.cannons[i].x == x && game->map.cannons[i].y == y) {
						cannonPresent = true;
						break;
					}
				}
				if (game->map.walls[x + y * game->map.width].active && !cannonPresent) {
					Cannon cannon = { 0 };
					cannon.hp = WALL_HP;
					cannon.x = (mouse.x - game->map.x) / TILE_SIZE;
					cannon.y = (mouse.y - game->map.y) / TILE_SIZE;
					game->map.cannons.push_back(cannon);
				}
			}
		}
	}

	//move camera
	if (is_key_down(KEY_LEFT) && game->map.x < 0)
		game->map.x += 8;
	if (is_key_down(KEY_RIGHT) && game->map.x > (-game->map.width * TILE_SIZE) + get_window_width())
		game->map.x -= 8;
	if (is_key_down(KEY_DOWN) && game->map.y > (-game->map.height * TILE_SIZE) + get_window_height())
		game->map.y -= 8;
	if (is_key_down(KEY_UP) && game->map.y < 0)
		game->map.y += 8;
}

static inline
void editor(RenderBatch* batch, Editor* editor, MapScene* scene, vec2 mouse) {
	Map* map = &editor->map;

	LOCAL EditorState state = EDIT_PLACE;
	if (is_key_released(KEY_F1)) {
		if (state == EDIT_TILE)
			state = EDIT_PLACE;
		else {
			editor->currLayer = 0;
			editor->selectedShip = 0;
			state = EDIT_TILE;
		}
	}
	if (is_key_released(KEY_F2)) {
		if (state == EDIT_TILE)
			state = EDIT_PLACE;
		else {
			editor->currLayer = 1;
			editor->selectedShip = 0;
			state = EDIT_TILE;
		}
	}
	if (is_key_released(KEY_F3)) {
		if (state == EDIT_TILE)
			state = EDIT_PLACE;
		else {
			editor->currLayer = 2;
			editor->selectedShip = 0;
			state = EDIT_TILE;
		}
	}
	if (is_key_released(KEY_F4)) {
		if (state == EDIT_UNIT)
			state = EDIT_PLACE;
		else
			state = EDIT_UNIT;
	}
	if (is_key_released(KEY_F5)) {
		//save
		FILE *file = fopen("data/map.txt", "w");
		if (file == NULL)
			BMT_LOG(FATAL_ERROR, "Error opening file");

		fprintf(file, "#map\n");
		fprintf(file, "#layer1\n");
		fprintf(file, "%d\n", editor->map.width);
		fprintf(file, "%d\n", editor->map.height);
		for (u16 i = 0; i < editor->map.width * editor->map.height; ++i)
			fprintf(file, "%d ", editor->map.grid[0][i]);
		fprintf(file, "\n");

		fprintf(file, "#layer2\n");
		for (u16 i = 0; i < editor->map.width * editor->map.height; ++i)
			fprintf(file, "%d ", editor->map.grid[1][i]);
		fprintf(file, "\n");

		fprintf(file, "#layer3\n");
		for (u16 i = 0; i < editor->map.width * editor->map.height; ++i)
			fprintf(file, "%d ", editor->map.grid[2][i]);
		fprintf(file, "\n");

		fprintf(file, "#units\n");
		fprintf(file, "%d\n", editor->map.units.size());
		for (u16 i = 0; i < editor->map.units.size(); ++i) {
			Unit* curr = &editor->map.units[i];
			fprintf(file, "%d %d %d %d %d %d %d %d %d\n", curr->pos.x, curr->pos.y, curr->hp, curr->maxHp, (i32)curr->type, (i32)curr->owner, curr->img.ID, curr->img.width, curr->img.height);
		}

		fprintf(file, "#buildings\n");
		for (u16 x = 0; x < editor->map.width; ++x) {
			for (u16 y = 0; y < editor->map.height; ++y) {
				Wall* curr = &editor->map.walls[x + y * editor->map.width];
				if (curr->active)
					fprintf(file, "%d %d %d\n", x, y, curr->hp);
			}
		}

		fclose(file);
	}
	if (is_key_released(KEY_F6)) {
		//load
		editor->map = load_map("data/map.txt");
	}
	if (is_key_released(KEY_F7)) {
		//new
		editor->map.units.clear();
		editor->map = create_map(map->width, map->height);
	}
	if (is_button_released(MOUSE_BUTTON_RIGHT)) {
		editor->selectedShip = 0;
		editor->selectedTile = 72;
		for (u16 i = 0; i < editor->map.units.size(); ++i) {
			Unit* curr = &editor->map.units[i];
			if (mouse.x > curr->pos.x + map->x && mouse.y > curr->pos.y + map->y && mouse.x < curr->pos.x + curr->img.width + map->x && mouse.y < curr->pos.y + curr->img.height + map->y) {
				editor->map.units.erase(editor->map.units.begin() + i);
				break;
			}
		}
	}

	draw_map(batch, map, scene);

	if (is_key_down(KEY_LEFT))
		map->x += 8;
	if (is_key_down(KEY_RIGHT))
		map->x -= 8;
	if (is_key_down(KEY_DOWN))
		map->y -= 8;
	if (is_key_down(KEY_UP))
		map->y += 8;

	if (state == EDIT_TILE) {
		if (is_button_released(MOUSE_BUTTON_LEFT)) {
			if (mouse.x < scene->tilesheet.width && mouse.y < scene->tilesheet.height) {
				i32 tilex = mouse.x / TILE_SIZE;
				i32 tiley = mouse.y / TILE_SIZE;
				editor->selectedTile = tilex + tiley * (scene->tilesheet.width / TILE_SIZE);
			}
		}
		draw_rectangle(batch, 0, 0, scene->tilesheet.width, scene->tilesheet.height, BLACK);
		draw_texture(batch, scene->tilesheet, 0, 0);
	}
	if (state == EDIT_UNIT) {
		if (is_button_released(MOUSE_BUTTON_LEFT)) {
			if (mouse.x < scene->redship[0].width && mouse.y < scene->redship[0].height)
				editor->selectedShip = 1;
			if (mouse.x > scene->redship[0].width && mouse.x < scene->redship[0].width * 2 && mouse.y < scene->redship[0].height)
				editor->selectedShip = 2;
			if (mouse.x > scene->redship[0].width * 2 || mouse.y > scene->redship[0].height)
				editor->selectedShip = 0;
		}
		draw_rectangle(batch, 0, 0, scene->redship[0].width * 2, scene->redship[0].height, BLACK);
		draw_texture(batch, scene->redship[0], 0, 0);
		draw_texture(batch, scene->greenship[0], scene->redship[0].width, 0);
	}

	if (editor->selectedShip == 1) {
		draw_texture(batch, scene->redship[0], mouse.x, mouse.y);
	}
	if (editor->selectedShip == 2) {
		draw_texture(batch, scene->greenship[0], mouse.x, mouse.y);
	}

	if (state == EDIT_PLACE) {
		mouse.x -= map->x;
		mouse.y -= map->y;
		if (is_button_released(MOUSE_BUTTON_LEFT)) {
			if (editor->selectedShip != 0) {
				Unit ship = { 0 };
				ship.img = scene->redship[0];
				ship.owner = (Owner)(editor->selectedShip - 1);
				ship.hp = ship.maxHp = 100;
				ship.type = UNIT_SHIP;
				ship.pos.x = mouse.x;
				ship.pos.y = mouse.y;
				map->units.push_back(ship);
			}
		}
		if (is_button_down(MOUSE_BUTTON_LEFT)) {
			if (editor->selectedShip == 0) {
				if (mouse.x > 0 && mouse.y > 0 && mouse.x < map->width * TILE_SIZE && mouse.y < map->height * TILE_SIZE) {
					i32 tilex = mouse.x / TILE_SIZE;
					i32 tiley = mouse.y / TILE_SIZE;
					map->grid[editor->currLayer][tilex + tiley * map->width] = editor->selectedTile;
				}
			}
		}
	}
}

#endif