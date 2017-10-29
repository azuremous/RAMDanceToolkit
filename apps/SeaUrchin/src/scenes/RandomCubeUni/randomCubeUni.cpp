/*-----------------------------------------------------------/
 randomCubeUni.cpp
 
 github.com/YCAMInterlab
 github.com/azuremous
 
 Created by Jung un Kim a.k.a azuremous on 10/27/17.
 /----------------------------------------------------------*/

#include "randomCubeUni.h"
const int newJointID[] = {
    0, //hip
    1, //abdomen
    2, //chest
    3, //neck
    4, //head
    
    5, //left hip
    6, //left knee
    7, //left ankle
    
    9, //right hip
    10, //right knee
    11, //right ankle
    
    14, //left collar
    15, //left shoulder
    16, //left elbow
    17, //left hand
    
    19, //right collar
    20, //right shoulder
    21, //right elbow
    22, //right hand
};

const string jointIDText[] = {
    "Hip",
    "Abdomen",
    "Chest",
    "Neck",
    "Head",
    
    "Left Hip",
    "Left Knee",
    "Left Toe",
    
    "Right Hip",
    "Right Knee",
    "Right Toe",
    
    "Left Collar",
    "Left Shoulder",
    "Left Elbow",
    "Left Hand",

    "Right Collar",
    "Right Shoulder",
    "Right Elbow",
    "Right Hand"
};

enum uni_joint{
    JOINT_HIPS              = 0,
    JOINT_ABDOMEN           = 1,
    JOINT_CHEST             = 2,
    JOINT_NECK              = 3,
    JOINT_HEAD              = 4,
    
    JOINT_LEFT_HIP          = 5,
    JOINT_LEFT_KNEE         = 6,
    JOINT_LEFT_ANKLE        = 7,
    //JOINT_LEFT_TOE          = 8,
    
    JOINT_RIGHT_HIP         = 9,
    JOINT_RIGHT_KNEE        = 10,
    JOINT_RIGHT_ANKLE       = 11,
    //JOINT_RIGHT_TOE         = 12,
    
    JOINT_LEFT_COLLAR       = 14,//13,
    JOINT_LEFT_SHOULDER     = 15,//14,
    JOINT_LEFT_ELBOW        = 16,//15,
    //JOINT_LEFT_WRIST        = 16,
    JOINT_LEFT_HAND         = 17,
    
    JOINT_RIGHT_COLLAR      = 19,//18,
    JOINT_RIGHT_SHOULDER    = 20,//19,
    JOINT_RIGHT_ELBOW       = 21,//20,
    //JOINT_RIGHT_WRIST       = 21,
    JOINT_RIGHT_HAND        = 22,
    
    NUM_JOINTS              = 23,//actual num of joint is 23 - 4 = 19
};

const ofColor tagColor[] = {
    ofColor(236, 1, 0), //0
    ofColor(236, 106, 12), //1
    ofColor(236, 217, 11), //2
    ofColor(153, 236, 12), //3
    ofColor(79, 236, 171), //4
    ofColor(23, 193, 236), //5
    ofColor(17, 38, 236), //6
    ofColor(144, 26, 236), //7
    ofColor(236, 168, 193), //8
    ofColor(236, 79, 171), //9
    ofColor(236, 109, 96), //10
    ofColor(223, 156, 236), //11
    ofColor(191, 213, 236), //12
    ofColor(176, 60, 54), //13
    ofColor(71, 38, 176), //14
    ofColor(40, 176, 110), //15
    ofColor(120, 139, 45), //16
    ofColor(112, 153, 255), //17
    ofColor(139, 34, 110), //18
    ofColor(16, 80, 0), //19
};


randomCubeUni::randomCubeUni():
trigger(false),
initParts(false),
resetMove(false),
showDebugBox(false),
maxHeight(42),
checkTime(0),
counter(0),
touchId(0),
boxSize(130),
distance(33.0),
spd(0.010),
length(0.5f),
firstActorName("")
{
    
}

//--------------------------------------------------------------
void randomCubeUni::setup()
{
    auto & cam = rdtk::CameraManager::instance().getActiveCamera(); // <-- Get ramCamera like this
    world.setup();
    world.setCamera(&cam);
    world.setGravity(ofVec3f(0, 90,0));
    
    //!
    ground = new ofxBulletBox();
    ground->create( world.world, ofVec3f(0., 0., 0.), 0., 600., 1.f, 600.f );
    ground->setProperties(.25, 10.);//10.
    ground->add();
 
    uniBall = new ofxBulletSphere();
    ((ofxBulletSphere*)uniBall)->create( world.world, ofVec3f(0, 0, 0), 4.f, 20 );
    uniBall->add();
    
    joints = new ofxBulletJoint();
    joints->create( world.world, uniBall, ofVec3f(0, 0, 0));
    joints->add();
    
    currentPos = joints->getPivotAWorldPos();
    
    touchId = (int)ofRandom(0, 19);
    touchIDColor = tagColor[touchId];
    infoFont.load(OF_TTF_SANS, 50);
}

