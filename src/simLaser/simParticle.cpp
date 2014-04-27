#include "simParticle.h"



//------------------------------------------------------------------
simParticle::simParticle()
{
	drawAxis = false;
	active = true;
	orientation = 0.0;
	rotateAngle = 0.0;

	p_color1.set(ofFloatColor(1.0,0.0,0.0,0.2));
	p_color2.set(ofFloatColor(1.0,0.0,0.0,0.2));
	p_color3.set(ofFloatColor(1.0,0.0,0.0,0.2));
	p_color4.set(ofFloatColor(1.0,0.0,0.0,0.2));
	p_isActive.set(active);
}

//------------------------------------------------------------------
void simParticle::init(float _orientation)
{
	orientation = _orientation;
	this->setPosition(ofVec3f(0.0,0.0,0.0));

	// gui
	gui.setup("particle"); 
	// most of the time you don't need a name
	gui_red1.setup("1_red", p_color1.get().r, 0, 1.0, 200,10);
	gui_green1.setup("1_green", p_color1.get().g, 0, 1.0, 200,10);
	gui_blue1.setup("1_blue", p_color1.get().b, 0, 1.0, 200,10);

	gui_red2.setup("2_red", p_color2.get().r, 0, 1.0, 200,10);
	gui_green2.setup("2_green", p_color2.get().g, 0, 1.0, 200,10);
	gui_blue2.setup("2_blue", p_color2.get().b, 0, 1.0, 200,10);

	gui_red3.setup("3_red", p_color3.get().r, 0, 1.0, 200,10);
	gui_green3.setup("3_green", p_color3.get().g, 0, 1.0, 200,10);
	gui_blue3.setup("3_blue", p_color3.get().b, 0, 1.0, 200,10);

	gui_red4.setup("4_red", p_color4.get().r, 0, 1.0, 200,10);
	gui_green4.setup("4_green", p_color4.get().g, 0, 1.0, 200,10);
	gui_blue4.setup("4_blue", p_color4.get().b, 0, 1.0, 200,10);
	
	gui_isActive.setup("on/off", p_isActive, 200, 10);
	
	
	gui.add(&gui_isActive);
	gui.add(&gui_red1);
	gui.add(&gui_green1);
	gui.add(&gui_blue1);
	gui.add(&gui_red2);
	gui.add(&gui_green2);
	gui.add(&gui_blue2);
	gui.add(&gui_red3);
	gui.add(&gui_green3);
	gui.add(&gui_blue3);
	gui.add(&gui_red4);
	gui.add(&gui_green4);
	gui.add(&gui_blue4);
	
	gui_red1.addListener(this,&simParticle::changeColorRed1);
	gui_green1.addListener(this,&simParticle::changeColorGreen1);
	gui_blue1.addListener(this,&simParticle::changeColorBlue1);
	gui_red2.addListener(this,&simParticle::changeColorRed2);
	gui_green2.addListener(this,&simParticle::changeColorGreen2);
	gui_blue2.addListener(this,&simParticle::changeColorBlue2);
	gui_red3.addListener(this,&simParticle::changeColorRed3);
	gui_green3.addListener(this,&simParticle::changeColorGreen3);
	gui_blue3.addListener(this,&simParticle::changeColorBlue3);
	gui_red4.addListener(this,&simParticle::changeColorRed4);
	gui_green4.addListener(this,&simParticle::changeColorGreen4);
	gui_blue4.addListener(this,&simParticle::changeColorBlue4);
	
	gui.setPosition(0,150);
	
}



