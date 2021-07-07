// Victor Barranca
// Neuron.h

#ifndef Neuron_h_
#define Neuron_h_
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <iomanip>
#include <ctime>
#include <cmath>
using namespace std;

class Neuron
{
public:
	// Default Constructor
	Neuron();
	// Regular Constructor
	Neuron(double t_0, double v_0);
	

	void reset();


	void initialize(const double & t_0, const double & v_0);

	double v_;							// the value of the voltage
	double t_;							// the current time (in continuous time)
	double T;                           // current time constant
	double I_;                          // current total going to a particular neuron
	double V_R;


	//some of the below variables are only used in more general contexts and not in the reconstruction application

	int numSpikes;                      //number of times neuron fires during simulation

	vector<double> spikes_;			// vector that stores AMPA excitatory conductance spike times
	vector<double> firing_times;		// stores the times that action potentials were generated
	

	vector<double> outgoingExc;   //store indices of outgoing connections
	vector<double> outgoingInh;
	vector<double> outgoingExcStr;   //store cprresponding conn str
	vector<double> outgoingInhStr;

	int sp_;						// index for excitatory conductance spikes
	
	bool comp;   //tracks completion of a time step
	bool spiked;

	double t_sp; //last time a spike occured for this neuron


	int spikeCounter;
	vector<int> spiketrain;

	double actAvg;
	double numSteps;
};
#endif