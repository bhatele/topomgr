/** \file TopoManager.C
 *  Author: Abhinav S Bhatele
 *  Date Created: March 19th, 2007
 *
 *  This is the top level interface for topology managers we have for different
 *  machines (Cray, IBM ... for tori, meshes ...) Currently, we have
 *  functionality for Blue Gene, Cray XT/XE/XK, BigSim and non-topo machines.
 */

#include "TopoManager.h"

TopoManager::TopoManager(int _numPes) : numPes(_numPes) {
#if CMK_BLUEGENEL
  bgltm = new BGLTorusManager(numPes);

  dimNX = bgltm->getDimNX();
  dimNY = bgltm->getDimNY();
  dimNZ = bgltm->getDimNZ();
  dimNT = bgltm->getDimNT();

  procsPerNode = bgltm->getProcsPerNode();
  int *torus;
  torus = bgltm->isTorus();
  torusX = torus[0];
  torusY = torus[1];
  torusZ = torus[2];
  torusT = torus[3];

#elif CMK_BLUEGENEP
  bgptm = new BGPTorusManager(numPes);

  dimNX = bgptm->getDimNX();
  dimNY = bgptm->getDimNY();
  dimNZ = bgptm->getDimNZ();
  dimNT = bgptm->getDimNT();

  procsPerNode = bgptm->getProcsPerNode();
  int *torus;
  torus = bgptm->isTorus();
  torusX = torus[0];
  torusY = torus[1];
  torusZ = torus[2];
  torusT = torus[3];

#elif CMK_BLUEGENEQ
  bgqtm = new BGQTorusManager(numPes);

  dimNA = bgqtm->getDimNA();
  dimNB = bgqtm->getDimNB();
  dimNC = bgqtm->getDimNC();
  dimND = bgqtm->getDimND();
  dimNE = bgqtm->getDimNE();
  dimNT = bgqtm->getDimNT();

  procsPerNode = bgqtm->getProcsPerNode();
  int *torus;
  torus = bgqtm->isTorus();
  torusA = torus[0];
  torusB = torus[1];
  torusC = torus[2];
  torusD = torus[3];
  torusE = torus[4];

#elif CMK_CRAYXT
  xttm = new XTTorusManager(numPes);

  dimNX = xttm->getDimNX();
  dimNY = xttm->getDimNY();
  dimNZ = xttm->getDimNZ();
  dimNT = xttm->getDimNT();

  procsPerNode = xttm->getProcsPerNode();
  int *torus;
  torus = xttm->isTorus();
  torusX = torus[0];
  torusY = torus[1];
  torusZ = torus[2];
  torusT = torus[3];

#else
  dimNX = numPes;
  dimNY = 1;
  dimNZ = 1;

  dimNT = procsPerNode = 1;
  torusX = true;
  torusY = true;
  torusZ = true;
  torusT = false;
#endif
}

#if !CMK_BLUEGENEQ
TopoManager::TopoManager(int NX, int NY, int NZ, int NT) : dimNX(NX), dimNY(NY), dimNZ(NZ), dimNT(NT) {
  procsPerNode = dimNT;
  torusX = true;
  torusY = true;
  torusZ = true;
  torusT = false;
  numPes = dimNX * dimNY * dimNZ * dimNT;
}
#endif

int TopoManager::hasMultipleProcsPerNode() const {
  if(procsPerNode == 1)
    return 0;
  else
    return 1;
}

#if CMK_BLUEGENEQ
void TopoManager::rankToCoordinates(int pe, int &a, int &b, int &c, int &d, int &e, int &t) {
  if(pe < 0 || pe >= numPes) {
    printf("Processor number %d out of range\n", pe);
    abort();
  }
  bgqtm->rankToCoordinates(pe, a, b, c, d, e, t);
}

int TopoManager::coordinatesToRank(int a, int b, int c, int d, int e, int t) {
  if( a<0 || a>=dimNA || b<0 || b>=dimNB || c<0 || c>=dimNC || d<0 || d>=dimND || e<0 || e>=dimNE || t<0 && t>=dimNT ) {
    printf("A coordinate is out of range\n");
    abort();
  }
  return bgqtm->coordinatesToRank(a, b, c, d, e, t);
}
#else
void TopoManager::rankToCoordinates(int pe, int &x, int &y, int &z, int &t) {
  if(pe < 0 || pe >= numPes) {
    printf("Processor number %d out of range\n", pe);
    abort();
  }
#if CMK_BLUEGENEL
  bgltm->rankToCoordinates(pe, x, y, z, t);
#elif CMK_BLUEGENEP
  bgptm->rankToCoordinates(pe, x, y, z, t);
#elif CMK_CRAYXT
  xttm->rankToCoordinates(pe, x, y, z, t);
#else
  if(dimNY > 1) {
    t = pe % dimNT;
    x = (pe % (dimNT*dimNX)) / dimNT;
    y = (pe % (dimNT*dimNX*dimNY)) / (dimNT*dimNX);
    z = pe / (dimNT*dimNX*dimNY);
  } else {
    t = pe % dimNT;
    x = (pe % (dimNT*dimNX)) / dimNT;
    y = 0;
    z = 0;
  }
#endif
}

