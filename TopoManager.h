/** \file TopoManager.h
 *  Author: Abhinav S Bhatele
 *  Date Created: March 19th, 2007
 *
 *  This is the top level interface for topology managers we have for different
 *  machines (Cray, IBM ... for tori, meshes ...) Currently, we have
 *  functionality for Blue Gene, Cray XT/XE/XK, BigSim and non-topo machines.
 *
 *  Any published work which utilizes this API should include the following
 *  reference:
 *
 *  Abhinav Bhatele, Eric Bohm, Laxmikant V. Kale, Optimizing communication for
 *  Charm++ applications by reducing network contention, Concurrency and
 *  Computation: Practice and Experience (EuroPar special issue), Vol: 23,
 *  Issue: 2, Pages: 211-222, 2011"
 *
 */

#ifndef _TOPO_MANAGER_H_
#define _TOPO_MANAGER_H_

#include <stdio.h>
#include <stdlib.h>

#if defined(__blrts__)
#define CMK_BLUEGENEL  1
#include "BGLTorus.h"

#elif defined(__bgp__)
#define CMK_BLUEGENEP  1
#include "BGPTorus.h"

#elif defined(__bgq__)
#define CMK_BLUEGENEQ  1
#include "BGQTorus.h"

#elif CMK_CRAYXT
#include "XTTorus.h"
#endif

class TopoManager {
  public:
    TopoManager(int _numPes);
#if !CMK_BLUEGENEQ
    TopoManager(int NX, int NY, int NZ, int NT);
#endif
    ~TopoManager() { }

#if CMK_BLUEGENEQ
    inline int getDimNA() const { return dimNA; }
    inline int getDimNB() const { return dimNB; }
    inline int getDimNC() const { return dimNC; }
    inline int getDimND() const { return dimND; }
    inline int getDimNE() const { return dimNE; }
#else
    inline int getDimNX() const { return dimNX; }
    inline int getDimNY() const { return dimNY; }
    inline int getDimNZ() const { return dimNZ; }
#endif
    inline int getDimNT() const { return dimNT; }

    inline int getProcsPerNode() const { return procsPerNode; }

    int hasMultipleProcsPerNode() const;
#if CMK_BLUEGENEQ
    void rankToCoordinates(int pe, int &a, int &b, int &c, int &d, int &e, int &t);
    int coordinatesToRank(int a, int b, int c, int d, int e, int t);
#else
    void rankToCoordinates(int pe, int &x, int &y, int &z, int &t);
    int coordinatesToRank(int x, int y, int z, int t);
#endif
    int getHopsBetweenRanks(int pe1, int pe2);
    void sortRanksByHops(int pe, int *pes, int *idx, int n);
    int pickClosestRank(int mype, int *pes, int n);
    int areNeighbors(int pe1, int pe2, int pe3, int distance);

    inline int absTorus(int x, int dim, int torus) {
      int px = abs(x);
      int sx = dim - px;
      // CmiAssert(sx>=0);
      if(torus)
        return ((px>sx) ? sx : px);
      else
        return px;
    }
    
  private:
#if CMK_BLUEGENEQ
    int dimNA, dimNB, dimNC, dimND, dimNE;
    int torusA, torusB, torusC, torusD, torusE;
#else
    int dimNX, dimNY, dimNZ;	// no. of nodes along X, Y, Z
    int torusX, torusY, torusZ;
#endif
    int dimNT;			// no. of cores per node
    int torusT;
    int numPes;
    int procsPerNode;

#if CMK_BLUEGENEL
    BGLTorusManager *bgltm;
#elif CMK_BLUEGENEP
    BGPTorusManager *bgptm;
#elif CMK_BLUEGENEQ
    BGQTorusManager *bgqtm;
#elif CMK_CRAYXT
    XTTorusManager *xttm;
#endif

    void quicksort(int pe, int *pes, int *arr, int left, int right);
    int partition(int pe, int *pes, int *idx, int left, int right);
};

#endif //_TOPO_MANAGER_H_
