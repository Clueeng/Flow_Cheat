#include "esp.h"
#include "constant.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "debug.h"
#include "settings.h"
#include "imgui.h"
#include "imgui/imgui_impl_opengl2.h"

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
		if (players->players[i] == nullptr || players->players[i]->vTable == nullptr) {
			continue;
		}
		PlayerEntity* player = players->players[i];
		if (player == playerPtr) continue;
		if (player->Team == playerPtr->Team) continue;
		float distance = playerPtr->Position.normalize().Distance(player->Position.normalize());
		if (distance < closestDistance) {
			closestDistance = distance;
			nearestPlayer = player;
		}
	}
	return nearestPlayer;
}

void esp::aimbot() {
	if (!Settings::Aimbot::enabled) return;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		PlayerEntity* target = getNearestEntity();
		//std::cout << "aimbotting \n";
		if (!target) return;
		if (target == nullptr) return;

		Vector3 targetPos = target->Position;
		Vector3 playerPos = playerPtr->Position;

		std::string debug = std::to_string(targetPos.normalize().x) + ", " + std::to_string(targetPos.normalize().y) + ", " + std::to_string(targetPos.normalize().z);
		std::string debug2 = std::to_string(targetPos.x) + ", " + std::to_string(targetPos.y) + ", " + std::to_string(targetPos.z);
		OutputDebugStringA(debug.c_str());
		OutputDebugStringA(debug2.c_str());

		std::cout << "aimbotting at " << targetPos.x << ", " << targetPos.y << ", " << targetPos.z;
		std::cout << "aimbotting at " << targetPos.normalize().x << ", " << targetPos.normalize().y << ", " << targetPos.normalize().z;
		Vec3 angle = CalcAngle(playerPos.normalize(), targetPos.normalize());
		playerPtr->Yaw = angle.x + 90;
		playerPtr->Pitch = angle.y;
	}
}

bool isSane(ImVec2 vec) {
	if (vec.x < 0 || vec.y < 0) return false;
	if (vec.x > ImGui::GetIO().DisplaySize.x || vec.y > ImGui::GetIO().DisplaySize.y) return false;
	return true;
}
bool areSane(ImVec2 vec1, ImVec2 vec2, ImVec2 vec3, ImVec2 vec4) {
	if (!isSane(vec1)) return false;
	if (!isSane(vec2)) return false;
	if (!isSane(vec3)) return false;
	if (!isSane(vec4)) return false;
	return true;
}

void esp::esp()
{
	if (!Settings::ESP::enabled) {
		OutputDebugStringA("ESP Disabled\n");
		return;
	}
	bool teammate = false;

	if (players == nullptr) {
		OutputDebugStringA("Players pointer is null\n");
		return;
	}

	for (int i = 0; i < *playerCount + 1; i++) {
		if (players->players[i] == nullptr) {
			continue;
		}
		if (players->players[i]->vTable == nullptr) continue;

		PlayerEntity* player = players->players[i];
		if (player == nullptr || player->vTable == nullptr) {
			OutputDebugStringA("Didnt find entity\n");
			continue;
		}
		teammate = player->Team == playerPtr->Team;
		if (teammate && !Settings::ESP::drawTeams) {
			continue;
		}

		Vec3 headPos = {
			player->Position.normalize().x,
			player->Position.normalize().y,
			player->Position.normalize().z
		};
		Vec3 feetPos = {
			player->Position.normalize().x,
			player->Position.normalize().y - player->EyeHeight,
			player->Position.normalize().z
		};								// obviously isn't a thing in esp.cpp
		Vec3 headScreenPos = OpenGLWorldToScreen(headPos);
		Vec3 feetScreenPos = OpenGLWorldToScreen(feetPos);
		if (headScreenPos.x == 0 && headScreenPos.y == 0) {
			OutputDebugStringA("0,0\n");
			continue;
		}

		float height = abs(headScreenPos.y - feetScreenPos.y);
		float width = height / 4.f;

		ImVec2 topLeft = ImVec2(headScreenPos.x - width, headScreenPos.y);
		ImVec2 topRight = ImVec2(headScreenPos.x + width, headScreenPos.y);

		ImVec2 bottomLeft = ImVec2(feetScreenPos.x - width, feetScreenPos.y);
		ImVec2 bottomRight = ImVec2(feetScreenPos.x + width, feetScreenPos.y);

		std::string debug = std::string(player->Name) + "'s points: (" + std::to_string(topLeft.x) + ", " + std::to_string(topLeft.y) + ") "
			+ "(" + std::to_string(topRight.x) + ", " + std::to_string(topRight.y) + ") " 
			+ "( " + std::to_string(bottomLeft.x) + ", " + std::to_string(bottomLeft.y) + ") "
			+ "( " + std::to_string(bottomRight.x) + ", " + std::to_string(bottomRight.y) + ") \n";
		OutputDebugStringA(debug.c_str());

		ImColor color = teammate ? *Settings::ESP::teamColor : *Settings::ESP::enemyColor;

		if (!areSane(topLeft, topRight, bottomLeft, bottomRight)) {
			OutputDebugStringA("Points are not sane\n");
			continue;
		}

		if (ImGui::GetBackgroundDrawList() == nullptr) continue;
		OutputDebugStringA("1\n");
		OutputDebugStringA("2\n");
		//if (ImGui::GetBackgroundDrawList()->_VtxWritePtr == nullptr) continue;
		OutputDebugStringA("3\n");
		//if (ImGui::GetBackgroundDrawList()->_Data == nullptr) continue;
		OutputDebugStringA("4\n");
		if (ImGui::GetBackgroundDrawList()->CmdBuffer.Size == 0) continue;

		OutputDebugStringA("Drawing\n");
		ImGui::GetBackgroundDrawList()->CmdBuffer.push_back(ImDrawCmd());
		ImGui::GetBackgroundDrawList()->AddQuad(topLeft, bottomLeft, bottomRight, topRight, color, 1.0f);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 40), color, player->Name);

	}
}
