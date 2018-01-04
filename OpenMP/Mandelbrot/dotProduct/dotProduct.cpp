// #include <omp.h>
#include <iostream>
#include <fstream>
#include "walltime.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "omp.h"
#define NUM_ITERATIONS 100

// Example benchmarks
// 0.03s ~0.8MB
// #define NMAX 100000
// 0.3s ~8MB
// #define NMAX 1000000
// 3.s ~80MB
// #define NMAX 10000000
// 30s ~800MB
// #define NMAX 100000000
#define EPSILON 0.1
// compile with g++ -O3 -fopenmp dotProduct.cpp -o dot

// TODO:
// 1. scalar product in parallel
//   i. Using reduction pragma
//   ii. Using critical pragma

using namespace std;

int main(int argc, char** argv) {
    double time_serial, time_start = 0.0;
    long double dotProduct;
    double *a, *b;

    long int NMAX_LIST[] = {100000,1000000,10000000,100000000}; //List of NMAX values we need to evaluate on
    int NUM_THREAD_LIST[] = {1,2,4,8}; //List of number of threads

    for(int nm_i = 0;nm_i<4;++nm_i){
        long int NMAX = NMAX_LIST[nm_i]; //Selection of NMAX


        // Allocate memory for the vectors as 1-D arrays
        a = new double[NMAX];
        b = new double[NMAX];

        // Initialize the vectors with some values
        for (int i = 0; i < NMAX; i++){
            a[i] = i;
            b[i] = i / 10.0;
        }


        // Serial execution
        long double alpha = 0;
        // Note that we do extra iterations to reduce relative timing overhead
        time_start = walltime(0);
        for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
            alpha = 0.0;
            for (int i = 0; i < NMAX; i ++) {
                alpha += a[i] * b[i];
            }
        }
        time_serial = walltime(time_start);
        // cout << "Serial execution time =" << time_serial << " sec" << endl;
        cout<<NMAX<<" "<<time_serial<<" ";
        long double alpha_parallel = 0;
        double time_red = 0;
        double time_critical = 0;
        //Iterate across the number of threads and limit it to those number.
        for(int nt_i=0;nt_i<4;++nt_i){
            omp_set_num_threads(NUM_THREAD_LIST[nt_i]); // Set the number of threads to use for the parallel process.
            cerr<<NUM_THREAD_LIST[nt_i]<<" ";

            // TODO: Write parallel version (2 ways!)
            //Parallel version with reduction clause
            time_start = walltime(0);
            for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
                alpha_parallel = 0.0;
                #pragma omp parallel for reduction(+:alpha_parallel)
                for (int i = 0; i < NMAX; i ++) {
                    alpha_parallel += a[i] * b[i];
                }
            }
            time_red = walltime(time_start);

            //Parallel version with parallel and critical clause
            time_start = walltime(0);
            for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
                alpha_parallel = 0.0;
                #pragma omp parallel
                {
                    long double localsum = 0; //Should be private to each thread.
                    #pragma omp for
                    for (int i = 0; i < NMAX; i ++) {
                        localsum += a[i] * b[i];
                    }
                    #pragma omp critical
                    {
                        alpha_parallel += localsum;
                    }
                }
            }
            time_critical = walltime(time_start);
            if ( (fabs(alpha_parallel - alpha) / fabs(alpha_parallel)) > EPSILON) {
                cout << "parallel reduction: " << alpha_parallel << " serial :" << alpha << "\n";
                cerr << "Alpha not yet implemented correctly!\n";
                exit(1);
            }
            cout<<time_red<<" "<<time_critical<<" ";


            //Calculate and output the strong scaling efficiency
            // t1 / ( N * tN ) * 100% if t1 is the time taken with 1 processing unit and N units take tN time.
            cerr<<time_serial/time_red<<" "<<time_serial/time_critical<<" "<<endl;

        }

        cout<<endl;
        cerr<<endl;
        // De-allocate memory
        delete [] a;
        delete [] b;
        // cout << "Time in Serial = " << time_serial
        //      << " sec time using reduction method = " << time_red
        //      << " sec, time using critical method " << time_critical
        //      << " sec" << endl;
    }
    return 0;
}
