#!/bin/bash

# Checks if Linear_Linux.so exists
if [[ -f ../../Implementation/Linear/Librairie/Linux/Linear_Linux.so ]]; then
    echo "Deleting Linear_Linux.so..."
    rm ../../Implementation/Linear/Librairie/Linux/Linear_Linux.so
fi

# Checks if Linear_Linux.o exists
if [[ -f ../../Implementation/Linear/Librairie/Linux/Linear_Linux.o ]]; then
    echo "Deleting Linear_Linux.o..."
    rm ../../Implementation/Linear/Librairie/Linux/Linear_Linux.o
fi

# Checks if MLP_Linux.so exists
if [[ -f ../../Implementation/MLP/Librairie/Linux/MLP_Linux.so ]]; then
    echo "Deleting MLP_Linux.so..."
    rm ../../Implementation/MLP/Librairie/Linux/MLP_Linux.so
fi

# Checks if MLP_Linux.o exists
if [[ -f ../../Implementation/MLP/Librairie/Linux/MLP_Linux.o ]]; then
    echo "Deleting MLP_Linux.o..."
    rm ../../Implementation/MLP/Librairie/Linux/MLP_Linux.o
fi

# Checks if RBF_Linux.so exists
if [[ -f ../../Implementation/RBF/Librairie/Linux/RBF_Linux.so ]]; then
    echo "Deleting RBF_Linux.so..."
    rm ../../Implementation/RBF/Librairie/Linux/RBF_Linux.so
fi

# Checks if RBF_Linux.o exists
if [[ -f ../../Implementation/RBF/Librairie/Linux/RBF_Linux.o ]]; then
    echo "Deleting RBF_Linux.o..."
    rm ../../Implementation/RBF/Librairie/Linux/RBF_Linux.o
fi

echo "Creating Linear_Linux.o..."
g++ -c -fPIC ../../Implementation/Linear/Linear.cpp -o ../../Implementation/Linear/Librairie/Linux/Linear_Linux.o
echo "Creating Linear_Linux.so..."
g++ -shared -Wl,-soname,../../Implementation/Linear/Librairie/Linux/Linear_Linux.so -o ../../Implementation/Linear/Librairie/Linux/Linear_Linux.so ../../Implementation/Linear/Librairie/Linux/Linear_Linux.o

echo "Creating MLP_Linux.o..."
g++ -c -fPIC ../../Implementation/MLP/MLP.cpp -o ../../Implementation/MLP/Librairie/Linux/MLP_Linux.o
echo "Creating MLP_Linux.so..."
g++ -shared -Wl,-soname,../../Implementation/MLP/Librairie/Linux/MLP_Linux.so -o ../../Implementation/MLP/Librairie/Linux/MLP_Linux.so ../../Implementation/MLP/Librairie/Linux/MLP_Linux.o

echo "Creating RBF_Linux.o..."
g++ -c -fPIC ../../Implementation/RBF/RBF.cpp -o ../../Implementation/RBF/Librairie/Linux/RBF_Linux.o
echo "Creating RBF_Linux.so..."
g++ -shared -Wl,-soname,../../Implementation/RBF/Librairie/Linux/RBF_Linux.so -o ../../Implementation/RBF/Librairie/Linux/RBF_Linux.so ../../Implementation/RBF/Librairie/Linux/RBF_Linux.o

