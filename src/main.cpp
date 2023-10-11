#include "raylib.h"
#include "raymath.h"

#include <iostream>
#include <vector>
#include <array>
#include <list>

#include "pallette.h"
#include "cp437_8x8.h"

constexpr size_t WIDTH = 80;
constexpr size_t HEIGHT = 60;
constexpr size_t TOTAL_TILES = WIDTH * HEIGHT;
constexpr float FONT_SIZE = 8;

constexpr int FRAME_PER_SECOND = 60;

const Image CP437_8X8{
	CP437_8X8_DATA,
	CP437_8X8_WIDTH,
	CP437_8X8_HEIGHT,
	1,
	CP437_8X8_FORMAT
};

enum class Scene
{
	START_SCENE,
	MAIN_GAME,
	GAME_OVER,
	VICTORY
};

struct Group {
	int width, offset_x, offset_y;
	std::vector<unsigned char> codepoints;
	std::vector<unsigned char> colors;

	constexpr Group(int width, int offset_x, int offset_y, std::vector<unsigned char> codepoints, std::vector<unsigned char> colors) : width(width), offset_x(offset_x), offset_y(offset_y), codepoints(codepoints), colors(colors) {};
};

const Group PLAYER_GROUP = Group(
	5, -2, -1,
	{
		0x00, 0x00, 0xef, 0x00, 0x00,
		0x00, 0xb4, 0x7f, 0xc3, 0x00,
		0x2f, 0x5b, 0xba, 0x5d, 0x5c,
		0x00, 0x5c, 0xc4, 0x2f, 0x00
	},
	{
		0x00, 0x00, 0x0f, 0x00, 0x00,
		0x00, 0x0f, 0x07, 0x0f, 0x00,
		0x9f, 0x9f, 0x0f, 0x9f, 0x9f,
		0x00, 0x9f, 0x9f, 0x9f, 0x00
	}
	);

