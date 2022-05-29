.PHONY = clean

CC = g++
PROJECTNAME = fluid
MAINFILE = main.cpp

current_dir = &(shell pwd)

main : main.cpp
	@${CC} ${MAINFILE} -o ${PROJECTNAME}.exe -O1 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
	.\${PROJECTNAME}.exe


clean:
	@rm ${PROJECTNAME}.exe

