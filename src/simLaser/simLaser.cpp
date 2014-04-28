#include "simLaser.h"

int bufferSpeedSize=60;


//------------------------------------------------------------------
simLaser::simLaser()
{
	name = "Lx";
	baseHeight=0;
	baseWidth=30;
    heightOffset=70;
    maxTiltSpeed = 250.0 / 1.3;
    lastTiltValue=0.0;
    lastTiltTime=ofGetElapsedTimeMillis();
    tiltSpeedLimit=false;
    bufferSpeedLim=new float[bufferSpeedSize];
    bufferIt=0;
    color = ofColor(0,0,255);
    aPointInLaser = ofPoint(0.,0.,0.);
    numLasers = 0;
    
	drawAxis = false;
	orientationX = 0.0;
	orientationY = 0.0;
	orientationZ = 0.0;
    
    
	font.loadFont("./FreePixel.ttf", 18, true, true, false, 1.0, 180.0);
	
}

//------------------------------------------------------------------
void simLaser::init(string _name,int _number,float _orientationX,float _orientationY, float _orientationZ)
{
	number = _number;
	name = _name;
    orientationX = _orientationX;
	orientationY = _orientationY;
	orientationZ = _orientationZ;
    
    // GUI
    ////////
    
	gui.setup(name); // most of the time you don't need a name
    
    gui.add(p_rotationZ.set("Z tilt",0,-125,125));
    gui.add(p_rotationY.set("Y pan",0,-270,270));
	gui.add(p_dimmer.set("dimmer",255,0,255));
    
    resizeWindow();
  	
}



