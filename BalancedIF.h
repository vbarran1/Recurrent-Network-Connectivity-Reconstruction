// Victor Barranca
// Balanced Network WITH IF DYNAMICS

#ifndef BalancedIF_h_
#define BalancedIF_h_
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <iomanip>
#include <ctime>
#include <cmath>
#include "Neuron.h"

using namespace std;

class BalancedIF : public Neuron
{
public:

	//CONSTRUCTORS
	//use to recons A but with efficient storage of A mtx vec based
	BalancedIF(double t_0, double v_0, int n1, int n2);

	void UpdateConst4();   //event driven updating
	void newforcing2();

	// FUNCTIONS AND VARIABLES
	double t() const { return t_; }
	double v() const { return v_; }

	vector<Neuron> neurons;	 //keeps track of dynamics for each neuron

	vector<double> firing_times;

	//tracks firings of individual neurons
	vector<double> times;    //time of spike 
	vector<int> number;      //number of neuron that spiked

	
	void reset();
	
	double V_R;							// reset potential

	double tau_ref;						// refractory period after firing
	double g_L;

	double randFact;


	bool fired;

	int NE; //# exc neurons
	int NI; //# inh neurons
	double JEE;  //connection scalings
	double JII;
	double JIE;  //order as E -> I
	double JEI;
	

	double thetae; //thresholds for neurons types
	double thetai;
	
	int K; //number of connections of a given type

	int N;  //total number neurons
	int con; //number of ganglion to ganglion connections
	int conB;

	double tau_;					// the excitatory time constant 

	double S;  //coupling strength
	double numberFired;
	double numberFiredCurrent;
	

	double r_; // base forcing


	vector<vector<double> >A; //matrix of recurrent connections between neruons in network
	vector<vector<double> > B; //matrix of feedforward connections

	vector<double> C;  //sampled pixel matrix put in vector form
	
	vector<double> numtimes;  //keeps track of how many times a neuron has advanced


	ifstream library_reader;


	double excMult; //need input into exc neurons on avg sufficiently greater than input into inh neurons; use this to make sure

	double prevInh;
	double prevExc;

};

#endif