//--------------------------------------------------------------
void randomCubeUni::update()
{
    //!
    if(showDebugBox && ofGetElapsedTimeMillis() - checkTime > 1000){
        showDebugBox = false;
    }
    world.update();
    //ofSetWindowTitle("randomCube"+ofToString(ofGetFrameRate(), 0));
    
    if(trigger){ //for uni
        currentPos += (randomPos - currentPos) * spd;
        //cout<<currentPos<<" with acc"<< (randomPos - currentPos) * spd <<endl;
        //pos, length = 2
        joints->updatePivotPos(currentPos, length);
        if(currentPos.distance(randomPos) <= 0.5){
            trigger = false;
            //cout<<"fin move"<<endl;
        }
    }
    
}

//--------------------------------------------------------------
void randomCubeUni::draw()
{
    auto & cam = rdtk::CameraManager::instance().getActiveCamera();
    cam.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofFill();
    ofSetColor(touchIDColor, 200);
    ofDrawBox(visibleBox, distance * 2);
    ofDisableAlphaBlending();
    ofPopStyle();
    cam.end();
    
}

void randomCubeUni::disaffectDraw(){
    if(bEnabled){
        ofSetColor(255, 0 , 0);
        infoFont.drawString(jointIDText[touchId], ofGetWidth()/2, 100);
    }
}

//--------------------------------------------------------------
void randomCubeUni::drawImGui(){
    ImGui::SliderFloat("distance", &distance, 1.0, 100.0);
    ImGui::SliderFloat("speed", &spd, 0.0001, 0.010);
    if(ImGui::SliderFloat("box size", &boxSize, 40, 1000)){
        showDebugBox = true;
        checkTime = ofGetElapsedTimeMillis();
    }
    
    if(ImGui::SliderInt("max height", &maxHeight, 10, 150)){
        showDebugBox = true;
        checkTime = ofGetElapsedTimeMillis();
    }
    
    if(ImGui::SmallButton("debug")){
        showDebug = !showDebug;
    }    
    //length
}

//--------------------------------------------------------------
void randomCubeUni::onActorSetup(const rdtk::Actor &actor)
{
    
}

//--------------------------------------------------------------
void randomCubeUni::drawActor(const rdtk::Actor &actor){
    
    if(!initParts){
        cout<<actor.getName()<<endl;
        firstActorName = "serio";//actor.getName();
        initParts = true;
        resetMove = true;
        counter = 0;
    }
    
    if(resetMove && firstActorName == actor.getName()){
        float half = boxSize/2.0 - distance/2.0;
        visibleBox = actor.getNode(JOINT_ABDOMEN).getGlobalPosition() + ofVec3f(ofRandom(-half, half), 0, ofRandom(-half, half));
        
        float y = ofRandom(distance, maxHeight);
        visibleBox.set(visibleBox.x, y, visibleBox.z);
        resetMove = false;
    }
    
    if(firstActorName == actor.getName() && showDebugBox){ //for debug
        
        ofPushStyle();
        ofNoFill();
        ofSetColor(255, 71, 119);
        ofVec3f boxPos = actor.getNode(JOINT_ABDOMEN).getGlobalPosition();
        
        float y = maxHeight /2.0;
        boxPos.set(boxPos.x, y, boxPos.z);
        float h = maxHeight;
        ofDrawBox(boxPos, boxSize, h, boxSize);
        ofPopStyle();
    }
    
    for(int j = 0; j < 19; j++){
        ofVec3f pos = actor.getNode(newJointID[j]).getGlobalPosition();//
        
        float dis = pos.distance(visibleBox);
        if(firstActorName == actor.getName()){ //first actor need touchID
            if(dis <= distance && touchId == j){
                randomPos.set(visibleBox.x, 20.0, visibleBox.z);
                //cout<<"set random pos: "<<randomPos<<endl;
                trigger = true;
                resetMove = true;
                touchId = (int)ofRandom(0, 19);
                touchIDColor = tagColor[touchId];
            }
        }else{ //another actor
            if(dis <= distance){
                randomPos.set(visibleBox.x, 20.0, visibleBox.z);
                //cout<<"set random pos from outsider: "<<randomPos<<endl;
                trigger = true;
                resetMove = true;
                touchId = (int)ofRandom(0, 19);
                touchIDColor = tagColor[touchId];
            }
        }
        
        
    }
    
    if(showDebug){
        ofPushStyle();
        ofSetLineWidth(1.f);
        ofNoFill();
        ofSetColor(255, 0, 0);
        ofDrawSphere(currentPos, 10);
        
        ofSetColor(255, 255, 0);
        ofDrawSphere(randomPos, 5);
    }
    
    ofSetColor(15,197,138);
    ofDrawSphere(uniBall->getPosition(), 20);
    ofPopStyle();
}

