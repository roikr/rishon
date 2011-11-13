/*
 *  Game.cpp
 *  rishonPrototype
 *
 *  Created by Doron Assayas on 10/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Game.h"
#include "RishonBase.h"
#include "Behaviors.h"

Game::Game():
	IsPaused(false),
	IsTspsMirror(false),
	_lastUpdateTime(0),
	FrameRate(60)
{
}

#define SPAWNBUFFER 30
void Game::SpawnBoid(BoidType* boidType)
{
	Boid* boid = new Boid(boidType);
	//boid->NextState().Position = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
	boid->NextState().Position = ofVec2f(ofRandom(SPAWNBUFFER, ofGetWidth()-SPAWNBUFFER), ofRandom(SPAWNBUFFER, ofGetHeight()-SPAWNBUFFER));
	boid->NextState().Velocity = ofVec2f(ofRandom(-1,1),ofRandom(-1,1))*5; // will be adjusted to base speed
	Entities.push_back(boid);
}

void Game::RegisterEntityType(GameEntityType* entityType)
{
	GameEntityType* existing = EntityTypes[entityType->Name];
	if (existing != NULL)
		throw new exception();
	
	this->EntityTypes[entityType->Name] = entityType;
}

void Game::setup()
{
	ofBackground(0,0,0);
	ofSetVerticalSync(true);
	ofSetFrameRate(this->FrameRate);
	
	// TODO: multiple receivers
	_receiver.setup( TSPS_OSC_PORT );
	
	// ...............................
	// Setup entities
	
	GameEntityType* scaryUser = new GameEntityType("Scary User");
	scaryUser->DetectInNextState = true; // updated before the boids sense
	RegisterEntityType(scaryUser);
	
	GameEntityType* attractiveUser = new GameEntityType("Attractive User");
	attractiveUser->DetectInNextState = true; // updated before the boids sense
	RegisterEntityType(attractiveUser);
	
	GameEntityType* blockingPoint = new GameEntityType("Blocking Point");
	RegisterEntityType(blockingPoint);
			
	BoidType* defaultBoidEntityType = new BoidType("Default Boid");
	defaultBoidEntityType->Scale = 0.1f;
	RegisterEntityType(defaultBoidEntityType);
	defaultBoidEntityType->LoadImage("05.png");
	
	BoidType* fatBoidEntityType = new BoidType("Fat Boid");
	fatBoidEntityType->Scale = 0.36f;
	fatBoidEntityType->LoadImage("04.png");
	RegisterEntityType(fatBoidEntityType);

	BoidType* sleekBoidEntityType = new BoidType("Sleek Boid");
	sleekBoidEntityType->Scale = 0.2f;
	sleekBoidEntityType->LoadImage("02.png");
	RegisterEntityType(sleekBoidEntityType);
	
	
	// ...............................
	// Default boid

	
	
	/*
	PointBehavior* scaryUsersBehavior = new PointBehavior();
	scaryUsersBehavior->Name = "Scary users behavior";
	scaryUsersBehavior->RelevantEntityTypes.push_back(scaryUser);
	defaultBoidEntityType->AddBehavior(scaryUsersBehavior);

	PointBehavior* attractiveUsersBehavior = new PointBehavior();
	attractiveUsersBehavior->Name = "Attractive users behavior";
	attractiveUsersBehavior->RelevantEntityTypes.push_back(attractiveUser);
	defaultBoidEntityType->AddBehavior(attractiveUsersBehavior);
	
	PointBehavior* blockingPointBehavior = new PointBehavior();
	blockingPointBehavior->Name = "Blocking point behavior";
	blockingPointBehavior->RelevantEntityTypes.push_back(blockingPoint);
	defaultBoidEntityType->AddBehavior(blockingPointBehavior);
	*/
	
	FlockingBehavior* flockingBehaviorDefault = new FlockingBehavior();
	flockingBehaviorDefault->Name = "Flocking behavior";
	flockingBehaviorDefault->RelevantEntityTypes.push_back(defaultBoidEntityType);
	defaultBoidEntityType->AddBehavior(flockingBehaviorDefault);
	
	
	PointBehavior* scaredOfEverythingBehavior = new PointBehavior();
	scaredOfEverythingBehavior->Name = "Scary fat";
	scaredOfEverythingBehavior->RelevantEntityTypes.push_back(fatBoidEntityType);
	defaultBoidEntityType->AddBehavior(scaredOfEverythingBehavior);	

	PointBehavior* defaultAvoidSleeks = new PointBehavior();
	defaultAvoidSleeks->Name = "Avoid sleek";
	defaultAvoidSleeks->RelevantEntityTypes.push_back(sleekBoidEntityType);
	defaultBoidEntityType->AddBehavior(defaultAvoidSleeks);	
	
	BoundsBehavior* boundsBehaviorDefault = new BoundsBehavior();
	boundsBehaviorDefault->Name = "Bounds behavior";
	defaultBoidEntityType->AddBehavior(boundsBehaviorDefault);	
	
	MovingBehavior* movingBehaviorDefault = new MovingBehavior();
	movingBehaviorDefault->Name = "Moving behavior";
	defaultBoidEntityType->AddBehavior(movingBehaviorDefault);
	

	// ...............................
	// Fat boid

	PointBehavior* avoidEachOther = new PointBehavior();
	avoidEachOther->Name = "Avoid eachother";
	avoidEachOther->RelevantEntityTypes.push_back(fatBoidEntityType);
	fatBoidEntityType->AddBehavior(avoidEachOther);	
	
	BoundsBehavior* boundsBehaviorFat = new BoundsBehavior();
	boundsBehaviorFat->Name = "Bounds behavior";
	fatBoidEntityType->AddBehavior(boundsBehaviorFat);	

	MovingBehavior* movingBehaviorFat = new MovingBehavior();
	movingBehaviorFat->Name = "Moving behavior";
	fatBoidEntityType->AddBehavior(movingBehaviorFat);
	
	
	// ...............................
	// Sleek boid
	
	FlockingBehavior* sleekFlocking = new FlockingBehavior();
	sleekFlocking->Name = "Flocking behavior";
	sleekFlocking->RelevantEntityTypes.push_back(sleekBoidEntityType);
	sleekBoidEntityType->AddBehavior(sleekFlocking);
	
	PointBehavior* sleekScaredOfFat = new PointBehavior();
	sleekScaredOfFat->Name = "Scary fat";
	sleekScaredOfFat->RelevantEntityTypes.push_back(fatBoidEntityType);
	sleekBoidEntityType->AddBehavior(sleekScaredOfFat);	
	
	BoundsBehavior* sleekBounds = new BoundsBehavior();
	sleekBounds->Name = "Bounds behavior";
	sleekBoidEntityType->AddBehavior(sleekBounds);	
	
	MovingBehavior* sleekMoving = new MovingBehavior();
	sleekMoving->Name = "Moving behavior";
	sleekBoidEntityType->AddBehavior(sleekMoving);
	
	
	// ...............................

	SetGameDefaults();
	
	
	for (int i = 0; i < 200; i++)
	{
		SpawnBoid(defaultBoidEntityType);
	}
	for (int i = 0; i < 3; i++)
	{
		SpawnBoid(fatBoidEntityType);
	}
	for (int i = 0; i < 80; i++)
	{
		SpawnBoid(sleekBoidEntityType);
	}
	
}


