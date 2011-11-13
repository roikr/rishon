/*
 *  Rules.cpp
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 10/30/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Behaviors.h"
#include "BoidState.h"
#include "BoidType.h"
#include "Boid.h"

void FlockingBehavior::Apply(Boid& boid)
{
	float m1 = 1.0f;
	float m2 = 1.0f;
	
	// Modifiers for flocking
	if (this->AvoidOnly)
	{
		m2 = 0.0f;
		m1 = -0.1f;
	}
	
	// ====================================
	// GROUP start
	ofVec2f group;
	
	// AVOID start
	ofVec2f avoid;
	
	// ALIGN start
	ofVec2f align;
	
	int boidCount = 0;
	ofVec2f offset;
	
	GameEntityState& boidState = boid.CurrentState();
	
	// ====================================
	
	for (RIterator it = GetRelevant(boid.Senses.EntitiesInRange); !it.End(); ++it)
	{
		boidCount++;
		GameEntity* entity = it.Current();
		
		// use next state if it has already been calculated, otherwise use
		GameEntityState& entityState = entity->NextState().IsReady ?  entity->NextState() : entity->CurrentState();
		offset = entityState.Position - boidState.Position;
		
		// ...................
		// GROUP iter
		group += entityState.Position;
		
		// ...................
		// AVOID iter
		if (offset.length() < AvoidDistance)
		{ 
			float inverseDistance = AvoidDistance - offset.length();
			// The further in the more effect we want it to have on the final heading
			// Normalize vector to get the direction 
			offset.normalize();
			// Multiply it by the inverse distance to give it magnitude
			// This gives us the vector from the edge of the avoidance circle to the boid b
			// Thus the closer boid b is to this boid the more wieght it will have on the avoidance heading
			ofVec2f weightb = offset * inverseDistance;
			avoid -= weightb;
		}
		
		// ...................
		// ALIGN iter
		align += entityState.Velocity;
		
	}
	if (boidCount < 1)
		return;
	
	// ====================================
	
	// GROUP end
	group /= boidCount;
	group = (group - boidState.Position) * GroupFactor;
	
	// AVOID end
	avoid *= 0.1f;
	
	// ALIGN end
	align /= boidCount;
	align = (align - boidState.Velocity) * AlignFactor;
	
	
	// ====================================
	// RESULT
	
	ofVec2f adjustment = ((m1*group) + avoid + (m2*align));
	//if (adjustment.length() > boid.CurrentState().Direction.length())
	//	adjustment.normalize();// *= boid.CurrentState().Direction.length();
	
	boid.NextState().Velocity += adjustment;
}

void PointBehavior::Apply(Boid& boid)
{
	ofVec2f displacement;
	int count = 0;
	
	GameEntityState& boidState = boid.CurrentState();
	
	for (RIterator it = GetRelevant(boid.Senses.EntitiesInRange); !it.End(); ++it)
	{
		count++;
		GameEntity* entity = it.Current();
		
		// use next state if it has already been calculated, otherwise use
		GameEntityState& entityState = entity->NextState().IsReady ?  entity->NextState() : entity->CurrentState();
		ofVec2f offset = entityState.Position - boidState.Position;
		
		if (offset.length() < Range)
		{ 
			float inverseDistance = Range - offset.length();
			offset.normalize();
			ofVec2f weight = offset * inverseDistance * (IsAttractive ? 1: -1);
			displacement += weight;
		}
	}
	
	if (count <1)
		return;
		
	boid.NextState().Velocity += displacement * this->Factor;
}

void BoundsBehavior::Apply(Boid& boid)
{
	ofVec2f curPos = boid.CurrentState().Position;
	float fovSize = boid.GetBoidType()->ViewDistance;	
	
	ofVec2f left(0, curPos.y);
	ofVec2f right(ofGetWidth(), curPos.y);
	ofVec2f top(curPos.x, 0);
	ofVec2f bottom(curPos.x, ofGetHeight());
	
	ofVec2f borderPoints[] = {left, right, top, bottom};
	ofVec2f newDir;
	
	for (int i = 0; i < 4; i++)
	{
		ofVec2f orthVec = borderPoints[i]-curPos;

		// if can't see orthogonal, ignore it
		if (orthVec.length() > fovSize)
			continue;
		
		// TODO: add hypotanuse checking
		
		// if orthogonal is out of range, ignore it
		if (orthVec.length() > this->Range)
			continue;
		
		float inverseDistance = fovSize - orthVec.length();
		orthVec.normalize();

		newDir -= orthVec * inverseDistance;
	}
	
	
	// ROEE: how to make a parabole
	boid.NextState().Velocity += newDir*this->Factor;
	
}

void MovingBehavior::Apply(Boid& boid)
{
	ofVec2f curDirection = boid.CurrentState().Velocity;
	ofVec2f curDirectionN = curDirection.getNormalized();
	boid.NextState().Velocity += curDirectionN*5;
	
	ofVec2f nextDirection = boid.NextState().Velocity;
	ofVec2f nextDirectionN = nextDirection.getNormalized();
	float curSpeed = curDirection.length();
	float nextSpeed = nextDirection.length();
	
	// .........................
	// Limit speed & accelaration
	
	/*
	if (nextSpeed < this->MinSpeed)
	{
		// Continue previous direction if nothing specified
		if (nextSpeed == 0)
			nextDirectionN = curDirectionN;
		
		nextSpeed = this->MinSpeed;
	}
	*/
	
	if (this->MaxSpeed > 0 && nextSpeed > this->MaxSpeed)
		nextSpeed = this->MaxSpeed;
	
	if (this->MaxAccelaration > 0)
	{
		if (nextSpeed > curSpeed+curSpeed*MaxAccelaration)
			nextSpeed = curSpeed+curSpeed*MaxAccelaration;
		if (nextSpeed < curSpeed-curSpeed*MaxAccelaration)
			nextSpeed = curSpeed-curSpeed*MaxAccelaration;
	}
	
	// .........................
	// Limit rotation
	
	if (this->MaxRotation > 0)
	{
		// Compare the before and after vectors to limit rotation
		float dotProduct = ofClamp(curDirectionN.dot(nextDirectionN), -1, 1);
		float angle = acos(dotProduct);
		
		if (angle > this->MaxRotation)
		{
			angle = this->MaxRotation;
			
			// cross product tells us if we need a left or right rotation
			ofVec3f A(nextDirectionN);
			ofVec3f B(curDirectionN);
			ofVec3f C = A.getCrossed(B);
			if (C.z > 0)
				angle *= -1;
			
			nextDirectionN.rotateRad(angle);
		}
	}
	
	boid.NextState().Velocity = nextDirectionN*nextSpeed;
	boid.NextState().Position = boid.CurrentState().Position + boid.NextState().Velocity;
}


