#pragma once
#include "ramMain.h"
/*
 add ofxBullet to {ofRoot}/addons folder
 add '../../../addons/ofxBullet/libs' to Header search path
 */

/*-----------------------------------------------------------/
randomCubeUni.h
 
 github.com/YCAMInterlab
 github.com/azuremous
 
 Created by Jung un Kim a.k.a azuremous on 10/27/17.
 /----------------------------------------------------------*/

#include "ofxBullet.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

class randomCubeUni : public rdtk::BaseScene
{
private:
    
    ofxBulletWorldRigid world;
    ofxBulletBox* ground;
    
    ofxBulletRigidBody* uniBall;
    ofxBulletJoint* joints;
    
    ofVec3f randomPos;
    ofVec3f currentPos;
    
    ofVec3f visibleBox;
    vector<ofVec3f> triggerBox;
    
    int changeType;
    
    bool trigger;
    bool showDebug;
    bool initParts;
    bool setPlayerPos;
    bool resetMove;
    
    float distance;
    float spd;
    float length;
    
    float boxSize;
    int boxHeight;
    
    int appearTime;
    int counter;
    int checkTime;
    int touchId;
    
    string firstActorName;
    
    ofColor touchIDColor;
    ofTrueTypeFont infoFont;
    
protected:
    void onCollision(ofxBulletCollisionData& data);
public:
    randomCubeUni();
    std::string getName() const { return "randomCubeUni"; }
    
    void setup();
    void update();
    void draw();
    void drawImGui();
    
    void onActorSetup(const rdtk::Actor &actor);
    void drawActor(const rdtk::Actor &actor);
    
};
