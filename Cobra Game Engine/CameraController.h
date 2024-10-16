#pragma once
#include "Behavior.h"
class CameraController : public Behavior {
	virtual void init() override;
	virtual void update(ComponentManager* manager) override;
	virtual void exit() override;
};