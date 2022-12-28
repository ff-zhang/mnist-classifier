//
// Created by Felix Zhang on 2022-12-28.
//

#ifndef MINST_CLASSIFIER_SGDLEARNER_H
#define MINST_CLASSIFIER_SGDLEARNER_H

#include <Eigen/Dense>

class SGDLearner {
    static const int imagePixels = 784;

    typedef Eigen::Matrix<float, imagePixels, 1> VecDom;
    typedef Eigen::Matrix<float, imagePixels * 10, 1> VecLab;

    const float learningRate;

    VecLab weights;

    VecLab embed(VecDom v, int y);

    VecLab sgd(int numSamples);
    double loss01(int y, int z);
    double lossKT(int y, int z);

    int predict(VecDom x);
    double TrainingError(Dataset &data);
    double GeneralizationError(Dataset &data);


    public:
        SGDLearner(float rate);
};


#endif //MINST_CLASSIFIER_SGDLEARNER_H
