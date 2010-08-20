# ======================================================================================
# Common Variables

# ======================================================================================
# Blue Gene/L

# ======================================================================================
# Blue Gene/P
CC      = mpixlc
CXX     = mpixlcxx
COPTS   = -c -O3 -DCMK_BLUEGENEP=1
LOPTS   =

# ======================================================================================
# Cray XT3 (BigBen)
#CC      = cc
#CXX     = CC
#COPTS   = -c -O3 -DCMK_CRAYXT -DXT3_TOPOLOGY=1 #-DCRAYNBORTABLE=`pwd`/CrayNeighborTable
#LOPTS   = 

# ======================================================================================
# Cray XT4/5 (Jaguar, Kraken)
#CC      = cc
#CXX     = CC
#COPTS   = -c -O3 -DCMK_CRAYXT -DXT5_TOPOLOGY=1
#LOPTS   = -lrca -lhpm 

all: libtmgr.a partial flow

libtmgr.a: TopoManager.o CrayNid.o
	ar -q libtmgr.a TopoManager.o CrayNid.o

TopoManager.o: TopoManager.h TopoManager.C BGLTorus.h BGPTorus.h XT3Torus.h XT4Torus.h
	$(CXX) $(COPTS) -o TopoManager.o TopoManager.C

CrayNid.o: CrayNid.c 
	$(CC) $(COPTS) -o CrayNid.o CrayNid.c

clean:
	rm -f *.o libtmgr.a bandwidth

bandwidthX: bandwidth.C
	$(CXX) $(COPTS) -o bandwidth.o bandwidth.C
	$(CXX) -o bandwidth bandwidth.o libtmgr.a -lhpm -L. $(LOPTS)

full: full_overlap.C
	$(CXX) $(COPTS) -o full.o full_overlap.C
	$(CXX) -o full full.o libtmgr.a -lhpm -L. $(LOPTS)

partial: partial_overlap.C
	$(CXX) $(COPTS) -o partial.o partial_overlap.C
	$(CXX) -o partial partial.o libtmgr.a -lhpm -L. $(LOPTS)

flow: flow.C 
	$(CXX) $(COPTS) -o flow.o flow.C
	$(CXX) -o flow flow.o libtmgr.a -lhpm -L. $(LOPTS)

test: test.C
	$(CXX) $(COPTS) -o test.o test.C
	$(CXX) -o test test.o libtmgr.a $(LOPTS)
