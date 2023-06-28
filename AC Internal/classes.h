#pragma once
#include <iostream>

struct Vector3 { float x, y, z; };
struct Vector2 { float x, y; };


class Player
{
public:
	bool isEnemy(Player* player) {
		return this->teamId != player->teamId;
	}
	char pad_0000[52]; //0x0000
	Vector3 position; //0x0034
	Vector3 angle; //0x0040
	char pad_004C[172]; //0x004C
	int32_t health; //0x00F8
	int32_t shield; //0x00FC
	char pad_0100[80]; //0x0100
	int32_t ammo; //0x0150
	char pad_0154[4]; //0x0154
	int32_t grenades; //0x0158
	char pad_015C[201]; //0x015C
	char name[16]; //0x0225
	char pad_0235[247]; //0x0235
	int32_t teamId; //0x032C
	char pad_0330[68]; //0x0330
	class Weapon* currentWeapon; //0x0374
	char pad_0378[288]; //0x0378
}; //Size: 0x0498

class Engine
{
public:
	char EntityList[1][4]; //0x0000
}; //Size: 0x0004

class Weapon
{
public:
	char pad_0000[20]; //0x0000
	class WeaponClip* clip; //0x0014
	char pad_0018[244]; //0x0018
}; //Size: 0x010C

class WeaponClip
{
public:
	int32_t ammo; //0x0000
}; //Size: 0x0004