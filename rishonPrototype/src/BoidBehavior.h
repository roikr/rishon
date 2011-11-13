/*
 *  BoidBehavior.h
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "Boid.h"

class GameEntityType;
class GameEntity;

class BoidBehavior
{
	
public:
	
	BoidBehavior();
	
	string Name;
	bool IsEnabled;
	vector<GameEntityType*> RelevantEntityTypes;
	bool RelevantEntityTypesExclude;
	
	virtual void Apply(Boid& boid) = 0;
	
	class RIterator// : public std::iterator<std::forward_iterator_tag, GameEntity*>
	{
	public:
		RIterator(BoidBehavior* behavior, vector<GameEntity*>::iterator begin, vector<GameEntity*>::iterator end);
		RIterator& operator=(const RIterator& other);
		RIterator& operator++();
		RIterator& operator++(int);
		//GameEntity& operator*();
		//GameEntity* operator->();
		GameEntity* Current();
		bool End();
		
	private:
		bool IsRelevant();
		BoidBehavior* _behavior;
		vector<GameEntity*>::iterator _current;
		vector<GameEntity*>::iterator _end;
	};
	
	RIterator GetRelevant(vector<GameEntity*>& entitiesToSearch);
};