void Game::SetGameDefaults()
{
	// ...............................
	// Default boid type
	
	BoidType* defaultBoidEntityType = dynamic_cast<BoidType*> (this->EntityTypes["Default Boid"]);
	defaultBoidEntityType->ViewDistance = 150.0f; // can see 250 pixels ahead
	defaultBoidEntityType->ViewField = DEG_TO_RAD * 360.0f; // 200 degree view angle

	FlockingBehavior* flockingBehaviorDefault = dynamic_cast<FlockingBehavior*>(defaultBoidEntityType->GetBehavior("Flocking behavior"));
	flockingBehaviorDefault->AvoidDistance = 50.0f;
	flockingBehaviorDefault->AlignFactor = 0.125f;
	flockingBehaviorDefault->GroupFactor = 0.01f;	
	flockingBehaviorDefault->AvoidOnly = false;
	//flockingBehaviorDefault->MaxSpeed = 10.0f;
	flockingBehaviorDefault->IsEnabled = true;
	
	/*
	PointBehavior* scaryUsersBehavior = dynamic_cast<PointBehavior*>(defaultBoidEntityType->GetBehavior("Scary users behavior"));
	scaryUsersBehavior->Factor = -0.01f;
	scaryUsersBehavior->Range = 60.0f;
	scaryUsersBehavior->Attraction = 0.0f; // no special accelation
	scaryUsersBehavior->IsEnabled = false;
	
	PointBehavior* attractiveUsersBehavior = dynamic_cast<PointBehavior*>(defaultBoidEntityType->GetBehavior("Attractive users behavior"));
	attractiveUsersBehavior->Factor = 0.01f;
	attractiveUsersBehavior->Range = 60.0f;
	attractiveUsersBehavior->Attraction = 0.0f; // no special accelation
	attractiveUsersBehavior->IsEnabled = false;
	
	PointBehavior* blockingPointBehavior = dynamic_cast<PointBehavior*>(defaultBoidEntityType->GetBehavior("Blocking point behavior"));
	blockingPointBehavior->Range = 60.0f;
	blockingPointBehavior->Factor = -0.5f;
	blockingPointBehavior->Attraction = -1.0f; // repulsion
	blockingPointBehavior->IsEnabled = false;
	*/
	
	//Everything is scary
	
	PointBehavior* scaredOfEverythingBehavior = dynamic_cast<PointBehavior*>(defaultBoidEntityType->GetBehavior("Scary fat"));
	scaredOfEverythingBehavior->Factor = 0.5f;
	scaredOfEverythingBehavior->Range = 120.0f;
	scaredOfEverythingBehavior->IsAttractive = false;
	scaredOfEverythingBehavior->IsEnabled = true;
	
	PointBehavior* defaultAvoidSleeks = dynamic_cast<PointBehavior*>(defaultBoidEntityType->GetBehavior("Avoid sleek"));
	defaultAvoidSleeks->Factor = 0.5f;
	defaultAvoidSleeks->Range = 120.0f;
	defaultAvoidSleeks->IsAttractive = false;
	defaultAvoidSleeks->IsEnabled = true;

	BoundsBehavior* boundsBehaviorDefault = dynamic_cast<BoundsBehavior*>(defaultBoidEntityType->GetBehavior("Bounds behavior"));
	boundsBehaviorDefault->Range = 100.0f;
	boundsBehaviorDefault->Factor = 0.03f;//0.003f;
	boundsBehaviorDefault->IsEnabled = true;		
	
	MovingBehavior* movingBehaviorDefault = dynamic_cast<MovingBehavior*>(defaultBoidEntityType->GetBehavior("Moving behavior"));
	movingBehaviorDefault->MaxSpeed = 10.0f;
	movingBehaviorDefault->MaxRotation = 0.0f; // unlimited
	movingBehaviorDefault->MaxAccelaration = 0.0f;
	movingBehaviorDefault->IsEnabled = true;
	
	// ...............................
	// Fat boid type
	BoidType* fatBoidEntityType = dynamic_cast<BoidType*> (this->EntityTypes["Fat Boid"]);
	fatBoidEntityType->ViewDistance = 150.0f; // can see 250 pixels ahead
	//fatBoidEntityType->ViewField = DEG_TO_RAD * 360.0f; // 200 degree view angle
	
	PointBehavior* avoidEachOther = dynamic_cast<PointBehavior*> (fatBoidEntityType->GetBehavior("Avoid eachother"));
	avoidEachOther->Factor = 0.04f;
	avoidEachOther->Range = 150.0f;
	avoidEachOther->IsAttractive = false;
	avoidEachOther->IsEnabled = true;

	BoundsBehavior* boundsBehaviorFat = dynamic_cast<BoundsBehavior*>(fatBoidEntityType->GetBehavior("Bounds behavior"));
	boundsBehaviorFat->Range = 200.0f;
	boundsBehaviorFat->Factor = 0.01f;//0.003f;
	boundsBehaviorFat->IsEnabled = true;		

	MovingBehavior* movingBehaviorFat = dynamic_cast<MovingBehavior*>(fatBoidEntityType->GetBehavior("Moving behavior"));
	movingBehaviorFat->MaxSpeed = 3.0f;
	movingBehaviorFat->MaxRotation = 0.0f; // unlimited
	movingBehaviorFat->MaxAccelaration = 0.0f;
	movingBehaviorFat->IsEnabled = true;
	
	// ...............................
	// Sleek
	
	BoidType* sleekBoidEntityType = dynamic_cast<BoidType*> (this->EntityTypes["Sleek Boid"]);
	sleekBoidEntityType->ViewDistance = 450.0f;
	
	FlockingBehavior* sleekFlocking = dynamic_cast<FlockingBehavior*>(sleekBoidEntityType->GetBehavior("Flocking behavior"));
	sleekFlocking->AvoidDistance = 20.0f;
	sleekFlocking->AlignFactor = 0.4f;
	sleekFlocking->GroupFactor = 0.05f;	
	sleekFlocking->AvoidOnly = false;
	sleekFlocking->IsEnabled = true;
		
	PointBehavior* sleekScaredOfFat = dynamic_cast<PointBehavior*> (sleekBoidEntityType->GetBehavior("Scary fat"));
	sleekScaredOfFat->Factor = 0.1f;
	sleekScaredOfFat->Range = 200.0f;
	sleekScaredOfFat->IsAttractive = false;
	sleekScaredOfFat->IsEnabled = true;

	BoundsBehavior* sleekBounds = dynamic_cast<BoundsBehavior*>(sleekBoidEntityType->GetBehavior("Bounds behavior"));
	sleekBounds->Range = 250.0f;
	sleekBounds->Factor = 0.03f;//0.003f;
	sleekBounds->IsEnabled = true;		
	
	MovingBehavior* sleekMoving = new MovingBehavior();
	sleekMoving->MaxSpeed = 7.0f;
	sleekMoving->MaxRotation = 0.0f; // unlimited
	sleekMoving->MaxAccelaration = 0.0f;
	sleekMoving->IsEnabled = true;
	
	
}

