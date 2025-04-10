#pragma once
#include <cstdint>
#include "geom.h";

// Created with ReClass.NET 1.2 by KN4CK3R

class Vector3 {
public:
	float x;
	float z;
	float y;
	Vector3() : x(0.0f), z(0.0f), y(0.0f) {}
	Vector3(float x, float z, float y) : x(x), z(z), y(y) {}
	Vector3f normalize() {
		return Vector3f(x, y, z);
	}
};

class PlayerEntity
{
public:
	void* vTable; // 0x0000
	Vector3 Camera; //0x0004
	Vector3 Velocity; //0x0010
	Vector3 Direction; //0x001C
	Vector3 Position; //0x0028
	float Yaw; //0x0034
	float Pitch; //0x0038
	float Roll; //0x003C
	char pad_0040[172]; //0x0040
	uint32_t Health; //0x00EC
	//char pad_00F0[276]; //0x00F0
	//char Name[10]; //0x0204
	//char pad_020E[264]; //0x020E
	char pad_00F0[274]; //0x00F0
	char Name[16]; //0x0202
	char pad_0212[250]; //0x0212
	uint16_t Team; //0x030C
	char pad_030E[8]; //0x030E
	uint32_t N000003BE; //0x0316
	char pad_031A[17]; //0x031A

}; //Size: 0x011C


class EntityList
{
public:
	PlayerEntity* players[64];
};