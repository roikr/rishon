/*
 *  Boid.cpp
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Boid.h"
#include "GameEntity.h"
#include "GameEntityState.h"
#include "BoidSenseReading.h"
#include "BoidType.h"
#include "BoidBehavior.h"
#include "Game.h"

Boid::Boid(BoidType* boidType):
	GameEntity(boidType),
	_debugColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255))
{};

void Boid::Detect(Game& game)
{
	// TODO: quad tree for performance
	
	this->Senses.Clear();
	
	for (vector<GameEntity*>::iterator it = game.Entities.begin(); it != game.Entities.end(); ++it)
	{
		GameEntity* entity = *it;
		
		// Some entities (users) are updated before the detection so use their 'next' state
		GameEntityState& stateData = entity->NextState().IsReady ? entity->NextState() : entity->CurrentState();
		
		// Ignore self
		if (entity == this)
			continue;
		
		// Check if within view distance
		ofVec2f directionToEntity = stateData.Position - this->CurrentState().Position;
		if (directionToEntity.length() > this->GetBoidType()->ViewDistance)
			continue;
		
		// Check if within view field
//		float angle	= this->CurrentState().Direction.angle(directionToEntity);
//		if (abs(angle > this->GetBoidType()->ViewField/2))
//			continue;
		
		this->Senses.EntitiesInRange.push_back(entity);
	}
}

BoidState& Boid::CurrentBoidState()
{
	return (BoidState&) this->CurrentState();
}

BoidState& Boid::NextBoidState()
{
	return (BoidState&) this->NextState();
}

BoidType* Boid::GetBoidType()
{
	return (BoidType*) GetType();
}

void Boid::Update()
{
	for(vector<BoidBehavior*>::iterator behavior = this->GetBoidType()->BehaviorsBegin(); behavior != this->GetBoidType()->BehaviorsEnd(); ++behavior)
	{
		if (!(*behavior)->IsEnabled)
			continue;
		
		(*behavior)->Apply(*this);
	}
	
	this->NextState().IsReady = true;
}

GameEntityState* Boid::NewStateObject()
{
	return new BoidState();
}

void Boid::Draw()
{
	ofVec2f pos = this->NextBoidState().Position;
	
//	ofSetColor(this->_debugColor);
	ofPushMatrix();
	ofEnableAlphaBlending();
	
	ofTranslate(pos.x, pos.y, 0);
	ofScale(GetBoidType()->Scale,GetBoidType()->Scale,1);
	ofRotateZ(-this->NextBoidState().Velocity.angle(ofVec2f(0,1))+180);
//	ofCircle(pos.x, pos.y, this->GetBoidType()->Scale);
//	ofSetColor(255, 255, 255);
	GetBoidType()->Draw(*this);
	ofDisableAlphaBlending();
	ofPopMatrix();
//	ofVec2f line;
//
//	/*
//	line = this->NextBoidState().Direction.getRotatedRad(this->GetBoidType()->ViewField/2).normalize()*this->GetBoidType()->ViewDistance;
//	ofLine(pos.x, pos.y, pos.x + line.x, pos.y + line.y);
//	
//	line = this->NextBoidState().Direction.getRotatedRad(-this->GetBoidType()->ViewField/2).normalize()*this->GetBoidType()->ViewDistance;
//	*/
//	line =	this->NextBoidState().Velocity.getNormalized()*this->GetBoidType()->Scale*1.5;
//	 
//	ofLine(pos.x, pos.y, pos.x + line.x, pos.y + line.y);

}