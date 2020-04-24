#pragma once

#include "InteractObj.h"


class Butler : public PlayerTriggerObj {

public:
	enum Phase {
		PHASE0 = 0,
		PHASE1
	};
	Phase currentPhase;
	Butler();
	void Appear(glm::vec3 pos, std::string dialogue);
	void SetTriggered(bool b);
	void action();
	bool IsTriggered();
	void Update();
};