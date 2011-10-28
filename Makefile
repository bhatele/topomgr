# ==============================================================================
# Common Variables

# ==============================================================================
# Blue Gene/L

# ==============================================================================
# Blue Gene/P
CC      = mpicc
CXX     = mpicxx
COPTS   = -c -O3
LOPTS   =

# ==============================================================================
# Cray XT3 (BigBen)
#CC      = cc
#CXX     = CC
#COPTS   = -c -O3 -DCMK_CRAYXT=1 -DXT3_TOPOLOGY=1 #-DCRAYNBORTABLE=`pwd`/CrayNeighborTable
#LOPTS   = 

# ==============================================================================
# Cray XT4/5 (Jaguar, Kraken) Cray XE6
#CC      = cc
#CXX     = CC
#COPTS   = -c -O3 -DCMK_CRAYXT=1 -DXT5_TOPOLOGY=1
#LOPTS   = -lrca

all: libtopomgr.a

libtopomgr.a: TopoManager.o CrayNid.o
	ar -q libtopomgr.a TopoManager.o CrayNid.o

TopoManager.o: TopoManager.h TopoManager.C BGLTorus.h BGPTorus.h XT3Torus.h XTTorus.h
	$(CXX) $(COPTS) -o TopoManager.o TopoManager.C

CrayNid.o: CrayNid.c 
	$(CC) $(COPTS) -o CrayNid.o CrayNid.c

test: test.C
	$(CXX) $(COPTS) -o test.o test.C
	$(CXX) -o test test.o libtopomgr.a $(LOPTS)

clean:
	rm -f *.o libtopomgr.a test

