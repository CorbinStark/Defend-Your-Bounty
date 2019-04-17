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
//X spawn boats randomly instead of at a set spot, and find the nearest land tile to drop invaders off at
//X money system (and display of current money)
//X boats seem to be rotated strangely (not facing the direction they are moving)
//X custom waves
//X prevent player from blocking unit paths with wall placement
//# overworld with multiple maps
//# maybe small cutscene at beginning? story was part of grading criteria
//# sound effects
//# rain?
//# fix place building display square so it locks to the grid when moving the camera
//# add rock throwers (invader and a cannon)
//# add building upgrades
//# add repair function

const i32 TILE_SIZE = 64;
const i32 NUM_LAYERS = 3;
const i32 CANNON_RANGE = 700;
const i32 CANNON_DAMAGE = 2;
const i32 WALL_HP = 80;
const i32 WALL_COST = 100;
const i32 GATE_COST = 20;
const i32 CANNON_COST = 150;
const i32 MAGE_COST = 400;
const i32 GOLD_BOUNTY = 60;
const i32 BOAT_HP = 25;
const i32 ANIMATION_DELAY = 7;
const i32 FIREBALL_RADIUS = 80;

const f32 UNIT_RANGE = 450;
const f32 GOLIATH_RANGE = 800;
const f32 MAX_SPEED = 2;
const f32 MAX_FORCE = 2.4;
const f32 MIN_SEPERATION = 25;
const f32 SCALING_FACTOR = (1 / 15.0f);
const f32 VELOCITY_MINIMUM = 0.20;

enum UnitType {
	UNIT_DINGHY,
	UNIT_ELITE_SHIP,
	UNIT_MAGE_SHIP,
	UNIT_BOSS_SHIP,
	UNIT_FOOTSOLDIER,
	UNIT_ELITE,
	UNIT_MAGE,
	UNIT_GOLIATH,
	UNIT_WORKER,
	UNIT_CANNON,

	//JOKE
	UNIT_ULTIMATE_BOSS,
	UNIT_ULTIMATE_BOSS_SHIP
};

enum UnitState {
	UNIT_IDLE,
	UNIT_WALKING,
	UNIT_RANGING,
	UNIT_ATTACKING
};

enum BuildingType {
	BUILDING_WALL,
	BUILDING_MINE,
	BUILDING_BARRACKS,
	BUILDING_CANNON,
	BUILDING_MAGE,
	BUILDING_GATE
};

enum Owner {
	OWNER_PLAYER,
	OWNER_INVADERS,
	OWNER_NEUTRAL
};

struct Wall;

struct Unit {
	vec2 pos;
	vec2 velocity;
	vec2 forceToApply;
	vec2 dest;
	vec2 origin;
	f32 rotation;
	i16 hp;
	i16 maxHp;
	u8 damage;
	Wall* wtarget;
	Unit* utarget;
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

enum TurretType {
	TURRET_CANNON,
	TURRET_MAGE
};

struct Turret {
	i16 x;
	i16 y;
	i16 timer;
	f32 rotation;
	u16 shotDelay;
	TurretType type;
};

enum ProjectileType {
	PROJECTILE_CANNONBALL,
	PROJECTILE_FIREBALL,
	PROJECTILE_BOULDER
};

struct Projectile {
	i16 x;
	i16 y;
	f32 rotation;
	ProjectileType type;
	Owner owner;
	Animation animation;
};

struct Explosion {
	i16 x;
	i16 y;
	Animation animation;
};

struct MapScene {
	Texture redship[3];
	Texture greenship[3];
	Texture bossShip;
	Texture cannon;
	Texture cannonBall;
	Texture dinghyLarge[3];
	Texture dinghySmall[3];
	Texture tilesheet;
	Texture walls;
	Texture walls_damaged;
	Texture mage;
	Texture attackers[2];
	Texture goliath;
	Texture explosion;
	Texture goldpile;
	Texture fire;
	Texture boulder;

	Texture barleft;
	Texture barmid;
	Texture barright;

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
	Texture priestbutton;
	Texture priestbutton_down;
	Texture trainbutton;
	Texture trainbuttonDown;
	Texture buildbutton;
	Texture buildbuttonDown;
	Texture cancelbutton;
	Texture cancelbutton_down;

	BitmapFont font;

	Texture ninepatch[9];

	Sound explosionBang;
	Sound click;
	Sound click2;
	Sound goliathGrowl;
	Sound coin[3];
	Sound swing[3];

	Texture corey;
};

struct Map {
	f32 x;
	f32 y;
	i16* grid[NUM_LAYERS];
	u16 width;
	u16 height;
	Wall* walls;
	std::vector<Turret> turrets;
	std::vector<Unit> units;
	std::vector<Projectile> projectiles;
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

struct Notification {
	f32 alpha;
	std::string text;
};

struct StatusText {
	f32 alpha;
	vec2 pos;
	std::string text;
};

const i32 MAX_RAIN = 255;

struct Game {
	GameState state;
	Map map;
	BuildingType selectedBuilding;
	std::vector<std::vector<Unit>> waves;
	std::vector<Notification> notifications;
	std::vector<StatusText> statusTexts;
	Rect rain[MAX_RAIN];
	u8 numRain;
	u32 money;
	u32 currentWave;
	u32 nextWaveTime;
	u32 timer;