const Group BOSS_WING_BASE = Group(
	16, 0, 1,
	{
		0xda, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xbf,
		0xb3, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb3,
		0xb3, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb3,
		0xb3, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb3,
		0xb3, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb2, 0xb3,
		0xc0, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc2, 0xc4, 0xc4, 0xc2, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xd9,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xb2, 0xb2, 0xd9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb2, 0xb2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9a, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{
		0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c,
		0x9c, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x9c,
		0x9c, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x9c,
		0x9c, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x9c,
		0x9c, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x9c,
		0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0x17, 0x17, 0x9c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0x9c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	}
	);

const Group BOSS_WING_COVER = Group(
	16, 0, 6,
	{
		0xc3, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xb4,
		0xb3, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb3,
		0xc0, 0xc4, 0xc2, 0xc2, 0xc4, 0xc4, 0xc4, 0xc2, 0xc2, 0xc4, 0xc4, 0xc4, 0xc2, 0xc2, 0xc4, 0xd9,

		0x00, 0x00, 0xb2, 0xb2, 0x00, 0x00, 0x00, 0xb2, 0xb2, 0x00, 0x00, 0x00, 0xb2, 0xb2, 0x00, 0x00,
	},
	{
		0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c,
		0x9c, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0x9c,
		0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c,

		0x00, 0x00, 0x9e, 0x9e, 0x00, 0x00, 0x00, 0x9e, 0x9e, 0x00, 0x00, 0x00, 0x9e, 0x9e, 0x00, 0x00,
	}
	);

const Group BOSS_BODY_BASE = Group(
	14, 0, 0,
	{
		0x00, 0x00, 0x00, 0xc9, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xbb, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xba, 0xba, 0x09, 0xcd, 0xcd, 0xcd, 0xcd, 0x09, 0xba, 0xba, 0x00, 0x00,
		0xcd, 0xcd, 0xba, 0xba, 0xba, 0x2f, 0xb0, 0xb0, 0x5c, 0xba, 0xba, 0xba, 0xcd, 0xcd,
		0x00, 0x00, 0xba, 0xba, 0xba, 0xb0, 0xb2, 0xb2, 0xb0, 0xba, 0xba, 0xba, 0x00, 0x00,
		0x00, 0x00, 0xba, 0xba, 0xba, 0xb0, 0xb2, 0xb2, 0xb0, 0xba, 0xba, 0xba, 0x00, 0x00,
		0xcd, 0xcd, 0xba, 0xba, 0xba, 0x5c, 0xb0, 0xb0, 0x2f, 0xba, 0xba, 0xba, 0xcd, 0xcd,
		0x00, 0x00, 0xba, 0xba, 0x09, 0xcd, 0xcd, 0xcd, 0xcd, 0x09, 0xba, 0xba, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xcc, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xb9, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xba, 0xc4, 0xb0, 0xb2, 0xb2, 0xb0, 0xc4, 0xba, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x9a, 0xc4, 0xb0, 0xb2, 0xb2, 0xb0, 0xc4, 0x9a, 0x00, 0x00, 0x00,
	},
	{
		0x00, 0x00, 0x00, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x9e, 0x9c, 0x9c, 0x9e, 0x9e, 0x9e, 0x9e, 0x9c, 0x9c, 0x9e, 0x00, 0x00,
		0x9e, 0x9e, 0x9e, 0x9c, 0x9e, 0x9e, 0x17, 0x17, 0x9e, 0x9e, 0x9c, 0x9e, 0x9e, 0x9e,
		0x00, 0x00, 0x9e, 0x9c, 0x9e, 0x17, 0x17, 0x17, 0x17, 0x9e, 0x9c, 0x9e, 0x00, 0x00,
		0x00, 0x00, 0x9e, 0x9c, 0x9e, 0x17, 0x17, 0x17, 0x17, 0x9e, 0x9c, 0x9e, 0x00, 0x00,
		0x9e, 0x9e, 0x9e, 0x9c, 0x9e, 0x9e, 0x17, 0x17, 0x9e, 0x9e, 0x9c, 0x9e, 0x9e, 0x9e,
		0x00, 0x00, 0x9e, 0x9c, 0x9c, 0x9e, 0x9e, 0x9e, 0x9e, 0x9c, 0x9c, 0x9e, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x9c, 0x9e, 0x17, 0x17, 0x17, 0x17, 0x9e, 0x9c, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x9c, 0x9e, 0x17, 0x17, 0x17, 0x17, 0x9e, 0x9c, 0x00, 0x00, 0x00,
	}
	);

const Group BOSS_BODY_COVER = Group(
	8, 3, 8,
	{
		0x00, 0x2f, 0xb0, 0xb0, 0xb0, 0xb0, 0x5c, 0x00,
		0x00, 0x2f, 0xb2, 0xb2, 0xb2, 0xb2, 0x5c, 0x00,
		0xef, 0x2f, 0xb2, 0xb2, 0xb2, 0xb2, 0x5c, 0xef,
		0x9a, 0xb3, 0xb2, 0xb2, 0xb2, 0xb2, 0xb3, 0x9a,
		0xb3, 0xb3, 0xc4, 0xc4, 0xc4, 0xc4, 0xb3, 0xb3,
		0xc3, 0x2f, 0xc4, 0xc4, 0xc4, 0xc4, 0x5c, 0xb4,
		0x5c, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x5c, 0x2f,
	},
	{
		0x00, 0x9e, 0xb7, 0xb7, 0xb7, 0xb7, 0x9e, 0x00,
		0x00, 0x9e, 0xb7, 0xb7, 0xb7, 0xb7, 0x9e, 0x00,
		0x9e, 0x9e, 0xb7, 0xb7, 0xb7, 0xb7, 0x9e, 0x9e,
		0x9e, 0x9e, 0xb7, 0xb7, 0xb7, 0xb7, 0x9e, 0x9e,
		0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e,
		0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e,
		0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e,
	}
	);

struct Screen {
	Texture2D cp437_8x8;

	std::array<unsigned char, TOTAL_TILES> codepoints;
	std::array<unsigned char, TOTAL_TILES> colors;

	Screen(const char* title) {
		InitWindow(WIDTH * FONT_SIZE, HEIGHT * FONT_SIZE, title);
		cp437_8x8 = LoadTextureFromImage(CP437_8X8);
		ClearScreen();
	}

	~Screen() {
		UnloadTexture(cp437_8x8);
		CloseWindow();
	}

	void DrawTile(int x, int y, unsigned char codepoint, unsigned char color) {
		if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && codepoint != 0x00) {
			codepoints.at(x + y * WIDTH) = codepoint;
			colors.at(x + y * WIDTH) = color;
		}
	}

	void DrawGroup(int x, int y, const Group& group, bool override_color = false, const unsigned char& color_to_override = 0xbf) {
		for (int i = 0; i < group.codepoints.size(); ++i) {
			DrawTile(x + group.offset_x + i % group.width, y + group.offset_y + i / group.width, group.codepoints[i], override_color ? color_to_override : group.colors[i]);
		}
	}

	void DrawBorder(
		unsigned char tl, unsigned char tr,
		unsigned char bl, unsigned char br,
		unsigned char ht, unsigned char vt,
		unsigned char color
	) {
		DrawTile(0, 0, tl, color);
		DrawTile(WIDTH - 1, 0, tr, color);
		DrawTile(0, HEIGHT - 1, bl, color);
		DrawTile(WIDTH - 1, HEIGHT - 1, br, color);
		for (int i = 1; i < WIDTH - 1; ++i) {
			DrawTile(i, 0, ht, color);
			DrawTile(i, HEIGHT - 1, ht, color);
		}
		for (int i = 1; i < HEIGHT - 1; ++i) {
			DrawTile(0, i, vt, color);
			DrawTile(WIDTH - 1, i, vt, color);
		}
	}

	void DrawText(const char* text, int x, int y, unsigned char color) {
		int textOffsetX = 0, textOffsetY = 0;
		for (int i = 0; i < TextLength(text); ++i) {
			if (text[i] == '\n') {
				textOffsetX = 0;
				++textOffsetY;
			}
			else {
				DrawTile(x + textOffsetX, y + textOffsetY, text[i], color);
				++textOffsetX;
			}
		}
	}

	void ClearScreen() {
		codepoints.fill(0x20);
		colors.fill(0x00);
	}

	void DrawScreen() const {
		for (int i = 0; i < TOTAL_TILES; ++i) {
			DrawTexturePro(cp437_8x8, SourceRect(codepoints.at(i)), DestRect(i), { 0, 0 }, 0, PALLETTE[colors.at(i)]);
		}
	}

	static constexpr Rectangle SourceRect(unsigned char codepoint) {
		return { static_cast<float>(8 * (codepoint % 16)), static_cast<float>(8 * (codepoint / 16)), 8, 8 };
	}

	static constexpr Rectangle DestRect(int tile_index) {
		return { FONT_SIZE * (tile_index % WIDTH), FONT_SIZE * (tile_index / WIDTH), FONT_SIZE, FONT_SIZE };
	}
};

