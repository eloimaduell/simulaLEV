#pragma once
#include "ofMain.h"
#include "ofxGui.h"

//#define SPRING_CONSTANT 0.1f
//#define MAX_VELOCITY 30.0f

class simLaser : public ofNode {

//	struct particle {
//		ofVec3f position;
//		ofVec3f velocity;
//		ofColor color;
//	};

	public:
	
	
	simLaser();
	void		init(int number,float orientationX,float orientationY,float _orientationZ);
	void		customDraw();
	// we call this update function ourselves
	//  at the beginning of customDraw
	void        update();
	
//    void        setPosition(float x, float y, float z);
//    ofPoint     getPosition();
	void		setRotationX(float f);
	int			getRotationX();
	void		setorientationX(float f);
	float		getorientationX();

	void		setRotationY(float f);
	int			getRotationY();
	void		setorientationY(float f);
	float		getorientationY();
	
	void		setorientationZ(float f);
	float		getorientationZ();
    void		setRotationZ(float f);
	int			getRotationZ();

	void		setDimmer(float f);
	int			getDimmer();
    void        setColor(ofColor color);
	
	int                 number;
	string              name;
    
	int                 baseHeight;
	int                 baseWidth;
	
    bool                drawAxis;

	ofParameter<float>    p_rotationX;
	ofParameter<float>    p_rotationY;
	ofParameter<float>    p_rotationZ;

    float               orientationX;
    float               orientationY;
    float               orientationZ;
    
    ofQuaternion        quatOrientation;

	ofParameter<int>    p_dimmer;
	ofColor             color;

    
    // speed lim Tilt
    float               maxTiltSpeed;
    float               lastTiltValue;
    bool                tiltSpeedLimit;
    unsigned long long  lastTiltTime;
    int                 interOscTime;
    float*                bufferSpeedLim;
    int                 bufferIt;

    ofPoint             aPointInLaser;
    
	
	// GUI
	ofxPanel            gui;
	ofxLabel            gui_status;
	int                 i_rotation;
	int                 heightOffset;
	void                sliderRota(int &f);
	void                resizeWindow();
	ofTrueTypeFont      font;
    
    int                 numLasers;
    
	
protected:

};