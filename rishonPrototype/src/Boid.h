/*
 *  Boid.h
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "GameEntity.h"
#include "BoidState.h"
#include "BoidSenseReading.h"

class GameEntity;
class BoidType;
class BoidSenseReading;
class Game;

class Boid: public GameEntity
{
	
public:
	
	Boid(BoidType* boidType);
	
	BoidSenseReading Senses;
	
	BoidType* GetBoidType();
	
	virtual void Detect(Game& game);
	virtual void Update();

	BoidState& CurrentBoidState();
	BoidState& NextBoidState();
	
	virtual void Draw();
	
protected:
	virtual GameEntityState* NewStateObject();
	
	ofColor _debugColor;
};

