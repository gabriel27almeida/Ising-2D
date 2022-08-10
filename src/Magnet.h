#ifndef __MAGNET__
#define __MAGNET__

#include <vector>
#include "TRandom3.h"
#include "TCanvas.h"

using namespace std;

/*
Energy = - k1 * Sum over spins of spin i
		 - k2 * Sum over pairs of (spin i * spin j) 
*/

class Magnet{
public:
	Magnet(
		int N, 
		double k1, // magnetic field coupling coefficient
		double k2, // interaction coefficient
		double beta // 1/(kB Temperature)
	);
	~Magnet();

	//Run
	void UpdateGrid();
	void Run(
		int Niter,
		int Nskip,
		vector<double>& M, 
		int N_therm = 0
	);

	//Getters
	int operator()(int i, int j);

	//Utility
	void Print();

	//Draw
	void Draw(TCanvas&);


private:
	int N;
	double beta;
	double k1;
	double k2;
	int NumUp, NumDown;
	int** grid; 

	TRandom3 TR;
};


#endif