struct Bullet {
	float pos_x, pos_y, angle, speed, accel, maxspeed;
	bool capped;
	Bullet(float pos_x, float pos_y, float angle, float speed, float accel = 0, float maxspeed = 0) :
		pos_x(pos_x),
		pos_y(pos_y),
		angle(angle),
		speed(speed),
		accel(accel),
		maxspeed(maxspeed),
		capped(accel >= 0 ? (speed >= maxspeed) : (speed <= maxspeed))
	{}

	void Update() {
		if (!capped) {
			speed += accel;
			capped = accel >= 0 ? (speed >= maxspeed) : (speed <= maxspeed);
		}

		pos_x += cosf(angle) * speed;
		pos_y -= sinf(angle) * speed;
	}

	int GetX() const { return static_cast<int>(pos_x + 0.5f); }
	int GetY() const { return static_cast<int>(pos_y + 0.5f); }
};

struct Boss {
	static constexpr int TOTAL_HEALTH = 1200;
	static constexpr int BODY_COVER_HEALTH = 200;
	static constexpr int WING_HEALTH = 400;
	static constexpr int WING_COVER_HEALTH = 200;


	float pos_x, pos_y, vel_x, vel_y, acc_x, acc_y;

	int total_health = TOTAL_HEALTH, body_cover_health = BODY_COVER_HEALTH, left_wing_health = WING_HEALTH, right_wing_health = WING_HEALTH;
	int flash_body_base_cd = 0, flash_body_cover_cd = 0, flash_left_wing_base_cd = 0, flash_left_wing_cover_cd = 0, flash_right_wing_base_cd = 0, flash_right_wing_cover_cd = 0;

	enum class State
	{
		ENTERING,
		LEFT, RIGHT,
		FINAL_INTO_POSITION,
		FINAL_SHOOTING
	};
	State state = State::ENTERING;