//------------------------------------------------------------------
void simParticle::customDraw()
{
	// We run the update ourselves manually. ofNode does
	//  not do this for us.
	if(p_isActive)
	{
		update();
		ofPushMatrix();
		glRotatef(orientation, 0.0,1.0,0.0);

		
		
	//	// CENTRAL POINT
	//	ofPushStyle();
	//	ofSetColor(255,255,0);	
	//	ofSetLineWidth(30.0);
	//	glPointSize(5.0);	
	//	glBegin(GL_POINTS);
	//	glVertex3f(this->getPosition().x,this->getPosition().y,this->getPosition().z);
	//	glEnd();
	//	ofPopStyle();	

		// BASE BOX
		ofPushStyle();
		ofPushMatrix();
		ofSetColor(40,40,40);
		//ofTranslate(0.,baseHeight/2.0,0.);
		//ofScale(baseWidth/10.0,baseHeight/10.0,baseWidth/10.0);
		//ofBox(0.,0.,0.,10.);
		ofSetLineWidth(8.0);
		ofLine(0.0,0.0,0.0,120.0f);
		ofPopMatrix();
		ofPopStyle();
		
		
		ofEnableAlphaBlending();
		
		// sphere 1
	//	ofPushMatrix();
	//	ofMaterial mat1;
	//	mat1.setColors(ofFloatColor(0.8,0.8,0.8,0.5), ofFloatColor(0.0,0.,0.,1.), ofFloatColor(0.0,0.0,0.0,1.0),p_color1.get());
	//	mat1.begin();

		ofSetColor(p_color1.get());
		if(drawAxis) ofDrawAxis(100.0f);

		glRotatef(rotateAngle, 0.0,1.0,0.0);
		ofTranslate(0.0,150.0,0.0);	
		ofSphere(30.0);
	//	mat1.end();
		ofPopMatrix();

		// sphere 2
		ofPushMatrix();
	//	ofMaterial mat2;
	//	mat2.setColors(ofFloatColor(0.8,0.8,0.8,0.5), ofFloatColor(0.0,0.,0.,1.), ofFloatColor(0.0,0.0,0.0,1.0),p_color2.get());
	//	mat2.begin();
		glRotatef(rotateAngle+60.0, 0.0,1.0,0.0);
		ofTranslate(0.0,150.0,0.0);	
		ofSetColor(p_color2.get());
		ofSphere(60.0);
	//	mat2.end();
		ofPopMatrix();

		// sphere 3
		ofPushMatrix();
	//	ofMaterial mat3;
	//	mat3.setColors(ofFloatColor(0.8,0.8,0.8,0.5), ofFloatColor(0.0,0.,0.,1.), ofFloatColor(0.0,0.0,0.0,1.0),p_color3.get());
	//	mat3.begin();
		glRotatef(rotateAngle+120.0, 0.0,1.0,0.0);
		ofSetColor(p_color3.get());
		ofTranslate(0.0,150.0,0.0);	
		ofSphere(90.0);
	//	mat3.end();
		ofPopMatrix();
		
		// sphere 4
		ofPushMatrix();
	//	ofMaterial mat4;
	//	mat4.setColors(ofFloatColor(0.8,0.8,0.8,0.5), ofFloatColor(0.0,0.,0.,1.), ofFloatColor(0.0,0.0,0.0,1.0),p_color4.get());
	//	mat4.begin();
		glRotatef(rotateAngle+180.0, 0.0,1.0,0.0);
		ofTranslate(0.0,150.0,0.0);	
		ofSetColor(p_color4.get());
		ofSphere(120.0);
	//	mat4.end();
		ofPopMatrix();
			
		ofDisableAlphaBlending();
		
		ofPopMatrix();
		/*
		ofMaterial matOld;
		matOld.setColors(ofColor(0.8,0.8,0.8,1.0), ofColor(0.,0.,0.,1.), ofColor(0.2,0.2,0.2,1.0), ofColor(0.0,0.0,0.0,1.0));
		matOld.begin();
	//	diffuse.set(0.8f, 0.8f, 0.8f, 1.0f);
	//    specular.set(0.0f, 0.0f, 0.0f, 1.0f);
	//    ambient.set(0.2f, 0.2f, 0.2f, 1.0f);
	//    emissive.set(0.0f, 0.0f, 0.0f, 1.0f);
	//    shininess = 0.2;
		
		
	//	// Turn on wireframe mode
	//	glPolygonMode(GL_FRONT, GL_LINE);
	//	glPolygonMode(GL_BACK, GL_LINE);
		
		ofPushMatrix();
		glRotatef(orientation, 0.0,1.0,0.0);
		glTranslatef(float(radialPosition),0.,0.);
		glRotatef(180.0,0.0,1.0,0.0);

		
		
		
		// draw local Axis
		ofSetLineWidth(1.0);
		
		// BASE BOX
		ofPushStyle();
		ofPushMatrix();
		ofSetColor(0,128,128);
		//ofTranslate(0.,baseHeight/2.0,0.);
		//ofScale(baseWidth/10.0,baseHeight/10.0,baseWidth/10.0);
		//ofBox(0.,0.,0.,10.);
		ofSetLineWidth(15.0);
		ofLine(0.0 , 0.0 , 0.0 ,0.0 ,  baseHeight ,0.0);
		ofPopMatrix();
		ofPopStyle();

		
		///////////////////////////
		// ALL THIS PART ROTATES !! 
		//
		ofPushMatrix();
		ofTranslate(0.0,baseHeight,0.0);
		ofRotate(-((float(p_rotation)/1023.0)*300.0)+150.0, 0.0, 0.0, -1.0);
		
		// LASER HEAD BOX
		ofPushStyle();
		ofNoFill();
		ofPushMatrix();
		ofSetColor(128,64,0);
		//ofScale(baseWidth/10.0,baseHeight/10.0,baseWidth/10.0);
		ofBox(0.,0.,0.,20.);
		ofPopMatrix();
		ofPopStyle();
		matOld.end();
		
		
		// LASER BEAM
		ofPushStyle();
		ofPushMatrix();
		ofMaterial mat;
		mat.setColors(ofColor(0,0,p_intensity.get()), ofColor(0,0,0), ofColor(0,0,0), ofColor(0,0,p_intensity.get()));
		//ofSetColor(0,0,p_intensity.get());
		//ofScale(baseWidth/10.0,baseHeight/10.0,baseWidth/10.0);
		mat.begin();
		ofSetLineWidth(2.0);
		glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,1000.0,0.0);
		glEnd();
		ofPopMatrix();
		mat.end();
		ofPopStyle();

		ofPopMatrix();
		//
		///////////////////////////

		
		// draw label
		ofPushMatrix();
		ofTranslate(20.0,0.0,0.0);
		ofRotate(180.0, 1.0, 0.0,0.0);
		ofDrawBitmapString(name,this->getPosition().x,this->getPosition().y,this->getPosition().z);
		ofPopMatrix();
		
		ofPopMatrix();

		 */
	}
}

