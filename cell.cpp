#include "cell.h"

#include <iostream>
#include <stdio.h>

using namespace std;

cell::cell(char InState, int InBit, float pL, float pR){
	if (InBit == 1 || InBit == 0){
		bit = InBit;
		state = InState;
		pLeft = pL;
		pRright = pR;
	}
	else
	{
		cout << "attempt set bit = " << InBit << endl;
		exit(1);
	}

}
void cell::printInfo() const{
	cout << state << bit << " " << pLeft << " " << pRright << endl;
}
float cell::getPLeft() const{
	return pLeft;
}
float cell::getPRight() const{
	return pRright;
}
int cell::getBit() const{
	return bit;
}
char cell::getState() const{
	return state;
}
