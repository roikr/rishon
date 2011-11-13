/*
 *  BoidBehavior.cpp
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "BoidBehavior.h"
#include "GameEntityType.h"
#include "GameEntity.h"
#include "Boid.h"

BoidBehavior::BoidBehavior():
	IsEnabled(true),
	RelevantEntityTypesExclude(false)
{
}

BoidBehavior::RIterator BoidBehavior::GetRelevant(vector<GameEntity*>& entitiesToSearch)
{
	return BoidBehavior::RIterator(this, entitiesToSearch.begin(), entitiesToSearch.end());
}


// =================================================================
// Iterator crap

BoidBehavior::RIterator::RIterator(BoidBehavior* behavior, vector<GameEntity*>::iterator begin, vector<GameEntity*>::iterator end):
	_behavior(behavior), _current(begin), _end(end)
{
	// Search for the first relevant entity
	while(!End() && !IsRelevant())
		++_current;
}

bool BoidBehavior::RIterator::End()
{
	return _current == _end;
}

BoidBehavior::RIterator& BoidBehavior::RIterator::operator=(const BoidBehavior::RIterator& other)
{
	this->_current = other._current;
	return (*this);
}

BoidBehavior::RIterator& BoidBehavior::RIterator::operator++()
{	
	// Search for the next relevant entity
	while(!End())
	{
		++_current;
		if (IsRelevant())
			break;
	}
	
	return (*this);
}

bool BoidBehavior::RIterator::IsRelevant()
{
	if (End())
		return false;
	
	bool relevant = _behavior->RelevantEntityTypesExclude ? true : false;
	for(vector<GameEntityType*>::iterator entityType = _behavior->RelevantEntityTypes.begin(); entityType != _behavior->RelevantEntityTypes.end(); ++entityType)
	{
		if ((*_current)->GetType() == (*entityType))
		{
			relevant = _behavior->RelevantEntityTypesExclude ? false : true;
			break;
		}
	}
	
	return relevant;
}

BoidBehavior::RIterator& BoidBehavior::RIterator::operator++(int)
{
	throw new exception();
}

/*
GameEntity& BoidBehavior::RIterator::operator*()
{
	return *(*_current);
}

GameEntity* BoidBehavior::RIterator::operator->()
{
	return *_current;
}
*/

GameEntity* BoidBehavior::RIterator::Current()
{
	return *_current;
}