//------------------------------------------------------------------
void simParticle::update()
{
	rotateAngle = rotateAngle + 18.0;
}

// -- 1
//------------------------------------------------------------------
void simParticle::changeColorRed1(float &f)
{
	p_color1.set(ofFloatColor(f,p_color1.get().g,p_color1.get().b,0.5f));
}
//------------------------------------------------------------------
void simParticle::changeColorGreen1(float &f)
{
	p_color1.set(ofFloatColor(p_color1.get().r,f,p_color1.get().b,0.5f));
}
//------------------------------------------------------------------
void simParticle::changeColorBlue1(float &f)
{
	p_color1.set(ofFloatColor(p_color1.get().r,p_color1.get().g,f,0.5f));
}
// -- 2
//------------------------------------------------------------------
void simParticle::changeColorRed2(float &f)
{
	p_color2.set(ofFloatColor(f,p_color2.get().g,p_color2.get().b,0.5f));
}
//------------------------------------------------------------------
void simParticle::changeColorGreen2(float &f)
{
	p_color2.set(ofFloatColor(p_color2.get().r,f,p_color2.get().b,0.5f));
}
//------------------------------------------------------------------
void simParticle::changeColorBlue2(float &f)
{
	p_color2.set(ofFloatColor(p_color2.get().r,p_color2.get().g,f,0.5f));
}
// -- 3
//------------------------------------------------------------------
void simParticle::changeColorRed3(float &f)
{
	p_color3.set(ofFloatColor(f,p_color3.get().g,p_color3.get().b,0.5f));
}
//------------------------------------------------------------------
void simParticle::changeColorGreen3(float &f)
{
	p_color3.set(ofFloatColor(p_color3.get().r,f,p_color3.get().b,0.5f));
}
//------------------------------------------------------------------
void simParticle::changeColorBlue3(float &f)
{
	p_color3.set(ofFloatColor(p_color3.get().r,p_color3.get().g,f,0.5f));
}
// -- 4
//------------------------------------------------------------------
void simParticle::changeColorRed4(float &f)
{
	p_color4.set(ofFloatColor(f,p_color4.get().g,p_color4.get().b,0.5f));
}
//------------------------------------------------------------------
void simParticle::changeColorGreen4(float &f)
{
	p_color4.set(ofFloatColor(p_color4.get().r,f,p_color4.get().b,0.05f));

}
//------------------------------------------------------------------
void simParticle::changeColorBlue4(float &f)
{
	p_color4.set(ofFloatColor(p_color4.get().r,p_color4.get().g,f,0.05f));
}
