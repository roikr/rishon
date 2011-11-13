/*
 *  BoidType.cpp
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "BoidType.h"
#include "BoidBehavior.h"

BoidType::BoidType(string name): GameEntityType(name),
	Scale(1.0f),
	ViewDistance(100.0f),
	ViewField(DEG_TO_RAD * 180.0f)
{
}

void BoidType::AddBehavior(BoidBehavior* behavior)
{
	BoidBehavior* existing = _behaviorsByName[behavior->Name];
	if (existing != NULL)
		throw new exception();
	
	_behaviorsByName[behavior->Name] = behavior;
	_behaviorsByIndex.push_back(behavior);
}

void BoidType::RemoveBehavior(string name)
{
}

void BoidType::RemoveBehavior(int index)
{
}

BoidBehavior* BoidType::GetBehavior(string name)
{
	return _behaviorsByName[name];
}

BoidBehavior* BoidType::GetBehavior(int index)
{
	return _behaviorsByIndex[index];
}

vector<BoidBehavior*>::iterator BoidType::BehaviorsBegin()
{
	return _behaviorsByIndex.begin();
}

vector<BoidBehavior*>::iterator BoidType::BehaviorsEnd()
{
	return _behaviorsByIndex.end();
}

//void BoidType::Draw(GameEntity& entity)
//{
//}