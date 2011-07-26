#include "skeletons.h"
#include <iostream>
using namespace std;

skeletons::skeletons()
{
	this->size = 4 + 12 * 15;
	this->data = new unsigned char[this->size];
	this->player_id = this->data;
	this->head = this->data+4+12*0;
	this->neck = this->data+4+12*1;
	this->torso = this->data+4+12*2;
	this->lshoulder = this->data+4+12*3;
	this->lelbow = this->data+4+12*4;
	this->lhand = this->data+4+12*5;
	this->rshoulder = this->data+4+12*6;
	this->relbow = this->data+4+12*7;
	this->rhand = this->data+4+12*8;
	this->lhip = this->data+4+12*9;
	this->lknee = this->data+4+12*10;
	this->lfoot = this->data+4+12*11;
	this->rhip = this->data+4+12*12;
	this->rknee = this->data+4+12*13;
	this->rfoot = this->data+4+12*14;
}

skeletons::~skeletons()
{
	delete [] data;
}