	bool leftSpawn;
	bool rightSpawn;
	bool upSpawn;
	bool downSpawn;
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
void push_notification(Game* game, std::string text) {
	Notification note = { 255, text };
	for (u16 i = 0; i < game->notifications.size(); ++i) {
		if (game->notifications.at(i).text == text)
			return;
	}
	game->notifications.push_back(note);
}

static inline
void push_status_text(Game* game, vec2 pos, std::string text) {
	StatusText note = { 255, pos, text };
	game->statusTexts.push_back(note);
}

static inline
void add_wave(Game* game, const char* params) {
	std::vector<Unit> wave;

	Unit boat = { 0 };
	boat.state = UNIT_WALKING;
	boat.owner = OWNER_INVADERS;
	boat.hp = boat.maxHp = BOAT_HP;

	u32 numTokens = 0;
	char** tokens = split_string(params, " ", &numTokens);

	for (u16 i = 0; i < numTokens; ++i) {
		boat.type = (UnitType)(atoi(tokens[i]));
		wave.push_back(boat);
	}

	for (u16 i = 0; i < numTokens; ++i)
		free(tokens[i]);
	free(tokens);

	game->waves.push_back(wave);
}

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
	scene.greenship[1] = load_texture("data/art/shipgreen1.png", GL_LINEAR);
	scene.greenship[2] = load_texture("data/art/shipgreen2.png", GL_LINEAR);
	scene.bossShip = load_texture("data/art/bossShip.png", GL_LINEAR);
	scene.cannon = load_texture("data/art/cannon.png", GL_LINEAR);
	scene.mage = load_texture("data/art/priest.png", GL_LINEAR);
	scene.cannonBall = load_texture("data/art/cannonBall.png", GL_LINEAR);
	scene.dinghyLarge[0] = load_texture("data/art/dinghyLarge0.png", GL_LINEAR);
	scene.dinghyLarge[1] = load_texture("data/art/dinghyLarge1.png", GL_LINEAR);
	scene.dinghyLarge[2] = load_texture("data/art/dinghyLarge2.png", GL_LINEAR);
	scene.dinghySmall[0] = load_texture("data/art/dinghySmall0.png", GL_LINEAR);
	scene.dinghySmall[1] = load_texture("data/art/dinghySmall1.png", GL_LINEAR);
	scene.dinghySmall[2] = load_texture("data/art/dinghySmall2.png", GL_LINEAR);
	scene.tilesheet = load_texture("data/art/tilesheet.png", GL_LINEAR);
	scene.walls = load_texture("data/art/walls.png", GL_LINEAR);
	scene.walls_damaged = load_texture("data/art/walls_damaged.png", GL_LINEAR);
	scene.attackers[0] = load_texture("data/art/attacker1.png", GL_LINEAR);
	scene.attackers[1] = load_texture("data/art/attacker2.png", GL_LINEAR);
	scene.explosion = load_texture("data/art/explosion.png", GL_LINEAR);
	scene.goldpile = load_texture("data/art/goldpile.png", GL_LINEAR);
	scene.fire = load_texture("data/art/fire.png", GL_LINEAR);
	scene.goliath = load_texture("data/art/goliath.png", GL_LINEAR);
	scene.boulder = load_texture("data/art/boulder.png", GL_LINEAR);

	scene.barleft = load_texture("data/art/bar_left.png", GL_NEAREST);
	scene.barmid = load_texture("data/art/bar_mid.png", GL_NEAREST);
	scene.barright = load_texture("data/art/bar_right.png", GL_NEAREST);

	scene.cannonbutton = load_texture("data/art/buttons/cannonbutton.png", GL_LINEAR);
	scene.cannonbutton_down = load_texture("data/art/buttons/cannonbutton_down.png", GL_LINEAR);
	scene.priestbutton = load_texture("data/art/buttons/priestbutton.png", GL_LINEAR);
	scene.priestbutton_down = load_texture("data/art/buttons/priestbutton_down.png", GL_LINEAR);

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

	scene.font = load_neighbors_font(2);

	load_ninepatch("data/art/panel_brown.png", scene.ninepatch);

	scene.explosionBang = load_sound("data/sounds/explosion.wav");
	set_sound_volume(scene.explosionBang, 255);
	scene.click = load_sound("data/sounds/click.wav");
	scene.click2 = load_sound("data/sounds/click2.wav");
	set_sound_volume(scene.click2, 100);
	scene.goliathGrowl = load_sound("data/sounds/goliath.wav");
	scene.coin[0] = load_sound("data/sounds/coin.wav");
	scene.coin[1] = load_sound("data/sounds/coin2.wav");
	scene.coin[2] = load_sound("data/sounds/coin3.wav");
	scene.swing[0] = load_sound("data/sounds/swing.wav");
	scene.swing[1] = load_sound("data/sounds/swing2.wav");
	scene.swing[2] = load_sound("data/sounds/swing3.wav");

	scene.corey = load_texture("data/art/The Ultimate Final Boss.png", GL_LINEAR);

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

	clamp(x0, 0, map->width);
	clamp(y0, 0, map->height);
	clamp(x1, 0, map->width);
	clamp(y1, 0, map->height);

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
					play_sound(scene->explosionBang);

					curr->active = false;
					Explosion explosion = { 0 };
					explosion.animation = create_animation("explode", scene->explosion, 5, 74, 75, ANIMATION_DELAY);
					explosion.x = (x*TILE_SIZE) + (TILE_SIZE / 2) - (74 / 2);
					explosion.y = (y*TILE_SIZE) + (TILE_SIZE / 2) - (75 / 2);
					map->explosions.push_back(explosion);
					orient_walls(map);

					//remove cannon if there is one on top of the wall
					for (u16 i = 0; i < map->turrets.size(); ++i) {
						Turret* turret = &map->turrets[i];
						if (turret->x == x && turret->y == y) {
							map->turrets.erase(map->turrets.begin() + i);
							break;
						}
					}
				}

