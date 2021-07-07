// Victor Barranca

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
#include "BalancedIF.h"
#include "Neuron.h"
using namespace std;




//network recons of A with vectorized storage
BalancedIF::BalancedIF(double t_0, double v_0 , int n1, int n2, short holder, double r) :Neuron(t_0, v_0)
{

	V_R = 0;
	T = 1;
	g_L = 1;
	
	tau_ref = 0;
	fired = false;
	
	NE = n1;
	NI = n2;
	N = NE + NI;
	v_ = N*v_0;
	t_ = t_0;

	thetae = 1; 
	thetai = 1;
	JEE = 1;
	JII = -1.8;  
	JIE = 1;
	JEI = -2;

	
	K = .0625 * NE;

	con = 0;
	conB = 0;


	S = 1;
	numberFired = 0;   
	numberFiredCurrent = 0;
	fired = false;

	excMult = 1.25; 

	prevInh = 0;
	prevExc = 0;

	

	for (int i = 0; i <NE; i++)  //initialize exc neurons
	{
		v_0 = thetae*((double)rand() / (double)(RAND_MAX));  //random initial voltage
		Neuron node(t_0, v_0);
		neurons.push_back(node);
	}

	for (int i = NE; i <NE + NI; i++)  //initialize inh neurons
	{
		v_0 = thetai*((double)rand() / (double)(RAND_MAX));  //random initial voltage
		Neuron node(t_0, v_0);
		neurons.push_back(node);
	}


	times.clear();
	number.clear();


	//srand ( time(NULL) );
	srand(1000);

	for (int i = 0; i<N; i++)   //template for A mtx (NxN)
	{
		vector <double> row;
		for (int j = 0; j<N; j++)
		{
			row.push_back(0);
		}
		A.push_back(row);
	}

	double r1 = 0;


	//create coupling matrix taking into account inh and exc
	//exc to exc
	for (int i = 0; i<NE; i++)
	{
		for (int j = 0; j<NE; j++)
		{

			r1 = (double)rand() / (double)(RAND_MAX);

			if (r1 <= ((double)K / NE) && i != j)
			{
				A[i][j] = JEE / sqrt(double(K));
				neurons.at(j).outgoingExc.push_back(i);
				neurons.at(j).outgoingExcStr.push_back(JEE / sqrt(double(K)));
				con++;
			}
			else
			{
				A[i][j] = 0;
			}
		}
	}
	//inh to exc
	for (int i = 0; i<NE; i++)
	{
		for (int j = NE; j<NE + NI; j++)
		{

			r1 = (double)rand() / (double)(RAND_MAX);

			if (r1 <= ((double)K / NI) && i != j)
			{
				A[i][j] = JEI / sqrt(double(K));
				neurons.at(j).outgoingInh.push_back(i);
				neurons.at(j).outgoingInhStr.push_back(JEI / sqrt(double(K)));
				con++;
			}
			else
			{
				A[i][j] = 0;
			}
		}
	}
	//exc to inh
	for (int i = NE; i<NE + NI; i++)
	{
		for (int j = 0; j<NE; j++)
		{

			r1 = (double)rand() / (double)(RAND_MAX);

			if (r1 <= ((double)K / NE) && i != j)
			{
				A[i][j] = JIE / sqrt(double(K));
				neurons.at(j).outgoingExc.push_back(i);
				neurons.at(j).outgoingExcStr.push_back(JIE / sqrt(double(K)));
				con++;
			}
			else
			{
				A[i][j] = 0;
			}
		}
	}

	// inh to inh
	for (int i = NE; i<NE + NI; i++)
	{
		for (int j = NE; j<NE + NI; j++)
		{

			r1 = (double)rand() / (double)(RAND_MAX);

			if (r1 <= ((double)K / NI) && i != j)
			{
				A[i][j] = JII / sqrt(double(K));
				neurons.at(j).outgoingInh.push_back(i);
				neurons.at(j).outgoingInhStr.push_back(JII / sqrt(double(K)));
				con++;
			}
			else
			{
				A[i][j] = 0;
			}
		}
	}


	r_ = 2.1;

	randFact = .25;

			  
			   for (int i = 0; i < NE; i++)
			   {
			   C.push_back(excMult*    (r_ + (randFact*(double)rand() / (double)(RAND_MAX)) ) );
			   }

			   for (int i = NE; i < NE + NI; i++)
			   {
			   C.push_back(r_ + (randFact*(double)rand() / (double)(RAND_MAX)));
			   }    

			


	double c1 = C.size();   //  //num entries in forcing vector

	double avg = 0.0;

	//need to decipher current received by each neuron from here
	for (int i = 0; i <N; i++)
	{
		neurons.at(i).I_ += C[i]; 
		avg += neurons.at(i).I_;
	}

	avg = avg / ((double)N);

}





