NAME = main
INCLUDES =
SOURCES = ${NAME}.c
OBJFILES = ${NAME}.o
EXE = ${NAME}
MSG = ${msg}
.SUFFIXES : .c .o
${OBJFILES}: ${SOURCES} ${INCLUDES}
.c.o:
    gcc -g -o ${EXE} ${OBJFILES}
run: ${EXE}
     ./${EXE}
     
clean:
    rm ${OBJFILES} ${EXE}
