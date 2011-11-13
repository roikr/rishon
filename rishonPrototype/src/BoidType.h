/*
 *  BoidType.h
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "GameEntityType.h"

class GameEntityType;
class BoidBehavior;

class BoidType: public GameEntityType
{
	
public:
	BoidType(string name);
	
	float Scale;
	
	float ViewDistance;
	float ViewField; // Radians
	
	void AddBehavior(BoidBehavior* behavior);
	void RemoveBehavior(string name);
	void RemoveBehavior(int index);
	BoidBehavior* GetBehavior(string name);
	BoidBehavior* GetBehavior(int index);
	
	vector<BoidBehavior*>::iterator BehaviorsBegin();
	vector<BoidBehavior*>::iterator BehaviorsEnd();
	
//	virtual void Draw(GameEntity& entity);
	
private:
	map<string,BoidBehavior*> _behaviorsByName;
	vector<BoidBehavior*> _behaviorsByIndex;
	

};