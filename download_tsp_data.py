#!/usr/bin/env python3

import os
import urllib.request

# if you change this, be sure to add it to .gitignore (if you don't want it to be added to the repo).
TSP_DIRECTORY = "input"

os.system("mkdir -p " + TSP_DIRECTORY)

urllib.request.urlretrieve("http://www.math.uwaterloo.ca/tsp/data/ml/mona-lisa100K.tsp", TSP_DIRECTORY + "/monalisa100K.tsp")
urllib.request.urlretrieve("http://www.math.uwaterloo.ca/tsp/data/ml/tour/monalisa_5757191.tour", TSP_DIRECTORY + "/monalisa100K_5757191.tour")


