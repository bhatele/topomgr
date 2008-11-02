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
#COPTS   = -c -O3 -DXT3_TOPOLOGY=1 -DCRAYNBORTABLE=`pwd`/CrayNeighborTable
#LOPTS   = 

# ======================================================================================
# Cray XT4 (Jaguar)
#CC      = cc
#CXX     = CC
#COPTS   = -c -O3 -DXT4_TOPOLOGY=1
#LOPTS   = -lrca

all: libtmgr.a test

libtmgr.a: TopoManager.o CrayNid.o
	ar -q libtmgr.a TopoManager.o CrayNid.o

TopoManager.o: TopoManager.h TopoManager.C BGLTorus.h BGPTorus.h XT3Torus.h XT4Torus.h
	$(CXX) $(COPTS) -o TopoManager.o TopoManager.C

CrayNid.o: CrayNid.c 
	$(CC) $(COPTS) -o CrayNid.o CrayNid.c

clean:
	rm -f *.o libtmgr.a test

test: test.C
	$(CXX) $(COPTS) -o test.o test.C
	$(CXX) -o test test.o libtmgr.a $(LOPTS)
