#pragma once
#include "ramMain.h"

/*-----------------------------------------------------------/
randomCubeUni.h
 
 github.com/YCAMInterlab
 github.com/azuremous
 
 add ofxBullet to {ofRoot}/addons folder
 add '../../../addons/ofxBullet/libs' to Header search path
 
 Created by Jung un Kim a.k.a azuremous on 10/27/17.
 /----------------------------------------------------------*/

#include "ofxBullet.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

const int maxSize = 500;

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
    
    int maxHeight;
    int counter;
    int checkTime;
    int touchId;
    
    bool trigger;
    bool showDebug;
    bool initParts;
    bool setPlayerPos;
    bool resetMove;
    bool showDebugBox;
    
    float distance;
    float spd;
    float length;
    float boxSize;
    
    string firstActorName;
    
    ofColor touchIDColor;
    ofTrueTypeFont infoFont;
    
protected:
    void onCollision(ofxBulletCollisionData& data);
    
public:
    randomCubeUni();
    std::string getName() const { return "Rolling"; }
    
    void setup();
    void update();
    void draw();
    void drawImGui();
    void disaffectDraw();
    
    void onActorSetup(const rdtk::Actor &actor);
    void drawActor(const rdtk::Actor &actor);
    
};
