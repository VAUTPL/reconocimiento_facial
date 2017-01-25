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

//include opencv core
#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/opencv.hpp"

//file handling
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;
using namespace cv::face;

//Se declara en la variable la ruta del clasificador
string classifier = "/home/agiladalton/Downloads/reconocimiento_facial/clasificador/haarcascade_frontalface_default.xml";

//Se declara en la variable la ruta del archivo antrenado
string entrenado = "/home/agiladalton/Downloads/reconocimiento_facial/yml/eigenface.yml";

//Se declara en la variable la ruta
string rutaRostros = "/home/agiladalton/Downloads/reconocimiento_facial/rostros/";

//Se crea un archivo de nuevos registros
void crearArchivoRegistro(string nombrePersona) {
	char cadena[128];
   	// Crea un fichero de salida
   	ofstream fs("registro.txt", fstream::app); 

   	// Enviamos una cadena al fichero de salida:
   	fs << nombrePersona << endl;
   	// Cerrar el fichero, 
   	// para luego poder abrirlo para lectura:
   	fs.close();
}

//Se realiza lectura de registros
vector<string> leerArchivoRegistro() {
	vector<string> listaNombres;
	char cadena[128];
	// Crea un fichero de entrada
   	ifstream fe("registro.txt");

   	while(!fe.eof()) {
   		//Realizamos lectura de la cadena
      	fe >> cadena;
      	listaNombres.push_back(cadena);
   	}
   	//SE cierra el fichero
   	fe.close();

   	return listaNombres;
}

//Metodo para relacionar identificador con el nombre de la persona
static void relacionImagenEtiqueta(vector<string> listaNombres, vector<Mat>& images, vector<int>& labels){
	for (int i = 0; i < listaNombres.size() - 1; i++) {
    	for (int j = 0; j < 50; j++) {
    		images.push_back(imread(rutaRostros + listaNombres[i] + "_" + std::to_string(j) + ".png", IMREAD_GRAYSCALE));
			for (int k = 0; k < listaNombres.size() - 1; k++) {
				if (listaNombres[i].find(listaNombres[k]) != string::npos)
				{
					labels.push_back(k);
					break;
				}
			}
    	}
	}
}	

//Metodo para entrenar el reconocimiento facial mediante el algoritmo eigenface
void eigenFaceTrainer(){
	vector<Mat> images; //Variable para almacenar imagenes
	vector<Mat> listaPersonas; //Variable para almacenar rostros
	vector<int> labels; //Variable para almacenar etiquetas
	vector<int> labelsNuevas; //Variable para almacenar etiquetas

	//Creando clase para usar algoritmo de eigenface
	Ptr<FaceRecognizer>  model = createEigenFaceRecognizer();
	CascadeClassifier face_cascade;
	vector<Rect> faces;
	Mat graySacleFrame;
	Mat original;

	try {
		//Se cargar archivo con rutas de imagenes
		string filename = "csv.ext";
		relacionImagenEtiqueta(leerArchivoRegistro(), images, labels);
		cout << "Iniciando entrenamiento...." << endl;
	} catch (cv::Exception& e){
		cerr << "Error abriendo el archivo: " << e.msg << endl;
		exit(1);
	}

	if (!face_cascade.load(classifier)){
		cout << "Error al cargar el archivo de clasificador." << endl;
		return;
	}

	//Se recorre las imagenes utilizadas para el entrenamiento
	for (int i = 0; i < images.size(); ++i) {
		cout << "Procesando imagen: " << i << endl;
		//Se verica que no este vacia la imagen
		if (!images[i].empty()) {
			//Se clona el frame original
			original = images[i].clone();

			//Detecta el rostro en escala de gris
			face_cascade.detectMultiScale(original, faces, 1.1, 3, 0|CASCADE_FIND_BIGGEST_OBJECT, Size(20, 20) );

			//cv::Mat roiImg;
			if (faces.size() > 0) {
				//Obtengo la region de interes
				Rect face_i = faces[0];

				//Corta la parte desde la imagen en escala de gris
				Mat face = images[i](faces[0]);

				//Cambiar el tamaño de la imagen recortada para ajustarse a los tamaños de imagen de la base de datos
				Mat face_resized;
				cv::resize(face, face_resized, Size(20, 20), 1.0, 1.0, INTER_CUBIC);

				listaPersonas.push_back(face_resized);
				labelsNuevas.push_back(labels[i]);
			} else {
				cout << "No se ha detectado ningun rostro en la imagen " << i <<  endl;
			}
		}
	}

	//Se entrena con la informacion obtenida
	model->train(listaPersonas, labelsNuevas);

	//Se guarda el entrenamiento
	model->save(entrenado);

	cout << "Entrenamiento finalizado...." << endl;
}

