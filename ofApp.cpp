#include "ofApp.h"
#include <thread>

#include <iostream>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <helper_cuda.h>

#include <chrono> 
using namespace std::chrono;

#include <fstream>
using namespace std;

ofImage     img;

extern "C" float* runTest();

class Axis {
public:
	float m_x11; float m_x12; float m_x21; float m_x22;
	float m_y11; float m_y12; float m_y21; float m_y22;

	void set(float x11, float x12, float x21, float x22,
		float y11, float y12, float y21, float y22) {
		m_x11 = x11; m_x12 = x12; m_x21 = x21; m_x22 = x22;
		m_y11 = y11; m_y12 = y12; m_y21 = y21; m_y22 = y22;
	}

	void show() {
		int n = 1;

		ofSetColor(10, 100);

		ofDrawLine(0, m_y11, 0, m_y12);
		ofDrawLine(m_x11, 0, m_x12, 0);

		ofDrawLine(m_x11, m_y12, m_x11, m_y11);
		ofDrawLine(m_x12, m_y12, m_x12, m_y11);

		ofDrawLine(m_x11, m_y11, m_x12, m_y11);
		ofDrawLine(m_x11, m_y12, m_x12, m_y12);

		float dx = (m_x12 / m_x22) / n;
		int n1 = floor(m_x12 / dx);

		float dy = (m_y12 / m_y22) / n;
		int n2 = floor(m_y12 / dy);

		ofSetColor(10, 15);

		for (int i = 0; i < n1; i++) {
			float x1 = i * dx;
			float x2 = -(i + 1)*dx;
			ofDrawLine(x1, m_y11, x1, m_y12);
			ofDrawLine(x2, m_y11, x2, m_y12);
		}

		for (int i = 0; i < n2; i++) {
			float y1 = i * dx;
			float y2 = -(i + 1)*dx;
			ofDrawLine(m_x11, y1, m_x12, y1);
			ofDrawLine(m_x11, y2, m_x12, y2);
		}
	}

	void e(float x11, float x21, float r11) {
		float x12 = ofMap(x11, m_x21, m_x22, m_x11, m_x12);
		float x22 = ofMap(x21, m_y21, m_y22, m_y11, m_y12);
		float r12 = ofMap(r11, 0, m_x22, 0, m_x12);
		ofDrawCircle(x12, x22, r11);
	}

	void r(float x11, float y11, float w1, float h1) {
		//ofSetRectMode(OF_RECTMODE_CENTER);
		float x12 = ofMap(x11, m_x21, m_x22, m_x11, m_x12);
		float y12 = ofMap(y11, m_y21, m_y22, m_y11, m_y12);
		float w2 = ofMap(w1, 0, m_x22, 0, m_x12);
		float h2 = ofMap(h1, 0, m_y22, 0, m_y12);
		ofDrawRectangle(x12, y12, w2, w2);
	}

	void l(float x11, float y11, float x21, float y21) {
		float x12 = ofMap(x11, m_x21, m_x22, m_x11, m_x12);
		float y12 = ofMap(y11, m_y21, m_y22, m_y11, m_y12);
		float x22 = ofMap(x21, m_x21, m_x22, m_x11, m_x12);
		float y22 = ofMap(y21, m_y21, m_y22, m_y11, m_y12);
		ofDrawLine(x12, y12, x22, y22);
	}

	ofVec2f mouse() {
		ofVec2f p;
		float x1 = ofGetMouseX(); float y1 = ofGetMouseY();
		float x2; float y2;
		float w = ofGetWidth() / 2; float h = ofGetHeight() / 2;
		x2 = ofMap(x1, w + m_x11, w + m_x12, m_x21, m_x22);
		y2 = ofMap(y1, h + m_y11, h + m_y12, -m_y21, -m_y22);
		p.set(x2, y2);
		return p;
	}
};

Axis a; float *A; int res = 1000; 

void resetSketch(float x11_i, float x12_i, float x21_i, float x22_i,
	float y11_i, float y12_i, float y21_i, float y22_i) {
	float x11 = x11_i;
	float x12 = x12_i;
	float x21 = x21_i;
	float x22 = x22_i;
	float y11 = y11_i;
	float y12 = y12_i;
	float y21 = y21_i;
	float y22 = y22_i;
	a.set(x11, x12, x21, x22, y11, y12, y21, y22);

}
//--------------------------------------------------------------

void second()
{
	A = runTest();
	/*ofstream myFile;
	myFile.open("test.csv");
	int dr = s / tr;
	for (int i = 0; i < tr; i++)
	{
		res += dr;
		auto start = high_resolution_clock::now();
		A = runTest();
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout << duration.count()*pow(10, -6) / 60.0 << "," << res << endl;
		myFile << duration.count()*pow(10, -6) / 60.0 << "," << res << endl;
	}*/
}


void ofApp::setup() {

	ofBackground(10);
	ofSetFrameRate(10);
	ofEnableAntiAliasing();
	ofEnableAlphaBlending();

	float x11 = -500.0;
	float x12 = 500.0;
	float x21 = -2.5;
	float x22 = 2.5;

	float y11 = -500.0;
	float y12 = 500.0;
	float y21 = -2.5;
	float y22 = 2.5;

	resetSketch(x11, x12, x21, x22, y11, y12, y21, y22);
	ofSetColor(30);

	float w = (a.m_x22 - a.m_x21) / res;
	std::cout << w << "\n";

	second();

}

//--------------------------------------------------------------
void ofApp::update() {

}
//--------------------------------------------------------------
void ofApp::draw() {
	ofSetLineWidth(2);
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2); ofScale(1, -1, 1);
	ofBackground(230);
	a.show();

	ofSetColor(20,200);
	float w = 5.0 / (res);
	
	for (int i = 0; i < pow(res, 2.0)*3; i += 3)
	{
		float d = A[i + 2];
		float d1, d2, d3;

		d1 = pow(1 - d, 2.0);
		d2 = pow(2 - d, 2.0);
		d3 = pow(0 - d, 2.0);

		if (d1 < 0.1)
		{
			ofSetColor(66, 153, 24, 230);
			//ofSetColor(255, 0, 0, 230);
			a.r(A[i], A[i + 1], w, w);
		}
		if (d2 < 0.1)
		{
			ofSetColor(40, 40, 40, 230);
			//ofSetColor(0, 255, 0, 230);
			a.r(A[i], A[i + 1], w, w);
		}
		if (d3 < 0.1)
		{
			ofSetColor(230, 230, 230, 230);
			//ofSetColor(0, 0, 255, 230);
			a.r(A[i], A[i + 1], w, w);
		}
	}

	ofSetColor(100,150,200);
	/*a.e(cos(ofDegToRad(60)), sin(ofDegToRad(60)), 10);
	a.e(cos(ofDegToRad(60 + 120)), sin(ofDegToRad(60 + 120)), 10);
	a.e(cos(ofDegToRad(60 + 120 + 120)), sin(ofDegToRad(60 + 120 + 120)), 10);*/
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	float x; float y;
	if (key == OF_KEY_UP) {
		x = a.m_x22 + a.m_x22 / 10; y = (x / a.m_x12)*a.m_y12;
		resetSketch(a.m_x11, a.m_x12, -x, x, a.m_y11, a.m_y12, -y, y);
	}
	else if (key == OF_KEY_DOWN) {
		x = a.m_x22 - a.m_x22 / 10; y = (x / a.m_x12)*a.m_y12;
		resetSketch(a.m_x11, a.m_x12, -x, x, a.m_y11, a.m_y12, -y, y);
	}

	if (key == 'x') {
		img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		img.save("pendulum.png");
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

