# **cli-procedural-openworld ⚠️ (Unfinished) **

![](https://github.com/An0n1mity/cli-procedural-openworld/blob/main/capture.gif)

Réalisé par :
- Cédric MARTY
- Alexandre MONTHOUEL
- Dimitri RUSSO

### FONCTIONNALITES
- Système d'inventaire avec quantité d'objets limitée
- Système de crafting
- Affichage en couleur + utilisation de caractères UNICODE
- Affichage du monde progressif (système de chunk)
- Génération du monde intelligent (génération procédurale)
- Taille de la fenêtre modifiable en temps réel
- Map infini
- Affichage des stats du joueur en temps réel
- Jeu en temps réel

### CONTRÔLES
- Parcourir l'inventaire : ```FLECHE DROITE/GAUCHE``` ou ```MOLETTE```
- Déplacement du joueur : ```Z-Q-S-D```
- Utiliser un objet de l'inventaire : ```CLIQUE DROIT```
- Casser un objet de la map : ```CLIQUE GAUCHE```
- Valider un craft : ```ENTREE```
- Afficher le menu : ```CTRL+C``` ou ```F1```

### LISTE DES CRAFTS
- 3 planches de bois = 1 planche de surf -> permet de naviguer sur l'eau (avec la planche de surf craftée)

### LISTES DES STATS
- Vie
- Faim
- Soif
- Actions en cours
- Directions
- Possibilité de crafts en cours

### INSTRUCTIONS
#### Instructions de compilation :
- Utilisation d'un OS LINUX (de préférence UBUNTU + avoir GNOME d'installé pour un rendu plus joli)
- Avoir installé la librairie NCURSES
- Utilisation cmake pour une bonne compilation du jeu

#### Que faire si j'ai pas le nécessaire pour compiler le programme ?

- Dans le cas où je n'ai pas Linux ?
Dualboot ou VM (VirtualBox ou VMWare) sera la solution, pour l'installation, Google is your friend ;)

- Dans le cas où je n'ai pas la librairie NCURSES ?
Pour DEBIAN/UBUNTU c'est assez simple, il vous faudra taper les commandes suivantes :
    1. Pour mettre à jour vos packages :
    ```$ sudo apt-update```
    2. Pour installer NCURSES :
    ```$ sudo apt-get install libncurses5-dev libncursesw5-dev```

- Le Makefile ?
Oui oui, le Makefile, c'est assez simple, il suffit de taper les commandes suivantes pour tout compiler proprement :
```
$ cd projet
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./CProject
```



    