	int state_cd = FRAME_PER_SECOND;
	int wing_shot_cd = FRAME_PER_SECOND + 30;
	int body_cover_shot_cd = FRAME_PER_SECOND;
	int final_state_shot_cd = 0;

	void Update() {
		vel_x += acc_x;
		vel_y += acc_y;

		pos_x += vel_x;
		pos_y += vel_y;

		if (left_wing_health <= 0 && right_wing_health <= 0 && body_cover_health <= 0 && state != State::FINAL_INTO_POSITION && state != State::FINAL_SHOOTING) {
			state = State::FINAL_INTO_POSITION;
			state_cd = static_cast<float>(FRAME_PER_SECOND);
			vel_x = 2 * (static_cast<float>(WIDTH) / 2 - 23 - pos_x) / state_cd;
			vel_y = 2 * (static_cast<float>(HEIGHT) / 2 - 4 - pos_y) / state_cd;
			acc_x = -vel_x / state_cd;
			acc_y = -vel_y / state_cd;
		}

		if (state_cd <= 0) {
			if (state == State::ENTERING || state == State::RIGHT) {
				state = State::LEFT;
				vel_x = -0.20;
				state_cd = 200;
				acc_x = -2 * vel_x / state_cd;
				vel_y = 0;
				acc_y = 0;
			}
			else if (state == State::LEFT) {
				state = State::RIGHT;
				vel_x = 0.20;
				state_cd = 200;
				acc_x = -2 * vel_x / state_cd;
				vel_y = 0;
				acc_y = 0;
			}
			else if (state == State::FINAL_INTO_POSITION) {
				state = State::FINAL_SHOOTING;
				vel_x = 0;
				vel_y = 0;
				acc_x = 0;
				acc_y = 0;
			}
		}

		if (wing_shot_cd <= 0) {
			wing_shot_cd = 30;
		}

		if (body_cover_shot_cd <= 0) {
			body_cover_shot_cd = 80;
		}

		if (final_state_shot_cd <= 0) {
			final_state_shot_cd = 10;
		}

		--state_cd;
		--wing_shot_cd;
		--body_cover_shot_cd;
		--final_state_shot_cd;
		--flash_body_base_cd;
		--flash_body_cover_cd;
		--flash_left_wing_base_cd;
		--flash_left_wing_cover_cd;
		--flash_right_wing_base_cd;
		--flash_right_wing_cover_cd;
	}

