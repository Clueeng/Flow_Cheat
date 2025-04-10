#pragma once
#include "genCode.h"

namespace esp {
	PlayerEntity* getNearestEntity();
	PlayerEntity* getNearestAngleEntity();
	void aimbot();
	void esp();

	extern bool aimbotToggled;
	extern bool espToggled;
}
