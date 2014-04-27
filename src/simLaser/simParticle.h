#pragma once
#include "ofMain.h"
#include "ofxGui.h"


class simParticle : public ofNode {

	public:
	
	
	simParticle();
	void		init(float orientation);
	void		customDraw();
	
	ofxPanel gui;
	
	// we call this update function ourselves
	//  at the beginning of customDraw
	void		update();
	
	float		rotateAngle1;
	float		rotateAngle2;
	float		rotateAngle3;
	float		rotateAngle4;
	
	bool		active;
	float		orientation;
	bool		drawAxis;
	int			rotateAngle;
	
	// GUI

	ofFloatColor	particleColor1;
	ofFloatColor	particleColor2;
	ofFloatColor	particleColor3;
	ofFloatColor	particleColor4;

	ofParameter<ofFloatColor> p_color1;
	ofParameter<ofFloatColor> p_color2;
	ofParameter<ofFloatColor> p_color3;
	ofParameter<ofFloatColor> p_color4;
	
	ofxFloatSlider gui_red1;
	ofxFloatSlider gui_green1;
	ofxFloatSlider gui_blue1;

	ofxFloatSlider gui_red2;
	ofxFloatSlider gui_green2;
	ofxFloatSlider gui_blue2;

	ofxFloatSlider gui_red3;
	ofxFloatSlider gui_green3;
	ofxFloatSlider gui_blue3;

	ofxFloatSlider gui_red4;
	ofxFloatSlider gui_green4;
	ofxFloatSlider gui_blue4;

	ofParameter<bool> p_isActive;
	ofxToggle	   gui_isActive;
	
	
	void			changeColorRed1(float &f);
	void			changeColorGreen1(float &f);
	void			changeColorBlue1(float &f);
	void			changeColorRed2(float &f);
	void			changeColorGreen2(float &f);
	void			changeColorBlue2(float &f);
	void			changeColorRed3(float &f);
	void			changeColorGreen3(float &f);
	void			changeColorBlue3(float &f);
	void			changeColorRed4(float &f);
	void			changeColorGreen4(float &f);
	void			changeColorBlue4(float &f);
	
	
protected:

};