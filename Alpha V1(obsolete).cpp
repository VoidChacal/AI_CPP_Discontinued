#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

class Neural {
private:
    std::vector<double> weights;
    double bias;
    int size;

public:
    Neural(int size) : size(size) {
        weights.resize(size);
        for (int i = 0; i < size; i++) {
            weights[i] = (double) rand() / RAND_MAX;
        }
        bias = (double) rand() / RAND_MAX;
    }
    double sigmoid(double x) {
        return 1 / (1 + std::exp(-x));
    }
    double run(std::vector<double> inputs) {
        double sum = 0.0;
        for (int i = 0; i < size; i++) {
            sum += inputs[i] * weights[i];
        }
        sum += bias;
        return sigmoid(sum);
    }
    void train(std::vector<double> inputs, double desired) {
        double y = run(inputs);
        double error = desired - y;
        for (int i = 0; i < size; i++) {
            weights[i] += error * inputs[i];
        }
        bias += error;
    }

    void save(std::string filename) {
        std::ofstream file(filename, std::ios::binary);
        file.write((char*)&size, sizeof(size));
        file.write((char*)weights.data(), sizeof(double) * size);
        file.write((char*)&bias, sizeof(bias));
        file.close();
    }

    void load(std::string filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.good()) {
            std::cerr << "Error opening file!\n";
            return;
        }

        file.read((char*)&size, sizeof(size));
        weights.resize(size);
        file.read((char*)weights.data(), sizeof(double) * size);
        file.read((char*)&bias, sizeof(bias));

        file.close();
    }
};

int main() {
    std::vector<std::pair<std::string, double>> training_data = {
        {"Is it raining?", 1.0},
        {"Are you hungry?", 0.0},
        {"Do you like pizza?", 1.0},
        {"Is the sky blue?", 1.0},
        {"Do you have a car?", 0.0},
        {"Are you tired?", 0.0},
        {"Is today Monday?", 0.0},
        {"Are you happy?", 1.0},
        {"Do you like to dance?", 1.0},
        {"Is the earth flat?", 0.0},
    };
    Neural net(training_data[0].first.size());
  
    for (int i = 0; i < 10000; i++) {
        for (auto& data : training_data) {
            std::vector<double> inputs;
            for (auto c : data.first) {
                inputs.push_back((double) c / 255.0);
            }
            net.train(inputs, data.second);
        }
    }

    net.save("weights.dat");

    Neural net2(training_data[0].first.size());
    net2.load("weights.dat");

    std::string input;
    while (true) {
        std::cout << "Ask a yes or no question: ";
        std::getline(std::cin, input);
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
        std::vector<double> inputs;
        for (auto c : input) {
            inputs.push_back((double) c / 255.0);
        }
        double output = net.run(inputs);
        if (output >= 0.5) {
            std::cout << "Yes!\n";
        } else {
            std::cout << "No!\n";
        }
    }
    return 0;
}