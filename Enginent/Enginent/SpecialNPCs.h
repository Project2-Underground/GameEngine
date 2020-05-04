#pragma once

#include "InteractObj.h"


class Butler : public PlayerTriggerObj {
	bool clickToInteract;
	bool disappearAfterAction;

	void MoveOut(std::string);
	void MoveIn(std::string);
public:
	enum Phase {
		PHASE0 = 0,
		PHASE1,
		PHASE2
	};
	Phase currentPhase;
	Butler();
	void Appear();
	void SetTriggered(bool b);
	void action();
	bool IsTriggered();
	void Update();
};