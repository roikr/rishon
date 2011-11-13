/*
 *  GameEntityType.h
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

class GameEntity;

class GameEntityType
{
public:
	string Name;
	bool DetectInNextState;
	
	virtual void Draw(GameEntity& entity);
	void LoadImage(string filename);
	
	GameEntityType(string name);
	
	ofImage image;
};