void Game::update()
{
	// .........................................................................
	// Handle user updates from OpenTSPS
	
	while( _receiver.hasWaitingMessages() )
	{
		ofxOscMessage m;
		_receiver.getNextMessage( &m );
		string messageType = m.getAddress();
		int pid = m.getArgAsInt32(TSPS_OSC_PARAM_PID);
		ofVec2f personPos(
						  (ofGetWidth() * m.getArgAsFloat(TSPS_OSC_PARAM_X)),
						  (ofGetHeight() * m.getArgAsFloat(TSPS_OSC_PARAM_Y))
						  );
		if (IsTspsMirror)
			personPos.set(ofGetWidth() - personPos.x, ofGetHeight() - personPos.y);
		
		User* user = Users[pid];
		if (messageType == TSPS_OSC_MSG_ENTERED  || messageType == TSPS_OSC_MSG_UPDATED)
		{
			bool createNewUser = false;
			if (messageType == TSPS_OSC_MSG_ENTERED && user != NULL)
			{
				// If for some reason we have a lingering person, delete his ass
				delete user;
				createNewUser = true;
			}
			if (messageType == TSPS_OSC_MSG_UPDATED && user == NULL)
			{
				// We somehow missed creating this user
				createNewUser = true;
			}
			
			if (createNewUser)
			{
				user = new User(this->EntityTypes["Scary User"]);
				user->TspsID = pid;
				user->EntranceTime = ofGetElapsedTimeMillis();
				Users[pid] = user;
			}
			
			user->LastUpdateTime = ofGetElapsedTimeMillis();
			user->NextState().Position = personPos;
		}
		else if (messageType == TSPS_OSC_MSG_EXITED)
		{
			delete user;
			Users.erase(pid);
		}
	}
	
	// Remove dead users (that haven't been updated in a while)
	_deadUsers.clear();
	for(std::map<int, User*>::iterator u = Users.begin(); u != Users.end(); ++u)
	{
		User* user = u->second;
		
		if (user->LastUpdateTime + User::MaxTimeWithoutUpdate < ofGetElapsedTimeMillis())
		{
			delete user;
			_deadUsers.push_back(u->first); // mark for removal
		}
	}
	// Perform removal
	for(vector<int>::iterator dead = _deadUsers.begin(); dead != _deadUsers.end(); ++dead)
	{
		Users.erase(*dead);
	}
	
	// .........................................................................
	// Update game logic (when not paused)
	
	if (!this->IsPaused)
		UpdateGame();
}

