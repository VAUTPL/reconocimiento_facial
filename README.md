Facial Recognition
====================
This project involved an investigation in face recognition C++ and OpenCV 3.2 API. The code is free to be used and modified by anyone wishing to do so.
It was designed with the purpose of of identifying or verifying a person from a digital image or a video frame from a video source.

#UTPL
###Professor:
- Rodrigo Barba [lrbarba@utpl.edu.ec](mailto:lrbarba@utpl.edu.ec)

###Students:
- Dalton Agila [daagila1@utpl.edu.ec](daagila1@utpl.edu.ec)
- Diego Peña [dfpena2@utpl.edu.ec](depenia@utpl.edu.ec)


System Requirements
-------------------
	•	An i3 or better processor. The faster the better, especially at high video resolutions.
	•	2 GB or more RAM.
	•	At least 100 MB Free Disk space Ubuntu 16.04 or later.

Installation
-------------
	1.	First, one should install the following libraries:
	◦	OpenCV version 3.2 from https://github.com/opencv/opencv and OpenCV Extra Modules from https://github.com/opencv/opencv_contrib, following the respective instructions for the installation of the extra modules.
	2.	Now download and extract this repository with one of several options:
	◦	Clone the repository with $ git clone https://github.com/agiladalton/reconocimiento_facial
	◦	Download the repository as a .zip or .tar.gz and then extract it.

Running
-------
From a command line in the folder of the repository:
First you have to compile main.cpp  
`$ g++ -std=c++11 -ggdb `pkg-config --cflags opencv` -o `basename main.cpp .cpp` main.cpp `pkg-config --libs opencv`

Then you can run main

`$ ./main

You can selected option of menu.

***** MENU *****
1. Cambiar camara
2. Agregar nuevo rostro
3. Entrenar
4. Iniciar reconocimiento
5. Salir
