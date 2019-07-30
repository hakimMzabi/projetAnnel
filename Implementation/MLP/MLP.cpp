#if _WIN32
#define SUPEREXPORT __declspec(dllexport)
#else
#define SUPEREXPORT
#endif

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>

struct MLP{
    int* npl; // Neuron per layers
    int layer_count; // Number of layer
    double*** W; // All W values
    double** X; // All X values
    double** deltas; // All delta values
    int lastLayerIndex;
};

extern "C" {

//IMPORT FROM ROSENBLATT ALGORITHM

SUPEREXPORT double getRand(double min, double max) {

    double val = (double) rand() / RAND_MAX;
    val = min + val * (max - min);

    return val;
}

SUPEREXPORT double* init_with_random(int inputCountPerSample) {
    auto res = new double[inputCountPerSample + 1];

    for (int i = 0; i < inputCountPerSample + 1; i++) {
        res[i] = getRand(-1.0, 1.0);
//        res[i] = 0.5;
    }

    return res;
}

SUPEREXPORT double** addMatrixBias(double** XtoPred1, int sampleCount, int inputCountPerSample){

    auto* res = new double*[sampleCount];

    for (int j = 0; j < sampleCount; j++) {
        res[j] = new double[inputCountPerSample + 1];
        res[j][0] = 1.0;

        for (int i = 0; i < inputCountPerSample; i++) {
            res[j][i+1] = XtoPred1[j][i];
        }
    }

    return res;
}

SUPEREXPORT double* addBias(const double* XLineToPred, int inputCountPerSample){
    auto res = new double[inputCountPerSample + 1];

    res[0] = 1.0;

    for (int j = 0; j < inputCountPerSample; j++) {
        res[j+1] = XLineToPred[j];
    }

    return res;
}

SUPEREXPORT double** convertToMatrix(const double* XTrain, int ligne, int col){
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

// MLP ALGORITHM

SUPEREXPORT void init_model(MLP* mlp){

    mlp->W = new double**[mlp->layer_count];

    for (int l = 1; l < mlp->layer_count; l++) {
        int prev_neuron_count = mlp->npl[l - 1] + 1; // +1 pour le biais
        int cur_neuron_count = mlp->npl[l] + 1; // +1 pour le biais

        mlp->W[l] = new double*[prev_neuron_count];

        for (int j = 0; j < prev_neuron_count; j++) { // HERE
            mlp->W[l][j] = new double[cur_neuron_count];
            mlp->W[l][j] = init_with_random(cur_neuron_count);
        }
    }
}

SUPEREXPORT MLP* create_MLP() {
    auto* ret = (struct MLP*) malloc(sizeof(struct MLP));
    ret->X = new double*;
    ret->npl = new int;
    ret->lastLayerIndex = 0;
    ret->deltas = new double*;
    ret->layer_count = 0;
    ret->W = new double**;

    return ret;
}

SUPEREXPORT MLP* init(const int* neurons, int size){

    srand(time(nullptr));

    MLP* mlp = create_MLP();

    mlp->layer_count = size;
    mlp->npl = (int*) malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++) {
        mlp->npl[i] = neurons[i];
    }

    mlp->X = new double*[size];
    mlp->deltas = new double*[size];

    init_model(mlp);

    mlp->lastLayerIndex = mlp->layer_count - 1;

    return mlp;
}

SUPEREXPORT MLP* create(const int* npl, int layer_count, const double* X){

    MLP* mlp = create_MLP();

    mlp->layer_count = layer_count;
    mlp->npl = (int*) malloc(sizeof(int) * layer_count);

    for (int i = 0; i < layer_count; i++) {
        mlp->npl[i] = npl[i];
    }

    mlp->X = new double*[layer_count];

    int pos = 0;

    mlp->W = new double**[mlp->layer_count];

    for (int l = 1; l < mlp->layer_count; l++) {
        int prev_neuron_count = mlp->npl[l - 1] + 1; //+1 pour le biais
        int cur_neuron_count = mlp->npl[l] + 1; //+1 pour le biais

        mlp->W[l] = new double*[prev_neuron_count];
        for (int j = 0; j < prev_neuron_count; j++) {

            mlp->W[l][j] = new double[cur_neuron_count];
            for (int i = 1; i < cur_neuron_count; i++) {
                mlp->W[l][j][i] = X[pos];
                pos += 1;
            }
        }
    }

    mlp->lastLayerIndex = mlp->layer_count - 1;

    return mlp;
}


SUPEREXPORT double sommWxDelta(const double* w, int cur_neuron_count, const double* delta){
    double res = 0;
    for (int j = 1; j < cur_neuron_count; j++) {
        res += w[j] * delta[j];
    }
    return res;
}

SUPEREXPORT void initAllDeltaExceptLast(MLP* mlp, int lastIndex){
    for (int l = lastIndex; l > 0 ; l--) {
        int prev_neuron_count = mlp->npl[l - 1] + 1;
        int cur_neuron_count = mlp->npl[l] + 1;
        mlp->deltas[l - 1] = new double[prev_neuron_count];
        for (int i = 1; i < prev_neuron_count; i++) {
            double som = sommWxDelta(mlp->W[l][i], cur_neuron_count, mlp->deltas[l]);
            double val = (1 - pow(mlp->X[l - 1][i], 2)) * som;
            mlp->deltas[l - 1][i] = val;
        }
    }
}

SUPEREXPORT void initLastDelta_classification(MLP* mlp, int lastIndex, const double* YTrain){
    mlp->deltas[lastIndex] = new double[mlp->npl[lastIndex] + 1];
    for (int j = 1; j < mlp->npl[lastIndex] + 1; j++) {
        double val = (1 - pow((mlp->X[lastIndex][j]),2)) * (mlp->X[lastIndex][j] - YTrain[j - 1]);
        mlp->deltas[lastIndex][j] = val;
    }
}

SUPEREXPORT void initLastDelta_regression(MLP* mlp, int lastIndex, const double* YTrain){
    mlp->deltas[lastIndex] = new double[mlp->npl[lastIndex] + 1];
    for (int j = 1; j < mlp->npl[lastIndex] + 1; j++) {
        double val = (mlp->X[lastIndex][j] - YTrain[j - 1]);
        mlp->deltas[lastIndex][j] = val;
    }
}

SUPEREXPORT void updateW(MLP* mlp, int lastIndex, double alpha){
    for (int l = 1; l < lastIndex + 1; l++) {
        int prev_neuron_count = mlp->npl[l - 1] + 1;
        int cur_neuron_count = mlp->npl[l] + 1;

        for (int j = 1; j < cur_neuron_count; j++) {
            for (int i = 0; i < prev_neuron_count; i++) {
                mlp->W[l][i][j] = mlp->W[l][i][j] - (alpha * mlp->X[l - 1][i] * mlp->deltas[l][j]);
            }
        }
    }
}

SUPEREXPORT void destroy(struct MLP* mlp) {
    if( mlp ){
        free( mlp );
    }
}

SUPEREXPORT void feedFoward(MLP* mlp, bool isClassif){
    for (int l = 1; l < mlp->layer_count; l++) {

        mlp->X[l] = new double[mlp->npl[l] + 1];
        mlp->X[l][0] = 1;

        int prev_neuron_count = mlp->npl[l - 1] + 1; //+1 pour le biais
        int cur_neuron_count = mlp->npl[l] + 1;

        for (int j = 1; j < cur_neuron_count; j++) { //+1 pour le biais
            double val = 0;

            for (int i = 0; i < prev_neuron_count; i++) {
                val += (mlp->W[l][i][j] * mlp->X[l - 1][i]);
            }

            if(l != mlp->lastLayerIndex or isClassif){
                mlp->X[l][j] = tanh(val);
            }else{
                mlp->X[l][j] = val;
            }
        }
    }
}

SUPEREXPORT double* predict(MLP* mlp, double* XtoPred, bool isClassif = false){

    int inputCountPerSample = mlp->npl[0];
    double* XtoPredWithBias = addBias(XtoPred, inputCountPerSample);
    mlp->X[0] = XtoPredWithBias;

    feedFoward(mlp, isClassif);

    int last_layer_neuron_count = mlp->npl[mlp->lastLayerIndex];
    auto* res = new double[last_layer_neuron_count];

    for (int i = 1; i < last_layer_neuron_count + 1; i++) {
        auto value = mlp->X[mlp->lastLayerIndex][i];
        res[i-1] = value;
    }

    return res;
}

SUPEREXPORT MLP* fit_classification(MLP* mlp, double* XTrain, double* YTrain, int sampleCount, int epochs, double alpha) {

    int startNeuron = mlp->npl[0];
    int endNeuron = mlp->npl[mlp->lastLayerIndex];

    double** XTrainFinal = convertToMatrix(XTrain, sampleCount, startNeuron);
    XTrainFinal = addMatrixBias(XTrainFinal, sampleCount, startNeuron);

    double** YTrainFinal = convertToMatrix(YTrain, sampleCount, endNeuron);


    for (int e = 0; e < epochs; e++) {
        std::cout << "- Epochs : " << e << " / " << epochs << std::endl;
        for (int i = 0; i < sampleCount; i++) {
//            std::cout << "      - sampleCount : " << i << " / " << sampleCount << std::endl;

            mlp->X[0] = XTrainFinal[i];
            feedFoward(mlp, true);

            int l = mlp->lastLayerIndex;
            int cur_neuron_count = mlp->npl[l] + 1;
            for (int j = 1; j < cur_neuron_count; j++) {
                mlp->X[l][j] = tanh(mlp->X[l][j]);
            }

            initLastDelta_classification(mlp, mlp->lastLayerIndex, YTrainFinal[i]);
            initAllDeltaExceptLast(mlp, mlp->lastLayerIndex);
            updateW(mlp, mlp->lastLayerIndex, alpha);
        }
    }

    return mlp;
}

SUPEREXPORT MLP* fit_regression(MLP* mlp, double* XTrain, double* YTrain, int sampleCount, int epochs, double alpha) {

    int startNeuron = mlp->npl[0];
    int endNeuron = mlp->npl[mlp->lastLayerIndex];

    double** XTrainFinal = convertToMatrix(XTrain, sampleCount, startNeuron);
    XTrainFinal = addMatrixBias(XTrainFinal, sampleCount, startNeuron);

    double** YTrainFinal = convertToMatrix(YTrain, sampleCount, endNeuron);

    for (int e = 0; e < epochs; e++) {
        for (int i = 0; i < sampleCount; i++) {

            mlp->X[0] = XTrainFinal[i];
            feedFoward(mlp, false);

            initLastDelta_regression(mlp, mlp->lastLayerIndex, YTrainFinal[i]);
            initAllDeltaExceptLast(mlp, mlp->lastLayerIndex);
            updateW(mlp, mlp->lastLayerIndex, alpha);
        }
    }

    return mlp;
}

// export

SUPEREXPORT int getWSize(MLP* mlp){
    int res = 0;

    for (int l = 1; l < mlp->layer_count; l++) {
        int prev_neuron_count = mlp->npl[l - 1] + 1; //+1 pour le biais
        int cur_neuron_count = mlp->npl[l] + 1;

        for (int j = 0; j < prev_neuron_count; j++) {
            for (int i = 1; i < cur_neuron_count; i++) { //+1 pour le biais
                res+= 1;
            }
        }
    }

    return res;
}

SUPEREXPORT double* getAllWValues(MLP* mlp){

    int size = getWSize(mlp);
    auto* res = new double[size];
    int counter = 0;

    for (int l = 1; l < mlp->layer_count; l++) {
        int prev_neuron_count = mlp->npl[l - 1] + 1; //+1 pour le biais
        int cur_neuron_count = mlp->npl[l] + 1;

        for (int j = 0; j < prev_neuron_count; j++) {
            for (int i = 1; i < cur_neuron_count; i++) { //+1 pour le biais
                res[counter] = mlp->W[l][j][i];
                counter += 1;
            }
        }
    }

    return res;
}

SUPEREXPORT int getNeuronSize(MLP* mlp){
    return mlp->layer_count;
}

SUPEREXPORT int getPredictLength(MLP* mlp){
    return mlp->npl[mlp->lastLayerIndex];
}

SUPEREXPORT int* getAllNeuronValues(MLP* mlp){
    return mlp->npl;
}

SUPEREXPORT void displayAllXValues(MLP* mlp){
    //  PRINT ALL X
    for (int l = 0; l < mlp->layer_count; l++) {
//        std::cout << "x[" << l << "] : ";
        for (int j = 0; j < mlp->npl[l] + 1; j++) {
//            std::cout << mlp->X[l][j] << " - ";
            std::cout << "x[" << l << "][" << j << "] : " <<  mlp->X[l][j] << " \n";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {

    // Init

//    int sampleCount = 4;
//    double alpha = 0.001;
//    int epochs = 10000;
//
//    double XTrain[8] = {
//            0, 0,
//            1, 0,
//            0, 1,
//            1, 1,
//    };
//    double YTrain[4] = {
//            1, -1, -1, 1
//    };
//
//    // MLP implementation
//
//    int neurons[3] = {2, 2, 1};
//    MLP* mlp = init(neurons, 3);
//    mlp = fit_classification(mlp, XTrain, YTrain, sampleCount, epochs, alpha);

    int neurons[3] = {2, 2, 1};
    double X[9] = {-0.334539, -0.417374, 0.524344, 1.94529, 0.57998, 2.07676, 0.707318, 1.81207, -1.73534};
    MLP* mlp = create(neurons, 3, X);

    double* values = getAllWValues(mlp);
    for (int i = 0; i < getWSize(mlp); ++i) {
        std::cout << values[i] << ", ";
    }


    // -------------------------------------
    std::cout << std::endl;

    double xtoPred[2] = {0, 0};
    auto res = predict(mlp, xtoPred);

    for (int i = 0; i < mlp->npl[mlp->lastLayerIndex]; ++i) {
        std::cout << res[i] << " - ";
    }

    // -------------------------------------
    std::cout << std::endl;

    double xtoPred_1[2] = {1, 0};
    auto res_1 = predict(mlp, xtoPred_1);

    for (int i = 0; i < mlp->npl[mlp->lastLayerIndex]; ++i) {
        std::cout << res_1[i] << " - ";
    }

    // -------------------------------------
    std::cout << std::endl;

    double xtoPred_2[2] = {0, 1};
    auto res_2 = predict(mlp, xtoPred_2);

    for (int i = 0; i < mlp->npl[mlp->lastLayerIndex]; ++i) {
        std::cout << res_2[i] << " - ";
    }

    // -------------------------------------
    std::cout << std::endl;

    double xtoPred_3[2] = {1, 1};
    auto res_3 = predict(mlp, xtoPred_3);

    for (int i = 0; i < mlp->npl[mlp->lastLayerIndex]; ++i) {
        std::cout << res_3[i] << " - ";
    }
}


}