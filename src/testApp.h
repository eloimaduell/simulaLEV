// TODO
// areglar que els simQuads ara no pinten per customdraw !

#pragma once

#include "ofMain.h"

// Custom objects for this example
#include "Grid.h"
#include "OrthoCamera.h"
#include "simLaser.h"
#include "simQuad.h"
//#include "simParticle.h"
#include "pmThreadedOscReceiver.h"
#include "ofxGameCamera.h"

#include "ofxAssimpModelLoader.h"
#include "ofxFBX.h"

#define N_CAMERAS 3

class testApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void setupViewports();
		void drawScene(int iCameraDraw);
        void drawLasers();
        void drawLightsHelpers();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

    
    void setupGUI();
    void setupFBOs(int w,int h);
    void setupLasers();
    void setupPars();
    
		//cameras (all these inherit from ofCamera)
		ofEasyCam camEasyCam;
        ofxGameCamera   gameCam;
		orthoCamera camFront;
		orthoCamera camTop;
		orthoCamera camLeft;
        ofVec2f windowSize;
        ofVec2f changedWindowSize;
        float windowAspect;
    
    void enableFog(float near, float far);
    void disableFog();

    
		//cameras have parent?
		bool bCamParent;

		//camera pointers
		ofCamera * cameras[N_CAMERAS];
		int iMainCamera;

		//viewports
		ofRectangle viewMain;
		ofRectangle viewGrid[N_CAMERAS];

		//my custom node
		grid nodeGrid;

		//ray drawn under mouse cursor [start,end]
		ofVec3f ray[2];
	
	
	//-------------
	
	// -- objects
	
	vector<simLaser*>    lasers;
	vector<simLaser*>    lasersM;
    vector<ofLight*>      OFlights;
    
	// -- camera
	void			setupCameras(bool resetTopFront);
	float			cameraDistance;
	float			cameraFov;
	float			camOrientX;
	float			camOrientY;
	float			camOrientZ;
	float			camOrientW;
	float			camPosX;
	float			camPosY;
	float			camPosZ;
	bool			drawGrid;
    float           floorWidth;
    float           floorHeight;
    vector<ofColor> colorTable;
	
	// gui
	ofParameter	<ofVec3f> p_orientation;
	ofParameter	<ofVec3f> p_orientation2;
	ofParameter	<ofVec3f> p_orientation3;
	ofParameter	<float> p_rotationZ;
	ofParameter	<int> p_dimmer;
	ofParameter	<int> p_alphaMix;
    ofParameter <int> p_viewMode;
	ofParameter	<bool> p_lasersActive;
	ofParameter	<bool> p_backgroundHigh;
    ofParameter <bool> p_showAxis;
    ofParameter <bool> p_resetCam;
    ofParameter <float> p_speedCam;
    ofParameter <bool> p_printOscOnConsole;
    ofParameter <bool> p_showGui;
    ofParameter <bool> p_showModel;
    ofParameter <bool> p_renderSolid;
    ofParameter <bool> p_renderWireframe;
    
    
    ofParameter <float> p_lightAttConstant;
    ofParameter <float> p_lightAttLinear;
    ofParameter <float> p_lightAttQuadratic;
    
    ofParameter <float> p_fov;
    ofParameter <ofVec3f> p_point;
    
	ofxPanel		gui;

    void            orientationChanged(ofVec3f &f);
    void            orientationChanged2(ofVec3f &f);
    void            orientationChanged3(ofVec3f &f);
	void			sliderRotationZAll(float &f);
	void			sliderDimmerAll(int &f);
    void            sliderFov(float &f);
    void            toggleDrawAxis(bool &b);
    void            sliderViewMode(int &f);
    void            toggleResetCam(bool &f);
    void            toggleShowGui(bool &f);
    void            changedCamSpeed(float &f);
    
    bool            setupFinished;
	
	//-- osc
	ofxOscReceiver	oscRec;
	void			receiveOscMessages();
    void            newOscMessageReceivedOnTestApp(pmOscMessage &mess);
	
	
	// -- graphics
	ofFbo			fbo;
    ofFbo           fboBlur;
    ofFbo           fboComp;

    ofImage         logo;
    
    ofMutex         mutex;
    
    /// 3d MODEL
    ofxFBXScene             fbx;
    vector<ofxFBXMesh*>     meshes;
    vector<ofxFBXLight*>    lights;
    vector<ofxFBXNode*>     nulls;
    
};
