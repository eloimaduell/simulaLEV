#include "simQuad.h"



//------------------------------------------------------------------
simQuad::simQuad()
{
	name = "Lx";
    
	drawAxis = false;
    
	font.loadFont("./FreePixel.ttf", 18, true, true, false, 1.0, 180.0);
	quadColor = ofColor(0,0,0,0);
}

//------------------------------------------------------------------
void simQuad::init(string _name,ofRectangle _rectangle, int _mode)
{
	name = _name;
    rectangle = _rectangle;
    mode = _mode;
    strobeFallOff = 5;
    p_dimmer = 1.0;
    
    image.loadImage("./app/arcCCCB.png");
    // GUI
    ////////
    
	gui.setup(name); // most of the time you don't need a name
    //gui.setWidthElements(float(ofGetWidth())-20.0)/8.0);
    //gui.add(p_rotationZ.set("Z tilt",0,-125,125));
    //gui.add(p_rotationY.set("Y pan",0,-270,270));
	//gui.add(p_dimmer.set("dimmer",255,0,255));
    resizeWindow();
	
}



//------------------------------------------------------------------
void simQuad::customDraw()
{
	
	ofPushMatrix();
    {
        
        //glTranslatef(this->getPosition().x,this->getPosition().y,this->getPosition().z);

//        // mark initial point
//            ofPushStyle();
//            {
//                glPointSize(20.0);
//                ofSetColor(255,0,0);
//                glBegin(GL_POINTS);
//                glVertex3f(0.,0.,0.);
//                glEnd();
//                ofPopStyle();	
//            }

        // draw local Axis
        ofSetLineWidth(1.0);
        if(drawAxis) ofDrawAxis(100.0f);

        // BASE BOX
        ofPushStyle();
        {

            
            switch(mode)
            {
                case 0:
                    //                    ofSetColor(quadColor*p_dimmer);
                    //                    //ofSetColor(255,0,0);
                    //                    //ofNoFill();
                    //                    ofSetLineWidth(2.0);
                    //                    ofRect(0,0,0,rectangle.width,rectangle.height);
                    
                    ofPushMatrix();
                {
                    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
                    //glEnable(GL_BLEND);
                    //glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
                    
                    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
                    ofTranslate(rectangle.width/2,rectangle.height/2,0);
                    ofRotate(-90,0,0,1);
                    ofTranslate(-rectangle.height/2,-rectangle.width/2,0);

//                    ofSetColor(0,0,0,255);
//                    
//                    ofPushMatrix();
//                        ofRect(0,0,0,rectangle.height,rectangle.width);
//                    ofPopMatrix();
                    
                    ofSetColor(quadColor*p_dimmer,254);
                    ofFill();
                    image.draw(0,0,rectangle.height,rectangle.width);
                    
                }
                    ofDisableBlendMode();
                    ofPopMatrix();
                    
                    
                    
                    break;
                case 1:
                    quadColor.a=255;
                    ofSetColor(quadColor);
                    ofFill();
                    ofSetLineWidth(2.0);
                    ofRect(0,0,0,rectangle.width,rectangle.height);
                    
                    //ofSetColor(255,0,0);
                    //ofBoxPrimitive(150,150,150).draw();
                    
                    break;
                    
            }
        }
        
        
        ofPopStyle();
        
        // draw label
        ofPushMatrix();
        {
            ofTranslate(20.0,0.0,0.0);
            ofRotate(180.0, 1.0, 0.0,0.0);
//            ofSetColor(255,255,255,255);
//            font.drawString(ofToString(interOscTime), this->getPosition().x, this->getPosition().y);

        }
        ofPopMatrix();
	}
	ofPopMatrix();
    
    // We run the update ourselves manually. ofNode does
	//  not do this for us.
	update();


}

//------------------------------------------------------------------
void simQuad::update()
{
    /*
    if((mode==1)&&(quadColor.r>0))
    {
        if(quadColor.r - strobeFallOff <0) quadColor.r = 0;
        else quadColor.r=quadColor.r-strobeFallOff;
    }
    if((mode==1)&&(quadColor.g>0))
    {
        if(quadColor.g - strobeFallOff <0) quadColor.g = 0;
        else quadColor.g=quadColor.g-strobeFallOff;
    }
    if((mode==1)&&(quadColor.b>0))
    {
        if(quadColor.b - strobeFallOff <0) quadColor.b = 0;
        else quadColor.b=quadColor.b-strobeFallOff;
    }
     */
    
//    if((mode==1)&&(quadColor.a>0))
//    {
//        if(quadColor.a - strobeFallOff <0) quadColor.a = 0;
//        else quadColor.a=quadColor.a-strobeFallOff;
//    }
    //printf("simQuad:: quadColor %d %d %d %d \n",quadColor.r,quadColor.g,quadColor.b,quadColor.a);
}


////------------------------------------------------------------------
//void simQuad::setPosition(float x, float y, float z)
//{
//    this->setPosition(
//}
////------------------------------------------------------------------
//ofPoint simQuad::getPosition()
//{
//    return this->getPosition();
//}

//------------------------------------------------------------------
void simQuad::setColor(ofColor _color)
{
    quadColor = _color;
}

//------------------------------------------------------------------
void simQuad::setDimmer(float f)
{
	p_dimmer.set(f);
}
//------------------------------------------------------------------
float simQuad::getDimmer()
{
	return p_dimmer.get();
}
//------------------------------------------------------------------
void simQuad::resizeWindow()
{
    
}

