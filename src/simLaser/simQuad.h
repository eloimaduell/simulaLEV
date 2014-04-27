#pragma once
#include "ofMain.h"
#include "ofxGui.h"

class simQuad : public ofNode {

//	struct particle {
//		ofVec3f position;
//		ofVec3f velocity;
//		ofColor color;
//	};

	public:
	
	
	simQuad();
	void		init(string name,ofRectangle _rectangle,int mode);
	void		customDraw();
	// we call this update function ourselves
	//  at the beginning of customDraw
	void        update();
	
	void		setDimmer(float f);
	float			getDimmer();
	
    ofRectangle         rectangle;
	string              name;
    bool                drawAxis;
	ofParameter<float>    p_dimmer;
    ofColor             quadColor;
    int                 mode;   // 0 : light  // 1 : strobe
    float               strobeFallOff;
    
    void setColor(ofColor _color);
    
	// GUI
	ofxPanel            gui;
	ofxLabel            gui_status;
	void                resizeWindow();
	ofTrueTypeFont      font;

    ofImage             image;
protected:

};