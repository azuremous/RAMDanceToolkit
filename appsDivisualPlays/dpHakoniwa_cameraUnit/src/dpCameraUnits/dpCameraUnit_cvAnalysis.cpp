//
//  dpCameraUnit_cvAnalysis.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#include "dpCameraUnit_cvAnalysis.h"

dpCameraUnit_cvAnalysis::dpCameraUnit_cvAnalysis(){

	mGui.setup();
	mGui.addLabel("Analysis",OFX_UI_FONT_LARGE);
	mGui.addSpacer();
	mGui.addLabel("Hakoniwa Name");
	mGui.addTextInput("hakoniwaName", "")->setAutoClear(false);
	mGui.addLabel("OSCOption");
	mGui.addSpacer();
	mGui.addToggle("SendOSC",		&mEnableSendOSC);
	mGui.addTextInput("OSCAddress", "localhost")->setAutoClear(false);
	mGui.addLabel("OSCSplit", "OSCSplit");
	oscMatrixUI = mGui.addToggleMatrix("OSCSpliter", 1, 7);
	mGui.addTextInput("OSCPort", "10000")->setAutoClear(false);
	mGui.addToggle("ContourFinder", &mEnableContourFinder);
	mGui.addToggle("OptFlow",		&mEnableOptFlow);
	mGui.addToggle("Flow_FarneBack", &mEnableOptFlowFarne);
	mGui.addToggle("Mean",			&mEnableMean);
//	mGui.addToggle("FAST",			&mEnableFAST);
//	mGui.addToggle("Histgram",		&mEnableHistgram);
	mGui.addSpacer();
	mGui.addToggle("ViewSource", &mViewSource);

	mGui.addLabel("ContourFinder");
	mGui.addSpacer();
	mGui.addToggle("Blob"	, false);
	mGui.addToggle("Pts"	, false);
	mGui.addToggle("Simplify"		, &mParamCF_Simplify);
	mGui.addToggle("UseTargetColor"	, &mParamCF_UseTargetColor);
	mGui.addRangeSlider("Area", 0.0, 10000.0, &mParamCF_MinArea, &mParamCF_MaxArea);
	mGui.addSlider("Threshold", 0.0, 255.0, &mParamCF_Threshold);
	mGui.addLabel("OptFlow");
	mGui.addSpacer();
	mGui.addSlider("filter_Speed", 0.0, 100.0, &mOptFlow_filterSpd);
	mGui.addSlider("smooth", 1.0, 10.0, &mOptFlowSmooth);

	ofAddListener(mGui.newGUIEvent, this, &dpCameraUnit_cvAnalysis::guiEvent);
	
	imgRefColor = NULL;
	imgRefGray = NULL;

	mGui.autoSizeToFitWidgets();

	//OSC Initialize
	addUI = (ofxUITextInput*)mGui.getWidget("OSCAddress");
	portUI = (ofxUITextInput*)mGui.getWidget("OSCPort");

	int pt = ofToInt(portUI->getTextString());
	string address = addUI->getTextString();

	sender.setup(address, pt);


	mEnableSendOSC			= false;
	mEnableContourFinder	= false;
	mEnableOptFlow			= false;
	mEnableFAST				= false;
	mEnableMean				= false;
	mEnableHistgram			= false;
	
	mOptFlow_filterSpd = 100.0;
	
}

dpCameraUnit_cvAnalysis::~dpCameraUnit_cvAnalysis(){

}

