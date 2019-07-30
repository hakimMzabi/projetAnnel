

#if _WIN32
#define SUPEREXPORT __declspec(dllexport)
#include <ctime>
#else
#define SUPEREXPORT
#endif

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>

#include "../Librairie/Eigen/Dense"
#include "../Librairie/Eigen/Eigen"

using namespace Eigen;
using namespace std;

extern "C" {

typedef struct RBF {
    MatrixXd W;
    MatrixXd X;
    double gamma;
} RBF;

int getRand(int iter, int max) {
    srand(time(0) * (iter + 1 * 5));
    int size = max;
    int res = rand() % size;
    return res;
}

RBF *initRBF(MatrixXd X, MatrixXd WMatrix, double gamma) {
    RBF *rbf = new RBF();
    rbf->W = WMatrix;
    rbf->X = X;
    rbf->gamma = gamma;
    return rbf;
}

SUPEREXPORT RBF *naive_rbf_train(double *X, double *Y, int inputCountPerSample, int sampleCount, double gamma = 100,
                                 bool useBias = false) {
    MatrixXd XMatrix = Map<Matrix<double, Dynamic, Dynamic, RowMajor>>(X, inputCountPerSample, sampleCount);
    MatrixXd YMatrix = Map<MatrixXd>(Y, inputCountPerSample, 1);
    MatrixXd phi(inputCountPerSample, inputCountPerSample);
    for (int i = 0; i < inputCountPerSample; i++) {
        for (int j = 0; j < inputCountPerSample; j++) {
            phi(i, j) = exp(-gamma * (pow(((XMatrix.row(i) - XMatrix.row(j)).norm()), 2)));
        }
    }
    MatrixXd W = phi.inverse() * YMatrix;
    RBF *rbf = initRBF(XMatrix, W, gamma);
    return rbf;
}

SUPEREXPORT double naive_rbf_regression_predict(RBF *rbf, double *sample) {
    MatrixXd sampleMatrix = Map<MatrixXd>(sample, 1, rbf->X.cols());
    MatrixXd gaussianOutputs(1, rbf->X.rows());
    for (int i = 0; i < rbf->X.rows(); i++) {
        gaussianOutputs(0, i) = exp((-rbf->gamma) * (pow(((rbf->X.row(i) - sampleMatrix).norm()), 2)));
    }
    return (gaussianOutputs * rbf->W).sum();
}

SUPEREXPORT int naive_rbf_classification_predict(RBF *rbf, double *sample) {
    if (naive_rbf_regression_predict(rbf, sample) < 0) {
        return -1;
    } else {
        return 1;
    }
}

SUPEREXPORT RBF *rbf_train(
        double *X,
        double *Y,
        int inputCountPerSample,
        int sampleCount,
        int epochs = 5,
        int k = 2,
        double gamma = 100) {

    MatrixXd XMatrix = Map<Matrix<double, Dynamic, Dynamic, RowMajor>>(X, inputCountPerSample, sampleCount);
    MatrixXd YMatrix = Map<MatrixXd>(Y, inputCountPerSample, 1);
    MatrixXd KMatrix = MatrixXd::Random(k, sampleCount);
    MatrixXd ClosestMatrix = Map<MatrixXd>(XMatrix.data(), inputCountPerSample, sampleCount);
    RowVectorXd ClosestCenter(sampleCount);
    RowVectorXd AveragePositionCenter(sampleCount);
    RowVectorXd VectorSum;
    double distance;
    double lastDistance;
    int numberOfPointsEqual;

    //Initialize k random centers

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < sampleCount; j++) {
            KMatrix(i, j) = (KMatrix(i, j) + 1) / 2;
        }
    }

    cout << "Kmatrix : " << KMatrix << endl;

    //Repeat for number of epochs
    for (int i = 1; i <= epochs; i++) {
        cout << "epoch " << i << endl;
        lastDistance = -1;
        //For each X row get the closest KMatrix
        for (int j = 0; j < inputCountPerSample; j++) {
            for (int m = 0; m < k; m++) {
                distance = (KMatrix.row(m) - XMatrix.row(j)).norm();

                /*cout << "    Current distance :" << distance << endl;
                cout << "    Current center :" << KMatrix.row(m) << endl;
                cout << "    Current lastDistance :" << lastDistance << endl;*/

                if (lastDistance != -1) {
                    if (distance < lastDistance) {
                        ClosestCenter = KMatrix.row(m);
                    }
                }
                lastDistance = distance;

                ClosestMatrix.row(j) = ClosestCenter;
            }
        }

        //cout << "ClosestMatrix : \n" << ClosestMatrix << endl;

        //Recalculate centers from the individuals
        for (int j = 0; j < k; j++) {
            numberOfPointsEqual = 0;
            VectorSum = RowVectorXd::Zero(sampleCount);
            for (int m = 0; m < inputCountPerSample; m++) {
                if (KMatrix.row(j) == ClosestMatrix.row(m)) {
                    numberOfPointsEqual++;
                    VectorSum += XMatrix.row(m);
                }
            }
            if (numberOfPointsEqual != 0) {
                KMatrix.row(j) = VectorSum / numberOfPointsEqual;
            }
        }
    }

    //Calculate a new phi from XMatrix and KMatrix
    MatrixXd phi(inputCountPerSample, k);
    for (int i = 0; i < inputCountPerSample; i++) {
        for (int j = 0; j < k; j++) {
            phi(i, j) = exp(-gamma * (pow(((XMatrix.row(i) - KMatrix.row(j)).norm()), 2)));
        }
    }


    cout << "Phi transpose : \n" << phi.transpose() << endl;
    cout << "Phi : \n" << phi << endl;
    cout << "Phi transpose * phi : \n" << phi.transpose() * phi << endl;
    cout << "inverse(Phi transpose * phi) : \n" << (phi.transpose() * phi).inverse() << endl;

    //Calculate W
    MatrixXd W = (phi.transpose() * phi).inverse() * phi.transpose() * YMatrix;

    cout << "W : \n" << W << endl;
    //cout << "\n" << W << endl;


    RBF *rbf = initRBF(KMatrix, W, gamma);
    return rbf;
}

SUPEREXPORT double rbf_predict(RBF *rbf, double *sample) {
    MatrixXd sampleMatrix = Map<MatrixXd>(sample, 1, rbf->X.cols());
    MatrixXd gaussianOutputs(1, rbf->X.rows());
    for (int i = 0; i < rbf->X.rows(); i++) {
        gaussianOutputs(0, i) = exp((-rbf->gamma) * (pow(((rbf->X.row(i) - sampleMatrix).norm()), 2)));
    }
    return (gaussianOutputs * rbf->W).sum();
}

SUPEREXPORT int getWSize(RBF *rbf) {
    int res = 0;
    for (int l = 0; l < rbf->W.rows(); l++) {
        res++;
    }
    return res;
}

SUPEREXPORT double *getAllWValues(RBF *rbf) {
    int size = getWSize(rbf);
    auto *res = new double[size];
    for (int i = 0; i < size; i++) {
        res[i] = rbf->W(i, 0);
    }
    return res;
}

SUPEREXPORT double getGamma(RBF *rbf){
    return rbf->gamma;
}

SUPEREXPORT RBF *create(double* WValues, int length){
    MatrixXd WMatrix = Map<MatrixXd>(WValues, length, 1);
    return initRBF(WMatrix, WMatrix, 100.0);
}

}

