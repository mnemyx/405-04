#----------------------------------------------
#    Example Makefile for compiling a program that uses
#    the Matrix and Vector classes
#----------------------------------------------
#
#  Programmer: Donald House
#  Date: March 8, 1999
#
#  Copyright (C) - Donald H. House. 2005
#

CC      = g++
C	= cpp
H	= h
CFLAGS		= -g `Magick++-config --cppflags`
LFLAGS		= -g `Magick++-config --ldflags`

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lMagick++ -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -L /usr/lib64/ -lglut -lGL -lMagick++ -lm
  endif
endif

HFILES 	= Matrix.${H} Vector.${H} Utility.${H} camera.${H} Color.${H} Light.${H} Collision.${H} Ray.${H} Material.${H} Sphere.${H}
OFILES 	= Matrix.o Vector.o Utility.o camera.o Color.o Light.o Collision.o Ray.o Material.o Sphere.o
PROJECT = raytrace

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

Matrix.o: Matrix.${C} Matrix.${H} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Matrix.${C}

Vector.o: Vector.${C} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} $(CFLAGS) -c Utility.${C}
	
camera.o: camera.${C} camera.${H}
	${CC} $(CFLAGS) -c camera.${C}

Color.o: Color.${C} Color.${H}
	${CC} $(CFLAGS) -c Color.${C}
	
Light.o: Light.${C} Light.${H}
	${CC} $(CFLAGS) -c Light.${C}

Collision.o: Collision.${C} Collision.${H}
	${CC} $(CFLAGS) -c Collision.${C}

Ray.o: Ray.${C} Ray.${H}
	${CC} $(CFLAGS) -c Ray.${C}
	
Material.o: Material.${C} Material.${H}
	${CC} $(CFLAGS) -c Material.${C}
	
Sphere.o: Sphere.${C} Sphere.${H}
	${CC} $(CFLAGS) -c Sphere.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}
