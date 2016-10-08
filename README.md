De grep à l'antivirus
=====================
Ce projet a pour but de développer des applications permettant de mettre en oeuvre les connaissances acquises du cours d'Algorithme du texte lors de ma formation en Master 1 Informatique à l'[Université Pars 13](https://www.univ-paris13.fr/).

SGrep
-----
**Simple Grep** permets de la détection d'un motif dans un fichier texte en utilisant un automate de localisation. C'est en quelque sorte une réplique de la commande [grep](http://www.linux-france.org/article/man-fr/man1/grep-1.html).

ACGrep
------
**ACGrep** permets de détecter un ensemble de mots dans un repertoire à l'aide de l'automate d'Aho-Corasick.

ACBGrep
-------
**Binary Grep** fait quasiment la même chose que le programme précédement, cependant avec des fichiers binaires.

Bonus
-----
`client.c` et `server.h` sont des programmes respectivement client et serveur. Ils simulent brièvement le fonctionnement d'un Firewall. Le client envoie au serveur un fichier binaire quelconque, tandis que le serveur reçoit le contenu de ce dit fichier à travers un "flux" de donnée et vérifie s'il n'y a pas de motif suspect apparaissant.

Essayez !
---------
````bash
git clone https://github.com/DamiX/Algo_texte_2016.git
cd Algo_texte_2016
make
./sgrep -n -l horloge txt/tdm.txt
./sgrep "Quelques jours" txt/sodome_et_gomorrhe_MARCEL_PROUST.txt
./acgrep txt/liste_mots.txt src/
./acbgrep txt/bin_bdd.txt /bin/
./acbgrep txt/bin_bdd.txt .
./server txt/bin_bdd.txt
./client /bin/ls
````

Limitation
----------
* Uniquement fonctionnel sur système Unix
* Supporte uniquement l'ASCII (7 bits)

Décision
--------
Le langage de programmation choisi est le C, tout simplement parce qu'au moment de la réalisation de ce projet, mes connaissances en C++ n'était pas assez riche.
L'histoire du C++ peut laisser prétendre à une simple évolution du C, du C orienté objet avec quelques fonctionnalités en plus. C'est vrai qu'il peut être utilisé de la sorte, cependant ce n'en est pas une utilisation optimale (selon Bjarne Stroustrup).
Pour plus d'informations, je vous conseille le merveilleux livre de l'un de ses créateurs : [The C++ Programming Language Forth Edition](http://www.amazon.com/The-Programming-Language-4th-Edition/dp/0321563840).

Parlons encodage ! Aucune contrainte n'a été définie sur le type d'encodage dont les applications serai capable d'interpreter. Pendant longtemps il y a eu beaucoup de divergence concernant l'encodage des caractères. Aujourd'hui il existe toujours des développeurs pensant qu'un caractère ce code sur 1 octet ou que l'Unicode se code sur 16 bits.
FAUX ! comme dirait Norman. Je ne vous ferai pas de cours complet à ce sujet, mais voici [un article intéressant](http://www.joelonsoftware.com/articles/Unicode.html).
Dans le cadre de ce devoir, le choix à été fait en faveur de l'ASCII... Pour des raisons de simplicité.

Credits - Contributeurs
-----------------------
[Damien Mehala](mailto:damien.mehala@me.com) / Professeur : [Julien David](https://lipn.univ-paris13.fr/~david/)

Licence
-------
This program is free software: you can redistribut it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

You should have received a copy of the GNU General Public License along with this program. If not, see : [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/)
