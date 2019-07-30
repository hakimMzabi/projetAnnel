g++ -c -std=c++17 ../Implementation/Rosenblatt/Rosenblatt.cpp -o ../Implementation/Rosenblatt/Librairie/Mac/Rosenblatt_Mac.o
g++ -shared -Wl -o ../Implementation/Rosenblatt/Librairie/Mac/Rosenblatt_Mac.so ../Implementation/Rosenblatt/Librairie/Mac/Rosenblatt_Mac.o
g++ -c -std=c++17 ../Implementation/MLP/MultiLayerPerceptron.cpp -o ../Implementation/MLP/Librairie/Mac/MultiLayerPerceptron_Mac.o
g++ -shared -Wl -o ../Implementation/MLP/Librairie/Mac/MultiLayerPerceptron_Mac.so ../Implementation/MLP/Librairie/Mac/MultiLayerPerceptron_Mac.o
g++ -c -std=c++17 ../Implementation/RBF/RBF.cpp -o ../Implementation/RBF/Librairie/Mac/RBF_Mac.o
g++ -shared -Wl -o ../Implementation/RBF/Librairie/Mac/RBF_Mac.so ../Implementation/RBF/Librairie/Mac/RBF_Mac.o