	std::vector<Bullet> Shoot(int player_x, int player_y) const {
		std::vector<Bullet> to_shoot;
		if (wing_shot_cd <= 0) {
			if (left_wing_health > WING_HEALTH - WING_COVER_HEALTH) {
				for (float i = 0; i < 3; ++i) {
					to_shoot.emplace_back(pos_x + 2.f, pos_y + 9, -PI / 2.f - PI / 6.f, 0.4f + i * 0.05f);
					to_shoot.emplace_back(pos_x + 7.f, pos_y + 9, -PI / 2.f, 0.4f + i * 0.05f);
					to_shoot.emplace_back(pos_x + 12.f, pos_y + 9, -PI / 2.f + PI / 6.f, 0.4f + i * 0.05f);
				}
			}
			else if (left_wing_health > 0) {
				for (float i = 1; i < 7; i += 2) {
					to_shoot.emplace_back(pos_x + 7.f, pos_y + 9, -i * PI / 7, 0.3f);
					to_shoot.emplace_back(pos_x + 7.f, pos_y + 9, -i * PI / 7 - PI / 21, 0.3f);
					to_shoot.emplace_back(pos_x + 7.f, pos_y + 9, -i * PI / 7 - 2 * PI / 21, 0.3f);
					to_shoot.emplace_back(pos_x + 7.f, pos_y + 9, -i * PI / 7 - PI / 7, 0.3f);
				}
			}
			if (right_wing_health > WING_HEALTH - WING_COVER_HEALTH) {
				for (float i = 0; i < 3; ++i) {
					to_shoot.emplace_back(pos_x + 32.f, pos_y + 9, -PI / 2.f - PI / 6.f, 0.4f + i * 0.05f);
					to_shoot.emplace_back(pos_x + 37.f, pos_y + 9, -PI / 2.f, 0.4f + i * 0.05f);
					to_shoot.emplace_back(pos_x + 42.f, pos_y + 9, -PI / 2.f + PI / 6.f, 0.4f + i * 0.05f);
				}
			}
			else if (right_wing_health > 0) {
				for (float i = 1; i < 7; i += 2) {
					to_shoot.emplace_back(pos_x + 37.f, pos_y + 9, -i * PI / 7, 0.3f);
					to_shoot.emplace_back(pos_x + 37.f, pos_y + 9, -i * PI / 7 - PI / 21, 0.3f);
					to_shoot.emplace_back(pos_x + 37.f, pos_y + 9, -i * PI / 7 - 2 * PI / 21, 0.3f);
					to_shoot.emplace_back(pos_x + 37.f, pos_y + 9, -i * PI / 7 - PI / 7, 0.3f);
				}
			}
		}
		if (body_cover_health > 0 && body_cover_shot_cd <= 0) {
			to_shoot.emplace_back(pos_x + 23.f, pos_y + 16.f + 1, atan2f(pos_y + 16.f - static_cast<float>(player_y), static_cast<float>(player_x) - (pos_x + 23.f)), 0.4f);
			to_shoot.emplace_back(pos_x + 23.f, pos_y + 16.f - 1, atan2f(pos_y + 16.f - static_cast<float>(player_y), static_cast<float>(player_x) - (pos_x + 23.f)), 0.4f);
			to_shoot.emplace_back(pos_x + 23.f + 1, pos_y + 16.f, atan2f(pos_y + 16.f - static_cast<float>(player_y), static_cast<float>(player_x) - (pos_x + 23.f)), 0.4f);
			to_shoot.emplace_back(pos_x + 23.f - 1, pos_y + 16.f, atan2f(pos_y + 16.f - static_cast<float>(player_y), static_cast<float>(player_x) - (pos_x + 23.f)), 0.4f);
		}
		if (state == State::FINAL_SHOOTING && final_state_shot_cd <= 0) {
			constexpr float STAR_SHOTS = 16;
			constexpr float ANGLE_PIECE = 2 * PI / STAR_SHOTS;
			for (float i = 0; i < STAR_SHOTS; ++i) {
				to_shoot.emplace_back(pos_x + 23.f, pos_y + 4.f, atan2f(pos_y + 4.f - static_cast<float>(player_y), static_cast<float>(player_x) - (pos_x + 23.f)) + i * ANGLE_PIECE, 0.5f);
			}
		}
		return to_shoot;
	}

	bool CheckCollision(int x, int y) {
		if (state == State::ENTERING) return false;

		int rel_x = x - static_cast<int>(pos_x), rel_y = y - static_cast<int>(pos_y);

		if (left_wing_health > 0 && rel_x >= 0 && rel_x < 16 && rel_y >= 1 && rel_y < 7) {
			--left_wing_health;
			if (left_wing_health > WING_HEALTH - WING_COVER_HEALTH)
				flash_left_wing_cover_cd = 2;
			else
				flash_left_wing_base_cd = 2;
			if (left_wing_health <= 0) {
				total_health -= WING_HEALTH;
			}
			return true;
		}

		if (right_wing_health > 0 && rel_x >= 30 && rel_x < 46 && rel_y >= 1 && rel_y < 7) {
			--right_wing_health;
			if (right_wing_health > WING_HEALTH - WING_COVER_HEALTH)
				flash_right_wing_cover_cd = 2;
			else
				flash_right_wing_base_cd = 2;
			if (right_wing_health <= 0) {
				total_health -= WING_HEALTH;
			}
			return true;
		}

		if (body_cover_health > 0 && rel_x >= 19 && rel_x < 27 && rel_y >= 8 && rel_y < 14) {
			--body_cover_health;
			flash_body_cover_cd = 2;
			if (body_cover_health <= 0) {
				total_health -= BODY_COVER_HEALTH;
			}
			return true;
		}
		if (total_health > 0 && rel_x >= 19 && rel_x < 27 && rel_y >= 0 && rel_y < 8) {
			--total_health;
			flash_body_base_cd = 2;
			return true;
		}
		return false;
	}

