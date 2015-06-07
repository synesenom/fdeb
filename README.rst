====
fdeb
====
----------------------------------------------------
Force-directed edge bundling for undirected networks
----------------------------------------------------

:Author: Enys Mones
:Version: 0.1
:License: MIT

This program implements the force-directed edge bundling algorithm of Holten and van Wijk [1]_.


Requirements
============

OpenGL, which is already available on most systems.


Compilation
===========

Just run make.


Usage
=====

If no arguments are passed, the program prints the help menu that contains all available options.


Input files
===========

Two input files are required: a CSV file (with spaces as delimiters) containing three columns (node label, x coordinate, y coordinate) and a space-delimitered CSV file containing the edge source and target labels.


Test
====

The test directory contains a test file of the US airline network.


Output files
============

TODO


Bibliography
============

.. [1] Danny Holten and Jarke van Wijk: Force-Directed Edge Bundling for Graph
	   Visualization. *Computer Graphics Forum* (Blackwell Publishing Ltd) 28, no. 3 (2009): 983-990.