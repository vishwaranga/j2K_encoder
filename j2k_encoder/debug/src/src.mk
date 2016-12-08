CC := g++
HEADER := ../header/
CC_FLAGS := -g -Wall -O -I ${HEADER}
OBJS += \
	./src/File_Format.o \
	./src/Quantizer.o \
	./src/Wavelet2D.o \
	./src/EbcotCoder.o \
	./src/MQcoder.o \
	./src/main.o

# Define the objects relative to project path

src/%.o:../src/%.cpp
	@echo 'Building File: $<'
	@echo 'Invoking: ${CC} compiler'
	${CC} ${CC_FLAGS} -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
