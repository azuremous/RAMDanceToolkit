/*-----------------------------------------------------------/
 glitchUni.h
 
 github.com/YCAMInterlab
 github.com/azuremous
 
 Created by Jung un Kim a.k.a azuremous on 10/26/17.
 /----------------------------------------------------------*/

#pragma once

#include "ofxXmlSettings.h"
#include "ramMain.h"
#include "ofxBt.h"
#include "btPicker.h"

const int totalNum = 6;

class glitchUni : public rdtk::BaseScene{
private:
    ofxBt::World world;
    ofxBt::RigidBody uniBall;
    ofxBt::RigidBody uniNode[totalNum];
    vector<ofxBt::Joint*> uniJoints;
    btPicker picker[totalNum];
    
    float uniBallSize;
    float jointBallSize;
    float normalLength;
    float angularMin;
    float angularMax;
    float linearMin;
    float linearMax;
    bool setNewUni;
    
    struct player{
        int id;
        string name;
        player(int n, string name){
            id = n;
            this->name = name;
            cout<<"set new player info:"<<id<<" name:"<<name<<endl;
        }
    };
    vector<player> actorInfo;
    
protected:
    ofVec3f getNormalizePos(const rdtk::Actor &actor, int n);
    void initPlayer(const rdtk::Actor &actor);
    
public:
    glitchUni();
    string getName() const { return "glitchUni"; }

    void setup();
    void update();
    void onValueChanged(ofxUIEventArgs& e);
    void draw();
    void drawImGui();
    
    void onActorSetup(const rdtk::Actor &actor);
    void drawActor(const rdtk::Actor &actor);
    
    
};
