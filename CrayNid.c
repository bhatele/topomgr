/** \file CrayNid.c
 *  Author: Abhinav S Bhatele
 *  Date created: October 10th, 2007  
 *  
 *  This file is needed because including the cnos_mpi_os.h in a C++ leads 
 *  to a compiler error. Hence we have defined a wrapper function here which
 *  can be called from C++ files
 */

#include <stdlib.h>

#if CMK_CRAYXT

#include <pmi.h>
#include <rca_lib.h>

#define MAXNID 6600 // Values for Hopper
#define TDIM 24

/** \function getXTNodeID
 *  returns nodeID corresponding to the MPI rank (possibly obtained
 *  from CmiMyNode()/CmiNodeOf(pe)) passed to it
 */
int getXTNodeID(int mpirank, int nummpiranks) {
  int nid = -1;

  PMI_Get_nid(mpirank, &nid);
  return nid;
}

int *pid2nid;                   /* rank to node ID */
int nid2pid[MAXNID][TDIM];      /* node ID to rank */

/** \function getMeshCoord
 *  wrapper function for rca_get_meshcoord
 */
void getMeshCoord(int nid, int *x, int *y, int *z) {
  rca_mesh_coord_t xyz;
  rca_get_meshcoord(nid, &xyz);
  *x = xyz.mesh_x;
  *y = xyz.mesh_y;
  *z = xyz.mesh_z;
}

/** \function pidtonid
 *  finds nids for pids 1 to CmiNumPes and stores them in an array
 *  correspondingly also creates an array for nids to pids
 */
void pidtonid(int numpes) {
  int i, l, nid;
  pid2nid = (int *)malloc(sizeof(int) * numpes);

  for(i=0; i<MAXNID; i++)
    for(l=0; l<TDIM; l++)
      nid2pid[i][l] = -1;

  for (i=0; i<numpes; i++) {
    PMI_Get_nid(i, &nid);
    pid2nid[i] = nid;

    l = 0;
    while(nid2pid[nid][l] != -1)
      l++;
    nid2pid[nid][l] = i;
  }
}

#endif /* CMK_CRAYXT */