				if (x > x0 - 1 && x < x1 && y > y0 - 1 && y < y1) {
					dest.x = (x * (TILE_SIZE)) + mapx;
					dest.y = (y * (TILE_SIZE)) + mapy;
					src.x = curr->adjacency * TILE_SIZE;
					src.y = 0;
					if (curr->hp < WALL_HP / 2)
						draw_texture_EX(batch, scene->walls_damaged, src, dest);
					else
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

		if (curr->type == UNIT_ELITE_SHIP) {
			if (curr->hp > 18)
				draw_texture_rotated(batch, scene->greenship[0], xPos, yPos, curr->rotation);
			if (curr->hp <= 18 && curr->hp > 10)
				draw_texture_rotated(batch, scene->greenship[1], xPos, yPos, curr->rotation);
			if (curr->hp <= 10)
				draw_texture_rotated(batch, scene->greenship[2], xPos, yPos, curr->rotation);
		}
		if (curr->type == UNIT_BOSS_SHIP || curr->type == UNIT_ULTIMATE_BOSS_SHIP) {
			scene->bossShip.width *= 1.5;
			scene->bossShip.height *= 1.5;
			draw_texture_rotated(batch, scene->bossShip, xPos, yPos, curr->rotation);
			scene->bossShip.width /= 1.5;
			scene->bossShip.height /= 1.5;
		}
		if (curr->type == UNIT_DINGHY) {
			draw_texture_rotated(batch, scene->dinghyLarge[0], xPos, yPos, curr->rotation);
			draw_texture_rotated(batch, scene->attackers[0],
				xPos - (scene->attackers[0].width / 6),
				yPos - (scene->attackers[0].height / 6), 0);
		}
		if (curr->type == UNIT_FOOTSOLDIER) {
			draw_texture_rotated(batch, scene->attackers[0], xPos, yPos, sin(curr->rotation) * 16);
		}
		if (curr->type == UNIT_ELITE) {
			draw_texture_rotated(batch, scene->attackers[1], xPos, yPos, sin(curr->rotation) * 16);
		}
		if (curr->type == UNIT_GOLIATH) {
			draw_texture_rotated(batch, scene->goliath, xPos, yPos, sin(curr->rotation) * 16);
		}
		if (curr->type == UNIT_ULTIMATE_BOSS) {
			draw_texture_rotated(batch, scene->corey, xPos, yPos, sin(curr->rotation) * 16);
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
		fscanf(file, "%d %d %d %d %d %d", &unit.pos.x, &unit.pos.y, &unit.hp, &unit.maxHp, &unit.type, &unit.owner);
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

	if (distance != NULL)
		*distance = shortest;
	return result;
}

static inline
vec2 get_closest_land(Map* map, vec2 origin) {
	vec2 result = { 0, 0 };
	f32 shortest = INT_MAX;

	for (u16 x = 0; x < map->width; ++x) {
		for (u16 y = 0; y < map->height; ++y) {
			f32 dist = getDistanceE(x * TILE_SIZE, y * TILE_SIZE, origin.x, origin.y);
			if (map->grid[0][x + y * map->width] != 72 && shortest > dist) {
				shortest = dist;
				result = { (f32)x, (f32)y };
			}
		}
	}

	return { result.x * TILE_SIZE, result.y * TILE_SIZE };
}

static inline
void game(RenderBatch* batch, Game* game, MapScene* scene, vec2 mouse, bool demo = false) {
	game->timer++;
	draw_map(batch, &game->map, scene);
	draw_texture(batch, scene->goldpile, (64 * 5) + game->map.x, (64 * 5) + game->map.y);

	//if (game->numRain < MAX_RAIN)
	//	game->rain[game->numRain++] = { (f32)random_int(0, get_window_width()), -10, 1, (f32)random_int(15, 25) };
	//
	//for (u32 i = 0; i < game->numRain; ++i) {
	//	game->rain[i].y += 20;
	//	if (game->rain[i].y > random_int(get_window_height(), get_window_height() + 275)) {
	//		game->rain[i].y = -15;
	//		game->rain[i].x = random_int(0, get_window_width());
	//	}
	//	draw_rectangle(batch, game->rain[i].x, game->rain[i].y, game->rain[i].width, game->rain[i].height, BLUE);
	//}

	if (game->map.turrets.size() == 0) {
		push_notification(game, "You have been defeated! (Just quit, there's no game over programmed in yet)");
	}

	//new wave spawns
	if (game->timer == game->nextWaveTime) {
		if (game->currentWave == game->waves.size() && game->map.units.size() == 0) {
			push_notification(game, "You have defeated the invaders!");
			//you win this map~!
		}
		else {
			push_notification(game, "An invasion force is spotted off the coast!");
			game->timer = 0;

			i32 sideToSpawn = random_int(0, 4);
			u16 numBoats = game->waves.at(game->currentWave).size();

			for (u32 i = 0; i < numBoats; ++i) {
				i32 sideToSpawn = random_int(0, 4);

				Unit boat = game->waves.at(game->currentWave).at(i);
				if (boat.type == UNIT_BOSS_SHIP)
					push_notification(game, "Uh oh! That ship looks like a fleet leader!");

				if (boat.type == UNIT_ULTIMATE_BOSS_SHIP) {
					push_notification(game, "It comes...");
					push_notification(game, "Pray to whatever god you believe in. You will be meeting them soon");
				}

				bool spawnedBoat = false;
				while (!spawnedBoat) {
					if (sideToSpawn == 0 && game->upSpawn) {
						boat.pos = { (f32)random_int(0, (f32)game->map.width * TILE_SIZE), 0 };
						spawnedBoat = true;
					}
					if (sideToSpawn == 1 && game->downSpawn) {
						boat.pos = { (f32)random_int(0, (f32)game->map.width * TILE_SIZE), (f32)game->map.height * TILE_SIZE };
						spawnedBoat = true;
					}
					if (sideToSpawn == 2 && game->rightSpawn) {
						boat.pos = { (f32)game->map.width * TILE_SIZE, (f32)random_int(0, (f32)game->map.width * TILE_SIZE) };
						spawnedBoat = true;
					}
					if (sideToSpawn == 3 && game->leftSpawn) {
						boat.pos = { 0, (f32)random_int(0, (f32)game->map.width * TILE_SIZE) };
						spawnedBoat = true;
					}
					sideToSpawn = random_int(0, 4);
				}
				boat.dest = get_closest_land(&game->map, boat.pos);
				boat.origin = boat.pos;
				game->map.units.push_back(boat);
			}
			game->currentWave++;
		}
	}

	//update cannons target and shoot on interval
	for (u16 i = 0; i < game->map.turrets.size(); ++i) {
		f32 dist = 0;

		Turret* turret = &game->map.turrets.at(i);
		Unit* target = get_closest_enemy(&game->map, { (f32)turret->x * TILE_SIZE, (f32)turret->y * TILE_SIZE }, OWNER_INVADERS, &dist);
		turret->timer++;

		if (turret->type == TURRET_CANNON)
			draw_texture_rotated(batch, scene->cannon, (turret->x * TILE_SIZE) + game->map.x, (turret->y * TILE_SIZE) + game->map.y, turret->rotation);
		if (turret->type == TURRET_MAGE)
			draw_texture(batch, scene->mage, (turret->x * TILE_SIZE) + game->map.x, (turret->y * TILE_SIZE) + game->map.y);

		if (turret->timer % turret->shotDelay == 0 && target != NULL && dist < CANNON_RANGE) {
			turret->rotation = get_angle({ (f32)turret->x * TILE_SIZE, (f32)turret->y * TILE_SIZE }, { target->pos.x + 15, target->pos.y + 15 });

			Projectile ball = { 0 };
			ball.owner = OWNER_PLAYER;
			ball.rotation = turret->rotation;

			if (turret->type == TURRET_CANNON) {
				ball.type = PROJECTILE_CANNONBALL;
				ball.x = (turret->x * TILE_SIZE) + (scene->cannon.width / 2) - (scene->cannonBall.width / 2);
				ball.y = (turret->y * TILE_SIZE) + (scene->cannon.height / 2) - (scene->cannonBall.height / 2);
			}
			if (turret->type == TURRET_MAGE) {
				ball.type = PROJECTILE_FIREBALL;
				ball.x = (turret->x * TILE_SIZE) + (scene->cannon.width / 2) - (18 / 2);
				ball.y = (turret->y * TILE_SIZE) + (scene->cannon.height / 2) - (39 / 2);
				ball.animation = create_animation("fire", scene->fire, 2, 18, 39, 6);
			}
			game->map.projectiles.push_back(ball);
		}
	}

	//calculate unit velocity and steering vectors
	for (u16 i = 0; i < game->map.units.size(); ++i) {
		Unit* unit = &game->map.units[i];

		if (unit->hp <= 0) {
			play_sound(scene->coin[random_int(0, 2)]);
			push_status_text(game, unit->pos, format_text("+%d gold", GOLD_BOUNTY));
			game->money += GOLD_BOUNTY;
			game->map.units.erase(game->map.units.begin() + i);
			continue;
		}

		if (unit->type == UNIT_ULTIMATE_BOSS) {
			draw_text(batch, &scene->font, "???", (get_window_width() / 2) - 50, get_window_height() - 55);

			f32 width = ((f32)((f32)unit->hp / (f32)unit->maxHp) * get_window_width());
			i32 xPos = (get_window_width() / 2) - ((scene->barleft.width + width) / 2);
			draw_texture(batch, scene->barleft, xPos, get_window_height() - 20);
			draw_texture_EX(batch, scene->barmid, { 0, 0, (f32)scene->barmid.width, (f32)scene->barmid.height }, { (f32)(xPos += scene->barleft.width), (f32)(get_window_height() - 20), width, (f32)scene->barmid.height });
			draw_texture(batch, scene->barright, xPos += width, get_window_height() - 20);
		}

		if (unit->type == UNIT_GOLIATH) {
			draw_text(batch, &scene->font, "GOLIATH", (get_window_width() / 2) - 50, get_window_height() - 55);

			f32 width = ((f32)((f32)unit->hp / (f32)unit->maxHp) * get_window_width());
			i32 xPos = (get_window_width() / 2) - ((scene->barleft.width + width) / 2);
			draw_texture(batch, scene->barleft, xPos, get_window_height() - 20);
			draw_texture_EX(batch, scene->barmid, { 0, 0, (f32)scene->barmid.width, (f32)scene->barmid.height }, { (f32)(xPos += scene->barleft.width), (f32)(get_window_height() - 20), width, (f32)scene->barmid.height });
			draw_texture(batch, scene->barright, xPos += width, get_window_height() - 20);
		}

		if (unit->owner == OWNER_INVADERS && unit->state == UNIT_IDLE) {
			f32 walldist = 0;
			f32 unitdist = 0;
			u16 wallx = -1;
			u16 wally = -1;
			//Unit* utarget = get_closest_enemy(&game->map, { unit->pos.x, unit->pos.y }, OWNER_RED, &unitdist);
			Wall* wtarget = get_closest_wall(&game->map, unit, &walldist, &wallx, &wally);
			unit->wtarget = wtarget;

			if (wallx >= 0 && wally >= 0) {
				if (unit->type == UNIT_GOLIATH) {
					unit->dest = { (f32)(wallx * TILE_SIZE), (f32)(wally * TILE_SIZE) };

					if (getDistanceE(unit->pos.x, unit->pos.y, unit->dest.x, unit->dest.y) > 250)
						unit->origin = { -1, -1 };

					unit->state = UNIT_WALKING;
				}
				else {
					//unit->utarget = utarget;
					unit->dest = { (f32)wallx * TILE_SIZE, (f32)wally * TILE_SIZE };
					unit->state = UNIT_WALKING;
				}
			}
		}
		if (unit->owner == OWNER_INVADERS && unit->state == UNIT_ATTACKING) {
			unit->rotation += 0.5;
			if ((i32)unit->rotation % 10 == 0) {
				play_sound(scene->swing[random_int(0, 2)]);
				unit->wtarget->hp -= unit->damage;
			}
		}
		if (unit->owner == OWNER_INVADERS && unit->state == UNIT_RANGING) {
			unit->rotation += 0.10;
			if ((i32)unit->rotation % 10 == 0) {

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

		if (unit->type == UNIT_ELITE_SHIP || unit->type == UNIT_DINGHY || unit->type == UNIT_BOSS_SHIP || unit->type == UNIT_MAGE_SHIP || unit->type == UNIT_ULTIMATE_BOSS_SHIP)
			unit->rotation = atan2(unit->velocity.y, unit->velocity.x) * (180 / 3.14159) - 90;
		unit->pos = unit->pos + unit->velocity;

		if (unit->wtarget != NULL && (unit->wtarget->hp <= 0 || !unit->wtarget->active)) {
			unit->state = UNIT_IDLE;
			unit->rotation = 0;
		}

		if (unit->type == UNIT_GOLIATH && unit->origin.x == -1 && unit->origin.y == -1 && getDistanceE(unit->pos.x, unit->pos.y, unit->dest.x, unit->dest.y) < GOLIATH_RANGE) {
			Projectile ball = { 0 };
			play_sound(scene->goliathGrowl);
			ball.owner = OWNER_INVADERS;
			ball.type = PROJECTILE_BOULDER;
			ball.x = unit->pos.x;
			ball.y = unit->pos.y;
			ball.rotation = get_angle(unit->pos, unit->dest);
			game->map.projectiles.push_back(ball);
			unit->origin = { 0, 0 };
		}

		//if unit reached destination (roughly)
		if ((abs(unit->velocity.x) < VELOCITY_MINIMUM && abs(unit->velocity.y) < VELOCITY_MINIMUM) &&
			(getDistanceE(unit->pos.x, unit->pos.y, unit->dest.x, unit->dest.y) < 90))
		{
			unit->dest = { -1, -1 };
			unit->velocity = { 0, 0 };
			unit->forceToApply = { 0, 0 };

			if (unit->owner == OWNER_INVADERS) {
				if (unit->state == UNIT_WALKING && (unit->type == UNIT_FOOTSOLDIER || unit->type == UNIT_ELITE || unit->type == UNIT_ULTIMATE_BOSS)) {
					unit->state = UNIT_ATTACKING;
				}
				if (unit->state == UNIT_WALKING && (unit->type == UNIT_GOLIATH)) {
					unit->state = UNIT_ATTACKING;
				}
				if (unit->type == UNIT_ELITE_SHIP || unit->type == UNIT_DINGHY || unit->type == UNIT_BOSS_SHIP || unit->type == UNIT_MAGE_SHIP || unit->type == UNIT_ULTIMATE_BOSS_SHIP) {
					if (getDistanceE(unit->pos.x, unit->pos.y, unit->origin.x, unit->origin.y) < 95) {
						game->map.units.erase(game->map.units.begin() + i);
						break;
					}
					//spawn invaders
					Unit invader = { 0 };
					invader.type = UNIT_FOOTSOLDIER;
					invader.state = UNIT_IDLE;
					invader.owner = OWNER_INVADERS;
					invader.hp = 4;
					invader.damage = 1;
					if (unit->type == UNIT_DINGHY) {
						for (u32 j = 0; j < 3; ++j) {
							invader.pos = { (f32)random_int(unit->pos.x - 5, unit->pos.x + 5), (f32)random_int(unit->pos.y - 5, unit->pos.y + 5) };
							game->map.units.push_back(invader);
						}
					}
					if (unit->type == UNIT_ELITE_SHIP) {
						invader.pos = { (f32)random_int(unit->pos.x - 5, unit->pos.x + 5), (f32)random_int(unit->pos.y - 5, unit->pos.y + 5) };
						game->map.units.push_back(invader);
						for (u32 j = 0; j < 3; ++j) {
							invader.hp = 6;
							invader.pos = { (f32)random_int(unit->pos.x - 5, unit->pos.x + 5), (f32)random_int(unit->pos.y - 5, unit->pos.y + 5) };
							invader.type = UNIT_ELITE;
							game->map.units.push_back(invader);
						}
					}
					if (unit->type == UNIT_BOSS_SHIP) {
						invader.type = UNIT_GOLIATH;
						invader.hp = invader.maxHp = 130;
						invader.damage = 6;
						invader.origin = { -1, -1 };
						invader.pos = { unit->pos.x, unit->pos.y };
						game->map.units.push_back(invader);
					}
					if (unit->type == UNIT_ULTIMATE_BOSS_SHIP) {
						invader.type = UNIT_ULTIMATE_BOSS;
						invader.hp = invader.maxHp = 1000;
						invader.damage = 30;
						invader.pos = { unit->pos.x, unit->pos.y };
						game->map.units.push_back(invader);
					}

					unit->dest = unit->origin;
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
	for (u16 i = 0; i < game->map.projectiles.size(); ++i) {
		Projectile* proj = &game->map.projectiles[i];

		if (proj->type == PROJECTILE_CANNONBALL)
			draw_texture_rotated(batch, scene->cannonBall, proj->x + game->map.x, proj->y + game->map.y, proj->rotation);
		if (proj->type == PROJECTILE_FIREBALL) {
			update_animation(&proj->animation, game->timer);
			draw_animation(batch, proj->animation, proj->x + game->map.x, proj->y + game->map.y);
		}
		if (proj->type == PROJECTILE_BOULDER) {
			draw_texture_rotated(batch, scene->boulder, proj->x + game->map.x, proj->y + game->map.y, proj->rotation);
		}

		proj->x += cos(deg_to_rad(proj->rotation)) * 6;
		proj->y += sin(deg_to_rad(proj->rotation)) * 6;

		if (proj->x > game->map.width * TILE_SIZE || proj->y > game->map.height * TILE_SIZE || proj->x < 0 || proj->y < 0) {
			game->map.projectiles.erase(game->map.projectiles.begin() + i);
			continue;
		}

		const i32 cboxOffset = 5;

		f32 width = proj->type == PROJECTILE_FIREBALL ? 18 : proj->type == PROJECTILE_BOULDER ? scene->boulder.width : scene->cannonBall.width;
		f32 height = proj->type == PROJECTILE_FIREBALL ? 39 : proj->type == PROJECTILE_BOULDER ? scene->boulder.height : scene->cannonBall.height;

		if (proj->owner == OWNER_INVADERS) {

			for (u16 x = 0; x < game->map.width; ++x) {
				for (u16 y = 0; y < game->map.height; ++y) {
					Wall* curr = &game->map.walls[x + y * game->map.width];

					if (curr->active) {
						if (colliding({ (f32)proj->x, (f32)proj->y, width, height }, { (f32)(x * TILE_SIZE), (f32)(y * TILE_SIZE), (f32)TILE_SIZE, (f32)TILE_SIZE })) {

							if (proj->type == PROJECTILE_BOULDER)
								curr->hp -= 1000; //instant kill
							else if (proj->type == PROJECTILE_FIREBALL)
								curr->hp -= 25;
							else
								curr->hp -= CANNON_DAMAGE;

							play_sound(scene->explosionBang);
							Explosion explosion = { 0 };
							explosion.animation = create_animation("explode", scene->explosion, 5, 74, 75, ANIMATION_DELAY, proj->type == PROJECTILE_CANNONBALL ? 0.6 : 1.5);
							explosion.x = proj->x + (scene->cannonBall.width / 2) - ((74 * (proj->type == PROJECTILE_CANNONBALL ? 0.6 : 1.5)) / 2);
							explosion.y = proj->y + (scene->cannonBall.height / 2) - ((75 * (proj->type == PROJECTILE_CANNONBALL ? 0.6 : 1.5)) / 2);
							game->map.explosions.push_back(explosion);
							game->map.projectiles.erase(game->map.projectiles.begin() + i);
							//gotos get a lot of shit but they are actually good for double loop breaks. The alternative is much messier and not worth it.
							goto doubleLoopBreak;
						}
					}

				}
			}

		}
	doubleLoopBreak:

		if (proj->owner == OWNER_PLAYER) {
			for (u16 j = 0; j < game->map.units.size(); ++j) {
				Unit* curr = &game->map.units[j];

				if (colliding(
					{ (f32)proj->x, (f32)proj->y, width, height },
					{ curr->pos.x + cboxOffset, curr->pos.y + cboxOffset, (f32)scene->attackers[0].width - (cboxOffset * 2), (f32)scene->attackers[0].height - (cboxOffset * 2) }
				) && curr->owner != proj->owner) {

					if (proj->type == PROJECTILE_FIREBALL) {
						for (u16 k = 0; k < game->map.units.size(); ++k) {
							Unit* u = &game->map.units[k];
							if (getDistanceE(u->pos.x + cboxOffset, u->pos.y + cboxOffset, proj->x, proj->y) < FIREBALL_RADIUS)
								u->hp -= CANNON_DAMAGE;
						}
					}
					else if (proj->type == PROJECTILE_BOULDER) {
						curr->hp -= 1000; //instant kill
					}
					else {
						curr->hp -= CANNON_DAMAGE;
					}

					play_sound(scene->explosionBang);
					Explosion explosion = { 0 };
					explosion.animation = create_animation("explode", scene->explosion, 5, 74, 75, ANIMATION_DELAY, proj->type == PROJECTILE_CANNONBALL ? 0.6 : 1.5);
					explosion.x = proj->x + (scene->cannonBall.width / 2) - ((74 * (proj->type == PROJECTILE_CANNONBALL ? 0.6 : 1.5)) / 2);
					explosion.y = proj->y + (scene->cannonBall.height / 2) - ((75 * (proj->type == PROJECTILE_CANNONBALL ? 0.6 : 1.5)) / 2);
					game->map.explosions.push_back(explosion);
					game->map.projectiles.erase(game->map.projectiles.begin() + i);
					break;
				}
			}
		}
	}

	//update explosion animation and remove when finished
	for (u32 i = 0; i < game->map.explosions.size(); ++i) {
		Explosion* curr = &game->map.explosions[i];
		update_animation(&curr->animation, game->timer);
		draw_animation(batch, curr->animation, curr->x + game->map.x, curr->y + game->map.y);
		if (curr->animation.current == curr->animation.frames - 1)
			game->map.explosions.erase(game->map.explosions.begin() + i);
	}

	if (!demo) {
		draw_text(batch, &scene->font, format_text("Current Wave: %d", game->currentWave), 80, 5);
		draw_text(batch, &scene->font, format_text("Next Wave in: %d seconds", (game->nextWaveTime - game->timer) / 100), 80, 5 + 32);
		draw_text(batch, &scene->font, format_text("Available Funds: %d gold", game->money), 80, 5 + 64);
	}

	//sidebar menu
	if (game->state == GAME_IDLE && !demo) {
		draw_panel(batch, scene->ninepatch, 0, 0, 1, 1);
		tooltip(batch, &scene->font, scene->ninepatch, "OPEN MENU\n---------------\nOpens the side\nmenu.", 7, 4, { 10, 10, (f32)scene->maximizebutton.width, (f32)scene->maximizebutton_down.height }, mouse);
		if (button(batch, scene->maximizebutton, scene->maximizebutton_down, 10, 10, mouse)) {
			play_sound(scene->click2);
			game->state = GAME_MENU;
			return;
		}
	}

	if (game->state == GAME_MENU) {
		draw_panel(batch, scene->ninepatch, 0, 0, 1, 10);
		tooltip(batch, &scene->font, scene->ninepatch, "CLOSE MENU\n---------------\nMinimizes the\nside menu.", 7, 4, { 10, 10, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->minimizebutton, scene->minimizebutton_down, 10, 10, mouse)) {
			play_sound(scene->click2);
			game->state = GAME_IDLE;
			return;
		}
		tooltip(batch, &scene->font, scene->ninepatch, "BUILD\n---------------\nOpens the menu\nfor constructing\nbuildings.", 7, 5, { 10, 70, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->buildbutton, scene->buildbuttonDown, 10, 70, mouse)) {
			play_sound(scene->click2);
			game->state = GAME_BUILD_MENU;
			return;
		}
		tooltip(batch, &scene->font, scene->ninepatch, "TRAIN\n---------------\nOpens the menu\nfor training\nunits.", 7, 5, { 10, 130, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->trainbutton, scene->trainbuttonDown, 10, 130, mouse)) {
			play_sound(scene->click2);
			game->state = GAME_BUILD_MENU;
			return;
		}
	}

	if (game->state == GAME_BUILD_MENU) {
		draw_panel(batch, scene->ninepatch, 0, 0, 1, 12);
		i32 yPos = -50;

		tooltip(batch, &scene->font, scene->ninepatch, "CANCEL\n---------------\nCancels current\noption and\nreturns to the\nprevious menu.", 7, 6, { 10, (f32)yPos + 60, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->cancelbutton, scene->cancelbutton_down, 10, yPos += 60, mouse)) {
			play_sound(scene->click2);
			game->state = GAME_MENU;
			return;
		}
		tooltip(batch, &scene->font, scene->ninepatch, "WALL\n---------------\nCOST: 100 gold\nBuilds a wall\nthat prevents\ninvaders from\npassing through.", 7, 7, { 10, (f32)yPos + 60, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->wallbutton, scene->wallbuttonDown, 10, yPos += 60, mouse)) {
			play_sound(scene->click2);
			game->selectedBuilding = BUILDING_WALL;
			game->state = GAME_BUILD;
		}
		tooltip(batch, &scene->font, scene->ninepatch, "GATE\n---------------\nCOST: 20 gold\nBuilds a gate\nthat friendly\nunits can pass\nthrough.", 7, 7, { 10, (f32)yPos + 60, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->gatebutton, scene->gatebutton_down, 10, yPos += 60, mouse)) {
			play_sound(scene->click2);
			game->selectedBuilding = BUILDING_GATE;
			game->state = GAME_BUILD;
		}
		tooltip(batch, &scene->font, scene->ninepatch, "CANNON\n---------------\nCOST: 150 gold\nBuilds a cannon\nthat shoots at\noncoming\ninvaders. Can\nonly be built on\ntop of walls.", 7, 9, { 10, (f32)yPos + 60, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->cannonbutton, scene->cannonbutton_down, 10, yPos += 60, mouse)) {
			play_sound(scene->click2);
			game->selectedBuilding = BUILDING_CANNON;
			game->state = GAME_BUILD;
		}
		tooltip(batch, &scene->font, scene->ninepatch, "MAGE\n---------------\nCOST: 400 gold\nTrains a mage\nto defend a wall.\nCasts fireballs at\noncoming invaders\nto do AOE damage.\nCan only be put on\ntop of walls.", 7, 10, { 10, (f32)yPos + 60, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->priestbutton, scene->priestbutton_down, 10, yPos += 60, mouse)) {
			play_sound(scene->click2);
			game->selectedBuilding = BUILDING_MAGE;
			game->state = GAME_BUILD;
		}
	}

	if (game->state == GAME_BUILD) {
		draw_panel(batch, scene->ninepatch, 0, 0, 1, 1);
		tooltip(batch, &scene->font, scene->ninepatch, "CANCEL\n---------------\nCancels current\noption and\nreturns to the\nprevious menu.", 7, 7, { 10, 30, (f32)scene->cancelbutton.width, (f32)scene->cancelbutton.height }, mouse);
		if (button(batch, scene->cancelbutton, scene->cancelbutton_down, 10, 10, mouse)) {
			play_sound(scene->click2);
			game->state = GAME_BUILD_MENU;
			return;
		}

		bool tileIsWater = false;
		if (game->map.grid[0][(i32)((i32)((mouse.x - game->map.x) / TILE_SIZE) + (i32)((mouse.y - game->map.y) / TILE_SIZE) * game->map.width)] == 72)
			tileIsWater = true;
		bool mouseOnSidebar = (mouse.x < scene->ninepatch[0].width * 3 && mouse.y < scene->ninepatch[0].height * 3);

		if (!mouseOnSidebar) {
			if (tileIsWater)
				draw_rectangle(batch, roundUp(mouse.x, TILE_SIZE) - TILE_SIZE, roundUp(mouse.y, TILE_SIZE) - TILE_SIZE, TILE_SIZE, TILE_SIZE, { 255, 100, 100, 100 });
			else
				draw_rectangle(batch, roundUp(mouse.x, TILE_SIZE) - TILE_SIZE, roundUp(mouse.y, TILE_SIZE) - TILE_SIZE, TILE_SIZE, TILE_SIZE, { 100, 255, 100, 100 });
		}

		bool enemiesNearby = false;
		for (u16 i = 0; i < game->map.units.size(); ++i) {
			Unit* curr = &game->map.units[i];
			if (getDistanceE(curr->pos.x, curr->pos.y, (mouse.x - game->map.x), (mouse.y - game->map.y)) < 1000) {
				enemiesNearby = true;
				break;
			}
		}

		if (enemiesNearby && is_button_down(MOUSE_BUTTON_LEFT))
			push_notification(game, "Cannot build with enemies nearby");

		i32 x = (mouse.x - game->map.x) / TILE_SIZE;
		i32 y = (mouse.y - game->map.y) / TILE_SIZE;
		//place selected building down where mouse is, then orient walls according to adjacencies
		if (is_button_down(MOUSE_BUTTON_LEFT) && !mouseOnSidebar && x >= 0 && y >= 0 && x < game->map.width && y < game->map.height && !enemiesNearby) {

			if (tileIsWater) {
				push_notification(game, "Walls cannot be placed on water");
			}
			else {

				if (game->selectedBuilding == BUILDING_WALL) {
					bool isGate = game->map.walls[x + y * game->map.width].gate;
					if (!game->map.walls[x + y * game->map.width].active || isGate) {
						if (WALL_COST <= game->money) {
							game->money -= WALL_COST;
							play_sound(scene->click);

							Wall wall = { 0 };
							wall.hp = WALL_HP;
							wall.adjacency = 6;
							wall.gate = false;
							wall.active = true;
							game->map.walls[x + y * game->map.width] = wall;

							orient_walls(&game->map);
						}
						else
							push_notification(game, "You do not have enough gold to build a wall");
					}
					else if (is_button_pressed(MOUSE_BUTTON_LEFT))
						push_notification(game, "Building location blocked");
				}
				if (game->selectedBuilding == BUILDING_GATE) {
					if (
						(game->map.walls[(x + 1) + y * game->map.width].active && game->map.walls[(x - 1) + y * game->map.width].active) ||
						(game->map.walls[x + (y + 1) * game->map.width].active && game->map.walls[x + (y - 1)*game->map.width].active)
						) {

						if (!game->map.walls[x + y * game->map.width].gate) {
							bool canPlace = false;
							if (game->map.walls[x + y * game->map.width].active) {
								if (GATE_COST <= game->money) {
									game->money -= GATE_COST;
									canPlace = true;
								}
								else
									push_notification(game, "You do not have enough gold to build a gate");
							}
							if (game->map.walls[x + y * game->map.width].active == false) {
								if (WALL_COST + GATE_COST <= game->money) {
									game->money -= WALL_COST + GATE_COST;
									canPlace = true;
								}
							}

							if (canPlace) {
								play_sound(scene->click);
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
				}
				if (game->selectedBuilding == BUILDING_CANNON) {
					bool cannonPresent = false;
					for (u16 i = 0; i < game->map.turrets.size(); ++i) {
						if (game->map.turrets[i].x == x && game->map.turrets[i].y == y) {
							cannonPresent = true;
							break;
						}
					}
					if (game->map.walls[x + y * game->map.width].active && !cannonPresent) {
						if (CANNON_COST <= game->money) {
							game->money -= CANNON_COST;
							play_sound(scene->click);

							Turret cannon = { 0 };
							cannon.type = TURRET_CANNON;
							cannon.shotDelay = 140;
							cannon.x = (mouse.x - game->map.x) / TILE_SIZE;
							cannon.y = (mouse.y - game->map.y) / TILE_SIZE;
							game->map.turrets.push_back(cannon);
						}
						else
							push_notification(game, "You do not have enough gold to build a cannon");
					}
					else if (is_button_pressed(MOUSE_BUTTON_LEFT))
						push_notification(game, "You must place the cannon on an empty wall");
				}
				if (game->selectedBuilding == BUILDING_MAGE) {
					bool cannonPresent = false;
					for (u16 i = 0; i < game->map.turrets.size(); ++i) {
						if (game->map.turrets[i].x == x && game->map.turrets[i].y == y) {
							cannonPresent = true;
							break;
						}
					}
					if (game->map.walls[x + y * game->map.width].active && !cannonPresent) {
						if (MAGE_COST <= game->money) {
							game->money -= MAGE_COST;
							play_sound(scene->click);

							Turret mage = { 0 };
							mage.type = TURRET_MAGE;
							mage.shotDelay = 300;
							mage.x = (mouse.x - game->map.x) / TILE_SIZE;
							mage.y = (mouse.y - game->map.y) / TILE_SIZE;
							game->map.turrets.push_back(mage);
						}
						else
							push_notification(game, "You do not have enough gold to train a mage");
					}
					else if (is_button_pressed(MOUSE_BUTTON_LEFT))
						push_notification(game, "You must place the mage on an empty wall");
				}
			}
		}
	}
	//notifications
	if (!demo) {
		i16 y = (get_window_height() / 2) - ((game->notifications.size() * 30) / 2);
		for (u16 i = 0; i < game->notifications.size(); ++i) {
			Notification* curr = &game->notifications[i];
			curr->alpha -= 1;
			draw_text(batch, &scene->font, curr->text.c_str(), 10, y, 255, 100, 100, curr->alpha);
			y += 30;
			if (curr->alpha <= 0) {
				game->notifications.erase(game->notifications.begin() + i);
				break;
			}
		}
	}
	for (u16 i = 0; i < game->statusTexts.size(); ++i) {
		StatusText* curr = &game->statusTexts[i];
		curr->alpha -= 1;
		curr->pos.y -= 1;
		draw_text(batch, &scene->font, curr->text.c_str(), curr->pos.x + game->map.x, curr->pos.y + game->map.y, 255, 255, 255, curr->alpha);
		if (curr->alpha <= 0) {
			game->statusTexts.erase(game->statusTexts.begin() + i);
			break;
		}
	}

	//move camera
	if (!demo) {
		if (is_key_down(KEY_LEFT) && game->map.x < 0)
			game->map.x += 8;
		if (is_key_down(KEY_RIGHT) && game->map.x > (-game->map.width * TILE_SIZE) + get_window_width())
			game->map.x -= 8;
		if (is_key_down(KEY_DOWN) && game->map.y > (-game->map.height * TILE_SIZE) + get_window_height())
			game->map.y -= 8;
		if (is_key_down(KEY_UP) && game->map.y < 0)
			game->map.y += 8;
	}
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
			fprintf(file, "%d %d %d %d %d %d\n", curr->pos.x, curr->pos.y, curr->hp, curr->maxHp, (i32)curr->type, (i32)curr->owner);
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
			if (mouse.x > curr->pos.x + map->x && mouse.y > curr->pos.y + map->y && mouse.x < curr->pos.x + 100 + map->x && mouse.y < curr->pos.y + 100 + map->y) {
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
				ship.owner = (Owner)(editor->selectedShip - 1);
				ship.hp = ship.maxHp = 100;
				ship.type = UNIT_ELITE_SHIP;
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