//Metodo para deteccion de rostros mediante la camara
void aplicarReconocimiento(int camara){
	vector<string> listaNombres = leerArchivoRegistro();
	cout << "Iniciando reconocimiento..." << endl;

	//Carga el archivo ya entrenado
	Ptr<FaceRecognizer>  model = createFisherFaceRecognizer();
	model->load(entrenado);


	CascadeClassifier face_cascade;
	string window = "Capturar - Detección de rostros";

	//Se carga clasificador
	if (!face_cascade.load(classifier)){
		cout << "Error al cargar el archivo de clasificador." << endl;
		return;
	}

	VideoCapture cap(camara); //Se establece que camara se va ha utilizar

	//Se verifica que la camara se encuentre disponible
	if (!cap.isOpened())
		return;

	namedWindow(window, 1);
	long count = 0;

	while (true) {
		vector<Rect> faces;
		Mat frame;
		Mat graySacleFrame;
		Mat original;

		cap >> frame;
		count++; // Se va contando los frames que se va capturando

		if (!frame.empty()) {
			//Clona desde el frame original
			original = frame.clone();

			//Convierte la imagen a escala de grises
			cvtColor(original, graySacleFrame, CV_BGR2GRAY);

			//Detecta el rostro en escala de gris
			face_cascade.detectMultiScale( graySacleFrame, faces, 1.1, 3, 0|CASCADE_FIND_BIGGEST_OBJECT, Size(20, 20) );

			//Se declara variable de imagenes detectadas
			std::string frameset = std::to_string(count); 

			double confidence = 0;

			//Recorro la lista de rostros obtenidos
			for (int i = 0; i < faces.size(); i++) {
				//Obtengo la region de interes
				Rect face_i = faces[i];

				//Corta la parte desde la imagen en escala de gris
				Mat face = graySacleFrame(face_i);

				//Cambiar el tamaño de la imagen recortada para ajustarse a los tamaños de imagen de la base de datos
				Mat face_resized;
				cv::resize(face, face_resized, Size(20, 20), 1.0, 1.0, INTER_CUBIC);

				//Reconoce que rostros han sido detectados
				int label = -1; 
				model->predict(face_resized, label, confidence);

				//Dibuja un rectangulo verde en donde se ha reconocido el rostro
				rectangle(original, face_i, CV_RGB(0, 255, 0), 1);

				int porcentaje = 0;

				porcentaje = 100 - ((int) ((confidence / 1200) * 100));

				string nombrePersona = "Desconocido";

				if (porcentaje > 30)
				{
					nombrePersona = listaNombres[label];
				}

				//Se declara variables para posicionar las etiquetas
				int pos_x = std::max(face_i.tl().x - 10, 0);
				int pos_y = std::max(face_i.tl().y - 10, 0);

				//Nombra a la persona que esta en la imagen
				putText(original, nombrePersona, Point(pos_x, pos_y), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);
			}
			
			//Se muestra en la ventana
			cv::imshow(window, original);
		}

		//Condicion para terminar el reconocimiento mediante la tecla ESC
		if (waitKey(30) == 27) break;
	}
}

//Metodo para capturar rostro
void capturarRostro(int camara, string nombrePersona) {
	cv::VideoCapture TheVideoCapturer; //Se declara variable para acceder a la camara
	cv::Mat bgrMap; //Se declara varialbe para almacenar imagenes

  	int numSnapshot = 0;
  	std::string snapshotFilename = "0"; //Nombre del archivo

  	TheVideoCapturer.open(camara); //Se asigna camara a ocupar

  	if (!TheVideoCapturer.isOpened()) { //Se verifica que la camara este disponible
    	std::cerr<<"No puede abrir el video para realizar la captura."<<std::endl;
    	return;
  	}

  	int count = 0;

  	//Condicion para capturar imagenes con el limite de 50
  	while (numSnapshot < 50 && TheVideoCapturer.grab()) {
    	TheVideoCapturer.retrieve(bgrMap);

    	cv::imshow("BGR image", bgrMap); //Se muestra captura de camara

    	//Valida cada cuantos frame se captura la imagen
    	if(count % 5 == 0 && count > 100) {
        	cv::imwrite(rutaRostros + nombrePersona + "_" + snapshotFilename + ".png", bgrMap); //Se crea el archivo
        	numSnapshot++;
        	snapshotFilename = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnapshot))->str();
    	}
    	
    	count++;
    	if (waitKey(30) == 27) break;
  	}
  	
  	cout << "Termino captura de rostro." << endl;
}
