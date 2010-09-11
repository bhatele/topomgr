# ==============================================================================
# Common Variables

# ==============================================================================
# Blue Gene/L

# ==============================================================================
# Blue Gene/P
CC      = mpixlc
CXX     = mpixlcxx
COPTS   = -c -O3 -DCMK_BLUEGENEP=1
LOPTS   =

# ==============================================================================
# Cray XT3 (BigBen)
#CC      = cc
#CXX     = CC
#COPTS   = -c -O3 -DCMK_CRAYXT -DXT3_TOPOLOGY=1 #-DCRAYNBORTABLE=`pwd`/CrayNeighborTable
#LOPTS   = 

# ==============================================================================
# Cray XT4/5 (Jaguar, Kraken)
#CC      = cc
#CXX     = CC
#COPTS   = -c -O3 -DCMK_CRAYXT -DXT5_TOPOLOGY=1
#LOPTS   = -lrca

all: libtmgr.a

libtmgr.a: TopoManager.o CrayNid.o
	ar -q libtmgr.a TopoManager.o CrayNid.o

TopoManager.o: TopoManager.h TopoManager.C BGLTorus.h BGPTorus.h XT3Torus.h XT4Torus.h
	$(CXX) $(COPTS) -o TopoManager.o TopoManager.C

CrayNid.o: CrayNid.c 
	$(CC) $(COPTS) -o CrayNid.o CrayNid.c

test: test.C
	$(CXX) $(COPTS) -o test.o test.C
	$(CXX) -o test test.o libtmgr.a $(LOPTS)

clean:
	rm -f *.o libtmgr.a test

