/** \file test.C
 *
 */

extern int CmiNumPes();

#include <mpi.h>
#include <stdio.h>

#include "TopoManager.h"
#ifdef CMK_BLUEGENEL
#include <bglpersonality.h>
#elif CMK_BLUEGENEP
#include <dcmf.h>
#endif

int main(int argc, char *argv[]) {
  int numprocs, myrank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  if(myrank == 0) {
#if CMK_BLUEGENEL
    BGLPersonality bgl_p;
    int i = rts_get_personality(&bgl_p, sizeof(BGLPersonality));
#elif CMK_BLUEGENEP
    DCMF_Hardware_t bgp_hwt;
    DCMF_Hardware(&bgp_hwt);
#elif CMK_CRAYXT
    XTTorusManager xttm(numprocs);
#endif

    int x, y, z, t;
    int a, b, c, d, e;
    printf("Testing TopoManager .... \n");
    printf("MPI Job Size: %d cores\n\n", numprocs);
    
    TopoManager tmgr = TopoManager(numprocs);
#if CMK_BLUEGENEQ
    printf("Torus Size [%d] [%d] [%d] [%d] [%d] [%d]\n\n", tmgr.getDimNA(), tmgr.getDimNB(), tmgr.getDimNC(), tmgr.getDimND(), tmgr.getDimNE(), tmgr.getDimNT());
#else
    printf("Torus Size [%d] [%d] [%d] [%d]\n\n", tmgr.getDimNX(), tmgr.getDimNY(), tmgr.getDimNZ(), tmgr.getDimNT());
#endif

    for(int i=0; i<numprocs; i++) {
#if CMK_BLUEGENEQ
      tmgr.rankToCoordinates(i, a, b, c, d, e, t);
      printf("---- Processor %d ---> a %d b %d c %d d %d e %d t %d\n", i, a, b, c, d, e, t);
#else
      tmgr.rankToCoordinates(i, x, y, z, t); 
      printf("---- Processor %d ---> x %d y %d z %d t %d\n", i, x, y, z, t);
#endif
#if CMK_BLUEGENEL
      unsigned int tmp_t, tmp_x, tmp_y, tmp_z;
      rts_coordinatesForRank(i, &tmp_x, &tmp_y, &tmp_z, &tmp_t);
      printf("Real Processor %d ---> x %d y %d z %d t %d\n", i, tmp_x, tmp_y, tmp_z, tmp_t);
#elif CMK_BLUEGENEP
      DCMF_NetworkCoord_t coords;
      DCMF_Messager_rank2network(i, DCMF_TORUS_NETWORK, &coords);
      printf("Real Processor %d ---> x %d y %d z %d t %d\n", i, coords.torus.x, coords.torus.y, coords.torus.z, coords.torus.t);
#elif CMK_CRAYXT
      int tmp_t, tmp_x, tmp_y, tmp_z;
      xttm.realRankToCoordinates(i, tmp_x, tmp_y, tmp_z, tmp_t);
      printf("Real Processor %d ---> x %d y %d z %d t %d\n", i, tmp_x, tmp_y, tmp_z, tmp_t);
#endif
    }
  
#if CMK_BLUEGENEQ
    int size = tmgr.getDimNA() * tmgr.getDimNB() * tmgr.getDimNC() * tmgr.getDimND() * tmgr.getDimNE();
#else
    int size = tmgr.getDimNX() * tmgr.getDimNY() * tmgr.getDimNZ();
#endif
    printf("\nTorus Contiguity Metric %d : %d [%f] \n", size, numprocs/tmgr.getDimNT(), (float)(numprocs)/(tmgr.getDimNT()*size) );
  }

  MPI_Finalize();
  return 0;
}
