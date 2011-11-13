/*
 *  GameEntity.cpp
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "GameEntity.h"
#include "GameEntityType.h"
#include "GameEntityState.h"

GameEntity::GameEntity(GameEntityType* entityType):
	_entityType(entityType),
	_currentState(NewStateObject()),
	_nextState(NewStateObject())
{
}

GameEntityType* GameEntity::GetType()
{
	return _entityType;
}

void GameEntity::Draw()
{
	//_entityType->Draw(&this);
	
}

void GameEntity::Detect(Game& game)
{
}

void GameEntity::Update()
{
}

GameEntityState* GameEntity::NewStateObject()
{
	return new GameEntityState();
}

GameEntityState& GameEntity::CurrentState()
{
	return *_currentState;
}

GameEntityState& GameEntity::NextState()
{
	return *_nextState;
}

void GameEntity::AdvanceState()
{
	delete _currentState;
	_currentState = _nextState;
	_nextState = NewStateObject();
}

