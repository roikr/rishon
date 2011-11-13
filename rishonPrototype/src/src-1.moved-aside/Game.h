#pragma once

#include "ofMain.h"

class Game : public ofBaseApp{
	
public:
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
	
	Texture2D boidTexture;
	Texture2D boidVectorTexture;
	Texture2D targetTexture;
	Texture2D blockTexture;
	SpriteFont font;
	BoidObject firstBoid;
	
	float groupFactor = 100f;
	float avoidFactor = 60f;
	float alignFactor = 8f;
	float viewDistanceR= 250f;
	
private:
	bool bDebugMode = false;
	bool bDebugFollowTarget = false;
	bool bPause = false;
	bool bBoidsClassicMode = false;
	bool bAvoidMouse = false;
	bool bAttractiveMouse = false;
	bool bFlock = true;
	ofRectangle viewportRect;
	
	vector<BoidObject> flock;
	vector<ofVec2f> attractingPoints;
	vector<ofVec2f> scaryPoints;
	vector<ofVec2f> avoidPoints;
	float debugRotation = 0f;
	Random randGen;
	int framerate = 60;
	double timeSinceLastUpdate = 0;
	int boidNumber = 25;
	ofVec2f boidTextureSize;
	
}; 
