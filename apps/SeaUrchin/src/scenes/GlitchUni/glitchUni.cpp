/*-----------------------------------------------------------/
 glitchUni.cpp
 
 github.com/YCAMInterlab
 github.com/azuremous
 
 Created by Jung un Kim a.k.a azuremous on 10/26/17.
 /----------------------------------------------------------*/

#include "glitchUni.h"

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

const int pairJoint[][2] = {
    {JOINT_HEAD, JOINT_NECK}, //0
    
    {JOINT_LEFT_HAND, JOINT_LEFT_ELBOW}, //4
    {JOINT_RIGHT_HAND, JOINT_RIGHT_ELBOW}, //6
    
    {JOINT_LEFT_KNEE, JOINT_LEFT_ANKLE}, //9 - 6
    {JOINT_RIGHT_KNEE, JOINT_RIGHT_ANKLE}, //11
    
    {JOINT_ABDOMEN, JOINT_HIPS}, //3
    
    {JOINT_LEFT_ELBOW, JOINT_LEFT_SHOULDER}, //5
    {JOINT_RIGHT_ELBOW, JOINT_RIGHT_SHOULDER}, //7
    
    {JOINT_LEFT_HIP, JOINT_LEFT_KNEE}, //8
    {JOINT_RIGHT_HIP, JOINT_RIGHT_KNEE}, //10
    
    {JOINT_NECK, JOINT_CHEST}, //1
    {JOINT_CHEST, JOINT_ABDOMEN}, //2
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

//--------------------------------------------------------------
glitchUni::glitchUni():
uniBallSize(30.0),
jointBallSize(3.0),
normalLength(358.537),
angularMin(0.),
angularMax(10.),
linearMin(0.732),
linearMax(18.699),
setNewUni(false)

{
    
}


//--------------------------------------------------------------
void glitchUni::setup(){
    cout<<"uni setup"<<endl;
    world.setup();
    world.setGravity(ofVec3f(0, -4, 0));
    world.addPlane(ofVec3f(0, 1200, 0), ofVec3f(0,0,0));
    uniBall = world.addSphere(uniBallSize, ofVec3f(0, 400, 0));
    for(int i = 0; i < totalNum; i++){
        picker[i].setWorld(world.getWorld());
    }
}

void glitchUni::onValueChanged(ofxUIEventArgs& e){
    const string name = e.widget->getName();
    cout<<"sp!!"<<endl;
    if (name == "AngularMin"){
        cout<<"check!!"<<endl;
    }
}

//--------------------------------------------------------------
void glitchUni::update(){
    world.update();
}

//--------------------------------------------------------------
void glitchUni::draw(){
    ofPushMatrix();
    ofTranslate(1000, 400);
    world.draw();
    ofPopMatrix();
}

//--------------------------------------------------------------
void glitchUni::drawImGui(){
    
    ImGui::SliderFloat("Length", &normalLength, 1.0, 1000.0);
    ImGui::SliderFloat("AngularMin", &angularMin, 0.0001, 10.0);
    ImGui::SliderFloat("AngularMax", &angularMax, 0.0001, 100.0);
    ImGui::SliderFloat("LinearMin", &linearMin, 0.0001, 1.0);
    ImGui::SliderFloat("LinearMax", &linearMax, 0.0001, 30.0);
   
    if(setNewUni){
        for(int i = 0; i < totalNum; i++){
            uniJoints[i]->setAngularMin(angularMin, angularMin, angularMin);
            uniJoints[i]->setAngularMax(angularMax, angularMax, angularMax);
            uniJoints[i]->setLinearMin(linearMin, linearMin, linearMin);
            uniJoints[i]->setLinearMax(linearMax, linearMax, linearMax);
        }
    }
}

//--------------------------------------------------------------
void glitchUni::initPlayer(const rdtk::Actor &actor){
//    
//    vector<ofxBulletRigidBody* >newParts;
//    for(int i = 0; i < 19; i++){
//        newParts.push_back(new ofxBulletBox());
//        int n = newParts.size() - 1;
//        //ofVec3f(ofRandom(-30, 30), ofRandom(-30, 30), ofRandom(-30, 30))
//        ofVec3f startPos = actor.getNode(newJointID[i]).getPosition();
//        ((ofxBulletBox*)newParts[n])->create(world.world, startPos, 0.2, 10., 10., 10.);
//        ((ofxBulletBox*)newParts[n])->setActivationState(DISABLE_DEACTIVATION);
//        ((ofxBulletBox*)newParts[n])->setProperties(.25, .95);
//        ((ofxBulletBox*)newParts[n])->add();
//    }
//    bodyParts.push_back(newParts);
//    cout<<"randomCube new actor:"<<actor.getName()<<endl;
}

void glitchUni::onActorSetup(const rdtk::Actor &actor)
{
    cout<<"glitch new actor"<<endl;
    
    /*
     if(actorInfo.size() == 0){
     initPlayer(actor);
     player p = player(actorInfo.size(), actor.getName());
     actorInfo.push_back(p);//add first name
     }else{
     for(int i = 0; i < actorInfo.size(); i++){
     if(actorInfo[i].name != actor.getName()){ //add new name
     initPlayer(actor);
     player p = player(actorInfo.size(), actor.getName());
     actorInfo.push_back(p);
     }
     }
     }
     */
}

//--------------------------------------------------------------
void glitchUni::drawActor(const rdtk::Actor &actor){
  /*
   int actorID = -1;
   for(int i = 0; i < actorInfo.size(); i++){
   if(actorInfo[i].name == actor.getName()){ //add new name
   actorID = actorInfo[i].id;
   }
   }
   */
    ofVec3f newCenter = uniBall.getPosition();
    
    if(!setNewUni){
        uniJoints.clear();
        for(int i = 0; i < totalNum; i++){
            ofVec3f pos = getNormalizePos(actor, i);
            float dis = newCenter.distance(pos);
            uniNode[i] = world.addSphere(jointBallSize, pos); //world.addCylinder(2, dis, pos);
            //rigid.setProperty(0.4, 0.75, 0.25, 0.25);
            uniNode[i].setProperty(0.01, 0.1, 0.01, 0.01);
            uniJoints.push_back(new ofxBt::Joint(world.getWorld(), uniNode[i], uniBall));
            picker[i].attatchRigidBody(uniNode[i]);
            
        }
        setNewUni = true;
    }
    
    ofPushStyle();
    ofSetLineWidth(0.5);
    ofNoFill();
    ofSetColor(129, 229, 255);
    ofDrawSphere(newCenter, uniBallSize);
    
    ofSetLineWidth(2);
    for(int i = 0; i < totalNum; i++){
        ofSetColor(tagColor[i]);
        ofVec3f pos = getNormalizePos(actor, i);
        ofDrawBitmapString(ofToString(pos), ofGetWidth()- 100, 40 * i);
        //float dis = newCenter.distance(pos);
        picker[i].updatePosition(btVector3(pos.x, pos.y, pos.z));
        ofVec3f p = uniNode[i].getPosition();
        ofDrawLine(newCenter, p);
    }
    
    ofPopStyle();
}
//--------------------------------------------------------------
ofVec3f glitchUni::getNormalizePos(const rdtk::Actor &actor, int n){
    int first = (n/6) * 2;//0, 1
    int second = 1 + first *-1;
    auto _pos = actor.getNode(pairJoint[n][0]).getPosition() - actor.getNode(pairJoint[n][1]).getPosition();
    ofVec3f v = _pos.normalize() * normalLength  * (-1.0 + first);
    return v;
}

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------
