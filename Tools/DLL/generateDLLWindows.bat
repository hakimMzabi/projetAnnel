@g++ -c ../Implementation/RBF/RBF.cpp -o ../Implementation/RBF/Librairie/Windows/RBF_Windows.o
@g++ -shared -o ../Implementation/RBF/Librairie/Windows/RBF_Windows.dll ../Implementation/RBF/Librairie/Windows/RBF_Windows.o -W
@g++ -c ../Implementation/MLP/MultiLayerPerceptron.cpp -o ../Implementation/MLP/Librairie/Windows/MultiLayerPerceptron_Windows.o
@g++ -shared -o ../Implementation/MLP/Librairie/Windows/MultiLayerPerceptron_Windows.dll ../Implementation/MLP/Librairie/Windows/MultiLayerPerceptron_Windows.o -W
@g++ -c ../Implementation/Rosenblatt/Rosenblatt.cpp -o ../Implementation/Rosenblatt/Librairie/Windows/Rosenblatt_Windows.o
@g++ -shared -o ../Implementation/Rosenblatt/Librairie/Windows/Rosenblatt_Windows.dll ../Implementation/Rosenblatt/Librairie/Windows/Rosenblatt_Windows.o -W