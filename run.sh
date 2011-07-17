#!/bin/bash

#F = ""
F=`pwd`
echo $F

#if( $LD_LIBRARY_PATH == "")
 LD_LIBRARY_PATH=$F
 export LD_LIBRARY_PATH
 echo $LD_LIBRARY_PATH
#endif

exec ./kTest.bin
exit 1

