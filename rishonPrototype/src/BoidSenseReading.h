/*
 *  BoidSenseReading.h
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

class GameEntity;

class BoidSenseReading
{
	
public:
	vector<GameEntity*> EntitiesInRange;
	
	BoidSenseReading();
	void Clear();
	
};
