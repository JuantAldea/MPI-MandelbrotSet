#!/bin/bash

mpirun -np 4 ./mandelbrot-mpi 4096 1000
otfprofile mandelbrot-mpi.otf
pdflatex result.tex
rm *.events*