	void Draw(Screen& sc) {
		sc.DrawGroup(pos_x + 16, pos_y, BOSS_BODY_BASE, flash_body_base_cd > 0);
		if (left_wing_health > 0) {
			sc.DrawGroup(static_cast<int>(pos_x), static_cast<int>(pos_y), BOSS_WING_BASE, flash_left_wing_base_cd > 0);
		}
		if (left_wing_health > WING_HEALTH - WING_COVER_HEALTH) {
			sc.DrawGroup(static_cast<int>(pos_x), static_cast<int>(pos_y), BOSS_WING_COVER, flash_left_wing_cover_cd > 0);
		}
		if (right_wing_health > 0) {
			sc.DrawGroup(static_cast<int>(pos_x) + 30, static_cast<int>(pos_y), BOSS_WING_BASE, flash_right_wing_base_cd > 0);
		}
		if (right_wing_health > WING_HEALTH - WING_COVER_HEALTH) {
			sc.DrawGroup(static_cast<int>(pos_x) + 30, static_cast<int>(pos_y), BOSS_WING_COVER, flash_right_wing_cover_cd > 0);
		}
		if (body_cover_health > 0) {
			sc.DrawGroup(static_cast<int>(pos_x) + 16, static_cast<int>(pos_y), BOSS_BODY_COVER, flash_body_cover_cd > 0);
		}
	}
};

struct GameManager {
	int player_x = WIDTH / 2, player_y = HEIGHT - 10, player_lives = 3;

	Boss boss{ WIDTH / 2 - 23, 0, 0, 0.30, 0, -0.005 };

	std::list<Bullet> player_bullets;
	std::list<Bullet> boss_bullets;
	int shot_cd = 0;
	int iframe_cd = 0;

	void Update() {
		int dir_x = 0, dir_y = 0;
		if (IsKeyDown(KEY_UP)) --dir_y;
		if (IsKeyDown(KEY_DOWN)) ++dir_y;
		if (IsKeyDown(KEY_LEFT)) --dir_x;
		if (IsKeyDown(KEY_RIGHT)) ++dir_x;

		if (IsKeyDown(KEY_C) and shot_cd <= 0) {
			player_bullets.emplace_back(player_x - 1, player_y - 1, PI / 2, 1);
			player_bullets.emplace_back(player_x, player_y - 2, PI / 2, 1);
			player_bullets.emplace_back(player_x + 1, player_y - 1, PI / 2, 1);
			shot_cd = 5;
		}

		if (boss.total_health > 0) {
			std::vector<Bullet> boss_bullets_to_spawn = boss.Shoot(player_x, player_y);
			boss_bullets.insert(boss_bullets.end(), boss_bullets_to_spawn.begin(), boss_bullets_to_spawn.end());
		}

		player_x = std::min(std::max(player_x + dir_x, 1), static_cast<int>(WIDTH - 2));
		player_y = std::min(std::max(player_y + dir_y, 1), static_cast<int>(HEIGHT - 2));

		if (boss.CheckCollision(player_x, player_y) && iframe_cd <= 0) {
			--player_lives;
			player_x = WIDTH / 2;
			player_y = HEIGHT - 10;
			iframe_cd = 2 * FRAME_PER_SECOND;
		}

		std::vector<std::list<Bullet>::iterator> player_bullets_to_remove;
		for (auto it = player_bullets.begin(); it != player_bullets.end(); ++it) {
			it->Update();
			if (it->pos_x < 1 || it->pos_x > WIDTH - 2 || it->pos_y < 1 || it->pos_y > HEIGHT - 2) {
				player_bullets_to_remove.push_back(it);
			}
			else if (boss.CheckCollision(it->pos_x, it->pos_y)) {
				player_bullets_to_remove.push_back(it);
			}
		}
		for (const std::list<Bullet>::iterator& it : player_bullets_to_remove) {
			player_bullets.erase(it);
		}

		std::vector<std::list<Bullet>::iterator> boss_bullets_to_remove;
		for (auto it = boss_bullets.begin(); it != boss_bullets.end(); ++it) {
			it->Update();
			if (it->pos_x < 1 || it->pos_x > WIDTH - 2 || it->pos_y < 1 || it->pos_y > HEIGHT - 2) {
				boss_bullets_to_remove.push_back(it);
			}
			else if (it->GetX() == player_x && it->GetY() == player_y) {
				boss_bullets_to_remove.push_back(it);
				if (iframe_cd <= 0) {
					--player_lives;
					player_x = WIDTH / 2;
					player_y = HEIGHT - 10;
					iframe_cd = 2 * FRAME_PER_SECOND;
				}
			}
		}
		for (const std::list<Bullet>::iterator& it : boss_bullets_to_remove) {
			boss_bullets.erase(it);
		}

		boss.Update();

		--shot_cd;
		--iframe_cd;
	}

