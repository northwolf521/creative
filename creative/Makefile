.PHONY : all clean nanopb
SHELL:=/bin/bash
all:
	make -C nanopb
	make -C conn
	make -C game
	make -C agent
clean:
	make -C nanopb clean
	make -C conn clean
	make -C game clean
	make -C agent clean
nanopb:
	sh ${PWD}/nanopb.sh
	make -C nanopb
