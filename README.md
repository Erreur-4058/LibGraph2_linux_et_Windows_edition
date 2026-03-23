# LibGraph2 - Version Linux (SFML) 🚀

**LibGraph2** est une bibliothèque graphique simplifiée conçue pour l'apprentissage de la programmation C++. Initialement développée pour Windows, cette version a été portée sous **Linux** en utilisant le moteur **SFML**.

Elle permet aux étudiants de passer de Windows à Linux sans modifier une seule ligne de leur code source original.

---

## ✨ Caractéristiques
- **Compatibilité Totale** : Supporte `WinMain`, `Windows.h` et les types Windows (`HINSTANCE`, `ARGB`, etc.) via une couche de compatibilité intégrée.
- **Moteur SFML** : Rendu fluide et performant sur Linux.
- **Niveau 0 à 4** : Support complet des différents niveaux d'expérience LibGraph2.
- **Mapping ASCII** : Les codes de touches clavier (`e.vkKeyCode`) sont synchronisés avec les codes ASCII standards.

---

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
- `Windows.h` & `tchar.h` (indispensables pour la compatibilité)

### 2. Compilation de votre programme
Utilisez la commande suivante pour lier votre code à LibGraph2 et SFML :

```bash
g++ main.cpp -L. -lLibGraph2 -lsfml-graphics -lsfml-window -lsfml-system -o mon_programme
```

### 3. Exécution
Sous Linux, le système ne cherche pas les bibliothèques dans le dossier courant par défaut. Utilisez cette commande :

```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
./mon_programme
```

---

## 💻 Exemple de Code Portable
Ce code est 100% compatible Windows (Visual Studio) et Linux (g++) :

```cpp
#include <Windows.h>
#include <tchar.h>
#include <LibGraph2.h>
using namespace LibGraph2;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    show();
    while(waitForEvent()) {
        if(getLastEventType() == evt_type::evtRefresh) {
            beginPaint();
            setSolidBrush(MakeARGB(255, 255, 0, 0)); // Rouge
            drawEllipse(100, 100, 200, 200);
            endPaint();
        }
    }
    ReleaseLibGraph2();
    return 0;
}
```

---

## 📝 Structure du projet
- `CMakeLists.txt` : Script de configuration pour la compilation automatisée.
- `LibGraph2.h` : En-tête principal corrigé pour Linux.
- `LibGraph2Common.cpp` : Logique commune de la bibliothèque.
- `LibGraph2impSFML.cpp` : Implémentation du moteur de rendu via SFML.
- `Windows.h` & `tchar.h` : Couche de compatibilité Windows.

---

## ⚖️ Licence
Ce projet est distribué sous licence **GPLv3**.
Copyright © 2010-2024 Benjamin ALBOUY-KISSI.

Portage Linux réalisé par **Jeremi Roux** (avec l'assistance d'Antigravity).
