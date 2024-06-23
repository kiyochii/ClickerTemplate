#pragma once
#include "click.h"
#include "gui.h"

namespace aux {
	int clicar() {
		if (gui::ligar == true) {
			click::clicar();
		}
		return 0;
	}
}