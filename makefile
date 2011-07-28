.PHONY: all build_libs build_game clean_libs clean_game clean_all clean

all: build_game
clean_all: clean_game
clean: clean_game

build_libs:
	make -C 3libs/noisepp -f Makefile all
	make -C 3libs/libpspmath -f Makefile all
	make -C 3libs/pthreads-emb-1.0/platform/psp -f Makefile all
	
build_game:
	make -f MakefileGame all
	
clean_libs:
	make -C 3libs/noisepp -f Makefile clean
	make -C 3libs/libpspmath -f Makefile clean
	make -C 3libs/pthreads-emb-1.0/platform/psp -f Makefile clean
	
clean_game:
	make -f MakefileGame clean