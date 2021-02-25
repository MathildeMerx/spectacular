# Spectacular

Spectacular est un programme de cadre photo numérique. Il affiche des images PNG après lecture d'une puce RFID. Si aucune image n'est associée à la puce RFID lue, l'utilisateur a la possibilité d'en ajouter depuis le site web hébergé sur le Raspberry Pi.

Le chemin utilisé pour développer ce projet sous Raspberry Pi est : `/home/pi/spectacular`

## Organisation projet

```
spectacular
|____ bin		: exécutables
|____ files		: fichiers de configuration
|____ img		: images à afficher
|____ include   : fichiers d'entête C
|____ lib		: bibliothèque C
|____ script  : scripts python
|____ src		: fichiers sources C du projet
|____ www		: fichiers web du projet
```

## Compilation et exécution

Pour reproduire ce projet, assurez-vous d'avoir un serveur apache fonctionnel sur Raspberry-Pi et ajoutez la configuration de l'hôte virtuelle décrite dans Configuration Apache (il se peut qu'il faille la modifier selon le répertoire dans lequel vous placez ce projet).

Puis veillez à installer la bibliothèque SPI-Py pour être en mesure de lire les puces RFID.

Dans certains cas, il pourrait être nécessaire de recompiler le programme fbvs. Veillez à le suffixer de _pc ou _pi selon le cas.

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
