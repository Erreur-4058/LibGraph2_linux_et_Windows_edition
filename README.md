# LibGraph2 - Édition Multiplateforme (Windows & Linux) 🚀

**LibGraph2** est une bibliothèque graphique simplifiée conçue pour l'apprentissage de la programmation C++.

Initialement développée pour Visual Studio sous Windows, cette version a été portée sur le moteur **SFML** pour offrir une compatibilité **100% transparente entre Windows et Linux**, sans avoir à modifier le code source de l'application !

---

## 📁 Structure du dépôt

Le projet est divisé en deux environnements de compilation pour s'adapter à votre système d'exploitation :

```text
📦 LibGraph2_linux_et_Windows_edition
 ┣ 📂 linux/                # Environnement pour Linux
 ┃ ┣ 📜 CMakeLists.txt      # Script CMake pour Linux (utilise SFML système)
 ┃ ┣ 📜 Windows.h           # Faux header pour la compatibilité Linux
 ┃ ┗ 📜 tchar.h             # Faux header pour la compatibilité Linux
 ┣ 📂 Windows/              # Environnement pour Windows
 ┃ ┣ 📜 CMakeLists.txt      # Script CMake Windows (télécharge SFML 2.6.2 auto)
 ┃ ┗ 📜 msys2-x86_64.exe    # Installateur MSYS2 fourni pour faciliter l'installation
 ┣ 📜 LibGraph2Common.cpp   # Code source partagé de la bibliothèque
 ┣ 📜 LibGraph2impSFML.cpp  # Code source partagé de la bibliothèque
 ┣ 📜 LibGraph2.h           # Header partagé de la bibliothèque
```


---

## 🪟 Guide de compilation pour WINDOWS

Sous Windows, nous utilisons **MSYS2** (MinGW UCRT64) pour compiler avec GCC et CMake, exactement comme sous Linux.

### 1. Installation des prérequis
1. Installez MSYS2 en utilisant l'exécutable fourni dans le dossier `Windows` (`msys2-x86_64.exe`).
2. Ouvrez le terminal **MSYS2 UCRT64** (Cherchez "UCRT64" dans votre menu Démarrer).
3. Installez les outils de compilation avec cette commande :
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake make
   ```
*(Note : Appuyez sur Y pour valider l'installation).*

### 2. Compilation
Dans le terminal UCRT64, notez que votre disque C:\ est accessible via le chemin /c/.

Important : Avant de compiler, vous devez copier le fichier CMakeLists.txt situé dans le dossier Windows/ et le placer à la racine du projet (là où se trouvent les fichiers .cpp).

```bash
# 1. Se déplacer à la racine du projet
# Exemple si votre projet est sur le bureau : cd /c/Users/NomUtilisateur/Desktop/LibGraph2_linux_et_Windows_edition

# 2. Sortir le CMakeLists du dossier Windows vers la racine
cp Windows/CMakeLists.txt .

# 3. Créer le dossier de build
mkdir build
cd build

# 4. Configurer et télécharger SFML
cmake -G "MSYS Makefiles" ..

# 5. Compiler l'application
make
```

### 3. Récupération des DLLs (Important)
Sous Windows, l'exécutable a besoin des bibliothèques (`.dll`) juste à côté de lui pour se lancer. Toujours dans le dossier `build`, tapez ces deux commandes :
```bash
# Copier les DLLs de SFML
cp _deps/sfml-build/lib/*.dll .

# Copier les DLLs systèmes du C++
cp /c/msys64/ucrt64/bin/libstdc++-6.dll /c/msys64/ucrt64/bin/libgcc_s_seh-1.dll /c/msys64/ucrt64/bin/libwinpthread-1.dll .
```

### 4. Lancement
Vous pouvez maintenant double-cliquer sur votre `.exe` depuis l'explorateur de fichiers Windows, ou le lancer via le terminal :
```bash
./test_libgraph2.exe
```


---

## 🐧 Guide de compilation pour LINUX (Ubuntu/Debian)

## 🛠️ Installation des dépendances

Avant de compiler, vous devez installer la bibliothèque **SFML** et **CMake** sur votre système Linux.

Sur **Ubuntu / Debian / Mint** :
```bash
sudo apt update
sudo apt install libsfml-dev cmake g++
```

---

## 🏗️ Compilation de la Bibliothèque

### 1. Méthode recommandée (CMake)
Important : Avant de compiler, vous devez sortir le fichier CMakeLists.txt situé dans le dossier linux/ pour le placer à la racine du projet.

Puisque le projet utilise CMake, voici la procédure standard pour générer le fichier `libLibGraph2.so` :

```bash
# Créez un dossier de build pour ne pas polluer les sources
mkdir build && cd build

# Générez le Makefile et compilez
cmake ..
make
```

### 2. Méthode manuelle (g++)
Si vous préférez compiler la bibliothèque en une seule ligne sans utiliser CMake :

```bash
g++ -shared -fPIC LibGraph2Common.cpp LibGraph2impSFML.cpp -o libLibGraph2.so -lsfml-graphics -lsfml-window -lsfml-system
```

---

## 🚀 Utilisation dans votre projet

### 1. Préparation
Une fois la compilation terminée, copiez les fichiers suivants dans le dossier de votre propre programme :
- `LibGraph2.h`
- `libLibGraph2.so`
- `Windows.h` & `tchar.h` (Ces deux fichiers se trouvent dans le dossier linux/ du dépôt et sont indispensables pour la compatibilité).

### 2. Compilation de votre programme
Utilisez la commande suivante pour lier votre code à LibGraph2 et SFML :

```bash
g++ main.cpp -L. -lLibGraph2 -lsfml-graphics -lsfml-window -lsfml-system -o test_libgraph2
```

### 3. Exécution
Sous Linux, le système ne cherche pas les bibliothèques dans le dossier courant par défaut. Utilisez cette commande :

```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
./test_libgraph2
```
---

## ⚖️ Licence
Ce projet est distribué sous licence **GPLv3**.
Copyright © 2010-2024 Benjamin ALBOUY-KISSI.

Portage Linux réalisé par **Jeremi Roux** (avec l'assistance d'Antigravity).
