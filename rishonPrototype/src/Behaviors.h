/*
 *  Rules.h
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 10/30/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "BoidBehavior.h"

class BoundsBehavior: public BoidBehavior
{
public:
	float Factor;
	float Range;
	
	virtual void Apply(Boid& boid);
};

class FlockingBehavior: public BoidBehavior
{
public:
	bool AvoidOnly;
	float GroupFactor;
	float AvoidDistance;
	float AlignFactor;
	float MaxSpeed;
	
	virtual void Apply(Boid& boid);
};

// Used to attract or avoid objects
class PointBehavior: public BoidBehavior
{
public:
	// Negative factor means avoid
	float Factor;
	bool IsAttractive; // less than -1.0 means moving backward
	float Range;
	
	virtual void Apply(Boid& boid);
};

class MovingBehavior: public BoidBehavior
{
public:
	float MinSpeed;
	float MaxSpeed;
	float MaxAccelaration;
	float MaxRotation;
	
	virtual void Apply(Boid& boid);
};