void dpCameraUnit_cvAnalysis::update(ofImage &pixColor, ofImage &pixGray,bool isFrameNew){

	imgRefColor = &pixColor;
	imgRefGray = &pixGray;
    
    float width = pixGray.getWidth();
    float height = pixGray.getHeight();

	if (mEnableContourFinder){
		mContFinder.setMaxArea(mParamCF_MaxArea);
		mContFinder.setMinArea(mParamCF_MinArea);
		mContFinder.setSimplify(mParamCF_Simplify);
		mContFinder.setTargetColor(mParamCF_targColor);
		mContFinder.setUseTargetColor(mParamCF_UseTargetColor);
		mContFinder.setThreshold(mParamCF_Threshold);

		mContFinder.findContours(pixGray);

		pts.clear();
		for (int i = 0;i < mContFinder.getContours().size();i++){
			
			ofRectangle rt = ofxCv::toOf(mContFinder.getBoundingRect(i));
			ofxOscMessage m;
			m.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/contour/boundingRect");
			m.addIntArg(i);
			m.addFloatArg(rt.x / width);
			m.addFloatArg(rt.y / height);
			m.addFloatArg(rt.width / width);
			m.addFloatArg(rt.height / height);
			sendMessageMulti(m);

			ofxOscMessage mm;
			mm.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/contour/blob");
			mm.addIntArg(i);
			
			int cnt = 0;
			for (int j = 0;j < mContFinder.getContour(i).size();j++){
				cnt++;
			}
			
			mm.addIntArg(cnt);
			
			for (int j = 0;j < mContFinder.getContour(i).size();j++){

				if (mEnableSendOSC){
					ofVec2f pt = ofxCv::toOf(mContFinder.getContour(i)[j]);
					mm.addFloatArg(pt.x / width);
					mm.addFloatArg(pt.y / height);
					pts.push_back(ofVec2f(pt));
				}
			}

			sendMessageMulti(mm);
		}

	}
	if (mEnableMean){
		means = ofxCv::mean(ofxCv::toCv(pixColor));
		means_gray = ofxCv::mean(ofxCv::toCv(pixGray));

		if (mEnableSendOSC){
			ofxOscMessage m;
			m.setAddress("/dp/cameraUnit/mean");
			m.addIntArg(means[0]);
			m.addIntArg(means[1]);
			m.addIntArg(means[2]);
			m.addIntArg(means_gray[0]);
			sendMessageMulti(m);
		}
	}

	if (mEnableOptFlowFarne){

		if ((ofxCv::mean(ofxCv::toCv(pixGray))[0] > 1.0f) &&
			(isFrameNew)) mOptFlowFarne.calcOpticalFlow(pixGray);


	}

	if (mEnableOptFlow){
		if ((ofxCv::mean(ofxCv::toCv(pixGray))[0] > 1.0f) &&
			(isFrameNew)) mOptFlow.calcOpticalFlow(pixGray);
		if ((ofGetFrameNum() % 150 == 0) ||
			(ofGetKeyPressed(' '))) mOptFlow.resetFlow();
		
		vector <ofVec2f> mot = mOptFlow.getMotion();
		
		for (int i = 0;i < 10;i++) mOptFlow_sumVecs[i].set(0.0,0.0);
		
		mOptFlow_angleVec = ofVec2f(0.0,0.0);
		
		for (int i = 0;i < mot.size();i++){
			if (mot[i].lengthSquared() < pow(mOptFlow_filterSpd,2.0f)){
				mOptFlow_sumVecs[i % 10] += mot[i];
				mOptFlow_angleVec += mot[i];
			}
		}
		mOptFlow_angleVec /= 100.0;

		
		for (int i = 0;i < 10;i++){
			mOptFlow_smoothVecs[i] += (mOptFlow_sumVecs[i] - mOptFlow_smoothVecs[i]) / mOptFlowSmooth;
		}
		
		if (mEnableSendOSC){


			ofxOscMessage m;
			m.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/features");
			m.addIntArg(mOptFlow.getFeatures().size());
			for (int i = 0;i < mOptFlow.getFeatures().size();i++){
				m.addFloatArg(mOptFlow.getFeatures()[i].x);
				m.addFloatArg(mOptFlow.getFeatures()[i].y);
			}
			sendMessageMulti(m);

			for (int i = 0;i < 10;i++){
				ofxOscMessage m;
				m.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/vector");
				m.addIntArg(i);
				m.addFloatArg(mOptFlow_smoothVecs[i].x);
				m.addFloatArg(mOptFlow_smoothVecs[i].y);
				sendMessageMulti(m);
			}

			for (int i = 0;i < 10;i++){
				ofxOscMessage m;
				m.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/vector/length");
				m.addIntArg(i);
				m.addFloatArg(mOptFlow_smoothVecs[i].length());
				sendMessageMulti(m);
			}
			
			ofxOscMessage m2;
			m2.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/vector/total");
			m2.addFloatArg(mOptFlow_angleVec.x);
			m2.addFloatArg(mOptFlow_angleVec.y);
			
			sendMessageMulti(m2);
		}
	}

}

