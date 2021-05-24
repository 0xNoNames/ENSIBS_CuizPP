# Cuiz++
Cuiz++ est un jeu multijoueur de culture général sous forme de QCM.  
Plusieurs types de questions sont disponible : du texte, des images ou encore des sons.

## Installation
- Windows
    - Il suffit de lancer le fichier Cuiz++.exe et de choisir le chemin d'installation.
    - Pour ouvrir le serveur sur internet, il faut au préalable ouvrir les ports 25565 en TCP.    
- Mac / Ubuntu
    - Il faut modifier les lignes suivantes et mettre le chemin absolu des fichiers json : 
        - Cuizpp_client/banquequestions.cpp -> ligne 129  
        - Cuizpp_client/client.cpp -> ligne 404, 415, 427  
        - CuizPP_Server/banquequestions.cpp -> ligne 94     
    - Puis compiler chaque sous-programmes. (Il suffit d'ouvrir le .pro à la racine du projet général)  
## Jouer
Pour jouer, il suffit de lancer le fichier "Cuiz++\CuizPP_Server\CuizPP_Server.exe" pour lancer un serveur.  

Ensuite, chaque joueurs ouvrent le fichier "Cuiz++\CuizPP_Client\CuizPP_Client.exe" pour démarrer le client et indiquent l'adresse IP et le nom de l'utilisateur souhaité.  

Enfin, il suffit de créer une partie et vous pouvez vous amuser !  

La banque de questions n'ayant pas été remplie, il faut seulement lancer une partie avec 5 question maximum !  
## Membres
- PETRE Arthur
- JULLION François
