#include "cell.h"

#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>

using namespace std;

void printCells(cell* myCell_ptr);

#define INBITITERATIONS 100
#define DELTA 0.5
#define TAUMAX 50
#define TAUINCREMENT 0.1
#define TAUMIN 0.1

//not a great function
void checkOFile(ofstream& outputFile){
	//Open and define filesteam at once
	if (outputFile.good()){
		cout << "outfile opened successfully" << endl;
		//outputFile << "writing to my new file!" << endl;
	}
	else
	{
		cerr << "outfile not opened successfully. Terminator." << endl;
	}
	cout << endl;
}

int genBit(){
	int bit;
	double p0 = 0.5*(1 + DELTA);
	double p1 = 0.5*(1 - DELTA);
	float randN = ((float)rand() / (float)(RAND_MAX));

	if (randN < p0){
		bit = 0;
	}
	else{
		bit = 1;
	}
	
	return bit;
}

void changeCell(int& bit, const cell (&myCells_ptr)[6], cell const *& currentCell_ptr){
	//cout << "changeCell" << endl;
	//cout << sizeof(myCells_ptr) << endl;
	for (int i = 0; i < sizeof(myCells_ptr) / sizeof(cell); i++){
		if ((myCells_ptr + i)->getState() == currentCell_ptr->getState() && (int)(myCells_ptr + i)->getBit() == bit){
			currentCell_ptr = myCells_ptr + i;
			break;
		}
	}
}

void demonInteract(cell const *& currentCell_ptr, const double & tau, const double & sojournTimeEdge){
	//cout << "in ";
	//currentCell_ptr->printInfo();

	//cout << "timesteps = " << tau*sojournTimeEdge << endl;
	for (double i = 0; i < tau*sojournTimeEdge; i++){
		//cout << "i = " << i << endl;

		//generate random number, tell thing to go left, right, stay.
		double num = ((double)rand() / (double)(RAND_MAX));
		//cout << num << endl;

		if (num <= 1 - (currentCell_ptr->getPLeft() + currentCell_ptr->getPRight())){
			//stay still
		}
		else if (num <= 1 - currentCell_ptr->getPRight()){
			//move left. pStill < num < pLeft + pStill
			currentCell_ptr --;
		}
		else{
			//move right. pLeft + PStill < num < 1
			currentCell_ptr ++;
		}
	
		//currentCell_ptr->printInfo();
		//cout << endl;

		if (currentCell_ptr->getBit() != 1 && currentCell_ptr->getBit() != 0){
			cout << "broken" << endl;
			currentCell_ptr->printInfo();
			exit(1);
		}
	}
	//cout << "out ";
	//currentCell_ptr->printInfo();
}

int main(){

	const cell a0('A', 0, 0, 0.1f);
	const cell b0('B', 0, 0.1f, 0.1f);
	const cell c0('C', 0, 0.1f, 0.1f);
	const cell a1('A', 1, 0.1f, 0.1f);
	const cell b1('B', 1, 0.1f, 0.1f);
	const cell c1('C', 1, 0.1f, 0);

	ofstream phiFile("phi.txt");
	checkOFile(phiFile);
	ofstream tauFile("tau.txt");
	checkOFile(tauFile);

	const cell myCells_ptr[6]{a0, b0, c0, a1, b1, c1};
	cell const * currentCell_ptr = myCells_ptr;

	float sojournTime[6];
	for (int i = 0; i < 6; i++){
		sojournTime[i] = 1 / (myCells_ptr[i].getPLeft() + myCells_ptr[i].getPRight()); //Is this formula correct?
	}

	srand((unsigned int)time(NULL)); //seed. Only needs to be seeded each time program runs. rand() produces a sequence of randome numbers

	//repeat with different TAU (plot the whole graph)
	for (double tau = TAUMIN; tau <= TAUMAX; tau += TAUINCREMENT){

		cout << "tau = " << tau << endl;
		tauFile << tau << endl;

		int netRotation = 0; //sum deltaChi

		//loop over the input bit stream
		for (int iteration = 1; iteration <= INBITITERATIONS; iteration++){

			//generate bit with probabilities p0 and p1, input delta, output a bit
			int bit = genBit();
			//cout << bit << endl;

			//change starting cell depending on input bit
			changeCell(bit, myCells_ptr, currentCell_ptr);

			//loop over timesteps (interaction with demon)
			demonInteract(currentCell_ptr, tau, sojournTime[0]);

			int deltaChi;
			deltaChi = currentCell_ptr->getBit() - bit;
			//cout << deltaChi << endl;

			netRotation += deltaChi;
		}

		cout << netRotation << endl;
		cout << (float)netRotation / INBITITERATIONS << endl;
		cout << "-----" << endl;

		phiFile << (float)netRotation / INBITITERATIONS << endl;
	}

	return 0;
}