De grep à l'antivirus
=====================
Ce projet s'appuie sur les connaissances du cours d'Algorithme du texte lors de ma formation en Master 1 Informatique à l'[Université Pars 13](https://www.univ-paris13.fr/).

TO DO
-----
 * Réécrite complètement `automate_det` et `algo`.
 * Améliorer l'affichage de acgrep et acbgrep.

Décision
--------
Aucune contrainte n'a été définie sur le type d'encodage dont les applications serai capable de gérer. Aujourd'hui le type d'encodage est vaste : UTF-8, Wide characteres, Multibyte ... Chacun ayant leurs avantages et inconénient.

Le choix s'est aisément porté vers le codage [ASCII](https://fr.wikipedia.org/wiki/American_Standard_Code_for_Information_Interchange) principalement pour sa simplicité (Il définit 128 caractères pouvant être codés sur 1 octet. À priori le type atomique char suffira). 

SGrep
-----
**Simple Grep** permets de la détection d'un motif passé en argument dans un fichier texte en utilisant un automate de localisation. C'est en quelque sorte une petite réplique de la commande [grep](http://www.linux-france.org/article/man-fr/man1/grep-1.html).

ACGrep
------
**ACGrep** permets de détecter un ensemble de mots à partir d'un fichier passé en argument dans une liste de fichier.

ACBGrep
-------
**Binary Grep** fait exactement la même chose que le programme précédement, cependant avec des fichiers binaires.

Bonus
-----
`client.c` et `server.h` sont des programmes respectivement client et serveur. Il simule brièvement le fonctionnement d'un Firewall.

Essayer !
---------
````bash
git clone https://github.com/DamiX/Algo_texte_2016.git
cd Algo_texte_2016
make
````

Credits - Contributeurs
-----------------------
[Damien Mehala](mailto:damien.mehala@me.com) / Professeur : [Julien David](https://lipn.univ-paris13.fr/~david/)

Licence
-------
This program is free software: you can redistribut it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

You should have received a copy of the GNU General Public License along with this program. If not, see : [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/)
