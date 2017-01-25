/*
##########################################
# Universidad Tecnica Particular de Loja #
##########################################
# Professor:
# Rodrigo Barba		lrbarba@utpl.edu.ec  #
##########################################
# Students:
# Dalton Agila		daagila1@utpl.edu.ec #
# Diego Peña		dfpena2@utpl.edu.ec  #
##########################################
*/

#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/opencv.hpp"

#include "FaceRec.h"
//#include "VideoCap.h"

using namespace std;
using namespace cv;
using namespace cv::face;

int main()
{
	int opcion;
	int camara = 0;
	string nombrePersona;
	do {
		cout << "***** MENU *****" << endl;
		cout << "1. Cambiar camara" << endl;
		cout << "2. Agregar nuevo rostro" << endl;
		cout << "3. Entrenar" << endl;
		cout << "4. Iniciar reconocimiento" << endl;
		cout << "5. Salir" << endl << endl;
		cout << "Ingresar opcion: ";
		
		cin >> opcion;

		switch (opcion) {
			case 1:
				cout << "Ingresar camara: ";
				cin >> camara;
				break;
			case 2:
				cout << "Nombre de la persona: ";
				cin >> nombrePersona;
				
				crearArchivoRegistro(nombrePersona);
				capturarRostro(camara, nombrePersona);
				break;
			case 3:
				eigenFaceTrainer();
				break;
			case 4:
				aplicarReconocimiento(camara);
				break;
			default:
				return 0;
		}
	} while (opcion != 5);

	system("pause");
	return 0;
}