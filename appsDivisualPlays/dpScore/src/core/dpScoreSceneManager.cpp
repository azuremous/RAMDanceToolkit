//
//  dpScoreSceneManager.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneManager.h"

DP_SCORE_NAMESPACE_BEGIN

const ofVec2f SceneManager::kGuiPosition{20.f, 40.f};
const float SceneManager::kGuiWidth{200.f};

SceneManager::SceneManager()
{
    mScenes.clear();
    
    mTabBar = new ofxUITabBar();
    mGuiColor = ofColor{color::kDarkPinkHeavy, 50.f};
#ifndef DP_MASTER_HAKONIWA
    mTabBar->setPosition(kGuiPosition.x, kGuiPosition.y);
    mTabBar->setColorBack(mGuiColor);
    mTabBar->setWidth(kGuiWidth);
#endif
}

SceneManager::~SceneManager()
{
    if (mTabBar) {
        delete mTabBar;
        mTabBar = nullptr;
    }
    
    clear();
}

#pragma mark ___________________________________________________________________
void SceneManager::add(SceneBase::Ptr scene)
{
    const string className = scene->getName();
    
    if (mScenes.empty() == false) {
        if (findScene(className) != mScenes.end())
            ofxThrowExceptionf(ofxException,
                               "class %s is already exists",
                               className.c_str());
        
        for (auto& s : mScenes) {
            if (s == scene) {
                ofxThrowExceptionf(ofxException,
                                   "same instance %s %x is already exists",
                                   className.c_str(),
                                   s.get());
            }
        }
    }
    scene->onInitialize();
    
    if (scene->getUICanvas() != nullptr) {
        auto* canvas = scene->getUICanvas();
#ifndef DP_MASTER_HAKONIWA
        if (canvas) canvas->setColorBack(mGuiColor);
#endif
        if (mTabBar && canvas) mTabBar->addCanvas(canvas);
        
    }
    
    scene->setId(mScenes.size());
    mScenes.push_back(scene);
}

void SceneManager::clear()
{
    if (mCurrentScene) mCurrentScene->onExit();
    mCurrentScene.reset();
    for (auto p : mScenes) {
        p->onShutDown();
    }
    mScenes.clear();
    
    mSceneId = 0;
}

#pragma mark ___________________________________________________________________
void SceneManager::next()
{
    if (mScenes.empty())
        ofxThrowException(ofxException, "mScenes is empty");
    ++mSceneId %= mScenes.size();
    change();
}

void SceneManager::prev()
{
    if (mScenes.empty())
        ofxThrowException(ofxException, "mScenes is empty");
    --mSceneId;
    if (mSceneId<0) mSceneId = mScenes.size()-1;
    change();
}

#pragma mark ___________________________________________________________________
void SceneManager::change(int index)
{
    if (index<0 || index>=mScenes.size())
        ofxThrowExceptionf(ofxException, "%d is out of range", index);
    mSceneId = index;
    change();
}

void SceneManager::change(const string& name)
{
    auto it = findScene(name);
    if (it == mScenes.end())
        ofxThrowExceptionf(ofxException,
                           "no scene named %s found",
                           name.c_str());
    mSceneId = (*it)->getId();
    change();
}

void SceneManager::change()
{
    if (mCurrentScene) mCurrentScene->onExit();
    mCurrentScene = mScenes.at(mSceneId);
    mCurrentScene->onEnter();
    
    ofLogNotice() << "changed scene: " << mSceneId << ", " << mCurrentScene->getName();
}

#pragma mark ___________________________________________________________________
void SceneManager::update(ofxEventMessage& m)
{
    if (mCurrentScene) {
        mCurrentScene->onUpdate(m);
    }
}

void SceneManager::draw()
{
    if (mCurrentScene) mCurrentScene->onDraw();
}

ofPtr<SceneBase> SceneManager::getCurrent()
{
    return mCurrentScene;
}

#pragma mark ___________________________________________________________________
void SceneManager::keyPressed(int key)
{
    if (mCurrentScene) mCurrentScene->onKeyPressed(key);
}

void SceneManager::keyReleased(int key)
{
    if (mCurrentScene) mCurrentScene->onKeyReleased(key);
}

void SceneManager::mouseMoved(int x, int y)
{
    if (mCurrentScene) mCurrentScene->onMouseMoved(x, y);
}

void SceneManager::mouseDragged(int x, int y, int button)
{
    if (mCurrentScene) mCurrentScene->onMouseDragged(x, y, button);
}

void SceneManager::mousePressed(int x, int y, int button)
{
    if (mCurrentScene) mCurrentScene->onMousePressed(x, y, button);
}

void SceneManager::mouseReleased(int x, int y, int button)
{
    if (mCurrentScene) mCurrentScene->onMouseReleased(x, y, button);
}

void SceneManager::windowResized(int w, int h)
{
    if (mCurrentScene) mCurrentScene->onWindowResized(w, h);
}

void SceneManager::dragEvent(ofDragInfo dragInfo)
{
    if (mCurrentScene) mCurrentScene->onDragEvent(dragInfo);
}

void SceneManager::gotMessage(ofMessage msg)
{
    if (mCurrentScene) mCurrentScene->onGotMessage(msg);
}

void SceneManager::guiEvent(ofxUIEventArgs &e)
{
    if (mCurrentScene) mCurrentScene->onGuiEvent(e);
    
    if (e.widget->getName() == "Scenes") {
        auto* radio = static_cast<ofxUIRadio*>(e.widget);
        if (radio->getActive()->getValue()) {
            change(radio->getActiveName());
        }
    }
}

#pragma mark ___________________________________________________________________
void SceneManager::makeChangeSceneTab()
{
    if (!mTabBar) return;
    
    auto* scenesTab = new ofxUICanvas();
    vector<string> names;
    for (auto p : mScenes) {
        names.push_back(p->getName());
    }
    scenesTab->setName("Change Scene");
    scenesTab->addRadio("Scenes", names);
    scenesTab->setColorBack(mGuiColor);
    scenesTab->autoSizeToFitWidgets();
    getTabBar()->addSpacer();
    getTabBar()->addCanvas(scenesTab);
    
    ofAddListener(scenesTab->newGUIEvent, this, &SceneManager::guiEvent);
}

#pragma mark ___________________________________________________________________
SceneManager::SceneVec::iterator
SceneManager::findScene(const string& name)
{
    return find_if(mScenes.begin(),
                   mScenes.end(),
                   [&name](const SceneBase::Ptr& rhs)
                   {
                       return name == rhs->getName();
                   });
}

DP_SCORE_NAMESPACE_END