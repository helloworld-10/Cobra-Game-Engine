#pragma once
#include "ComponentManager.h"
class Behavior {
public:
	virtual void init() =0;
	virtual void update(ComponentManager* manager)=0;
	virtual void exit()=0;
};