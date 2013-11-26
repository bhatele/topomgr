# ==============================================================================
# Common Variables

# ==============================================================================
# Blue Gene/P
#CC      = bgxlc
#CXX     = bgxlC
#COPTS   = -c -O3 -I/bgsys/drivers/ppcfloor/comm/include/
#LOPTS   =

# ==============================================================================
# Blue Gene/Q
CC      = bgxlc
CXX     = bgxlC
MPICXX	= mpixlcxx
COPTS   = -c -O3
INCL	= -I/bgsys/drivers/ppcfloor/ -I/bgsys/drivers/ppcfloor/spi/include/kernel/cnk/
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
	$(CXX) $(COPTS) $(INCL) -o TopoManager.o TopoManager.C

CrayNid.o: CrayNid.c
	$(CC) $(COPTS) $(INCL) -o CrayNid.o CrayNid.c

test: test.C
	$(MPICXX) $(COPTS) -o test.o test.C
	$(MPICXX) -o test test.o libtopomgr.a $(LOPTS)

clean:
	rm -f *.o libtopomgr.a test

