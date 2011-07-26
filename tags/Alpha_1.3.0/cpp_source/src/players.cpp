#include "players.h"
#include <iostream>
using namespace std;

players::players()
{
	this->size = 4 + 12 * 1;
	this->data = new unsigned char[this->size];
	this->player_id = this->data;
	this->player_data = this->data+4+12*0;
}

players::~players()
{
	delete [] data;
}