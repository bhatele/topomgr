# ==============================================================================
# Common Variables

# ==============================================================================
# Blue Gene/P
CC      = bgxlc
CXX     = bgxlC
COPTS   = -c -O3 -I/bgsys/drivers/ppcfloor/comm/include/
LOPTS   =

# ==============================================================================
# Cray XT5 (Jaguar, Kraken) Cray XE6
#CC      = cc
#CXX     = CC
#COPTS   = -c -O3 -DCMK_CRAYXT=1
#LOPTS   = -lrca

all: libtopomgr.a

libtopomgr.a: TopoManager.o CrayNid.o
	ar -q libtopomgr.a TopoManager.o CrayNid.o

TopoManager.o: TopoManager.h TopoManager.C BGLTorus.h BGPTorus.h BGQTorus.h XTTorus.h
	$(CXX) $(COPTS) -o TopoManager.o TopoManager.C

CrayNid.o: CrayNid.c
	$(CC) $(COPTS) -o CrayNid.o CrayNid.c

test: test.C
	$(CXX) $(COPTS) -o test.o test.C
	$(CXX) -o test test.o libtopomgr.a $(LOPTS)

clean:
	rm -f *.o libtopomgr.a test