//------------------------------------------------------------------
void simLaser::customDraw()
{
	// We run the update ourselves manually. ofNode does
	//  not do this for us.
	update();
    
    //	// Turn on wireframe mode
    //	glPolygonMode(GL_FRONT, GL_LINE);
    //	glPolygonMode(GL_BACK, GL_LINE);
	
    
    //    // lines
    //    float floorW = 1800.0;
    //    float floorH = 4400.0;
    //
    //	ofSetLineWidth(4.0);
    //	ofSetColor(255,128,0);
    //    ofLine(0,0,0,-floorW/2.0,0,0);
    
    
    
	ofPushMatrix();
    {
        
        glTranslatef(this->getPosition().x,this->getPosition().y,this->getPosition().z);
        
        ofQuaternion q;
        //q.set();
        
        setOrientation(ofVec3f(orientationX,orientationY,orientationZ));
        
        //        // CASI
        //        glRotatef(orientationY, 0.0,1.0,0.0);
        //        glRotatef(orientationX, 1.0,0.0,0.0);
        //        glRotatef(orientationZ, 0.0,0.0,1.0);
        // CASI
        glRotatef(orientationY, 0.0,1.0,0.0);
        glRotatef(orientationZ, 0.0,0.0,1.0);
        glRotatef(orientationX, 1.0,0.0,0.0);
        
        
        //glTranslatef(-this->getPosition().x,-this->getPosition().y,-this->getPosition().z);
        
        
        // calculate a point in Laser
        float dist = 8000;
        aPointInLaser = ofPoint(dist*sin(ofDegToRad(p_rotationZ))*sin(ofDegToRad(90.0+p_rotationY)) ,
                                baseHeight +  dist*cos(ofDegToRad(p_rotationZ)),
                                dist*cos(ofDegToRad(90.0+p_rotationY))*sin(ofDegToRad(p_rotationZ) ));
        
        //printf("calculating aPointInLaser dist * %f * %f = %f\n",cos(ofDegToRad(90.0+p_rotationY)),sin(ofDegToRad(p_rotationZ)),dist*cos(ofDegToRad(90.0+p_rotationY))*sin(ofDegToRad(p_rotationZ) ));
        // draw reference lines for aPointInLaser
        /*
         ofPushStyle;
         ofPushMatrix();
         {
         ofFill();
         ofSetColor(255,0,0);
         ofLine(0.,0.+baseHeight,0.,aPointInLaser.x,aPointInLaser.y,aPointInLaser.z);
         ofTranslate(aPointInLaser);
         ofBox(0.,0.,0.,10);
         }
         ofPopMatrix();
         ofPopStyle();
         */
        
        
        // mark initial point
        if(tiltSpeedLimit)
        {
            ofPushStyle();
            {
                glPointSize(20.0);
                ofSetColor(255,0,0);
                glBegin(GL_POINTS);
                glVertex3f(0.,0.,0.);
                glEnd();
                ofPopStyle();
            }
        }
        
        
        // draw local Axis
        ofSetLineWidth(1.0);
        if(drawAxis) ofDrawAxis(100.0f);
        
        
        // BASE BOX
        ofPushStyle();
        {
            ofSetColor(60,60,60);
            //ofTranslate(0.,baseHeight/2.0,0.);
            //ofScale(baseWidth/10.0,baseHeight/10.0,baseWidth/10.0);
            //ofBox(0.,0.,0.,10.);
            ofSetLineWidth(5.0);
            ofLine(0.0 , 0.0 , 0.0 ,0.0 ,  baseHeight ,0.0);
        }
        ofPopStyle();
        
        ofPushStyle();
        ofPushMatrix();
        {
            ///////////////////////////
            // ALL THIS PART ROTATES !!
            
            ofTranslate(0.0,baseHeight,0.0);
            //ofRotate(-((float(p_rotationZ)/1023.0)*300.0)+150.0, 0.0, 0.0, -1.0);
            ofRotate(float(p_rotationY), 0.0, 1.0, 0.0);
            ofRotate(float(p_rotationZ), 0.0, 0.0, -1.0);
            
            // LASER HEAD BOX
            ofNoFill();
            ofSetColor(128,128,128);
            //ofScale(baseWidth/10.0,baseHeight/10.0,baseWidth/10.0);
            ofDrawBox(0.,0.,0.,2.);
            // LASER BEAM
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofPushStyle();
            ofPushMatrix();
            {
                // LASER
                
                ofSetColor(color.r*(float(p_dimmer.get())/255.0),color.g*(float(p_dimmer.get())/255.0),color.b*(float(p_dimmer.get())/255.0),(float(p_dimmer.get())/1.0));
                //ofSetColor(255,255,0,p_dimmer.get());
                ofSetLineWidth(4.0);
                glBegin(GL_LINES);
                glVertex3f(0.0,0.0,0.0);
                glVertex3f(0.0,40000.0,0.0);
                glEnd();
                
                
                /// SHARPY CONE
                /*
                 //ofSetColor(color.r*(float(p_dimmer.get())/255.0),color.g*(float(p_dimmer.get())/255.0),color.b*(float(p_dimmer.get())/255.0),255);
                 float reducedAlphaDimmer = ofMap(p_dimmer,0.0,255.0,0.0,200.0);
                 ofSetColor(color.r,color.g,color.b,reducedAlphaDimmer);
                 ofTranslate(0,1500/2,0);
                 ofConePrimitive(25, 1500.0, 24, 1).draw();
                 */
            }
            ofPopMatrix();
            ofPopStyle();
            ofDisableBlendMode();
            
            //
            ///////////////////////////
            
        }
        ofPopMatrix();
        
        
        // draw label
        ofPushMatrix();
        
        {
            
            // Render label
            /*
            ofSetColor(255, 255, 255);
            ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
            ofDrawBitmapString("LASER",this->getPosition());
            */
            
            ofPushMatrix();
            
            {
                /// BILLBOARDING
                float modelview[16];
                int i,j;
                
                // save the current modelview matrix
                glPushMatrix();
                
                // get the current modelview matrix
                glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
                
                // undo all rotations
                // beware all scaling is lost as well
                for( i=0; i<3; i++ )
                    for( j=0; j<3; j++ ) {
                        if ( i==j )
                            modelview[i*4+j] = 1.0;
                        else
                            modelview[i*4+j] = 0.0;
                    }
                
                // set the modelview with no rotations
                glLoadMatrixf(modelview);
                ofSetColor(255);
                ofPushStyle();
                ofEnableAlphaBlending();
                //ofDisableDepthTest();
                font.drawString(this->name, 0, 0);
                //ofEnableDepthTest();
                ofPopStyle();
                
                
                
                glPopMatrix();
            }
            
            ofPopMatrix();
            
        }
        ofPopMatrix();
	}
	ofPopMatrix();
    
    
}

//------------------------------------------------------------------
void simLaser::update()
{
    
}

////------------------------------------------------------------------
//void simLaser::setPosition(float x, float y, float z)
//{
//    this->setPosition(
//}
////------------------------------------------------------------------
//ofPoint simLaser::getPosition()
//{
//    return this->getPosition();
//}

/// X
//------------------------------------------------------------------
void simLaser::setRotationX(float f)
{
	p_rotationX.set(f);
}
//------------------------------------------------------------------
int simLaser::getRotationX()
{
	return p_rotationX.get();
}
//------------------------------------------------------------------
void simLaser::setorientationX(float f)
{
	orientationX = f;
}
//------------------------------------------------------------------
float simLaser::getorientationX()
{
	return orientationX;
}
/// Y
//------------------------------------------------------------------
void simLaser::setRotationY(float f)
{
	p_rotationY.set(f);
}
//------------------------------------------------------------------
int simLaser::getRotationY()
{
	return p_rotationY.get();
}
//------------------------------------------------------------------
void simLaser::setorientationY(float f)
{
	orientationY = f;
}
//------------------------------------------------------------------
float simLaser::getorientationY()
{
	return orientationY;
}

