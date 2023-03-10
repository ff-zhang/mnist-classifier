//
// Created by Felix Zhang on 2022-12-28.
//

#ifndef MNIST_CLASSIFIER_DATASET_H
#define MNIST_CLASSIFIER_DATASET_H

#include "globals.h"
#include "fileHelpers.h"

#include <iostream>
#include <fstream>
#include <filesystem>

template<int NUM_TRAIN, int NUM_TEST>
class Dataset {

private:
    // only call on initialization
    // This function returns a pointer to heap-allocated memory
    template<int N>
    DataPoint* readSet(const std::filesystem::path& xFile, const std::filesystem::path& yFile);

public:
    DataPoint *trainSet, *testSet;

    Dataset(const std::filesystem::path& trainImages, const std::filesystem::path& trainLabels, const std::filesystem::path& testImages,
            const std::filesystem::path& testLabels) {
        trainSet = readSet<NUM_TRAIN>(TRAIN_IMAGES, TRAIN_LABELS);
        testSet = readSet<NUM_TEST>(TEST_IMAGES, TEST_LABELS);
    }

    ~Dataset() {
        // Free memory allocated with the "readSet" function
        delete[] trainSet;
        delete[] testSet;
    }
};

// template functions MUST be defined in the header file
template<int NUM_TRAIN, int NUM_TEST>
template<int N>
DataPoint* Dataset<NUM_TRAIN, NUM_TEST>::readSet(const std::filesystem::path& xFile, const std::filesystem::path& yFile) {
    std::ifstream fDom, fLab;

    // Retrieve training set directory
    auto srcDir = getSrcDir(std::filesystem::current_path());

    std::filesystem::path xFileAbs = (srcDir / xFile).make_preferred();
    std::filesystem::path yFileAbs = (srcDir / yFile).make_preferred();

    fDom.open(xFileAbs, std::ios::binary | std::ios::in);
    fLab.open(yFileAbs, std::ios::binary | std::ios::in);

    if (!fDom.is_open()) {
        std::cout << "Error opening " << xFileAbs << "!" << std::endl;
    };
    if (!fLab.is_open()) {
        std::cout << "Error opening " << yFileAbs << "!" << std::endl;
    };

    // skip the header
    char bufferHeader[16];
    fDom.read(bufferHeader, 16);
    fLab.read(bufferHeader, 8);

    char* bufferX = new char[IMAGE_SIZE];

    auto* points = new DataPoint[N];
    char bufferLab;

    int i = 0;
    while (!fDom.eof() && i < N) {
        fDom.read(bufferX, IMAGE_SIZE);
        fLab.get(bufferLab);

        auto bufferIm = reinterpret_cast<unsigned char*>(bufferX);
        auto* bufferDom = new double[IMAGE_SIZE];
        std::transform(bufferIm, bufferIm + IMAGE_SIZE, bufferDom,
                       [](const unsigned char c) -> double { return (double) c; });
        Eigen::Map<VecDom> point(bufferDom);

        points[i] = *new DataPoint{point, *reinterpret_cast<unsigned char*>(&bufferLab)};
        i++;
    }
    delete[] bufferX;

    fDom.close();
    fLab.close();

    return points;
}

#endif //MNIST_CLASSIFIER_DATASET_H