void BalancedIF::reset()  //RETURN PARAMETERS TO RESTING/DEFAULT VALUES
{
	

	for (int i = 0; i<N; i++)
	{
		neurons.at(i).reset();
	}
	numberFired = 0;
	numberFiredCurrent = 0;
	fired = false;
	v_ = 0;
	t_ = 0;
	times.clear();
	number.clear();

}




void BalancedIF::UpdateConst4() // event-based (spike) updating scheme
{

	

	//NECESSARY FOR UPDATING
	int check = 0;  //keeps track of case when neurons need to be reupdated due to sp event. 
	int index = 0;   //index of soonest spiking neuron
	int newindex = 0;  //next index to update
	double minTime = 10;   //soonest time until spike 
	double newT = 0;  //computed time
	int tempindex = 0; //tracks who is getting spike

	for (int i = 0; i<NE; i++)
	{
		

		//ABRIDGED FOR gl=1,vr=0,cON=1 etV
		newT = log(  (neurons.at(i).v_ - (neurons.at(i).I_))/ (thetae - ((neurons.at(i).I_)))  );


		
		if (newT < minTime && newT > 0)
		{
			minTime = newT;
			index = i;
		}
		
	}

	

	for (int i = NE; i<NE+NI; i++)
	{
		
		newT = log((neurons.at(i).v_ - (neurons.at(i).I_)) / (thetai - ((neurons.at(i).I_))));


		
		if (newT < minTime && newT > 0)
		{
			minTime = newT;
			index = i;
		}
		
	}

	


	// UPDATE NON-SPIKING NEURONS
	for (int j = 0; j<index; j++)
	{
		
		neurons.at(j).v_ = neurons.at(j).v_*exp(-g_L * minTime) + (( neurons.at(j).I_))*(1 - exp(-g_L * minTime));

	}
	for (int k = index + 1; k<N; k++)   //do not include neuron that just fired!	
	{
		
		neurons.at(k).v_ = neurons.at(k).v_*exp(-g_L * minTime) + ((neurons.at(k).I_) )*(1 - exp(-g_L * minTime));


	}

	while (check == 0)  //account for all firing events, including cascades
	{
		check = 1;   //exit loop if no new neuron spikes
		neurons.at(index).v_ = V_R;   // update variables to the spike_time FOR FIRING NEURON

		
		neurons.at(index).numSpikes++;

		for (int j = 0; j < neurons.at(index).outgoingExc.size(); j++)
		{
			tempindex = neurons.at(index).outgoingExc.at(j);
			if (neurons.at(tempindex).v_ != V_R)  //do no receive input spike in a given timestep if at reset
			{
				neurons.at(tempindex).v_ = neurons.at(tempindex).v_ + neurons.at(index).outgoingExcStr.at(j);    
			}

			if (neurons.at(tempindex).v_ >= thetae)  //check if neuron spiked
			{
				check = 0;
				newindex = tempindex;
			}
		}

		for (int j = 0; j < neurons.at(index).outgoingInh.size(); j++)
		{
			tempindex = neurons.at(index).outgoingInh.at(j);
			if (neurons.at(tempindex).v_ != V_R)  
			{
				neurons.at(tempindex).v_ = neurons.at(tempindex).v_ + neurons.at(index).outgoingInhStr.at(j); 
			}

			if (neurons.at(tempindex).v_ >= thetai)  
			{
				check = 0;
				newindex = tempindex;
			}
		}

		if (check == 1)
		{
			for (int i = 0; i < NE; i++)
			{

				if (neurons.at(i).v_ > thetae)  //somehow somes spikes slip through, ie v0>vT
				{
					check = 0;
					newindex = i;
				}


			}

			for (int i = NE; i < NE + NI; i++)
			{
				if (neurons.at(i).v_ > thetai)  //somehow somes spikes slip through, ie v0>vT
				{
					check = 0;
					newindex = i;
				}
			}
		}

		index = newindex;
	}

	t_ = t_ + minTime;  //update time to time of sp event. iterate this fn until final t is reached.
}



void BalancedIF::newforcing2()  //RETURN PARAMETERS TO RESTING/DEFAULT VALUES
{

	C.clear();


	for (int i = 0; i < NE; i++)
	{
		C.push_back(excMult*    (r_ + (randFact*(double)rand() / (double)(RAND_MAX))));
	}

	for (int i = NE; i < NE + NI; i++)
	{
		C.push_back(r_ + (randFact*(double)rand() / (double)(RAND_MAX)));
	}


	double avg = 0.0;

	
	for (int i = 0; i <N; i++)
	{
		neurons.at(i).I_ += C[i];  
		avg += neurons.at(i).I_;
	}


	avg = avg / ((double)N);

	cout << "average: " << avg << endl;

}







