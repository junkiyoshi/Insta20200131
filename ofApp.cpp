#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-bold.ttf", 250, true, true, true);

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
	this->fbo.begin();
	ofClear(0);

	string word = "PCD";
	font.drawString(word, ofGetWidth() * 0.5 - font.stringWidth(word) * 0.5, ofGetHeight() * 0.5 - font.stringHeight(word) * 0.1);
	word = "TOKYO";
	font.drawString(word, ofGetWidth() * 0.5 - font.stringWidth(word) * 0.5, ofGetHeight() * 0.5 + font.stringHeight(word) * 1.1);

	this->fbo.end();
	this->fbo.readToPixels(this->pixels);

	this->number_of_agent = 120;
	this->len_of_tail = 100;
	while (this->log_list.size() < this->number_of_agent) {

		int x = ofRandom(ofGetWidth());
		int y = ofRandom(ofGetHeight());
		if (this->pixels.getColor(x, y) != ofColor(0, 0)) {

			deque<glm::vec2> log;
			log.push_front(glm::vec2(x, y));
			this->log_list.push_back(log);
			this->direction_list.push_back(glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1))));
		}
	}

	ofSetLineWidth(1.5);
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->number_of_agent; i++) {

		glm::vec2 future = this->log_list[i].front() + this->direction_list[i] * 5;
		int x = future.x;
		int y = future.y;

		while (this->pixels.getColor(x, y) == ofColor(0, 0)) {

			this->direction_list[i] = glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1)));

			future = this->log_list[i].front() + this->direction_list[i] * 10;
			x = future.x;
			y = future.y;
		}

		this->log_list[i].push_front(future);
		while (this->log_list[i].size() > this->len_of_tail) {

			this->log_list[i].pop_back();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->number_of_agent; i++) {

		ofSetColor(39);
		for (int l = 0; l < this->log_list[i].size() - 1; l++) {

			ofDrawLine(this->log_list[i][l], this->log_list[i][l + 1]);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}