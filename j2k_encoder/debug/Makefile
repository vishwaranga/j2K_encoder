-include src/src.mk
 
LIBS :=
LD_FLAGS := -g -Wall ${LIBS}
EXEC := main
#EXECF := TextGenerator


 
#All Target
all: ${EXEC}
 
${EXEC} : ${OBJS}
	@echo 'Building target: $@'
	${CC} ${LD_FLAGS} -o ${EXEC} ${OBJS}
	@echo 'Finished building target: $@'
	@echo ' '
 
clean :
	-rm -rf ${EXEC} ${OBJS}
	-echo ' '

files :
	#g++ ../src/TextGenerator.cpp
	./a.out