void Game::UpdateGame()
{	
	double timePerFrame = 1000.0f / this->FrameRate;
	int currentTime = ofGetElapsedTimeMillis();
	
	// Detect time
	bool shouldUpdate = ((currentTime - _lastUpdateTime) >= timePerFrame);
	if (shouldUpdate)
		_lastUpdateTime = currentTime;
	else
		return;
	
	// Detect (senses)
	for (std::vector<GameEntity*>::iterator entity = Entities.begin(); entity != Entities.end(); ++entity)
	{
		// Start a new state
		(*entity)->AdvanceState();
		
		// Detect stuff around us
		(*entity)->Detect(*this);

		(*entity)->Update();
	}	
}

void Game::draw()
{
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofBackground(0,0,0);
	ofSetColor(255, 255, 255);
	
	// Detect (senses)
	for (std::vector<GameEntity*>::iterator entity = Entities.begin(); entity != Entities.end(); ++entity)
	{
		// Draw and advance the state
		(*entity)->Draw();
	}
	
	ofDisableSmoothing();
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void Game::keyPressed  (int key){
	if (key == 'f')
		ofToggleFullscreen();
}

void Game::keyReleased(int key){
}

//--------------------------------------------------------------
void Game::mouseMoved(int x, int y ){
	
}
void Game::mouseDragged(int x, int y, int button){
	
}

void Game::mousePressed(int x, int y, int button){
}

void Game::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void Game::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void Game::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void Game::dragEvent(ofDragInfo dragInfo){ 
	
}