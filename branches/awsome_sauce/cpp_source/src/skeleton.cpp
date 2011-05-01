#include "skeleton.h"
#include <iostream>
using namespace std;

skeleton::skeleton()
{
	this->size = 4 + 12 * 2;
	this->data = new unsigned char[this->size];
	this->user_id = this->data;
	this->flag = this->data+4+12*0;
	this->cpoint = this->data+4+12*1;
}

skeleton::~skeleton()
{
	delete [] data;
}