/// Z
//------------------------------------------------------------------
void simLaser::setRotationZ(float f)
{
	p_rotationZ.set(f);
}
//------------------------------------------------------------------
int simLaser::getRotationZ()
{
	return p_rotationZ.get();
}
//------------------------------------------------------------------
void simLaser::setorientationZ(float f)
{
	orientationZ = f;
}
//------------------------------------------------------------------
float simLaser::getorientationZ()
{
	return orientationZ;
}



////------------------------------------------------------------------
//void simLaser::setRotationZ(float f)
//{
//    unsigned long long now = ofGetElapsedTimeMillis();
//
//    p_rotationZ.set(f);
//
//
//    float actualTiltSpeed = fabs(lastTiltValue - p_rotationZ) / (float((now-lastTiltTime)/1000.0));
///*
//    if(number==0)
//    {
//        printf("____\n");
//        printf("NOW is %lld lastTime %lld ...... %ld\n",now,lastTiltTime,now-lastTiltTime);
//        printf("TILT %f lastTILT %f ..... \n",f,lastTiltValue,fabs(f-lastTiltValue));
//        if(actualTiltSpeed>192.0) printf("!!!!!!!!!!!!!!!!!");
//        printf("......................... %f deg/cycle\n",actualTiltSpeed);
//    }
// */
//    if(actualTiltSpeed!=INFINITY) bufferSpeedLim[bufferIt] = actualTiltSpeed;
//    else bufferSpeedLim[bufferIt]=0;
//
//    bufferIt=((bufferIt+1)%bufferSpeedSize);
////    if(number==0)
////    {
//
//    float total=0.0;
//    for(int i=0;i<bufferSpeedSize;i++)
//    {
//        total=total+bufferSpeedLim[i];
//    }
//
//    float medianSpeed = total / float(bufferSpeedSize);
//
//    if((medianSpeed>192.0) && (true))
//    {
//     //printf("median speed of last 30 osc receptions is : %f total %f\n",medianSpeed, total);
//        //tiltSpeedLimit=true;
//    }
//    else
//    {
//        //tiltSpeedLimit=false;
//    }
//
//    //}
//
//
//    //interOscTime = int(now-lastTiltTime);
//
//    //printf("%d\n",int(now-lastTiltTime));
//    /*
//    if(number==0)
//    {
//        printf("NOW is %lld lastTime %lld ...... dif %lld\n",now,lastTiltTime,now-lastTiltTime);
//
//        if(false)
//        //if(actualTiltSpeed>192.0)
//        {
//
//            if(actualTiltSpeed!=INFINITY)
//            {
//                tiltSpeedLimit=true;
//                printf("____\n");
//                printf("NOW is %lld lastTime %lld ...... dif %lld\n",now,lastTiltTime,now-lastTiltTime);
//                printf("TILT %f lastTILT %f ..... dif %f\n",f,lastTiltValue,fabs(f-lastTiltValue));
//                printf("!!!!!!!!!!!!!!!!! %f",actualTiltSpeed);
//            }
//        }
//        else tiltSpeedLimit=false;
//    }
//        */
//    lastTiltValue=p_rotationZ;
//    lastTiltTime=now;
//
//
//
//}
//

//------------------------------------------------------------------
void simLaser::setDimmer(float f)
{
	p_dimmer.set(f);
}
//------------------------------------------------------------------
int simLaser::getDimmer()
{
	return p_dimmer.get();
}
//------------------------------------------------------------------
void simLaser::setColor(ofColor c)
{
    color = c;
}
//------------------------------------------------------------------
void simLaser::resizeWindow()
{
    int xSpan = int(float(ofGetWidth())-(10+10+7*5))/(float(numLasers)/2.0);
    int ySpan = int(float(numLasers)/2.0) *15;
    if(number<int(float(numLasers)/2.0))
    {
        gui.setPosition(10+(number*(xSpan+5)),ofGetHeight()-ySpan*2);
    }
    else
    {
        gui.setPosition(10+((number-int(float(numLasers)/2.0))*(xSpan+5)),ofGetHeight()-ySpan*1);
    }
    
    gui.setSize(xSpan,ySpan);
    gui.setDefaultWidth(xSpan);
    gui.setWidthElements(xSpan);
    
    
    //gui.setPosition(100,600);
}


