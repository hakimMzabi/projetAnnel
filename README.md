# Projet Annuel

### Présentation


**Thématique de l'application : Application permettant de différencier les drapeaux (photos) issus de différents pays**

Membres de l'équipe:
* Hakim MZABI (hakimMzabi)
* Théo HUCHARD (TheoHd)
* Baptiste VASSEUR (BaptisteVasseur)

<br>

### Compte-rendu


<br>

### Compiler la/les librairie(s)

Il faut tout d'abord se placer dans le répertoire du projet

```bash
cd ProjetAnnuel/
```


puis pour compiler le fichier .cpp avec ses dépendances :

#### Modèle Linéaire : 

* Windows : 

```bash
g++ -c Implementation/Linear/Linear.cpp && g++ -shared -o Librairie/Windows/Implementation/Linear/Linear.dll Librairie/Windows/Implementation/Linear/Linear.o -W
```

* Mac : 

```bash
g++ -c -std=c++17 Implementation/Linear/Linear.cpp -o Implementation/Linear/Librairie/Mac/Linear_Mac.o && 
g++ -shared -Wl -o Implementation/Linear/Librairie/Mac/Linear_Mac.so Implementation/Linear/Librairie/Mac/Linear_Mac.o
```

* Linux : 

```bash
g++ -c -std=c++17 Implementation/Linear/Linear.cpp -o Implementation/Linear/Librairie/Linux/Linear_Linux.o && 
g++ -shared -Wl -o Implementation/Linear/Librairie/Linux/Linear_Linux.so Implementation/Linear/Librairie/Linux/Linear_Linux.o
```

#### MLP : 

* Windows : 

```bash
g++ -c Implementation/MLP/MLP.cpp && g++ -shared -o Librairie/Windows/Implementation/MLP/MLP.dll Librairie/Windows/Implementation/MLP/MLP.o -W
```

* Mac : 

```bash
g++ -c -std=c++17 Implementation/MLP/MLP.cpp -o Implementation/MLP/Librairie/Mac/MLP_Mac.o && 
g++ -shared -Wl -o Implementation/MLP/Librairie/Mac/MLP_Mac.so Implementation/MLP/Librairie/Mac/MLP_Mac.o
```

* Linux : 

```bash
g++ -c -std=c++17 Implementation/MLP/MLP.cpp -o Implementation/MLP/Librairie/Linux/MLP_Linux.o && 
g++ -shared -Wl -o Implementation/MLP/Librairie/Linux/MLP_Linux.so Implementation/MLP/Librairie/Linux/MLP_Linux.o
```

#### RBF : 

* Windows : 

```bash
g++ -c Implementation/RBF/RBF.cpp && g++ -shared -o Librairie/Windows/Implementation/RBF/RBF.dll Librairie/Windows/Implementation/RBF/RBF.o -W
```

* Mac : 

```bash
g++ -c -std=c++17 Implementation/RBF/RBF.cpp -o Implementation/RBF/Librairie/Mac/RBF_Mac.o && 
g++ -shared -Wl -o Implementation/RBF/Librairie/Mac/RBF_Mac.so Implementation/RBF/Librairie/Mac/RBF_Mac.o
```

* Linux : 

```bash
g++ -c -std=c++17 Implementation/RBF/RBF.cpp -o Implementation/RBF/Librairie/Linux/RBF_Linux.o && 
g++ -shared -Wl -o Implementation/RBF/Librairie/Linux/RBF_Linux.so Implementation/RBF/Librairie/Linux/RBF_Linux.o
```

#### Librairie C + Python 

Il faut modifier la ligne d'importation de la librairie en fonction de votre OS dans les fichiers suivant : 
```cpp 
Implentation/Linear/Linear.cpp
Implentation/MLP/MLP.cpp
```

<br>

### Interface Web :

L'interface Web étant développé en DJango (Python) il n'est pas nécessaire d'avoir une installation de serveur Web au préalable. L'installation de Python et de DJango suffit. Pour accéder à l'interface Web du projet : 

```bash
python ProjetAnnuel/Interface/manage.py runserver
``` 
