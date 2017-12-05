#!/bin/bash
cd `dirname $0`

PROTOC_BIN=protoc
PROTOC_PLUGIN="--plugin=protoc-gen-nanopb=third_lib/nanopb-0.3.8/generator/protoc-gen-nanopb"
NANOPB_OUT="--nanopb_out=."
NANOPB_IMPORT="-I. -Ithird_lib/nanopb-0.3.8/generator/proto"
PROTO_FILES=./nanopb/*.proto
for FILE in ${PROTO_FILES}
do
	${PROTOC_BIN} ${NANOPB_IMPORT} ${PROTOC_PLUGIN} ${NANOPB_OUT} ${FILE}
done
