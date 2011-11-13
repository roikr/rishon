/*
 *  GameEntity.h
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

class GameEntityType;
class GameEntityState;
class Game;

class GameEntity
{
	
public:
	GameEntity(GameEntityType* entityType);
	
	GameEntityType* GetType();
	
	virtual void Detect(Game& game);
	virtual void Update();
	virtual void Draw();
	
	virtual GameEntityState& CurrentState();
	virtual GameEntityState& NextState();
	
	void AdvanceState();
	
protected:
	virtual GameEntityState* NewStateObject();
	
private:
	GameEntityState* _currentState;
	GameEntityState* _nextState;
	GameEntityType* _entityType;
	
};
