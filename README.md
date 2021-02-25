# Spectacular

Spectacular est un programme de cadre photo numérique. Il affiche des images PNG après lecture d'une puce RFID. Si aucune image n'est associée à la puce RFID lue, l'utilisateur a la possibilité d'en ajouter depuis le site web hébergé sur le Raspberry Pi.

Le chemin utilisé pour développer ce projet sous Raspberry Pi est : `/home/pi/spectacular`

## Organisation projet

```
spectacular
|____ bin		: exécutables
|____ files		: fichiers de configuration
|____ img		: images à afficher
|____ include           : fichiers d'entête C
|____ lib		: bibliothèque C
|____ script            : scripts python
|____ src		: fichiers sources C du projet
|____ www		: fichiers web du projet
```

## Compilation et exécution

Pour reproduire ce projet, assurez-vous d'avoir un serveur apache fonctionnel sur Raspberry-Pi et ajoutez la configuration de l'hôte virtuelle disponible dans le dossier files (il se peut qu'il faille la modifier selon le répertoire dans lequel vous placez ce projet).

Puis veillez à installer la bibliothèque SPI-Py (https://github.com/lthiery/SPI-Py) pour être en mesure de lire les puces RFID.

Dans certains cas, il pourrait être nécessaire de recompiler le programme fbvs (https://github.com/kaihs/fbvs). Veillez à le suffixer de _pc ou _pi selon le cas.

Enfin rendez-vous dans le répertoire src puis lancez le makefile.
```
make
-- compile spectacular et place l'exécutable dans bin
-- efface le résultat d'une précédente génération avec make

make VERBOSE=yes
-- affiche les commandes du makefile et les exécute

make CROSS=yes
-- compile spectacular pour votre PC et pour Raspberry Pi mais nécessite que
-- le répertoire du compilateur croisé arm-linux-gnueabihf-gcc soit dans le PATH

make PI=yes
-- option à activer pour compiler spectacular sur Raspberry Pi

make clean
-- efface les fichier générés 
```

Pour exécuter le programme C, rendez-vous dans le répertoire bin (cette étape est nécessaire car le programme utilise des chemins relatifs lors de son exécution) et lancez l'exécution de spectacular.
```
cd /home/pi/bin
./spectacular
```

## Description des programmes

### Fichiers source C

Le fichier ``main.c`` lance le cadre photo via la fonction ``run``.

A son tour, lorsque ``run`` est exécuté, il lance crée un processus fils, infini (l'utilisateur devra donc fare un Ctrl+C pour arrêter ce programme). 

Le processus fils lit l'ID de la carte badgée (d'après un script Python). 

Le processus père, lui, qui ouvre un menu (grâce à la librairie nCurses). Ici, l'utilisateur peut régler 4 paramètres : 3 fixant la taille de l'image (si elle doit prendre toute la largeur de l'écran, toute sa hauteur, ou seulement sa taille réelle), le dernier déterminant le nombre de secondes durant lequel l'image doit s'afficher. Les 3 premiers paramètres seront (de)activés lorsque l'utilisateur appuyera sur la touche Entr, le 4e sera modifié grâce aux flèches latérales. Ces paramètres et leurs choix sont gérés par la fonction ``parameters.c``.

Enfin, le processus père (toujours dans ``run.c``) vérifie si l'ID de la carte correspond à une image ou non. Si oui, il l'affiche (en utilisant le programme suivant). Si non, l'utilisateur a la possibilité dans l'interface web d'en télécharger une (en utilisant le programme ``file.c``, voir ci-dessous). 

Le programme ``image.c`` s'occupe d'afficher l'image liée à la carte, avec les paramètres choisis par l'utilisateur. 

Enfin, le programme ``file.c`` cherche dans le fichier de correspondance ID carte <-> fichier image, si la carte est bien liée à une image. Si oui, il lance l'affichage. Si non, il demande à l'utilisateur d'en télécharger une. 

### Fichiers web
On trouve tout d'abord le fichier ``index.html``. Il s'agit du front utilisé pour demander (potentiellement) à l'utilisateur d'ajouter une photo. 

Il y a également le fichier ``img_upload.ph``. Celui-ci se charge de gérer le téléchargement d'image: lorsque l'utilisateur envoie un fichier PNG, le programme lájouter à un dossier img. De plus, il ajoute dans le fichier match (fichier de correspondance ID carte <-> fichier image) la ligne ``[dernière ID de carte lue]:[nom de l'image dernièrement téléchargée]``. Pour éviter les doublons de nom d'image, le programme génère lui-même un identifiant unique pour chaque photo. 

### Script Python

Ici encore, deux fichiers. Ils viennent tous deux du repo github de Pimylifeup, et permettent de lire une puce RFID. 

