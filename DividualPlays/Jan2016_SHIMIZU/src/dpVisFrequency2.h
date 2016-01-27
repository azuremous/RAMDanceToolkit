//
//  dpVisFrequency2.h
//  RAMDanceToolkit
//
//  Created by Junichi Shimizu on 1/26/16.
//
//

#ifndef __RAMDanceToolkit__dpVisFrequency2__
#define __RAMDanceToolkit__dpVisFrequency2__

#include "ramMain.h"

class Frequency2: public ramBaseScene{
    
public:
    // GUIに表示されるシーン名をここで指定します。
    string getName() const { return "Frequency2"; }
    
    void setup();
    void update();
    void draw();
    
    void receiveOSC();
    void caculateFrequency();
    
    void radiusCaculate();
    
private:
    
    ramOscReceiveTag mReceiver1;
    ramOscReceiveTag mReceiver2;
    string oscString;
    
    static const int NUM = 3;
    float blinkInterval[NUM];
    float blinkSpeed[NUM];
    float frequency;
    
    
    //position
    ofVec2f position[11];
    //speed
    ofVec2f velocity[NUM];
    
    float radius[11];
    float frequncy_array[11];
    int red[11];
    int green[11];
    int blue[11];
    int j[11];

};

#endif
    /* defined(__RAMDanceToolkit__dpVisFrequency2__) */
