#include "testApp.h"
#include <string>

int nColor;

//--------------------------------------------------------------
void testApp::setup(){

    
    ofSetLogLevel(OF_LOG_VERBOSE);

	ofBackground(0, 0,0);
    setupFinished = false;
    /// INIT VARIABLES
    

	drawGrid = true;
    nColor=0;
	iMainCamera = 0;

	camOrientX =0.06696;
	camOrientY =-0.0125;
	camOrientZ =-0.0030;
	camOrientW =0.9975;
    
    camPosX =-12;
    camPosY =178;
    camPosZ =2475;
    
    logo.loadImage("./app/logoPM.png");

    
	/// OSC
    cout << "setting up OSC at port " <<9000 <<" ." <<endl;
    oscRec.setup(9000);
    
    /// SETUP ALL
 
    setupLasers();
    
    setupPars();
    
    setupGUI();
    
    setupFBOs(ofGetWidth(),ofGetHeight());
	
    setupCameras(true);
    // reset to load new params
    gameCam.loadCameraPosition();
    windowResized(ofGetWidth(),ofGetHeight());

    /// DEFERRED SHADING
    // needed for ofxDeferredShading
    ofDisableArbTex();

    deferred.setup(&gameCam);
    
    
    /// 3D MODEL DDD
    
    string modelFilename = "levLightsScaled.fbx";
    fbx.load(modelFilename);
    
    /// MESHES
    meshes = fbx.getMeshes();
    for(int i=0;i<meshes.size();i++)
    {
        //cout << "mesh " <<i <<" : " <<meshes[i]->getName().c_str() <<endl;
        cout << "MESH Name " <<i <<" : " <<fbx.meshNames[i] <<endl;
    }
    /// LIGHTS
    //ofSetSmoothLighting(true);
    //ofSetGlobalAmbientColor(ofColor(0, 0, 0));
    
    lights = fbx.getLights();
    for(int i=0;i<lights.size();i++)
    {
        cout << "adding FBX light at " << lights[i]->getPositionAtFrame(0);
        //void addLight(ofVec3f _pos, ofColor _ambient, ofColor _diffuse, ofColor _specular, ofVec3f _attenuation);
		deferred.addLight(lights[i]->getPositionAtFrame(0), ofFloatColor(0.0f, 0.0f, 0.0f), ofFloatColor(1.0, 1.0, 1.0), ofFloatColor(1.0, 1.0, 1.0),p_attenuationContLinQuad);
    }
    
    
    /// NULLS
    
    /// setup LASERS VECTORS
    
    for(int i=0;i<8;i++)
    {
        // 8 x LASERS MOVILES
        simLaser* las = new simLaser();
        lasersM.push_back(las);
    }
    for(int i=0;i<16;i++)
    {
        // 16 x LASERS FIJOS
        simLaser* las = new simLaser();
        lasers.push_back(las);
    }
    
    nulls = fbx.getNulls();
    
    int countMoviles=0;
    int countFixed=0;

    for(int i=0;i<nulls.size();i++)
    {
        cout << "NULL Name " <<i <<" : " <<fbx.nullNames[i] <<endl;
        cout << "---- position : " << nulls[i]->getPositionAtFrame(0) <<endl;
        cout << "---- rotationEuler : " << nulls[i]->getNode().getOrientationEuler() <<endl;
        cout << "---- quaternion : " << nulls[i]->getNode().getOrientationQuat() <<endl;

        if(fbx.nullNames[i].find("laser")!=-1)
        {
            
            
            ofVec3f *vec = new ofVec3f();
            ofVec3f *pos = new ofVec3f();
            ofQuaternion* quat = new ofQuaternion();
            ofVec3f *vecCorrect = new ofVec3f();
            
            *quat = nulls[i]->getNode().getOrientationQuat();
            *vec = nulls[i]->getNode().getOrientationEuler();
            *pos = nulls[i]->getPositionAtFrame(0);
            
            // EXCHANGE Z and X ?!?
            vecCorrect->x = vec->z;
            vecCorrect->y = vec->y;
            vecCorrect->z = vec->x;
        
            int whereIsUnderscore=fbx.nullNames[i].find("_");
            int whichLaser = ofToInt(fbx.nullNames[i].substr(whereIsUnderscore+1))-1;
            
            if(fbx.nullNames[i].find("laserM")!=-1)
            {
                // LASER MOVIL
                cout << "found a laser MOVIL with id : " <<" _ " << fbx.nullNames[i].substr(whereIsUnderscore+1) <<" which is ID :: " << whichLaser <<endl;
                cout << endl;
                cout << endl;
                
                // BLUE LASER
                lasersM[whichLaser]->setColor(ofColor(0,0,255));

                lasersM[whichLaser]->numLasers=8;
                lasersM[whichLaser]->name=fbx.nullNames[countMoviles];
                string laserName = "L_" +ofToString(countMoviles+1);
                lasersM[whichLaser]->init(laserName,1, 0, 0,0);
                lasersM[whichLaser]->setPosition(*pos);
                
                lasersM[whichLaser]->setorientationX(vecCorrect->x);
                lasersM[whichLaser]->setorientationY(vecCorrect->y);
                lasersM[whichLaser]->setorientationZ(vecCorrect->z);
                
                lasersM[whichLaser]->quatOrientation = *quat;
                
                countMoviles=countMoviles+1;

            }
            else if((fbx.nullNames[i].find("laserR")!=-1)||(fbx.nullNames[i].find("laserB")!=-1))
            {
                // LASER MOVIL
                cout << "found a laser FIEXED with id : " <<" _ " << fbx.nullNames[i].substr(whereIsUnderscore+1) <<" which is ID :: " << whichLaser <<endl;
                
                string letter;
                if((fbx.nullNames[i].find("laserB")!=-1))
                {
                    lasers[whichLaser]->setColor(ofColor(0,0,255));
                    letter="B";
                }
                else
                {
                    lasers[whichLaser]->setColor(ofColor(255,0,0));
                    letter="R";
                }
                
                lasers[whichLaser]->numLasers=16;
                lasers[whichLaser]->name=fbx.nullNames[i];
                string laserName = "L" + letter + "_" +ofToString(countFixed+1);

                lasers[whichLaser]->init(laserName,1, 0, 0,0);
                lasers[whichLaser]->setPosition(*pos);
                
                lasers[whichLaser]->setorientationX(vecCorrect->x);
                lasers[whichLaser]->setorientationY(vecCorrect->y);
                lasers[whichLaser]->setorientationZ(vecCorrect->z);
                
                lasers[whichLaser]->quatOrientation = *quat;
                
                countFixed=countFixed+1;
                
            }
        }
    }
    
    setupFinished = true;
    
    /*
    // LIGHTS
    ///////////
    ofSetSmoothLighting(true);
    ofSetGlobalAmbientColor(ofColor(0, 0, 0));
    
    //ofEnableSeparateSpecularLight();

    ofLight light;
    
    light.setup();
    light.setPointLight();
    light.setAmbientColor(ofColor(0.0f, 0.0f, 0.0f));
    light.setDiffuseColor( ofFloatColor(1.0, 0.5, 0.0) );
    light.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    light.setPosition(0,0, 75);
    light.enable();

    lights.push_back(light);

    ofLight light2;

    light2.setup();
    light2.setPointLight();
    light2.setAmbientColor(ofColor(0.0f, 0.0f, 0.0f));
    light2.setDiffuseColor( ofFloatColor(0.0, 0.5, 1.0) );
    light2.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    light2.setPosition(0,0, 75);
    light2.enable();

    lights.push_back(light2);

    ofLight light3;
    
    light3.setup();
    light3.setPointLight();
    light3.setAmbientColor(ofColor(0.0f, 0.0f, 0.0f));
    light3.setDiffuseColor( ofFloatColor(0.75, 0.75, 0.75) );
    light3.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    light3.setPosition(0,0, 75);
    light3.enable();
    
    lights.push_back(light3);

     */
    
    //some model / light stuff
    //glShadeModel(GL_SMOOTH);
    //  light.enable();
 
}


