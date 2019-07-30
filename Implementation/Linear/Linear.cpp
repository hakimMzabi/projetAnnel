#if _WIN32
    #define SUPEREXPORT __declspec(dllexport)
#else
    #define SUPEREXPORT
#endif

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>

#include "../Librairie/Matrix.h"

extern "C" {

/*
 *
 * Utils
 *
 */

// ajouter une autre fonction sign(somme de 0 à sampleCount) (w_i * x_i) ** 0
// bias = position de la droite, a et b orientation de la droite

SUPEREXPORT double getRand(double min, double max) {

    double val = (double) rand() / RAND_MAX;
    val = min + val * (max - min);

    return val;
}

SUPEREXPORT double* create_linear_model(int inputCountPerSample) {
    auto res = new double[inputCountPerSample + 1];

    for (int i = 0; i < inputCountPerSample + 1; i++) {
        res[i] = getRand(-1.0, 1.0);
    }

    return res;
}

SUPEREXPORT Matrix<double> transformDoubleToMatrix(double *mat, int rows, int cols = 1) {
    int currentPos = 0;
    Matrix<double> res(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            res.put(i, j, mat[currentPos]);
            currentPos += 1;
        }
    }

    return res;
}

SUPEREXPORT void displayMatrix(double* matrix, int rows, int cols){
    int count = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[count];

            if(j+1 < cols){
                std::cout << " | ";
            }

            count++;
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

SUPEREXPORT double* addBias(const double* mat, int lines, int cols) {
    int bias = 1;

    auto XTrain = new double[lines * (cols+1)];

    int pos = 0;
    int count = 0;

    for (int i = 0; i < lines; ++i) {
        XTrain[count] = bias;
        count += 1;

        for (int j = 0; j < cols; ++j) {
            XTrain[count] = mat[pos];
            count += 1;
            pos += 1;
        }
    }

    return XTrain;

//    Matrix<double> matrix = transformDoubleToMatrix(mat, lines, cols);
//    Matrix<double> result(lines, cols);
//    Matrix<double> result(lines, cols + 1);
//
//    for (int i = 0; i < matrix.getRows(); ++i) {
//        result.put(i, 0, bias);
//        for (int j = 0; j < matrix.getColumns(); ++j) {
//            result.put(i, j + 1, matrix.get(i, j));
//        }
//    }

//    double* res = result.convertToDouble();
//    return res;
}

/*
 *
 * Implementation
 *
 */

SUPEREXPORT int sign(double val){
    return (val >= 0.0) ? 1 : -1 ;
}

SUPEREXPORT double predict(
        const double* W,
        double* XLineToPredict,
        int inputCountPerSample,
        bool predictState = true
) {
    if(predictState){
        XLineToPredict = addBias(XLineToPredict, 1, inputCountPerSample);
        inputCountPerSample += 1;
    }

    double res = 0.0;
    for (int j = 0; j < inputCountPerSample; ++j) {
        res = res + (W[j] * XLineToPredict[j]);
    }

    return res ;
}

SUPEREXPORT double predict_regression(
        double* W,
        double* XLineToPredict,
        int inputCountPerSample,
        bool predictState = true
) {
    return predict(W, XLineToPredict, inputCountPerSample, predictState);
}

SUPEREXPORT double predict_classification(
        double* W,
        double* XLineToPredict,
        int inputCountPerSample,
        bool predictState = true
) {
    return sign(predict(W, XLineToPredict, inputCountPerSample, predictState));
}

SUPEREXPORT double* fit_regression(
        double *W, // to remove
        double *XTrain,
        double *YTrain,
        int sampleCount, // nombre d'image (ligne)
        int inputCountPerSample //nombre de pixel par img (colonne)
) {
    XTrain = addBias(XTrain, sampleCount, inputCountPerSample);

    Matrix<double> XTrainFin = transformDoubleToMatrix(XTrain, sampleCount, inputCountPerSample);
    Matrix<double> YTrainFin = transformDoubleToMatrix(YTrain, sampleCount, 1);

    Matrix<double> XTrainTranspose = XTrainFin.getTranspose();

    Matrix<double> XTmultX = XTrainTranspose * XTrainFin;

    Matrix<double> XTmultXInverse = XTmultX.getInverse();

    Matrix<double> res = (XTmultXInverse * XTrainTranspose * YTrainFin).getTranspose();
    return res.convertToDouble();
}

SUPEREXPORT double** oneDimToTwo(const double* XTrain, int ligne, int col){
    int pos = 0;
    auto** XTrainFin = new double*[ligne];

    for (int i = 0; i < ligne; i++) {
        XTrainFin[i] = new double[col];

        for (int j = 0; j < col; j++) {
            XTrainFin[i][j] = XTrain[pos];
            pos += 1;
        }
    }

    return XTrainFin;
}


SUPEREXPORT double* fit_classification(
        double* W,
        double* XTrain,
        const double* YTrain,
        int sampleCount, // nombre d'image (ligne)
        int inputCountPerSample, //nombre de pixel par img (colonne)
        double alpha, // Learning Rate (0,01 par exemple)
        int epochs // Nombre d'itération
) {
    XTrain = addBias(XTrain, sampleCount, inputCountPerSample);
    inputCountPerSample += 1;
    double** XTrainFin = oneDimToTwo(XTrain, sampleCount, inputCountPerSample);

    for (int i = 0; i < epochs; i++) {
        std::cout << "- Epochs : " << i << " / " << epochs << std::endl;
        for (int j = 0; j < sampleCount; j++) {
            auto tmp = XTrainFin[j];

            for (int k = 0; k < inputCountPerSample; k++) {
                double predict = predict_classification(W, tmp, inputCountPerSample, false);
                auto value = W[k] + alpha * (YTrain[j] - predict) * XTrainFin[j][k];
                W[k] = value;
            }
        }
    }

    return W;
}

SUPEREXPORT void delete_linear_model(const double *W) {
    delete[] W;
}

int main() {

    srand(time(nullptr)); // Enable rand() function

    int inputCountPerSample = 2;
    int sampleCount = 13;
    int epochs = 5000;
    double alpha = 0.001;

    double* model1 = create_linear_model(inputCountPerSample);
    double* model2 = create_linear_model(inputCountPerSample);

    // 26 car sampleCount * inputCountPerSample = 13 * 2 = 26 (soit 13 images de 2 pixels donc 26 pixels au total)
    double Xtrains[26] = {
            0, 0,
            1, 0,
            0, 1,
            2, 2,
            1, 2,
            2, 1,
            0.25, 0.25,
            0.1, 0.1,
            0.15, 0.15,
            0.3, 0.3,
            3, 3,
            1.5, 1.5,
            2.5, 2.5
    };

    // 13 car sampleCount = 13 (soit 13 images)
    double Ytrains[13] = {-1, 1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1};


    /*
     * Classification
     */

    std::cout << std::endl <<  "Classification : " << std::endl;

    std::cout <<  "Before Rosenblatt : " << std::endl;
    displayMatrix(model1, 1, inputCountPerSample+1);
    double* modelClas = fit_classification(model1, Xtrains, Ytrains, sampleCount, inputCountPerSample, alpha, epochs);
    std::cout <<  "After Rosenblatt : " << std::endl;
    displayMatrix(modelClas, 1, inputCountPerSample+1);

    double valC_1[2] = {0.25, 0.25};
    double valC_2[2] = {2.5, 2.5};
    double valC_3[2] = {1, 2};
    double valC_4[2] = {0.3, 0.3};
    double valC_5[2] = {3, 3};

    std::cout << "- Prediction des points [0.25;0.25] (-1) : " << predict_classification(modelClas, valC_1, inputCountPerSample) << std::endl; // -1
    std::cout << "- Prediction des points [2.5;2.5] (1) : " << predict_classification(modelClas, valC_2, inputCountPerSample) << std::endl; // 1
    std::cout << "- Prediction des points [1;2] (1) : " << predict_classification(modelClas, valC_3, inputCountPerSample) << std::endl; // 1
    std::cout << "- Prediction des points [0.3;0.3] (-1) : " << predict_classification(modelClas, valC_4, inputCountPerSample) << std::endl; // -1
    std::cout << "- Prediction des points [3;3] (1) : " << predict_classification(modelClas, valC_5, inputCountPerSample) << std::endl; // 1

    /*
     * Regression
     */

    std::cout << std::endl <<  "Regression : " << std::endl;

    std::cout << "Before regression : " << std::endl;
    displayMatrix(model2, 1, inputCountPerSample+1);
    double* modelReg = fit_regression(model2, Xtrains, Ytrains, sampleCount, inputCountPerSample);
    std::cout << "After regression : " << std::endl;
    displayMatrix(modelReg, 1, inputCountPerSample+1);

    double valR_1[2] = {0, 0};
    double valR_2[2] = {2.5, 2.5};
    double valR_3[2] = {1.5, 1.5};
    double valR_4[2] = {0.3, 0.3};
    double valR_5[2] = {3, 3};

    std::cout << "- Régression des points [0;0] (-1) : " << predict_regression(modelReg, valR_1, inputCountPerSample) << std::endl; // -1
    std::cout << "- Régression des points [2.5;2.5] (1) : " << predict_regression(modelReg, valR_2, inputCountPerSample) << std::endl; // 1
    std::cout << "- Régression des points [1.5;1.5] (1) : " << predict_regression(modelReg, valR_3, inputCountPerSample) << std::endl; // 1
    std::cout << "- Régression des points [0.3;0.3] (-1) : " << predict_regression(modelReg, valR_4, inputCountPerSample) << std::endl; // -1
    std::cout << "- Régression des points [3;3] (1) : " << predict_regression(modelReg, valR_5, inputCountPerSample) << std::endl; // 1

    /*
     * Graphe
     */

    // Classification

//    std::cout << std::endl << std::endl;
//    std::cout << ">> Graphe Classification :";
//    std::cout << std::endl << std::endl;
//
//    for (double i = 2; i >= -0.05; i-=0.05) {
//        printf("%4.2f | ", i < 0 ? 0 : i );
//        for (double j = 0; j <= 2.05; j+=0.05) {
//            double value[2] = {i, j};
//            if(predict_classification(model1, value, 2) == 1){
//                std::cout << " - ";
//            }else{
//                std::cout << " x ";
//            }
//        }
//        std::cout << std::endl;
//    }

    // Regression

    std::cout << std::endl << std::endl;
    std::cout << ">> Graphe Regression :";
    std::cout << std::endl << std::endl;

    for (double i = 2; i >= -0.05; i-=0.05) {
        printf("%4.2f | ", i < 0 ? 0 : i );
        for (double j = 0; j <= 2.05; j+=0.05) {
            double value[2] = {i, j};
            if(predict_regression(model2, value, 2) == 1){
                std::cout << " - ";
            }else{
                std::cout << " x ";
            }
        }
        std::cout << std::endl;
    }


}

}