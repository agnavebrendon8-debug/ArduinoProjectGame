# ArduinoProjectGame

🎮 Un projet Arduino implémentant le jeu classique **Snake** sur une matrice LED MAX7219.

## 📋 Description

Ce projet simule le jeu Snake classique sur une grille de LEDs. Le serpent se déplace sur une matrice LED 8x8 contrôlée par un joystick analogique. Le joueur doit diriger le serpent pour manger le fruit et grandir, tout en évitant de rentrer en collision avec les murs.

## 🎯 Caractéristiques

- **Contrôle par joystick analogique** : Direction intuitive du serpent
- **Système de score** : Le serpent grandit chaque fois qu'il mange un fruit
- **Animations** : 
  - Animation de démarrage : "SNAKE GAME" qui défile
  - Animation de fin de partie : "YOU LOSE ??"
- **Gestion dynamique du serpent** : Structure de liste chaînée pour gérer les segments du serpent
- **Fruit aléatoire** : Génération pseudo-aléatoire des positions du fruit
- **Difficulté progressive** : La vitesse du jeu augmente avec la taille du serpent

## 🔧 Matériel Requis

- Arduino UNO
- 4 matrices LED 8x8 MAX7219 (chaînées)
- Joystick analogique
- Fils de connexion
- Alimentation (5V)

## 📌 Brochage

| Composant | Arduino UNO |
|-----------|------------|
| Matrix DIN | 11 |
| Matrix CLK | 13 |
| Matrix CS | 10 |
| Joystick Vertical (Y) | A0 |
| Joystick Horizontal (X) | A1 |
| Joystick Button | 6 |

## 📂 Structure du Projet

```
SnakeLedGame/
├── codeC++.cpp          # Code principal du jeu
└── SnakeGame.json       # Configuration Wokwi pour simulation
```

## 🚀 Installation & Compilation

1. Télécharger la bibliothèque **LedControl** dans l'IDE Arduino
2. Ouvrir le fichier `codeC++.cpp` dans l'IDE Arduino
3. Sélectionner la carte : Arduino UNO
4. Téléverser le code sur l'Arduino

## 🎮 Utilisation

1. **Démarrer le jeu** : L'animation "SNAKE GAME" s'affiche au démarrage
2. **Diriger le serpent** : Utiliser le joystick analogique
   - Haut/Bas : contrôler le mouvement vertical
   - Gauche/Droite : contrôler le mouvement horizontal
3. **Objectif** : Manger les fruits (LED individuels) pour grandir
4. **Fin du jeu** : Toucher un mur ou sortir de la grille affiche "YOU LOSE ??"

## 📊 Configuration du Jeu

Les paramètres suivants peuvent être ajustés dans le code :

```cpp
const int nb_matrix = 4;        // Nombre de matrices LED chaînées
const int parametre = 10;       // Facteur de difficulté de vitesse
int Speed = 500;                // Vitesse initiale (ms)
const int high_lim = 800;       // Seuil joystick haut
const int min_lim = 200;        // Seuil joystick bas
```

## 🔍 Architecture du Code

### Structures de Données

- **Snakepoint** : Représente un segment du serpent (position + pointeurs prev/next)
- **Snake** : Liste chaînée contenant la tête, la queue et la taille
- **Fruit** : Position du fruit à manger

### Fonctions Principales

- `setup()` : Initialisation des matrices, joystick et démarrage
- `loop()` : Boucle principale du jeu
- `move_snake()` : Gère le mouvement et les collisions
- `print_snake()` : Affiche le serpent et le fruit sur les LEDs
- `createFruit()` : Génère une nouvelle position de fruit aléatoire
- `beginAnimation()` / `endAnimation()` : Animations de démarrage et fin

## 🧪 Simulation

Vous pouvez tester ce projet avec la simulation **Wokwi** en important le fichier `SnakeGame.json`.

## 📝 Notes Développement

- Le code utilise du **C procédural** avec structures et pointeurs
- Gestion manuelle de la mémoire (malloc/free)
- Affichage basé sur le contrôle bit par bit des LEDs
- Communication série pour le débogage

## 🤝 Contributions

Les contributions sont bienvenues ! N'hésitez pas à suggérer des améliorations :
- Optimisation du code
- Nouvelles fonctionnalités
- Amélioration des performances

## 📄 Licence

Libre d'utilisation pour fins éducatives.

---

**Auteur** : Brendon  
**Date de création** : 2024  
**IDE** : Arduino IDE / Wokwi
