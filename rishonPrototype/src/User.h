/*
 *  User.h
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/3/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "GameEntity.h"

class GameEntityType;

class User: public GameEntity
{

public:
	
	User(GameEntityType* entityType);
	
	static const int MaxTimeWithoutUpdate = 3000;
	
	int TspsID;
	int EntranceTime;
	int LastUpdateTime;
};