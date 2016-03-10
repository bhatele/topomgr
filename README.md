TopoMgr v1.1
============

The Topology Manager Library

The TopoManager API provides runtime information about the allocated job
partition on IBM Blue Gene and Cray XT/XE machines. For documentation, read
"TopoManager.h". This is the only file which needs to interface with the
application.


### Build

```bash
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../BGP-gnu.cmake ..
make
make install
```

### Reference

Any published work which utilizes this API should include the following
reference:
```
"Abhinav Bhatele, Eric Bohm, Laxmikant V. Kale, Optimizing communication for
Charm++ applications by reducing network contention, Concurrency and
Computation: Practice and Experience (EuroPar special issue), Vol: 23, Issue:
2, Pages: 211-222, 2011"
```

### Release

Copyright (c) 2008, Parallel Programming Laboratory. All rights reserved.

Written by:
```
    Abhinav Bhatele <bhatele@llnl.gov>
```