	void Draw(Screen& sc) {
		sc.DrawGroup(player_x, player_y, PLAYER_GROUP, (iframe_cd / 8) % 2 == 1);
		for (const Bullet& player_bullet : player_bullets) {
			sc.DrawTile(player_bullet.GetX(), player_bullet.GetY(), 0x13, 0x37);
		}
		for (const Bullet& boss_bullet : boss_bullets) {
			sc.DrawTile(boss_bullet.GetX(), boss_bullet.GetY(), 0x04, 0x4f);
		}
		if (boss.total_health > 0)
			boss.Draw(sc);
		sc.DrawBorder(0xc9, 0xbb, 0xc8, 0xbc, 0xcd, 0xba, 0x9f);
		sc.DrawText(" LIVES:  ", 3, HEIGHT - 1, 0xbf);
		sc.DrawText(TextFormat("%d", player_lives), 10, HEIGHT - 1, 0x07);
	}
};

int main()
{
	Scene current_scene = Scene::START_SCENE;
	Screen sc("u tell me a Tung text-based this game jam");
	GameManager g;
	SetTargetFPS(FRAME_PER_SECOND);
	while (!WindowShouldClose())
	{
		switch (current_scene) {
		case Scene::START_SCENE:
			sc.ClearScreen();
			sc.DrawBorder(0xc9, 0xbb, 0xc8, 0xbc, 0xcd, 0xba, 0x9f);
			sc.DrawText("An entry for GDC 4th text-based game jam", 1, 1, 0xbf);
			sc.DrawText(
				"  _____                            _   _\n\
 |  __ \\                          | | (_)\n\
 | |  | | ___  _ __ ___   ___  ___| |_ _  ___\n\
 | |  | |/ _ \\| '_ ` _ \\ / _ \\/ __| __| |/ __|\n\
 | |__| | (_) | | | | | |  __/\\__ \\ |_| | (__ \n\
 |_____/ \\___/|_| |_| |_|\\___||___/\\__|_|\\___|\n\
 | |                         (_)              \n\
 | |_ ___ _ __ _ __ ___  _ __ _ ___ _ __ ___  \n\
 | __/ _ \\ '__| '__/ _ \\| '__| / __| '_ ` _ \\ \n\
 | ||  __/ |  | | | (_) | |  | \__ \ | | | | | |\n\
  \\__\\___|_|  |_|  \\___/|_|  |_|___/_| |_| |_|", 16, 16, 0xbf);

			sc.DrawText("Arrow keys to move\n\nC to shoot\n\n\n\nPress C to start", 16, 32, 0xbf);

			sc.DrawText("Made in raylib", 1, HEIGHT - 2, 0xbf);
			if (IsKeyPressed(KEY_C)) {
				current_scene = Scene::MAIN_GAME;
			}
			break;
		case Scene::MAIN_GAME:
			sc.ClearScreen();
			if (g.player_lives >= 0) {
				g.Update();
				g.Draw(sc);
				if (g.boss.total_health <= 0) {
					current_scene = Scene::VICTORY;
				}
			}
			else {
				current_scene = Scene::GAME_OVER;
			}
			break;
		case Scene::GAME_OVER:
			sc.ClearScreen();
			sc.DrawBorder(0xc9, 0xbb, 0xc8, 0xbc, 0xcd, 0xba, 0x9f);

			sc.DrawText("Damn, you failed", 16, 16, 0xbf);

			sc.DrawText("Press C to try again", 16, 32, 0xbf);

			if (IsKeyPressed(KEY_C)) {
				g = GameManager();
				current_scene = Scene::MAIN_GAME;
			}
			break;
		case Scene::VICTORY:
			sc.ClearScreen();
			sc.DrawBorder(0xc9, 0xbb, 0xc8, 0xbc, 0xcd, 0xba, 0x9f);

			sc.DrawText("You have successfully domestically terrorized space", 16, 16, 0xbf);

			sc.DrawText("Press C to try again", 16, 32, 0xbf);

			if (IsKeyPressed(KEY_C)) {
				g = GameManager();
				current_scene = Scene::MAIN_GAME;
			}
			break;
		}

		BeginDrawing();
		ClearBackground(BLACK);
		sc.DrawScreen();
		EndDrawing();
	}
	return 0;
}