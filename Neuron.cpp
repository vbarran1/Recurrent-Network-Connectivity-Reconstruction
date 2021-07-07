// Victor Barranca
// Neuron.cpp

#define _USE_MATH_DEFINES
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <iomanip>
#include <ctime>
#include <cmath>
#include "Neuron.h"
using namespace std;

// Default Constructor
Neuron::Neuron()
{
	
	initialize(0, 0);
}
// Regular Constructor
Neuron::Neuron(double t_0, double v_0)
{
	// call the initialization function with the given arguments
	initialize(t_0, v_0);
}

// Initialization Function
void Neuron::initialize(const double & t_0, const double & v_0)
{
	// initialize variables
	t_ = t_0;						// initial time
	v_ = v_0;						// initial voltage
	I_=0;
	t_sp = t_0;
	T = 1;   //current time constant
	numSpikes=0;


	V_R=0;
	sp_ = 0;					

	spikeCounter = 0;
	actAvg = 0;
	numSteps = 0;

	firing_times.clear();
	comp = false;
	spiked = false;
	return;
}
void Neuron::reset()
{

	t_=0;
	
	v_ = (double)rand()/(double)(RAND_MAX);
	numSpikes=0;
	firing_times.clear();
	comp = false;
	spiked = false;

	I_=0;  

	spikeCounter = 0;
	actAvg = 0;
	numSteps = 0;
}


