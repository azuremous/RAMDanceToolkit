#include "ofApp.h"
#include "UniScene.h"
#include "randomCubeUni.h"

#pragma mark - oF methods
//--------------------------------------------------------------
void ofApp::setup()
{
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    currentSceneNum = 1;
    
    /// ram setup
    // ------------------
    rdtk::Initialize(10000, true);
    
    rdtk::SceneManager& sceneManager = rdtk::SceneManager::instance();
    
    rdtk::Preference & pref = rdtk::Preference::instance();
    pref.bg.set(1.0, 1.0, 1.0);
    pref.useShadows = true;
    
    sceneManager.addScene<UniScene>();
    sceneManager.addScene<randomCubeUni>();
    
    infoFont.load(OF_TTF_SANS, 50);
}

//--------------------------------------------------------------
void ofApp::update()
{
    
}

//--------------------------------------------------------------
void ofApp::draw()
{
    
    rdtk::SceneManager& sceneManager = rdtk::SceneManager::instance();
    ofPtr<randomCubeUni> t = std::dynamic_pointer_cast<randomCubeUni>(sceneManager.getScene(2));
    t->disaffectDraw();

}



#pragma mark - ram methods
//--------------------------------------------------------------
void ofApp::drawActor(const rdtk::Actor &actor)
{
    
    
}

//--------------------------------------------------------------
void ofApp::drawRigid(const rdtk::RigidBody &rigid)
{
    
}

#pragma mark - ram Events

//--------------------------------------------------------------
void ofApp::onActorSetup(const rdtk::Actor &actor)
{
    cout<<"ofApp actor setup"<<endl;
    //rdtk::SceneManager& sceneManager = rdtk::SceneManager::instance();
    //sceneManager.getScene(currentSceneNum)->onActorSetup(actor);
    
}

//--------------------------------------------------------------
void ofApp::onActorExit(const rdtk::Actor &actor)
{
    
}

//--------------------------------------------------------------
void ofApp::onRigidSetup(const rdtk::RigidBody &rigid)
{
    
}

//--------------------------------------------------------------
void ofApp::onRigidExit(const rdtk::RigidBody &rigid)
{
    
}


#pragma mark - of Event
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    rdtk::SceneManager& sceneManager = rdtk::SceneManager::instance();
    if(key == '1'){
        //0 : actor
        sceneManager.getScene(2)->disable();
        sceneManager.getScene(1)->enable();
        currentSceneNum = 1;
    }
    if(key == '2'){
        //0 : actor
        sceneManager.getScene(1)->disable();
        sceneManager.getScene(2)->enable();
        currentSceneNum = 2;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    
}
