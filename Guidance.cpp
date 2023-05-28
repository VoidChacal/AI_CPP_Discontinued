#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

using namespace std;

class Neural {

	private:
		double* weights;
		double bias;

		int SIZE;

	public:
		Neural(int size) {
		
			this->SIZE = size;

			this->weights = (double*)malloc(sizeof(double) * size);


			for(int i = 0; i < size; i++) {
			
				this->weights[i] = (double) rand() / RAND_MAX;
			
				printf("w: %.10f\n", this->weights[i]);

			}

			this->bias = (double) rand() / RAND_MAX;	
			printf("b: %.10f\n", this->bias);
		
		}

		Neural(int size, double* w, double b) {
		
			this->SIZE = size;

			this->weights = (double*)malloc(sizeof(double) * size);

			for(int i = 0; i < size; i++) {
			
				this->weights[i] = w[i];
			
				printf("w: %.10f\n", this->weights[i]);

			}

			this->bias = b;
			printf("b: %.10f\n", this->bias);
		
		}

		double sigmoid(double x) {
		
			return 1 / (1 + exp(-x));
			
		}

		double run(double* inputs) {
		
			double sum = 0.0;

			for(int i = 0; i < this->SIZE; i++) {
			
				sum += (inputs[i] * this->weights[i]);
			
			}

			sum += this->bias;

			return this->sigmoid(sum);
		
		}

		double train(double* inputs, double desired) {
		
			double y = this->run(inputs);

			for(int i = 0; i < this->SIZE; i++) {
			
			
				this->weights[i] += (desired - y) * inputs[i];
			
			}
			this->bias += (desired - y);
		
		}

		void save(string filename) {
			ofstream file(filename.c_str(), ios::binary);
			file.write((char*)&this->SIZE, sizeof(this->SIZE));
			file.write((char*)this->weights, sizeof(double) * this->SIZE);
			file.write((char*)&this->bias, sizeof(this->bias));
			file.close();
		}

		static Neural* load(string filename) {
			ifstream file(filename.c_str(), ios::binary);
			if (!file.good()) {
				return nullptr;
			}

			int size;
			file.read((char*)&size, sizeof(size));

			double* weights = (double*)malloc(sizeof(double) * size);
			file.read((char*)weights, sizeof(double) * size);

			double bias;
			file.read((char*)&bias, sizeof(bias));

			file.close;
    }
};