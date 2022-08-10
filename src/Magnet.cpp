#include "Magnet.h"

#include <time.h>
#include <iostream>
#include "TBox.h"


////////////////////////
///////// CONSTRUCTOR 

Magnet::Magnet(
	int N_, 
	double k1_, //magnetic field coupling coefficient
	double k2_, //interaction coefficient
	double beta_ //Temperature
) : N(N_), k1(k1_), k2(k2_), beta(beta_) 
{
	grid = new int*[N];

	for(int i=0; i<N; i++){
		grid[i] = new int[N];

		for(int j=0; j<N; j++){
			grid[i][j] = -1;
		}
	}

	NumUp = 0;
	NumDown = N*N;

	TR = TRandom3(time(NULL));
}


////////////////////////
///////// DESTRUCTOR 

Magnet::~Magnet(){
	for(int i=0; i<N; i++){
		delete[] grid[i];
	}
	delete[] grid;
}


////////////////////////
///////// OPERATOR() 

int Magnet::operator()(int i, int j){
	int ii = i%N, jj = j%N;
	ii = max(ii, -ii);
	jj = max(jj, -jj);
	return grid[ii][jj]; //periodic conditions
}


////////////////////////
///////// PRINT METHOD

void Magnet::Print(){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			bool up = true;
			if (grid[i][j] == -1) up = false;
			cout << up << " ";
		}
		cout << "\n";
	}
}


////////////////////////
///////// UPDATE

void Magnet::UpdateGrid(){
	// choose spin to flip
	int i = (int) (N * TR.Rndm());
	int j = (int) (N * TR.Rndm());


	double dE = 2 * k1 * (*this)(i,j);
	dE += 2 * k2 * (*this)(i,j) * (
		(*this)(i-1, j) + 
		(*this)(i+1, j) + 
		(*this)(i, j-1) +
		(*this)(i, j+1)
	);
	
	double p = TR.Rndm();
	if (dE < 0 || p < exp(-dE*beta)){
		//accept
		
		if (grid[i][j] == 1){
			NumUp--;
			NumDown++;
			grid[i][j] = -1;
		}
		else{
			NumUp++;
			NumDown--;
			grid[i][j] = 1;
		}
	}
} 

////////////////////////
///////// RUN

void Magnet::Run(
		int Niter,
		int Nskip, 
		vector<double>& M,
		int N_therm
){
	for(int iter = 0; iter < Niter; iter++){
		
		for(int subiter = 0; subiter < N*N; subiter++) this->UpdateGrid();

		if (iter < N_therm or (iter - N_therm) % Nskip) continue;

		// Process configuration
		M.push_back((NumUp - NumDown)/(double)(N*N));
	}
}

////////////////////////
///////// DRAW

void Magnet::Draw(TCanvas& c){
	c.Clear();
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			TBox* b =new TBox(j/(double)N, 1- i/(double)N, (j+1.0)/N, 1-(i+1.0)/N);
			if (grid[i][j] == 1) b->SetFillColor(kBlack);

			b->Draw();
		}
	}
}