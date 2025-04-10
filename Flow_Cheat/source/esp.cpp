#include "esp.h"
#include "constant.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "debug.h"

namespace esp {
	bool aimbotToggled = false;
	bool espToggled = false;
}

const float FOV = 90.0f;


void normalizeAngle(Vec3 angle) {
	if (angle.x > 360.0f) angle.x -= 360.0f;
	if (angle.x < 0.0f) angle.x += 360.0f;
	if (angle.y > 90.0f) angle.y = 90.0f;
	if (angle.y < -90.0f) angle.y = -90.0f;
}
bool isInFOV(PlayerEntity* target, Vector3 camera) {
	return true;
}

PlayerEntity* esp::getNearestEntity() {
	PlayerEntity* nearestPlayer = nullptr;
	float closestDistance = 999999.0f;
	for (int i = 0; i < *playerCount + 1; i++) {
		std::cout << "entity i: " << i << std::endl;
		PlayerEntity* player = players->players[i];
		if (player == nullptr || player == playerPtr) {
			if (player == playerPtr) {
				std::cout << "self " << i;
				continue;
			}
			std::cout << "i: " << i << " is null" << std::endl;
			continue;
		}
		if (playerPtr == nullptr) {
			std::cout << "player pointer null: " << i << std::endl;
			continue;
		}
		std::cout << "passed player pointer null: " << i << std::endl;
		std::cout << "passed skipping: " << i << std::endl;
		float distance = player->Position.normalize().Distance(playerPtr->Position.normalize());
		if (distance < closestDistance) {
			std::cout << "ent: " << player->Name << " " << distance << std::endl;
			closestDistance = distance;
			nearestPlayer = player;
		}
		if (nearestPlayer) {
			std::cout << "Nearest player: " << nearestPlayer->Name << " at distance: " << closestDistance << std::endl;
		}
		else {
			//std::cout << "No players found in range." << std::endl;
		}
		return nearestPlayer;
	}
}

void esp::aimbot() {
	if (!aimbotToggled) return;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		PlayerEntity* target = getNearestEntity();
		//std::cout << "aimbotting \n";
		if (!target) return;
		if (target == nullptr) return;

		Vector3 targetPos = target->Position;
		Vector3 playerPos = playerPtr->Position;
		std::cout << "aimbotting at " << targetPos.x << ", " << targetPos.y << ", " << targetPos.z;
		Vec3 angle = CalcAngle(playerPos.normalize(), targetPos.normalize());
		playerPtr->Yaw = angle.x + 90;
		playerPtr->Pitch = angle.y;
	}
}