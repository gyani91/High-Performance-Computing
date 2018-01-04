/****************************************************************
 *                                                              *
 * This file has been written as a sample solution to an        *
 * exercise in a course given at the CSCS Summer School.        *
 * It is made freely available with the understanding that      *
 * every copy of this file must include this header and that    *
 * CSCS take no responsibility for the use of the enclosed      *
 * teaching material.                                           *
 *                                                              *
 * Purpose: Exchange ghost cell in 2 directions using a topology*
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/

/* Use only 16 processes for this exercise
 * Send the ghost cell in two directions: left<->right and top<->bottom
 * ranks are connected in a cyclic manner, for instance, rank 0 and 12 are connected
 *
 * process decomposition on 4*4 grid
 *
 * |-----------|
 * | 0| 1| 2| 3|
 * |-----------|
 * | 4| 5| 6| 7|
 * |-----------|
 * | 8| 9|10|11|
 * |-----------|
 * |12|13|14|15|
 * |-----------|
 *
 * Each process works on a 10*10 (SUBDOMAIN) block of data
 * the D corresponds to data, g corresponds to "ghost cells"
 * xggggggggggx
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * xggggggggggx
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SUBDOMAIN 10
#define DOMAINSIZE (SUBDOMAIN+2)

int main(int argc, char *argv[])
{
    int rank, size, i, j, dims[2], periods[2], rank_top, rank_bottom, rank_left, rank_right;
    double data[DOMAINSIZE*DOMAINSIZE];
    MPI_Request request;
    MPI_Status status;
    MPI_Comm comm_cart;
    MPI_Datatype data_ghost;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size!=16) {
        printf("please run this with 16 processors\n");
        MPI_Finalize();
        exit(1);
    }

    // initialize the domain
    for (i=0; i<DOMAINSIZE*DOMAINSIZE; i++) {
        data[i]=rank;
    }

    // TODO: set the dimensions of the processor grid and periodic boundaries in both dimensions
    dims[0]=4; //Dimensions of the processor grid
    dims[1]=4;
    periods[0]=1; //Periodic boundaries in both the axis. Hence = 1 (True)
    periods[1]=1;

    // we do not allow the reordering of ranks here
    // an alternative solution would be to allow the reordering and to use the new communicator for the communication
    // then the MPI library has the opportunity to choose the best rank order with respect to performance
    // CREATE a cartesian communicator (4*4) with periodic boundaries and use it to find your neighboring
    // ranks in all dimensions in a cyclic manner.

    // TODO: create new communicator comm_cart using the dims and periods defined above, see MPI_Cart_create()
    // MPI_Cart_create(comm_old, number of dimensions, dimension size, periods, reorder (false), comm_new)
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm_cart);
    
    // TODO: find your top/bottom/left/right neighbor using the new communicator, see MPI_Cart_shift()
    // rank_top, rank_bottom
    // rank_left, rank_right

    MPI_Cart_shift(comm_cart, 0, 1, &rank_top, &rank_bottom);
    MPI_Cart_shift(comm_cart, 1, 1, &rank_left, &rank_right);

    //  TODO: create derived datatype data_ghost, create a datatype for sending the column, see MPI_Type_vector() and MPI_Type_commit()
    // data_ghost
    MPI_Type_vector(SUBDOMAIN, 1, DOMAINSIZE, MPI_DOUBLE, &data_ghost);
    MPI_Type_commit(&data_ghost);

    //  TODO: ghost cell exchange with the neighbouring cells in all directions
    //  use MPI_Irecv(), MPI_Send(), MPI_Wait() or other viable alternatives
    //  to the top
    MPI_Irecv(&data[133], SUBDOMAIN, MPI_DOUBLE, rank_bottom, 0, MPI_COMM_WORLD, &request);
    MPI_Send(&data[13], SUBDOMAIN, MPI_DOUBLE, rank_top, 0, MPI_COMM_WORLD);
    MPI_Wait(&request, &status);

    //  to the bottom
    MPI_Irecv(&data[1], SUBDOMAIN, MPI_DOUBLE, rank_top, 0, MPI_COMM_WORLD, &request);
    MPI_Send(&data[121], SUBDOMAIN, MPI_DOUBLE, rank_bottom, 0, MPI_COMM_WORLD);
    MPI_Wait(&request, &status);

    //  to the left
    MPI_Irecv(&data[23], 1, data_ghost, rank_right, 0, MPI_COMM_WORLD, &request);
    MPI_Send(&data[13], 1, data_ghost, rank_left, 0, MPI_COMM_WORLD);
    MPI_Wait(&request, &status);

    //  to the right
    MPI_Irecv(&data[12], 1, data_ghost, rank_left, 0, MPI_COMM_WORLD, &request);
    MPI_Send(&data[22], 1, data_ghost, rank_right, 0, MPI_COMM_WORLD);
    MPI_Wait(&request, &status);

    if (rank==9) {
        printf("data of rank 9 after communication\n");
        for (j=0; j<DOMAINSIZE; j++) {
            for (i=0; i<DOMAINSIZE; i++) {
                printf("%.1f ", data[i+j*DOMAINSIZE]);
            }
            printf("\n");
        }
    }

    // TODO: uncomment when done with tasks above
    MPI_Type_free(&data_ghost);
    MPI_Comm_free(&comm_cart);
    MPI_Finalize();

    return 0;
}