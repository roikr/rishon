/*
 *  GameEntityType.cpp
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 11/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "GameEntityType.h"


GameEntityType::GameEntityType(string name):
	Name(name)
{
}

void GameEntityType::Draw(GameEntity& entity)
{	
	image.draw(0, 0);
}

void GameEntityType::LoadImage(string filename) {
	image.loadImage(filename);
}