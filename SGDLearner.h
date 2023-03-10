//
// Created by Felix Zhang on 2022-12-28.
//

#ifndef MNIST_CLASSIFIER_SGDLEARNER_H
#define MNIST_CLASSIFIER_SGDLEARNER_H

#include "globals.h"
#include "Dataset.h"
#include <memory>
#include <filesystem>

class SGDLearner {
    const float LEARNING_RATE;

private:
    VecLab weights;

    static VecLab embed(const VecDom& v, int y);

    double loss01(int y, int z);
    double lossKT(std::array<int,10> y, std::array<int,10> z);

    std::unique_ptr<VecLab> sgd(VecLab& w_t, DataPoint& p);

public:
    double TrainingError(Dataset<NUM_TRAIN, NUM_TEST>& data);
    double GeneralizationError(Dataset<NUM_TRAIN, NUM_TEST>& data);

    VecLab& getWeights() { return weights; };

    void train(Dataset<NUM_TRAIN, NUM_TEST> &data, int numSteps);

    int predict(const VecDom &x);
    std::array<int,10> predictRanking(VecDom x);

    explicit SGDLearner(float learningRate);

    void saveWeigths(std::filesystem::path relFilepath);

    void loadWeigths(std::filesystem::path relFilepath);
};

#endif //MNIST_CLASSIFIER_SGDLEARNER_H
