# TreeModellingAndDynamicsSimulation

This Code implements the paper by FU Tian-shuanga, LI Yi-binga ,SHEN Dong-xub, titled, "Tree Modelling and Dynamics Simulation":
http://www.cse.iitd.ernet.in/~pkalra/col781/Papers/science-direct.pdf

The tree is modelled using Fractals and is adopted from the OpenGL website.

The code for viewing has been taken from OpenGL website: https://www.opengl.org/archives/resources/code/samples/glut_examples/contrib/agviewer.h, and https://www.opengl.org/archives/resources/code/samples/glut_examples/contrib/agviewer.c

Usage:
1. g++ tree.cpp -lGL -lGLU -lglut
2. ./a.out > treefile.txt
3. Open treefile.txt and add the number of lines at the top. e.g. If there are 200 lines, write 200 on the top of the file.
4. g++ readtree.cpp -lGL -lGL -lglut
5. ./a.out < treefile.txt
