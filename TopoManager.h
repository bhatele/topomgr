 /*****************************************************************************
 * $Source$
 * $Author$
 * $Date$
 * $Revision$
 *****************************************************************************/

 /** \file TopoManager.h
 *  Author: Abhinav S Bhatele
 *  Date Created: March 19th, 2007
 *
 *  This would be the top level interface for all topology managers we
 *  will write for different machines (cray, bg/l ... for tori, meshes ...)
 *  Current plan is to have functionality for Blue Gene/L, Cray XT3,
 *  BigSim and non-topo machines.
 *
 * Any published work which utilizes this API should include the following
 * reference:
 *
 * "A. Bhatele and L. V. Kale, Benefits of Topology Aware Mapping for Mesh 
 * Interconnects, Parallel Processing Letters (LSPP special issue), 2008."
 *
 */

#ifndef _TOPO_MANAGER_H_
#define _TOPO_MANAGER_H_

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#if CMK_BLUEGENEL
#include "BGLTorus.h"
#elif CMK_BLUEGENEP
#include "BGPTorus.h"
#elif XT3_TOPOLOGY
#include "XT3Torus.h"
#elif XT4_TOPOLOGY
#include "XT4Torus.h"
#endif

class TopoManager {
  private:
    int dimX;	// dimension of the allocation in X (no. of processors)
    int dimY;	// dimension of the allocation in Y (no. of processors)
    int dimZ;	// dimension of the allocation in Z (no. of processors)
    int dimNX;	// dimension of the allocation in X (no. of nodes)
    int dimNY;	// dimension of the allocation in Y (no. of nodes)
    int dimNZ;	// dimension of the allocation in Z (no. of nodes)
    int dimNT;  // dimension of the allocation in T (no. of processors per node)
    int torusX, torusY, torusZ, torusT; 
    int procsPerNode;

#if CMK_BLUEGENEL
    BGLTorusManager bgltm;
#elif CMK_BLUEGENEP
    BGPTorusManager bgptm;
#elif XT3_TOPOLOGY
    XT3TorusManager xt3tm;
#elif XT4_TOPOLOGY
    XT4TorusManager xt4tm;
#endif

  public:

    TopoManager() {
#if CMK_BLUEGENEL
      dimX = bgltm.getDimX();
      dimY = bgltm.getDimY();
      dimZ = bgltm.getDimZ();
    
      dimNX = bgltm.getDimNX();
      dimNY = bgltm.getDimNY();
      dimNZ = bgltm.getDimNZ();
      dimNT = bgltm.getDimNT();
      
      procsPerNode = bgltm.getProcsPerNode();
      int *torus;
      torus = bgltm.isTorus();
      torusX = torus[0];
      torusY = torus[1];
      torusZ = torus[2];
      torusT = torus[3];

#elif CMK_BLUEGENEP
      dimX = bgptm.getDimX();
      dimY = bgptm.getDimY();
      dimZ = bgptm.getDimZ();
    
      dimNX = bgptm.getDimNX();
      dimNY = bgptm.getDimNY();
      dimNZ = bgptm.getDimNZ();
      dimNT = bgptm.getDimNT();

      procsPerNode = bgptm.getProcsPerNode();
      int *torus;
      torus = bgptm.isTorus();
      torusX = torus[0];
      torusY = torus[1];
      torusZ = torus[2];
      torusT = torus[3];

#elif XT3_TOPOLOGY
      dimX = xt3tm.getDimX();
      dimY = xt3tm.getDimY();
      dimZ = xt3tm.getDimZ();
    
      dimNX = xt3tm.getDimNX();
      dimNY = xt3tm.getDimNY();
      dimNZ = xt3tm.getDimNZ();
      dimNT = xt3tm.getDimNT();

      procsPerNode = xt3tm.getProcsPerNode();
      int *torus;
      torus = xt3tm.isTorus();
      torusX = torus[0];
      torusY = torus[1];
      torusZ = torus[2];
      torusT = torus[3];

#elif XT4_TOPOLOGY
      dimX = xt4tm.getDimX();
      dimY = xt4tm.getDimY();
      dimZ = xt4tm.getDimZ();
    
      dimNX = xt4tm.getDimNX();
      dimNY = xt4tm.getDimNY();
      dimNZ = xt4tm.getDimNZ();
      dimNT = xt4tm.getDimNT();

      procsPerNode = xt4tm.getProcsPerNode();
      int *torus;
      torus = xt4tm.isTorus();
      torusX = torus[0];
      torusY = torus[1];
      torusZ = torus[2];
      torusT = torus[3];

#else
      dimX = CkNumPes();
      dimY = 1;
      dimZ = 1;

      dimNX = dimX;
      dimNY = 1;
      dimNZ = 1;

      procsPerNode = 1;
      torusX = true;
      torusY = true;
      torusZ = true;
#endif
    }

    TopoManager(int NX, int NY, int NZ, int NT) : dimNX(NX), dimNY(NY), dimNZ(NZ), dimNT(NT) {
      // we rashly assume only one dimension is expanded 
      procsPerNode = dimNT;
      dimX = dimNX * dimNT;
      dimY = dimNY;
      dimZ = dimNZ;
      torusX = true;
      torusY = true;
      torusZ = true;
    }

    ~TopoManager() {
     }

    inline int getDimX() { return dimX; }
    inline int getDimY() { return dimY; }
    inline int getDimZ() { return dimZ; }

    inline int getDimNX() { return dimNX; }
    inline int getDimNY() { return dimNY; }
    inline int getDimNZ() { return dimNZ; }
    inline int getDimNT() { return dimNT; }

    inline int getProcsPerNode() { return procsPerNode; }
    
    inline int absX(int x) {
      int px = abs(x);
      int sx = dimX - px;
      if (sx<0) { // CmiAssert(sx>=0);
        printf("ERROR!!!\n");
        abort();
      }
      if(torusX)
        return ((px>sx) ? sx : px);
      else
        return px;
    }
    
    inline int absY(int y) {
      int py = abs(y);
      int sy = dimY - py;
      if (sy<0) { // CmiAssert(sy>=0);
        printf("ERROR!!!\n");
        abort();
      }
      if(torusY)
        return ((py>sy) ? sy : py);
      else
        return py;
    }

    inline int absZ(int z) {
      int pz = abs(z);
      int sz = dimZ - pz;
      if (sz<0) { // CmiAssert(sz>=0);
        printf("ERROR!!!\n");
        abort();
      }
      if(torusZ)
        return ((pz>sz) ? sz : pz);
      else
        return pz;
    }
    
    int hasMultipleProcsPerNode();
    void rankToCoordinates(int pe, int &x, int &y, int &z);
    void rankToCoordinates(int pe, int &x, int &y, int &z, int &t);
    int coordinatesToRank(int x, int y, int z);
    int coordinatesToRank(int x, int y, int z, int t);
    int getHopsBetweenRanks(int pe1, int pe2);
    void sortRanksByHops(int pe, int *pes, int *idx, int n); 
    int pickClosestRank(int mype, int *pes, int n);
    int areNeighbors(int pe1, int pe2, int pe3, int distance);

  private:
    void quicksort(int pe, int *pes, int *arr, int left, int right);
    int partition(int pe, int *pes, int *idx, int left, int right);

};

#endif //_TOPO_MANAGER_H_
