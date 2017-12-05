#!/bin/bash
PROTOC_BIN=protoc
PROTOC_PLUGIN="--plugin=protoc-gen-nanopb=../generator/protoc-gen-nanopb"
NANOPB_OUT="--nanopb_out=."
NANOPB_IMPORT="-I. -I../generator/proto"
PROTO_FILES=s2s.proto
for FILE in ${PROTO_FILES}
do
	${PROTOC_BIN} ${NANOPB_IMPORT} ${PROTOC_PLUGIN} ${NANOPB_OUT} ${FILE}
done
