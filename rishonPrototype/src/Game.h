/*
 *  Game.h
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 10/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "GameEntityType.h"
#include "GameEntity.h"
#include "BoidType.h"
#include "User.h"

#define MOUSE_LEFT 0
#define MOUSE_RIGHT 2

#define TSPS_OSC_PORT 12000
#define TSPS_OSC_PARAM_PID 0
#define TSPS_OSC_PARAM_X 2
#define TSPS_OSC_PARAM_Y 3
#define TSPS_OSC_MSG_ENTERED "TSPS/personEntered/"
#define TSPS_OSC_MSG_UPDATED "TSPS/personMoved/"
#define TSPS_OSC_MSG_EXITED "TSPS/personWillLeave/"

class Game : public ofBaseApp{
	
public:
	Game();
	
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	int FrameRate;
	bool IsPaused;
	bool IsTspsMirror;
	map<string,GameEntityType*> EntityTypes;
	vector<GameEntity*> Entities;
	map<int,User*> Users;
	
	void RegisterEntityType(GameEntityType* entityType);
	void SetGameDefaults();
	void SpawnBoid(BoidType* boidType);
	
private:
	ofxOscReceiver  _receiver;
	int _lastUpdateTime;
	vector<int> _deadUsers;
	
	void UpdateGame();
}; 
