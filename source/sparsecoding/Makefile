#include makefiles/defaults.mk
include makefiles/opencv.mk


#  files
FILES += \
	main.cpp \
    core/stdafx.cpp \
    core/Logger.cpp \
	graphics/Visualizer.cpp \
	sparsecoding/SparseCoder.cpp \
	sparsecoding/conversion.cpp \
	sparsecoding/dct.cpp \

default: main

main: $(FILES)
	clang++ -o main $(FILES) $(LIBS) $(INCS) $(OPTS)

clean:
	rm make
