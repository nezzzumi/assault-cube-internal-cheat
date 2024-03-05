#pragma once
#include <iostream>

struct Vector3 { float x, y, z; };
struct Vector2 { float x, y; };
#define M_PI 3.14159265358979323846

//class Player
//{
//public:
//	bool isEnemy(Player* player) {
//		return this->teamId != player->teamId;
//	}
//	char pad_0000[52]; //0x0000
//	Vector3 position; //0x0034
//	Vector3 angle; //0x0040
//	char pad_004C[172]; //0x004C
//	int32_t health; //0x00F8
//	int32_t shield; //0x00FC
//	char pad_0100[80]; //0x0100
//	int32_t ammo; //0x0150
//	char pad_0154[4]; //0x0154
//	int32_t grenades; //0x0158
//	char pad_015C[201]; //0x015C
//	char name[16]; //0x0225
//	char pad_0235[247]; //0x0235
//	int32_t teamId; //0x032C
//	char pad_0330[8]; //0x0330
//	bool isDead; //0x0338
//	char pad_0339[59]; //0x0339
//	class Weapon* currentWeapon; //0x0374
//	char pad_0378[288]; //0x0378
//}; //Size: 0x0498

class Player
{
public:
	char pad_0000[4]; //0x0000
	Vector3 position; //0x0004
	char pad_0010[36]; //0x0010
	Vector3 angle; //0x0034
	char pad_0040[172]; //0x0040
	int32_t health; //0x00EC
	int32_t shield; //0x00F0
	char pad_00F4[76]; //0x00F4
	int32_t ammo; //0x0140
	int32_t grenades; //0x0144
	char pad_0148[464]; //0x0148
	bool dead; //0x0318
	char pad_0319[75]; //0x0319
	class Weapon* currentWeapon; //0x0364
}; //Size: 0x0368

class Weapon
{
public:
	char pad_0000[16]; //0x0000
	class WeaponMagazine* magazines; //0x0010
	class WeaponMagazine* currentMagazine; //0x0014
}; //Size: 0x0018

class WeaponMagazine
{
public:
	int32_t ammo; //0x0000
}; //Size: 0x0004

// movss xmm2,xmm2
BYTE recoilPatchBytes[] = { 0xf3, 0xf, 0x10, 0xd2, 0x90 };
// movss dword [esi+0x38], xmm2
BYTE recoilOriginalBytes[] = { 0xf3, 0xf, 0x11, 0x56, 0x38 };
