#/bin/bash

## Example startup script for bnagen

GEN_ARGS="-w 2560 -h 1024 -i list -s 3366 -I 49000 -o bitmap.bmp"

if [ -e /usr/bin/bnagen ]; then
	bnagen $GEN_ARGS
	exit 0
fi

if [ -e bnagen ]; then
        ./bnagen $GEN_ARGS
        exit 0
fi

echo Fatal: bnagen not found on your machine.
exit 1
