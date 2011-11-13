/*
 *  GameEntityState.h
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

class GameEntityState
{
public:
	GameEntityState();
	
	ofVec2f Velocity;
	ofVec2f Position;
	
	bool IsReady;
};