/*
* main.cpp
* 
* VARPA, University of Coruña
* Mondejar Guerra, Victor M.
* 29 Jun 2017
*/

#include "ECG.h"

// Parameters for the classifier
std::string svm_model_name = "../svm_models/..."; // full-path to svm model 
int w_l = 90;// Window Left of the beat centered on R-peak
int w_r = 90;// Window Right of the beat centered on R-peak


/// Load file .csv and push the data into std::vector ecg
bool load_signal(std::vector<float> &ecg, float &fs, float &minA, float &maxA, 
				 int &n_bits, std::string filename)
{
	//Check that filename exists
	std::ifstream file ( filename.c_str() ); 

	if(file.is_open())
	{
		std::string value;

		getline(file, value); 
		fs = atof(value.c_str());

		getline(file, value); 
		minA = atof(value.c_str());

		getline(file, value); 
		maxA = atof(value.c_str());

		getline(file, value); 
		n_bits = atof(value.c_str());

		while ( file.good() )
		{
			getline(file, value); 
			ecg.push_back( atof(value.c_str()) );
		}
		return true;
	}
	else
		return false;
}


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout<< "Error: a file that contains the ecg signal is required!\n\
					 Example of use: main_GU ../data/220.csv" << std::endl;
		return 0;	
	}

	std::vector<float> ecg;
	float fs, minA, maxA;
	int n_bits;
	// Load signal given by the .csv file 
	if(load_signal(ecg, fs, minA, maxA, n_bits, argv[1]))
	{
		std::string output_filename = std::string(argv[1]) + ".out";

		std::cout<< " fs readed from csv: = "<< fs << std::endl;
		std::cout<< " min Amp readed from csv: = "<< minA << std::endl;
		std::cout<< " max Amp readed from csv: = "<< maxA << std::endl;
		std::cout<< " n_bits readed from csv: = "<< n_bits << std::endl;
		std::cout<< " Signal size from csv: = "<< ecg.size() << std::endl;

		std::vector<std::vector<int> > output;
		ECG* ecg_classifier = new ECG(svm_model_name, w_l, w_r, true, false);

		// Run classifier 
		ecg_classifier->predict_ecg(ecg, fs, minA, maxA, n_bits, output, output_filename);

		// Show Output
		//	for(int i = 0; i < output.size(); i++)
		//		std::cout<< "Beat: "<< i << " r_pose: "<< output[i][0] << " \
						 prediction: "<< output[i][1] << std::endl;	
	
		delete ecg_classifier;
	}
	else
		std::cout<< "Error: an error ocurred file not open" << std::endl;

return 0;

}
