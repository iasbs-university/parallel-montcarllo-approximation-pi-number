#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <random>

using namespace std;

#include "mpi.h"
#pragma comment(lib, "msmpi.lib")

/*Random Parameters*/
random_device rd;
mt19937 mt(rd());
normal_distribution<double> normal_dist(0.740121, 0.74018);

const long int counter = 100000;

double MontCarllo(){
	int count = 0, number = 0;
	double X, Y;
	while (number++ < counter){
		X = fabs(normal_dist(mt));
		Y = fabs(normal_dist(mt));
		if ((X *X) + (Y * Y) <= 1)
			count++;
	}
	return (double)counter / count;
}

void main(int argc, char *argv[]){

	int myid, numprocs;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	if (!myid){
		double *proccess = NULL;
		proccess = new double[numprocs];
		proccess[numprocs] = 0;
		for (int i = 0; i < numprocs - 1; i++){
			MPI_Recv(&proccess[i], 1, MPI_DOUBLE, (i + 1), 9, MPI_COMM_WORLD, &status);
			proccess[numprocs] += proccess[i];
		}
		printf("Final Pi : %f", (double)proccess[numprocs] / (double)(numprocs - 1));
		MPI_Finalize();
		exit(0);
	}
	double p = MontCarllo();
	MPI_Send(&p, 1, MPI_DOUBLE, 0, 9, MPI_COMM_WORLD);
	MPI_Finalize();
}