int TopoManager::coordinatesToRank(int x, int y, int z, int t) {
  if(x<0 || x>=dimNX || y<0 || y>=dimNY || z<0 || z>=dimNZ || t<0 || t>=dimNT) {
    printf("A coordinate is out of range\n");
    abort();
  }

#if CMK_BLUEGENEL
  return bgltm->coordinatesToRank(x, y, z, t);
#elif CMK_BLUEGENEP
  return bgptm->coordinatesToRank(x, y, z, t);
#elif CMK_CRAYXT
  return xttm->coordinatesToRank(x, y, z, t);
#else
  if(dimNY > 1)
    return t + (x + (y + z*dimNY) * dimNX) * dimNT;
  else
    return t + x * dimNT;
#endif
}
#endif

int TopoManager::getHopsBetweenRanks(int pe1, int pe2) {
#if CMK_BLUEGENEQ
  int a1, b1, c1, d1, e1, t1, a2, b2, c2, d2, e2, t2;
  rankToCoordinates(pe1, a1, b1, c1, d1, e1, t1);
  rankToCoordinates(pe2, a2, b2, c2, d2, e2, t2);
  return (absTorus(a2-a1, dimNA, torusA) + absTorus(b2-b1, dimNB, torusB) + absTorus(c2-c1, dimNC, torusC) + absTorus(d2-d1, dimND, torusD) + absTorus(e2-e1, dimNE, torusE));
#else
  int x1, y1, z1, x2, y2, z2, t1, t2;
  rankToCoordinates(pe1, x1, y1, z1, t1);
  rankToCoordinates(pe2, x2, y2, z2, t2);
  return (absTorus(x2-x1, dimNX, torusX) + absTorus(y2-y1, dimNY, torusY) + absZ(z2-z1, dimNZ, torusZ));
#endif
}

void TopoManager::sortRanksByHops(int pe, int *pes, int *idx, int n) {
  for(int i=0; i<n; i++)
    idx[i] = i;
  quicksort(pe, pes, idx, 0, n-1);
}

int TopoManager::pickClosestRank(int mype, int *pes, int n){
  int minHops = getHopsBetweenRanks(mype, pes[0]);
  int minIdx=0;
  int nowHops; 
  for(int i=1; i<n; i++) {
    nowHops = getHopsBetweenRanks(mype, pes[i]);
    if(nowHops < minHops) {
      minHops = nowHops;
      minIdx=i;
    }
  }
  return minIdx;
}

int TopoManager::areNeighbors(int pe1, int pe2, int pe3, int distance) {
#if CMK_BLUEGENEQ
  int pe1_a, pe1_b, pe1_c, pe1_d, pe1_e, pe1_t;
  int pe2_a, pe2_b, pe2_c, pe2_d, pe2_e, pe2_t;
  int pe3_a, pe3_b, pe3_c, pe3_d, pe3_e, pe3_t;
  rankToCoordinates(pe1, pe1_a, pe1_b, pe1_c, pe1_d, pe1_e, pe1_t);
  rankToCoordinates(pe2, pe2_a, pe2_b, pe2_c, pe2_d, pe2_e, pe2_t);
  rankToCoordinates(pe3, pe3_a, pe3_b, pe3_c, pe3_d, pe3_e, pe3_t);

  if ( (absTorus((pe1_a - (pe2_a+pe3_a)/2), dimNA, torusA) + absTorus((pe1_b - (pe2_b+pe3_b)/2), dimNB, torusB) + absTorus((pe1_c - (pe2_c+pe3_c)/2), dimNC, torusC) + absTorus((pe1_d - (pe2_d+pe3_d)/2), dimND, torusD) + absTorus((pe1_e - (pe2_e+pe3_e)/2), dimNE, torusE) <= distance ) )
    return 1;
  else
    return 0;
#else
  int pe1_x, pe1_y, pe1_z, pe1_t;
  int pe2_x, pe2_y, pe2_z, pe2_t;
  int pe3_x, pe3_y, pe3_z, pe3_t;

  rankToCoordinates(pe1, pe1_x, pe1_y, pe1_z, pe1_t);
  rankToCoordinates(pe2, pe2_x, pe2_y, pe2_z, pe2_t);
  rankToCoordinates(pe3, pe3_x, pe3_y, pe3_z, pe3_t);

  if ( (absTorus(pe1_x - (pe2_x+pe3_x)/2, dimNX, torusX) + absTorus(pe1_y - (pe2_y+pe3_y)/2, dimNY, torusY) + absTorus(pe1_z - (pe2_z+pe3_z)/2, dimNZ, torusZ)) <= distance )
    return 1;
  else
    return 0;
#endif
}

void TopoManager::quicksort(int pe, int *pes, int *arr, int left, int right) {
  if(left<right) {
    int split = partition(pe, pes, arr, left, right);
    quicksort(pe, pes, arr, left, split);
    quicksort(pe, pes, arr, split+1, right);
  }
}

int TopoManager::partition(int pe, int *pes, int *idx, int left, int right) {
  int val = getHopsBetweenRanks(pe, pes[idx[(left+right)/2]]);
  int lm = left-1;
  int rm = right+1;
  for(;;) {
    do
      rm--;
    while(getHopsBetweenRanks(pe, pes[idx[rm]]) > val);
    do
      lm++;
    while(getHopsBetweenRanks(pe, pes[idx[lm]]) < val);
    if(lm < rm) {
      int tmp = idx[rm];
      idx[rm] = idx[lm];
      idx[lm] = tmp;
    }
    else
      return rm;
  }
}

