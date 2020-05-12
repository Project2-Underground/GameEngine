#pragma once

#include "InteractObj.h"


class Butler : public PlayerTriggerObj {
	int currentLevel;
	void MoveOut(std::string);
	void MoveIn(std::string);
public:
	enum Phase {
		PHASE0 = 0,
		PHASE1,
		PHASE2,
		PHASE3,
		ROUTE_A,
		ROUTE_B
	};
	Phase currentPhase;
	Butler();
	void Appear();
	void SetTriggered(bool b);
	void action();
	bool IsTriggered();
	void Update();
};

class BackAlleyEmma : public NonPlayer {
public:
	BackAlleyEmma(std::string name) :NonPlayer(name) {}
	void action();
};