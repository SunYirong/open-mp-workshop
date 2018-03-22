#include <stdlib.h>
#include <stdio.h>

#include <omp.h>
#include "traffic.h"

#define NCELL 100000

int main(int argc, char **argv)
{
  int *oldroad, *newroad;

  int i, iter, nmove, ncars;
  int maxiter, printfreq; 
  double startt, endt;

  float density; 

  oldroad = (int *) malloc((NCELL+2)*sizeof(int));
  newroad = (int *) malloc((NCELL+2)*sizeof(int));

  maxiter = 200000000/NCELL; 
  printfreq = maxiter/10; 

  // Set target density of cars

  density = 0.52;

  printf("Length of road is %d\n", NCELL);
  printf("Number of iterations is %d \n", maxiter);
  printf("Target density of cars is %f \n", density);

  // Initialise road accordingly using random number generator

  printf("Initialising road ...\n");
  
  ncars = initroad(&oldroad[1], NCELL, density, SEED);

  printf("...done\n");

  printf("Actual density of cars is %f\n\n", (float) ncars / (float) NCELL);

  startt = omp_get_wtime();

//  #pragma omp parallel for private(iter, i) shared(maxiter, printfreq, nmove, ncars, newroad, oldroad)\
default(none)
  for (iter=1; iter<=maxiter; iter++)
    { 
      updatebcs(oldroad, NCELL);

      // Apply CA rules to all cells
      
      
      nmove = updateroad(newroad, oldroad, NCELL);
      
      // Copy new to old array
      #pragma omp parallel for private(i) shared(oldroad, newroad)  default(none)
      for (i=1; i<=NCELL; i++)
	{
	  oldroad[i] = newroad[i]; 
	}
      

      if (iter%printfreq == 0)
	{
	  printf("At iteration %d average velocity is %f \n",
		 iter, (float) nmove / (float) ncars);
	} 
    }

  endt = omp_get_wtime();
  printf("Time is %f \n", endt-startt);
 

  free(oldroad);
  free(newroad);
  
  printf("\nFinished\n");
}