/** \file BGQTorus.h
 *  Author: Wei Jiang
 *  Date created: March 1st, 2012
 *
 */

#ifndef _BGQ_TORUS_H_
#define _BGQ_TORUS_H_

#if CMK_BLUEGENEQ

#include "spi/include/kernel/process.h"
#include "spi/include/kernel/location.h"
#include <firmware/include/personality.h>

class BGQTorusManager {
  private:
    Personality_t pers;
    int hw_NA;	// dimension of the allocation in A (no. of nodes, booted partition)
    int hw_NB;	// dimension of the allocation in B (no. of nodes, booted partition)
    int hw_NC;	// dimension of the allocation in C (no. of nodes, booted partition)
    int hw_ND;  // dimension of the allocation in D (no. of nodes, booted partition)  
    int hw_NE;  // dimension of the allocation in E (no. of nodes, booted partition)
    int hw_NT;  // dimension of processors per node.

    int rn_NA;  // acutal dimension of the allocation in A for the job
    int rn_NB;  // acutal dimension of the allocation in B for the job
    int rn_NC;  // acutal dimension of the allocation in C for the job
    int rn_ND;  // acutal dimension of the allocation in D for the job
    int rn_NE;  // acutal dimension of the allocation in E for the job

    int thdsPerProc;	// the number of threads per process (the value of +ppn)
    int procsPerNode;	// the number of processes per node
    int torus[5];
    int order[6], dims[6];
    char *mapping;	// temporarily only conside default mapping ABCDET

  public:
    BGQTorusManager(int _numPes) {
      order[0] = 5;
      order[1] = 4;
      order[2] = 3;
      order[3] = 2;
      order[4] = 1;
      order[5] = 0;

      Kernel_GetPersonality(&pers, sizeof(pers));

      hw_NA = pers.Network_Config.Anodes;
      hw_NB = pers.Network_Config.Bnodes;
      hw_NC = pers.Network_Config.Cnodes;
      hw_ND = pers.Network_Config.Dnodes;
      hw_NE = pers.Network_Config.Enodes;

      procsPerNode = Kernel_ProcessCount();
      thdsPerProc = 1;

      mapping = getenv("RANK_ORDER");
      if(mapping != NULL) {
	sscanf(mapping,"%d %d %d %d %d %d",&order[5],&order[4],&order[3],&order[2],&order[1],&order[0]);
      }
      //printf("Mapping %d %d %d %d %d %d\n",order[0],order[1],order[2],order[3],order[4],order[5]);
      hw_NT = procsPerNode*thdsPerProc;

      rn_NA = hw_NA;
      rn_NB = hw_NB;
      rn_NC = hw_NC;
      rn_ND = hw_ND;
      rn_NE = hw_NE;

      int max_t = 0;
      if(rn_NA * rn_NB * rn_NC * rn_ND * rn_NE != _numPes/hw_NT) {
	rn_NA = rn_NB = rn_NC = rn_ND = rn_NE = 0;
	int rn_NT=0;
	int min_a, min_b, min_c, min_d, min_e, min_t;
	min_a = min_b = min_c = min_d = min_e = min_t = (~(-1));
	int tmp_t, tmp_a, tmp_b, tmp_c, tmp_d, tmp_e;
	uint64_t numentries;
	BG_CoordinateMapping_t *coord;

	int nranks = _numPes/thdsPerProc;
	coord = (BG_CoordinateMapping_t *) malloc(sizeof(BG_CoordinateMapping_t)*nranks);
	Kernel_RanksToCoords(sizeof(BG_CoordinateMapping_t)*nranks, coord, &numentries);

	for(int c = 0; c < nranks; c++) {
	  tmp_a = coord[c].a;
	  tmp_b = coord[c].b;
	  tmp_c = coord[c].c;
	  tmp_d = coord[c].d;
	  tmp_e = coord[c].e;
	  tmp_t = coord[c].t;


	  if(tmp_a > rn_NA) rn_NA = tmp_a;
	  if(tmp_a < min_a) min_a = tmp_a;
	  if(tmp_b > rn_NB) rn_NB = tmp_b;
	  if(tmp_b < min_b) min_b = tmp_b;
	  if(tmp_c > rn_NC) rn_NC = tmp_c;
	  if(tmp_c < min_c) min_c = tmp_c;
	  if(tmp_d > rn_ND) rn_ND = tmp_d;
	  if(tmp_d < min_d) min_d = tmp_d;
	  if(tmp_e > rn_NE) rn_NE = tmp_e;
	  if(tmp_e < min_e) min_e = tmp_e;
	  if(tmp_t > rn_NT) rn_NT = tmp_t;
	  if(tmp_t < min_t) min_t = tmp_t;
	}
	rn_NA = rn_NA - min_a + 1;
	rn_NB = rn_NB - min_b + 1;
	rn_NC = rn_NC - min_c + 1;
	rn_ND = rn_ND - min_d + 1;
	rn_NE = rn_NE - min_e + 1;
	procsPerNode = rn_NT - min_t + 1;
	hw_NT = procsPerNode * thdsPerProc;
      }

      dims[0] = rn_NA;
      dims[1] = rn_NB;
      dims[2] = rn_NC;
      dims[3] = rn_ND;
      dims[4] = rn_NE;
      dims[5] = hw_NT;

      torus[0] = ((rn_NA % 4) == 0)? true:false;
      torus[1] = ((rn_NB % 4) == 0)? true:false;
      torus[2] = ((rn_NC % 4) == 0)? true:false;
      torus[3] = ((rn_ND % 4) == 0)? true:false;
      torus[4] = true;

    }