void dpCameraUnit_cvAnalysis::drawUI(int x, int y){
	mGui.setPosition(x, y);
}

void dpCameraUnit_cvAnalysis::drawThumbnail(int x, int y, float scale){
	
//	for (int i = 0;i < pts.size();i++){
//		cout << "Circle" << i << endl;
//		ofCircle(pts[i], 5);
//	}
	
	
	ofPushMatrix();
	ofTranslate(x, y);
	glScaled(scale, scale, scale);
	ofDrawBitmapString(hakoniwa_name, 0,400);

	if (mViewSource && imgRefGray != NULL) imgRefGray->draw(0,0);
	ofSetColor(255, 255, 0);
	if (mEnableContourFinder)	mContFinder.draw();
	if (mEnableOptFlow)			mOptFlow.draw();
	if (mEnableOptFlowFarne)	mOptFlowFarne.draw();

	if (mEnableOptFlow){
		ofSetColor(255, 0, 0);
		ofPushMatrix();
		ofTranslate(160, 120);
		for (int i = 0;i < 10;i++){
			ofLine(0, 0, mOptFlow_smoothVecs[i].x*4.0, mOptFlow_smoothVecs[i].y*4.0);
			ofCircle(mOptFlow_smoothVecs[i], 3);
		}
		ofSetColor(0, 255, 0);
		ofLine(0, 0, mOptFlow_angleVec.x*30.0, mOptFlow_angleVec.y*30.0);
		ofPopMatrix();
	}

	ofSetColor(255, 0, 0);
	ofRect(0, imgRefGray->getHeight()     , means[0], 10);
	ofSetColor(0, 255, 0);
	ofRect(0, imgRefGray->getHeight() + 10, means[1], 10);
	ofSetColor(0, 0, 255);
	ofRect(0, imgRefGray->getHeight() + 20, means[2], 10);
	ofSetColor(255);
	ofRect(0, imgRefGray->getHeight() + 30, means_gray[0], 10);

	ofPopMatrix();

	ofSetColor(255);
}

void dpCameraUnit_cvAnalysis::draw(int x,int y){
	drawUI(x, y);
	drawThumbnail(x+240, y);
}

void dpCameraUnit_cvAnalysis::guiEvent(ofxUIEventArgs &ev){
	ofxUIWidget* w = ev.widget;

	if (w->getName() == "hakoniwaName"){
		ofxUITextInput* ti = (ofxUITextInput*)w;
		ti->setTextString(ti->getTextString());
		hakoniwa_name = ti->getTextString();
	}
	
}

void dpCameraUnit_cvAnalysis::sendMessageMulti(ofxOscMessage &m){
	if (!mEnableSendOSC) return;

	string defAdd = addUI->getTextString();
	int defPort = ofToInt(portUI->getTextString());

	sender.setup(defAdd, defPort);
	sender.sendMessage(m);

	for (int i = 0;i < oscListPtr->size();i++){
		if (oscMatrixUI->getState(0, i)){
			sender.setup((*oscListPtr)[i], defPort);
			sender.sendMessage(m);
		}

	}
}