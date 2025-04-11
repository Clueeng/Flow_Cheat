#include "settings.h"
#include "imgui.h"


class ImColor;

namespace Settings {
	namespace ESP {
		extern bool enabled = true;
		extern bool drawTeams = true;
		extern ImColor* teamColor = new ImColor(0, 255, 0, 255);
		extern ImColor* enemyColor = new ImColor(255, 0, 0, 255);

	}
	namespace Aimbot {
		extern bool enabled = false;
	}
}