    ~BGQTorusManager() {
    }

    inline int getDimNA() { return rn_NA; }
    inline int getDimNB() { return rn_NB; }
    inline int getDimNC() { return rn_NC; }
    inline int getDimND() { return rn_ND; }
    inline int getDimNE() { return rn_NE; }
    inline int getDimNT() { return hw_NT; }

    inline int getProcsPerNode() { return procsPerNode; }
    inline int* isTorus() { return torus; }

    inline void rankToCoordinates(int pe, int &a, int &b, int &c, int &d, int &e, int &t) {
      int tempdims[6];

      tempdims[order[0]] = pe % dims[order[0]];
      tempdims[order[1]] = (pe / dims[order[0]]) % dims[order[1]];
      tempdims[order[2]] = (pe / (dims[order[0]]*dims[order[1]])) % dims[order[2]];
      tempdims[order[3]] = (pe / (dims[order[0]]*dims[order[1]]*dims[order[2]])) % dims[order[3]];
      tempdims[order[4]] = (pe / (dims[order[0]]*dims[order[1]]*dims[order[2]]*dims[order[3]])) % dims[order[4]];
      tempdims[order[5]] = (pe / (dims[order[0]]*dims[order[1]]*dims[order[2]]*dims[order[3]]*dims[order[4]])) % dims[order[5]];

      a = tempdims[0];
      b = tempdims[1];
      c = tempdims[2];
      d = tempdims[3];
      e = tempdims[4];
      t = tempdims[5];

      /*t = pe % (thdsPerProc*procsPerNode);
	e = pe / (thdsPerProc*procsPerNode) % rn_NE;
	d = pe / (thdsPerProc*procsPerNode*rn_NE) % (rn_ND);
	c = pe / (thdsPerProc*procsPerNode*rn_NE*rn_ND) % (rn_NC);
	b = pe / (thdsPerProc*procsPerNode*rn_NE*rn_ND*rn_NC) % (rn_NB);
	a = pe / (thdsPerProc*procsPerNode*rn_NE*rn_ND*rn_NC*rn_NB);
	*/
    }

    inline int coordinatesToRank(int a, int b, int c, int d, int e, int t) {
      int pe;
      int tempdims[6];
      tempdims[0] = a;
      tempdims[1] = b;
      tempdims[2] = c;
      tempdims[3] = d;
      tempdims[4] = e;
      tempdims[5] = t;

      pe = 0;

      pe += tempdims[order[0]];
      pe += (tempdims[order[1]]*dims[order[0]]);
      pe += (tempdims[order[2]]*dims[order[0]]*dims[order[1]]);
      pe += (tempdims[order[3]]*dims[order[0]]*dims[order[1]]*dims[order[2]]);
      pe += (tempdims[order[4]]*dims[order[0]]*dims[order[1]]*dims[order[2]]*dims[order[3]]);
      pe += (tempdims[order[5]]*dims[order[0]]*dims[order[1]]*dims[order[2]]*dims[order[3]]*dims[order[4]]);

      /*
	 int a_mult = rn_NB * rn_NC * rn_ND * rn_NE;
	 int b_mult = rn_NC * rn_ND * rn_NE;
	 int c_mult = rn_ND * rn_NE;
	 int d_mult = rn_NE;

	 pe = (a * a_mult + b * b_mult + c * c_mult + d * d_mult + e) * thdsPerProc * procsPerNode + t;
	 */

      return pe;
    }

    inline int getTotalPhyNodes() {
      return rn_NA * rn_NB * rn_NC * rn_ND * rn_NE;
    }

    inline int getMyPhyNodeID(int pe) {
      int a, b, c, d, e, t;
      rankToCoordinates(pe, a, b, c, d, e, t);
      return a * rn_NB * rn_NC * rn_ND * rn_NE + b * rn_NC * rn_ND * rn_NE + c * rn_ND * rn_NE + d * rn_NE + e;
    }

};

#endif // CMK_BLUEGENEQ
#endif //_BGQ_TORUS_H_
