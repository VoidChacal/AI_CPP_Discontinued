#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

class Neural {
public:
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

void train(Neural& net) {
    // Training dataset
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

    // Train neural network
    for (int i = 0; i < 10000; i++) {
        for (auto& data : training_data) {
            std::vector<double> inputs;
            for (char c : data.first) {
                inputs.push_back(static_cast<double>(c));
            }
            double maxCharValue = *std::max_element(inputs.begin(), inputs.end());
            for (auto& input : inputs) {
                input /= maxCharValue;
            }
            net.train(inputs, data.second);
        }
    }
}
int main() {
    Neural net(15);

    bool exit = false;
    while (!exit) {
        std::cout << "Menu:\n";
        std::cout << "1. Train the neural network\n";
        std::cout << "2. Test the neural network\n";
        std::cout << "3. Save the neural network\n";
        std::cout << "4. Load a neural network\n";
        std::cout << "5. Exit\n";

        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            train(net);
            std::cout << "Training completed!\n";
            break;
        case 2: {
            std::cout << "Enter input string: ";
            std::cin.ignore(); 
            std::string input;
            std::getline(std::cin, input);

            std::vector<double> inputs;
            for (char c : input) {
                inputs.push_back(static_cast<double>(c) / 255.0);
            }

            double output = net.run(inputs);
            std::cout << "Output: " << (output >= 0.5 ? "Yes" : "No") << "\n";
            break;
        }
        case 3: {
            std::cout << "Enter the filename to save the neural network: ";
            std::cin.ignore();  
            std::string filename;
            std::getline(std::cin, filename);

            net.save(filename);
            std::cout << "Neural network saved to file: " << filename << "\n";
            break;
        }
        case 4: {
            std::cout << "Enter the filename to load the neural network: ";
            std::cin.ignore(); 
            std::string filename;
            std::getline(std::cin, filename);

            net.load(filename);
            std::cout << "Neural network loaded from file: " << filename << "\n";
            break;
        }
        case 5:
            // Exit
            exit = true;
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Invalid choice, please try again.\n";
            break;
        }
    }

    return 0;
}