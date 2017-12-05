#!/bin/bash
cd `dirname $0`

NANOPB_DIR=$PWD
cd third_lib/nanopb-0.3.8/generator/proto
make
cd $NANOPB_DIR 

FRAME_DIR=$PWD
cd third_lib/libframe-0.0.1/
rm lib/* -rf
make clean && make
mv libframe.a lib
cd $FRAME_DIR

chmod a+x third_lib/nanopb-0.3.8/generator/protoc-gen-nanopb
chmod a+x third_lib/nanopb-0.3.8/generator/nanopb_generator.py
make nanopb
make all
