#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <map>
#include "Neuron.h"
#include "BalancedIF.h"



using namespace std;


void recSetBalancedRecurrentIFmean();


int main()
{

	recSetBalancedRecurrentIFmean();
	return 3;
}










void recSetBalancedRecurrentIFmean()    
{
	
	double run_time = 200; 

	//number of E and I neurons, respectively					
	int n1 = 5000;   
	int n2 = 5000;          

					// Create the output files
	ofstream times("Times.txt");  //stores each time stats are computed
	ofstream volt("Voltage.txt"); //stores mean voltage across network (LFP)

	ofstream fr("firingRates.txt");

	double vTot = 0;  //used to store total voltage across network

	ofstream A("A.txt");

	//ofstream B("B.txt");
	//ofstream C("C.txt");
	//ofstream eT("eT.txt");
	//ofstream eN("eN.txt");

	ofstream conNumA("conNumA.txt");  //number of connections in A

	ofstream numIt("numIt.txt");  

							
	double fval = 1.5; //value of constant stimulus  //often 1.5 in 1000 neuron net, but 1.1 seems to work well for smaller 100 neuron net
	double finc = 0; //amount to increment forcing after each iteration

	int iterations = 1000;  //number of iterations (how many different stimuli use to measure system)
	numIt << iterations << endl;


	vector<double> avvolt;


	time_t start = time(NULL);


	//place holder variable, to distinguish between multiple constructors used in other code
	short holder2 = 2;

	BalancedIF nodes(0, 0, n1, n2, holder2, fval);

	srand((unsigned)time(NULL));

	//write recurrent matrix A to text doc to be used by matlab
	for (int i = 0; i<nodes.N; i++)
	{
		for (int j = 0; j< nodes.N; j++)
		{
			A << nodes.A[i][j];
			A << " ";
		}
		A << endl;
	}

	for (int i = 0; i < nodes.N; i++)
	{
		avvolt.push_back(0);
	}


	for (int k = 1; k <= iterations; k++)
	{

		

		string fname = "firingRates";

		string Result;          // string which will contain the result
		string Result2;
		string Result3;

		ostringstream convert;   // stream used for the conversion

		convert << k;      // insert the textual representation of 'Number' in the characters in the stream

		Result = convert.str(); // set 'Result' to the contents of the stream 
								

		fname.append(Result);


		fname.append(".txt");

		ofstream fr(fname);



		ostringstream convert2;   

		convert2 << k;      

		Result2 = convert2.str(); 
								  

		string fname2 = "forcing";

		fname2.append(Result2);



		fname2.append(".txt");

		ofstream forc(fname2);



		ostringstream convert3;   

		convert3 << k;      

		Result3 = convert3.str();

		string fname3 = "volt";

		fname3.append(Result3);



		fname3.append(".txt");

		ofstream mvolt(fname3);

		int tcount = 0;

		cout << "here" << endl;
		// RUN THE NEURONS
		time_t start = time(NULL);
		cout << "Running the Network." << endl;
		while (nodes.t_<run_time)
		{



			
			for (int i = 0; i<nodes.N; i++)
			{
				avvolt.at(i) += nodes.neurons.at(i).v_;

			}
			tcount = tcount + 1;

			nodes.UpdateConst4();


		}

		for (int i = 0; i<nodes.N; i++)
		{
			avvolt.at(i) = avvolt.at(i) / tcount;
		}


		//compute firing rates for neurons in network, write to text document
		for (int i = 0; i < nodes.N; i++)
		{
			fr << (nodes.neurons.at(i).numSpikes) / (run_time) << endl;

			mvolt << avvolt.at(i) << endl;


		}


		for (int i = 0; i<nodes.N; i++)  //reuse in code so B need not be regenerated with each run 
		{
			forc << nodes.neurons.at(i).I_ << endl;
		}

		fval = fval + finc; //increment forcing

		for (int i = 0; i<nodes.N; i++)
		{
			avvolt.at(i) = 0;
		}

		nodes.reset();   //MAKE SURE I=0 IS NOT COMMENTED OUT
		nodes.newforcing2(); 
		cout << k << endl;
	}

	time_t end = time(NULL);
	double etime = difftime(end, start);
	double hours = floor(etime / 3600);
	double minutes = floor((etime - hours * 3600) / 60);
	double seconds = etime - hours * 3600 - minutes * 60;

	cout << "TIME RESULTS" << endl;
	cout << "     Elapsed Time: " << hours << "h " << minutes << "m " << seconds << "s " << endl;

	system("pause");
}