//----------------------------------------------------------------------------------------------------------------------------
void testApp::setupGUI()
{
	/// GUI
    ////////
    
    cout << "setting up GUI ." ;

    gui.setUseTTF(true);
	p_lasersActive.set(true);
	p_backgroundHigh.set(false);
    p_showGui.set(false);
    
    

    // we add this listener before setting up so the initial state is correct
    p_rotationZ.addListener(this,&testApp::sliderRotationZAll);
    //p_rotationY.addListener(this,&testApp::sliderRotationYAll);
    p_viewMode.addListener(this,&testApp::sliderViewMode);
	p_dimmer.addListener(this,&testApp::sliderDimmerAll);
    p_showAxis.addListener(this, &testApp::toggleDrawAxis);
    p_resetCam.addListener(this, &testApp::toggleResetCam);
    p_showGui.addListener(this,&testApp::toggleShowGui);
    p_fov.addListener(this,&testApp::sliderFov);
    p_speedCam.addListener(this,&testApp::changedCamSpeed);
    
    gui.setup("simula v1.0 LEV"); // most of the time you don't need a name
	gui.setPosition(10,10);

    gui.add(p_rotationZ.set("rot_Z",0,-90,90));
    //gui.add(p_rotationY.set("pan_Y",0,-270,270));
	gui.add(p_dimmer.set("dimmer aLL",255,0,255));
    gui.add(p_viewMode.set("view mode",1,0,3));
    gui.add(p_alphaMix.set("alpha trail",50,0,255));
    gui.add(p_fov.set("cam angle",120.0,0,180));
    gui.add(p_speedCam.set("cam speed",10.00,0.001,20.0));
    gui.add(p_showAxis.set("axis",false));
    gui.add(p_resetCam.set("reset cam",false));
    gui.add(p_backgroundHigh.set("bckgrnd color",false));
    gui.add(p_showGui.set("show Gui",false));
    gui.add(p_lasersActive.set("show Lasers",true));
    gui.add(p_showModel.set("show 3D",true));
    gui.add(p_renderSolid.set("render solid",true));
    gui.add(p_renderWireframe.set("render wire",false));

    gui.add(p_printOscOnConsole.set("OSC console",false));
    
    gui.add(p_attenuationContLinQuad.set("light attenuation Const Lin Quadr",ofVec3f(0.0,0.0,0.000006),ofVec3f(0,0,0),ofVec3f(2.0,0.025,0.00015)));
    
    gui.setSize(200,400);
    gui.setWidthElements(200);
    
    //gui.loadFromFile(ofToDataPath("",true) +"./simulaLEV.xml");
    float f;
    f = p_fov;
    sliderFov(f);
    setupCameras(false);
    camEasyCam.setFov(f);
    
    cout << "... DONE" <<endl;
	
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::setupLasers()
{
//
//    
//    ofPoint lasersOrigin;
//    lasersOrigin.set(365.119,-207.071,316.862);
//    float posStep = 400 / ((numLasers/2)-1);
//    
//    cout << "setting up LASERS ." <<endl;
//
//    for(int i=0;i<numLasers;i++)
//    {
//        simLaser* las = new simLaser();
//        lasers.push_back(las);
//        lasers[i]->baseHeight=0;
//        lasers[i]->numLasers=numLasers;
//        
//        //lasers[i].init((i),i*(angleStep));
//        if(i+1>ceil(numLasers/2.0))
//            //if(true)
//        {
//            cout << " Second Half of lasers : ";
//
//            lasers[i]->init((i),0,0,0);
//            lasers[i]->setPosition(-lasersOrigin.x,lasersOrigin.y+posStep*(i-ceil(numLasers/2.0)),lasersOrigin.z);
//        }
//        else
//        {
//            cout << " First half of lasers ";
//            lasers[i]->init((i),0,0,0);
//            lasers[i]->setPosition(lasersOrigin.x,lasersOrigin.y+posStep*i,lasersOrigin.z);
//            
//        }
//        
//        lasers[i]->setColor(ofColor(0,0,255));
//        
//        cout << "Laser " <<i <<" setup at " <<lasers[i]->getPosition() <<endl;
//    }
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::setupPars()
{
    cout << "setting up PARS .";
    
    cout << "... DONE." <<endl;

}

//----------------------------------------------------------------------------------------------------------------------------

void testApp::setupFBOs(int w, int h)
{
    cout << "setting up FBO at " <<w <<" . " <<h <<" .";

    windowSize.x = w-200.0;
    windowSize.y = h-170.0;
    windowAspect = windowSize.x / windowSize.y;
    
    fbo.allocate(windowSize.x,windowSize.y, GL_RGBA);
    fboComp.allocate(windowSize.x,windowSize.y, GL_RGBA);
    fboBlur.allocate(windowSize.x,windowSize.y, GL_RGBA);
    
    fbo.begin();
    ofClear(255, 255, 255, 0);
    fbo.end();
    fboComp.begin();
    ofClear(255, 255, 255, 255);
    fboComp.end();
    fboBlur.begin();
    ofClear(255, 255, 255, 0);
    fboBlur.end();
    
    cout << "... DONE" <<endl;

    
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::setupCameras(bool resetTopFront)
{
    cout << "setting up Cameras .";

    
	bCamParent = false;
	
    // game camera
    gameCam.setup();
    gameCam.autosavePosition=false;
    
	// user camera
	cameraDistance=1000.0;
	camEasyCam.setDistance(cameraDistance);
	camEasyCam.setFov(p_fov);
	cameras[0] = &gameCam;
	camEasyCam.setFarClip(50);
	camEasyCam.setNearClip(10);
    camEasyCam.setTranslationKey('z');
	camEasyCam.setOrientation(ofQuaternion(camOrientX,camOrientY,camOrientZ,camOrientW));
    camEasyCam.setPosition(camPosX, camPosY, camPosZ);
	
    //strange thing happens with p_resetCam behaviour where front and top cameras flip when reset is pressed; trying to avoid it
    if(resetTopFront)
    {
        // front
        camFront.scale = 800;
        camFront.roll(180);
        camFront.pan(0);
        camFront.setScale(-1,1,1);
        camFront.setPosition(0,400,0);
        cameras[1] = &camFront;
        
        // top
        camTop.scale = 1600;
        camTop.tilt(-90);
        camTop.setScale(-1, 1,1);
        camTop.roll(180);
        camTop.setPosition(0,0,0);
        cameras[2] = &camTop;
        
        // Define viewports
        setupViewports();
    }
    cout << "... DONE" <<endl;

}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::setupViewports()
{
    
	//call here whenever we resize the window
	cout << "setting up VIEWPORTS .";
    
    if(p_viewMode==0)
    {
        viewMain.x = 0;
        viewMain.y = changedWindowSize.y/2.0;
        viewMain.width = changedWindowSize.x;
        viewMain.height = changedWindowSize.y/2.0;

        // cam front
        viewGrid[0].x = 0;
        viewGrid[0].y = 0;
        viewGrid[0].width = changedWindowSize.x/2;
        viewGrid[0].height = changedWindowSize.y/2;
        
        // cam top
        viewGrid[1].x = changedWindowSize.x/2;
        viewGrid[1].y = 0;
        viewGrid[1].width = changedWindowSize.x/2;
        viewGrid[1].height = changedWindowSize.y/2;
    }
    else if (p_viewMode==1)
    {
        viewMain.x = 0;
        viewMain.y = 0;
        viewMain.width = changedWindowSize.x;
        viewMain.height = changedWindowSize.y;
        
    }
    else if (p_viewMode==2)
    {
        // cam front
        viewGrid[0].x = 0;
        viewGrid[0].y = 0;
        viewGrid[0].width = changedWindowSize.x;
        viewGrid[0].height = changedWindowSize.y;
    }
    else if (p_viewMode==3)
    {
        // cam top
        viewGrid[1].x = 0;
        viewGrid[1].y = 0;
        viewGrid[1].width = changedWindowSize.x;
        viewGrid[1].height = changedWindowSize.y;
    }
    cout << "... DONE" <<endl;

}

//----------------------------------------------------------------------------------------------------------------------------
ofVec2f calculatePolarAlfaBeta(ofPoint laser,ofPoint point,int orient)
{
  
    ofVec2f result;
    ofPoint myVector = point - laser;
    
    /*
    float R = myVector.length();

    
    result[0]=ofRadToDeg(atan2(myVector.y,myVector.x));
    result[1]=ofRadToDeg(acos(-myVector.z/R));

    if(orient==0)
    {
        result[0]=90.0-result[0];
        result[1]=90.0-result[1];
    }
    else
    {
        result[0]=90.0-result[0];
        result[1]=90.0+result[1];
    }
    cout << "alfa beta " <<result  <<" \ R = " <<R <<" VECTOR : " <<myVector << endl;
*/
    
                          
    float R1 = sqrt((myVector.x*myVector.x) + (myVector.z*myVector.z));
    float R2 = sqrt((R1*R1) + (myVector.y*myVector.y));

    

    if(orient==0)
    {
        result[1]=-ofRadToDeg(asin(myVector.z/R1));
        result[0]=90+ofRadToDeg(asin(-myVector.y/R2));
    }
    else
    {
        result[1]=ofRadToDeg(asin(myVector.z/R1));
        result[0]=90+ofRadToDeg(asin(-myVector.y/R2));
    }

    
    
    //cout << "alfa beta " <<result  <<" \ R = " <<R1 <<" " <<R2 <<" VECTOR : " <<myVector << endl;
    
    return(result);
}



//----------------------------------------------------------------------------------------------------------------------------
void testApp::update(){

	receiveOscMessages();
    
    
    
    if(p_resetCam)
    {
        camOrientX =0.06696;
        camOrientY =-0.0125;
        camOrientZ =-0.0030;
        camOrientW =0.9975;
        
        camPosX =-12;
        camPosY =178;
        camPosZ =2475;

        setupCameras(false);
        p_resetCam=false;
    }
    
    
//    camEasyCam.setFarClip(50);
//	camEasyCam.setNearClip(10);

}


//----------------------------------------------------------------------------------------------------------------------------
void testApp::draw()
{
    //ofBackground(35,35,35);

    // DRAW INTO FBO
    /////////////////
    
    fbo.begin();
    {
        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        
        
         if(!p_backgroundHigh)
         {
             //ofClear(0,0,0,p_alphaMix);
             //ofBackground(ofColor(128,128,128,p_alphaMix));
             ofClear(0,0,0,255);
             
         }
        else
        {
            //ofClear(128,128,128,p_alphaMix);
            ofClear(255,255,0,255);
            
        }
        

        
        // draw main viewport
        //////////////////////
        if(iMainCamera ==0) 
        {
            if(p_viewMode==0 || p_viewMode==1)
            {
                cameras[0]->begin(viewMain);
                ofPushMatrix();
                drawScene(0);
                ofPopMatrix();
                cameras[0]->end();
            }

            if(p_viewMode==0 || p_viewMode==2)
            {
                cameras[1]->begin(viewGrid[0]);
                ofPushMatrix();
                drawScene(1);
                ofPopMatrix();
                cameras[1]->end();
            
            }
            if(p_viewMode==0 || p_viewMode==3)
            {
                cameras[2]->begin(viewGrid[1]);
                ofPushMatrix();
                drawScene(2);
                ofPopMatrix();
                cameras[2]->end();
            }
        }
        else {
            cameras[iMainCamera]->begin(viewGrid[iMainCamera-1]);
            ofPushMatrix();
            drawScene(iMainCamera);
            ofPopMatrix();
            cameras[iMainCamera]->end();		
        }
        
    ofDisableBlendMode();
                          
    }
	fbo.end();


    // composite old and new
	fboComp.begin();
	{
		
        ofSetColor(255, 255, 255, 255);
		fboBlur.draw(0, 0, fboComp.getWidth (), fboComp.getHeight());
		ofSetColor(255, 255, 255, 255);
		fbo.draw(0, 0, fboComp.getWidth(), fboComp.getHeight());
	}
	fboComp.end();
	fboBlur.begin();
	{
		ofSetColor(255, 255, 255, 255);
		fboComp.draw(0, 0, fboBlur.getWidth(), fboBlur.getHeight());
	}
	fboBlur.end();
    
    // draw FBO
    ////////////
    
	ofSetColor(255);
	ofPushMatrix();
//	glTranslated((ofGetWidth()/2)+220,ofGetHeight()/2,0);
//	glRotatef(180.0, 0.0, 0.0,-1.0);
//	glTranslated(-ofGetWidth()/2,-ofGetHeight()/2,0);
    
    windowAspect = windowSize.x / windowSize.y;
    windowSize.x = ofGetWidth()-200.0;
    if(p_showGui) windowSize.y = ofGetHeight()-170.0;
    else windowSize.y = ofGetHeight()-10;

	fboComp.draw(200+10,10,changedWindowSize.x,changedWindowSize.y); //,ofGetWidth()-220,ofGetHeight()-200);
	ofPopMatrix();
	
	// draw gui
    ////////////
	gui.draw();

    if(p_showGui)
    {
        for(int i=0;i<lasers.size();i++)
        {
            ofPushMatrix();
            //mutex.lock();
            lasers[i]->gui.draw();
            //mutex.unlock();
            ofPopMatrix();
        }
        for(int i=0;i<lasersM.size();i++)
        {
            ofPushMatrix();
            //mutex.lock();
            lasersM[i]->gui.draw();
            //mutex.unlock();
            ofPopMatrix();
        }
    }
    
    ofSetColor(128);
    ofDrawBitmapString(ofToString(ofGetFrameRate()),ofGetWidth()-20,10);
    
    ofSetColor(255,255,255);
    ofPushMatrix();
    logo.draw(30,ofGetHeight()-50,165,25);
    ofPopMatrix();
    
}


/*
float DistancePointLine( ofPoint Point, ofPoint LineStart, ofPoint LineEnd)
{
    float Distance;
    float LineMag;
    float U;
    ofPoint Intersection;
    
    LineMag = ofPoint(LineEnd-LineStart).length();
    
    U = ( ( ( Point.x - LineStart.x ) * ( LineEnd.x - LineStart.x ) ) +
         ( ( Point.y - LineStart.y ) * ( LineEnd.y - LineStart.y ) ) +
         ( ( Point.z - LineStart.z ) * ( LineEnd.z - LineStart.z ) ) ) /
    ( LineMag * LineMag );
    
//    if( U < 0.0f || U > 1.0f )
//        return 0;   // closest point does not fall within the line segment
    
    Intersection.x = LineStart.x + U * ( LineEnd.x - LineStart.x );
    Intersection.y = LineStart.y + U * ( LineEnd.y - LineStart.y );
    Intersection.z = LineStart.z + U * ( LineEnd.z - LineStart.z );
    
    Distance = ofPoint(Point-Intersection).length();
    
    return Distance;
}
*/

//----------------------------------------------------------------------------------------------------------------------------
float distancePointSegment(ofPoint P, ofPoint P0, ofPoint P1)
{
    /*
     
     NEW PSEUDO_CODE !!
     
     distance( Point P,  Segment P0:P1 )
     {
     v = P1 - P0
     w = P - P0
     
     if ( (c1 = wáv) <= 0 )  // before P0
     return d(P, P0)
     if ( (c2 = váv) <= c1 ) // after P1
     return d(P, P1)
     
     b = c1 / c2
     Pb = P0 + bv
     return d(P, Pb)
     }
     */

    ofPoint v = P1-P0;
    ofPoint w = P-P0;
    
    float c1 = w.dot(v);
    float c2 = v.dot(v);
    
    if(c1<=0)
    {
        // before P0
        ofPoint PP0 = P-P0;
        return(PP0.length());
    }
    if(c2<=c1)
    {
        // after P1
        ofPoint PP1 = P-P1;
        return(PP1.length());
    }
    
    float b = c1/c2;
    ofPoint Pb = P0 + (b*v);
    ofPoint PPb = P-Pb;
    return(PPb.length());
}
//----------------------------------------------------------------------------------------------------------------------------
void testApp::drawLasers()
{
    // DRAW LASERS DLL
    ////////////////
    
	if(p_lasersActive)
	{
		for(int i=0;i<lasers.size();i++)
		{
            ofPushMatrix();
            
            if((i>=0)&&(i<16)) //||((i>=8)&&(i<=14)))
            {
             lasers[i]->customDraw();
            }

            ofPopMatrix();
            
		}
		for(int i=0;i<lasersM.size();i++)
		{
            ofPushMatrix();
            
            lasersM[i]->customDraw();
            
            ofPopMatrix();
            
		}
	}

}
//----------------------------------------------------------------------------------------------------------------------------
void testApp::drawLightsHelpers()
{
    for(int i=0;i<OFlights.size();i++)
    {
        glPointSize(20);
        glBegin(GL_POINTS);
        ofSetColor(255,0,0);
        glVertex3f(OFlights[i]->getPosition().x,OFlights[i]->getPosition().y,OFlights[i]->getPosition().z);
        glEnd();

        
    }
    
}
//----------------------------------------------------------------------------------------------------------------------------
void testApp::drawScene(int iCameraDraw)
{
    // CCC
    //enableFog(50,700);
    
    //glEnable(GL_DEPTH_TEST);

    
	// draw grid?
	if (p_showAxis)
    {
        //ofEnableAlphaBlending();
        
        //nodeGrid.draw();
        // AXIS
        ofPushMatrix();
        int axisSize = 1000;
        ofSetColor(255,0,0);
        ofLine(0,0,0,axisSize,0,0);
        ofSetColor(0,255,0);
        ofLine(0,0,0,0,axisSize,0);
        ofSetColor(0,0,255);
        ofLine(0,0,0,0,0,axisSize);
        ofPopMatrix();
    }

    //ofEnableDepthTest();
    
    if(p_showAxis) drawLightsHelpers();
    
    drawLasers();
    
    if(p_showModel)
    {
        /// DEFERRED SHADING
        deferred.begin();
        
        ofPushMatrix();
        ofPushStyle();

        for(int i=0;i<meshes.size();i++)
        {
            

            if(p_renderSolid)
            {
                ofMaterial m1;
                ofColor ambientColor = ofColor(0,0,0);
                ofColor diffuseColor = ofColor(200,0,0);
                ofColor specularColor = ofColor(0,255,200);
                m1.begin();
                m1.setShininess(99.26);
                m1.setAmbientColor(ambientColor);
                m1.setDiffuseColor(diffuseColor);
                m1.setSpecularColor(specularColor);
                
                meshes[i]->draw();

                m1.end();
            }
            
            if (p_renderWireframe)
            {
                ofPushMatrix();
                ofScale(1.005f,1.005f,1.005f);
                
                ofMaterial mW;
                ofColor ambientColor = ofColor(0,0,0);
                ofColor diffuseColor = ofColor(0,255,0);
                ofColor specularColor = ofColor(0,255,0);
                mW.begin();
                mW.setShininess(99.26);
                mW.setAmbientColor(ambientColor);
                mW.setDiffuseColor(diffuseColor);
                mW.setSpecularColor(specularColor);

                meshes[i]->drawWireframe();
                
                mW.end();
                
                ofPopMatrix();
            }
        }
        
        deferred.end();
        
        
    }
    //ofDisableLighting();
    
    ofPopMatrix();
    
    ofPopStyle();
    
	//ofDisableDepthTest();
    
    //disableFog();

}


//----------------------------------------------------------------------------------------------------------------------------
void testApp::keyPressed(int key){

    if(key == 'L')
	{
        gui.saveToFile(ofToDataPath("",true) +"simulaLEV.xml");
        cout << "trying to save to : " <<ofToDataPath("",true) +"simulaLEV.xml" <<endl;
	}
    else if(key == 'l')
	{
        gui.loadFromFile(ofToDataPath("",true) +"./simulaLEV.xml");
	}

	else if(key == '1')
	{
        p_viewMode = 0;
	}
	if(key == '2')
	{
        p_viewMode = 1;
	}
	if(key == '3')
	{
        p_viewMode = 2;
	}
	if(key == '4')
	{
        p_viewMode = 3;
	}
	if(key == 'r')
    {
        float fR = ofRandomf();
        float fG = ofRandomf();
        float fB = ofRandomf();
        for(int i=0;i<OFlights.size();i++)
        {
            OFlights[i]->setDiffuseColor(ofFloatColor(fR,fG,fB));
        }
        OFlights[0]->setDiffuseColor(ofFloatColor(1,1,1));
        OFlights[7]->setDiffuseColor(ofFloatColor(1,1,1));
        
    }
	if(key == 'f'){
		ofToggleFullscreen();
        windowResized(ofGetWidth(),ofGetHeight());
	}

	if(key == 'P'){
        gameCam.saveCameraPosition();
        printf("!! saved game camera position\n");
	}
	if(key == 'p'){
        gameCam.loadCameraPosition();
        printf("load game camera position\n");
	}

}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::keyReleased(int key){
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	camOrientX = camEasyCam.getOrientationQuat().x();
	camOrientY = camEasyCam.getOrientationQuat().y();
	camOrientZ = camEasyCam.getOrientationQuat().z();
	camOrientW = camEasyCam.getOrientationQuat().w();
	
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    //printf("mouse %d %d\n",x,y);
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
    cout << "Window is resizing ." <<endl;

    windowSize.x = ofGetWidth()-200.0;
    
    if(p_showGui) windowSize.y = ofGetHeight()-170.0;
    else windowSize.y = ofGetHeight()-10;
    
    windowAspect = windowSize.x / windowSize.y;
    
    if(windowAspect>1.3333)
    {
        // we need to shorten X
        changedWindowSize.y = windowSize.y;
        changedWindowSize.x = changedWindowSize.y * 1.33333;
    }
    else
    {
        // we need to shorten X
        changedWindowSize.x = windowSize.x;
        changedWindowSize.y = changedWindowSize.x / 1.3333;
    }

    // update easyCam
    //camEasyCam.viewportForMouse.x=210;
    //camEasyCam.viewportForMouse.y=10;
//    camEasyCam.viewportForMouse.width=changedWindowSize.x;
  //  camEasyCam.viewportForMouse.height=changedWindowSize.y;
    
    // update gameCam
    gameCam.appViewport = ofRectangle(210,10,changedWindowSize.x,changedWindowSize.y);
    
    setupFBOs(changedWindowSize.x,changedWindowSize.y);

    setupCameras(false);
	setupViewports();
	//camEasyCam.setDistance(cameraDistance);
	//camEasyCam.setFov(cameraFov);
	
	for(int i =0;i<lasers.size();i++)
	{
		lasers[i]->resizeWindow();
	}
	for(int i =0;i<lasersM.size();i++)
	{
		lasersM[i]->resizeWindow();
	}

	deferred.resizeBuffersAndTextures();
}
//----------------------------------------------------------------------------------------------------------------------------
void testApp::sliderRotationZAll(float &f)
{
	for(int i=0;i<lasers.size();i++)
	{
        lasers[i]->p_rotationZ = -f;
	}
	for(int i =0;i<lasersM.size();i++)
	{
        lasersM[i]->p_rotationZ = -f;
	}

    
}
//----------------------------------------------------------------------------------------------------------------------------
void testApp::sliderDimmerAll(int &f)
{
	for(int i=0;i<lasers.size();i++)
	{
		lasers[i]->setDimmer(f);
	}
    for(int i =0;i<lasersM.size();i++)
	{
		lasersM[i]->setDimmer(f);
	}

}
//----------------------------------------------------------------------------------------------------------------------------
void testApp::sliderViewMode(int &f)
{
    windowResized(ofGetWidth(), ofGetHeight());
}
//----------------------------------------------------------------------------------------------------------------------------
void testApp::sliderFov(float &f)
{
    gameCam.setFov(f);
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::toggleDrawAxis(bool &b)
{
	for(int i=0;i<lasers.size();i++)
	{
		lasers[i]->drawAxis=b;
	}
    for(int i =0;i<lasersM.size();i++)
	{
		lasersM[i]->drawAxis=b;
	}

    
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::toggleResetCam(bool &f)
{
	//camEasyCam.setOrientation(ofQuaternion(camOrientX,camOrientY,camOrientZ,camOrientW));
    //camEasyCam.setPosition(camPosX, camPosY, camPosZ);
    //setupCameras(false);
    gameCam.loadCameraPosition();
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::toggleShowGui(bool &f)
{
    windowResized(ofGetWidth(), ofGetHeight());
}
//----------------------------------------------------------------------------------------------------------------------------
void testApp::changedCamSpeed(float &f)
{
    gameCam.speed = p_speedCam;
}
//----------------------------------------------------------------------------------------------------------------------------
void testApp::receiveOscMessages()
{
	ofxOscMessage m;
	int ii,rr,value;
	
	while(oscRec.hasWaitingMessages())
	{
		oscRec.getNextMessage(&m); 
		if (p_printOscOnConsole && (m.getNumArgs()==1)) printf("-> osc in :: %s value : %f\n",m.getAddress().c_str(),m.getArgAsFloat(0));
        else if(p_printOscOnConsole && (m.getNumArgs()==2)) printf("-> osc in :: %s value : %f , %f \n",m.getAddress().c_str(),m.getArgAsFloat(0),m.getArgAsFloat(1));
		string address = m.getAddress();
		
        // find first /
        int whichLaser=0;
        int firstSlash=address.find("/");
        int secondSlash=address.find("/",firstSlash+1);
        int thirdSlash=address.find("/",secondSlash+1);
        int firstR=address.find("r",0);
        
        string laserSufix = address.substr(firstR+1,secondSlash-firstR-1);
        string whichL = address.substr(secondSlash+1);
        whichLaser=ofToInt(whichL)-1;

        //        cout << "laser sufix " << laserSufix <<endl;
        
        if(laserSufix=="M")
        {
            //cout << "which laser M : " << whichLaser <<endl;
            
            lasersM[whichLaser]->setDimmer(m.getArgAsFloat(1)*255.0);
            lasersM[whichLaser]->setRotationZ((m.getArgAsFloat(0)-0.5f)*180.0f);

        }
        else if((laserSufix=="B")||((laserSufix=="R")))
        {
            lasers[whichLaser]->setDimmer(m.getArgAsFloat(0)*255.0);
            
        }
        
        if(secondSlash==2)
        {
            whichLaser=ofToInt(address.substr(1,1));
        }
        else if(secondSlash==3)
        {
            whichLaser=ofToInt(address.substr(1,2));
        }
        
        int firstWash = address.find("wash");
        int whichWash = ofToInt(address.substr(firstWash+5,1));

        int firstWdim = address.find("wdim");
        int whichWdim = ofToInt(address.substr(firstWdim+5,1));

        
        int whichPar = ofToInt(address.substr(secondSlash+1,thirdSlash-secondSlash-1));
        //cout << "par " <<whichPar  <<firstSlash <<secondSlash <<thirdSlash <<thirdSlash-secondSlash -1<<endl;
        
        //printf(" / %d / %d whichLaser %d\n",firstSlash,secondSlash,whichLaser);

//        if(address.find("follow")!=-1)
//        {
//            p_point.set(ofVec3f(ofMap(m.getArgAsFloat(0),0,1,-lasersX/2,+lasersX/2),
//                                ofMap(m.getArgAsFloat(1),0,1,0,1000),
//                                ofMap(m.getArgAsFloat(2),0,1,-lasersZ/2,lasersZ/2)));
//        }
        if(address.find("tilt")!=-1)
        {
            lasers[whichLaser-1]->setRotationZ(ofMap(m.getArgAsFloat(0),0.0,1.0,-125.0,125.0));
        }
        else if(address.find("pan")!=-1)
        {
            lasers[whichLaser-1]->setRotationY(ofMap(m.getArgAsFloat(0),0.0,1.0,-270.0,270.0));
        }
        else if((address.find("dim")!=-1) && (address.find("wdim")==-1))
        {
            lasers[whichLaser-1]->setDimmer(ofMap(m.getArgAsFloat(0),0.0,1.0,0.0,255.0));
        }
        else if(address.find("color")!=-1)
        {
            lasers[whichLaser-1]->setColor(colorTable[m.getArgAsInt32(0)]);
        }
        
	}
    
}
//----------------------------------------------------------------------------------------------------------------------------
void testApp::enableFog(float near, float far) {
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    GLfloat fogColor[4]= {0, 0, 0, 1};
    glFogfv(GL_FOG_COLOR, fogColor);
    glHint(GL_FOG_HINT, GL_FASTEST);
    glFogf(GL_FOG_START, near);
    glFogf(GL_FOG_END, far);
}

//----------------------------------------------------------------------------------------------------------------------------
void testApp::disableFog() {
    glDisable(GL_FOG);
}



