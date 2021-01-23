De grep à l'antivirus
=====================
Ce projet a pour but de développer des applications permettant de mettre en oeuvre les connaissances acquises du cours d'Algorithme du texte lors de ma formation en Master 1 Informatique à l'[Université Pars 13](https://www.univ-paris13.fr/).

SGrep
-----
**Simple Grep** permets de la détection d'un motif dans un fichier texte en utilisant un automate de localisation. C'est en quelque sorte une réplique de la commande [grep](http://www.linux-france.org/article/man-fr/man1/grep-1.html).

````bash
$ ./build/bin/sgrep
Usage: ./build/bin/sgrep [options] pattern file
Search for pattern in file.
    Options:
    -n      Print number of matches
    -l      Print line number containing match
    -i      Ignore case-sensitivity

$ ./build/bin/sgrep -n -l horloge txt/tdm.txt
l.496 - magnifique horloge, posee au-dessus du " drawing-office", sonna
l.1190 - sonner l'horloge du port.
l.1984 - pendule d'une horloge astronomique, il se dirigea vers le bureau
l.3606 - horloge du pretoire.
l.6354 - horlogerie!  J'etais bien sur qu'un jour ou l'autre, le soleil
l.6358 - eut ete divise en vingt-quatre heures comme les horloges
l.9347 - Une heure sonna a l'horloge de Custom-house.  Mr. Fogg constata
l.9348 - que sa montre avancait de deux minutes sur cette horloge.
l.9406 - moins dix sonnaient a toutes les horloges de Londres.
l.9722 - Au moment ou l'horloge du grand salon marqua huit heures
l.9775 - En ce moment l'horloge du salon sonna huit heures quarante.
l.9794 - leur regard se fixait sur l'horloge.  On peut affirmer que,
l.9804 - l'horloge battait la seconde avec une regularite mathematique.
l.9892 - L'horloge marquait huit heures quarante-cinq, quand il parut
Found: 14 occurrences
````

ACGrep
------
**ACGrep** permets de détecter un ensemble de mots dans un repertoire à l'aide de l'automate d'Aho-Corasick.

````bash
$ cat txt/liste_mots.txt
stdio
stdlib
memset
printf

$ ./build/bin/acgrep txt/liste_mots.txt src/
=================
File: acgrep.c
=================
l.1 -  #include <stdio.h>
l.2 -  #include <stdlib.h>
l.16 -          printf("usage: %s file directory\n", appname);
l.34 -                  fprintf(stderr, "%s : fichier \"%s\" introuvable\n", __file__, fichier);
l.72 -                  fprintf(stderr, "%s: impossible d'acceder au repertoire \"%s\"\n", __file__, repertoire);
l.79 -          memset(buffer, 0, buffer_size);
l.85 -                          memset(path, 0, buffer_size);
l.91 -                                  fprintf(stderr, "%s : fichier \"%s\" introuvable\n", __file__, cur_file->d_name);
l.108 -                                                 if(tmpocc == 0) {printf("=================\nfichier: %s\n=================\n", cur_file->d_name);}
l.110 -                                                 printf("\x1b[1;36ml.%d\x1b[0m -  %s\n", line, buffer);
l.116 -                                         memset(buffer, 0, buffer_size);
l.135 -                 //printf("\n");
=================
File: algorithm.c
=================
l.13 -                  memset(bord, 0, sizeof(int)*sz);
l.106 -         memset(*alphabet, sentinel_value, alphabet_size);
l.163 -                 memset(m->correspondance[i], 0, 128);
l.206 -         memset(failure, 0, sizeof(int)*dx->nstates);
=================
File: automata.c
=================
l.7 -           fprintf(stderr, "%s: impossible d'allouer de la mémoire\n%s\n", __file__, strerror(errno));
l.18 -                          fprintf(stderr, "%s: impossible d'allouer de la mémoire\n%s\n", __file__, strerror(errno));
l.22 -                  memset(*(matrice+i), -1, sz_int_netats);
l.53 -                  printf("etat %d:\t", i);
l.56 -                          printf("%d\t", m->mattransition[i][j]);
l.58 -                  printf("\n");
=================
File: queue.c
=================
l.10 -                  // fprintf(stderr, "%s: allocation impossible", __file__);
l.35 -                  // fprintf(stderr, "%s: allocation impossible", __file__);
=================
File: simple_grep.c
=================
l.1 -  #include <stdio.h>
l.2 -  #include <stdlib.h>
l.28 -          printf("usage: %s [options] pattern file\n \
l.99 -                  fprintf(stderr, "%s : fichier \"%s\" introuvable\n", __file__, input_file);
l.106 -         memset(buffer, 0, sz_buffer);
l.122 -                                         printf("\x1b[1;36ml.%d\x1b[0m - ", line);
l.125 -                                 printf("%s\n", buffer);
l.131 -                         memset(buffer, 0, sz_buffer);
l.151 -                 printf("found: %d occurrences\n", occ);
````

ACBGrep
-------
**Binary Grep** fait quasiment la même chose que le programme précédement, cependant avec des fichiers binaires.

Bonus
-----
`client.c` et `server.h` sont des programmes respectivement client et serveur. Ils simulent brièvement le fonctionnement d'un Firewall. Le client envoie au serveur un fichier binaire quelconque, tandis que le serveur reçoit le contenu de ce dit fichier et vérifie s'il n'y a pas de motif suspect apparaissant.

Essayez !
---------
````bash
git clone https://github.com/DamiX/Algo_texte_2016.git
cd Algo_texte_2016
make
````

Limitation
----------
* Compile & run mainly on Unix system
* Only support ASCII charset

Décision
--------
Le langage de programmation choisi est le C, tout simplement parce qu'au moment de la réalisation de ce projet, mes connaissances en C++ n'étaient pas assez riches.
L'histoire du C++ peut laisser prétendre à une simple évolution du C, du C orienté objet avec quelques fonctionnalités en plus. C'est vrai qu'il peut être utilisé de la sorte, cependant ce n'en est pas une utilisation optimale.
Pour plus d'informations, je vous conseille le livre de l'un de ses créateurs : [The C++ Programming Language Forth Edition](http://www.amazon.com/The-Programming-Language-4th-Edition/dp/0321563840).

Parlons encodage ! Aucune contrainte n'a été définie sur le type d'encodage dont les applications seraient capable d'interpréter. Pendant longtemps il y a eu beaucoup de divergence concernant l'encodage des caractères. Aujourd'hui, il existe toujours des développeurs pensant qu'un caractère ce code sur 1 octet ou que l'Unicode se code sur 16 bits.
FAUX ! Je ne vous ferai pas de cours complet à ce sujet, mais voici [un article intéressant](http://www.joelonsoftware.com/articles/Unicode.html).
Dans le cadre de ce devoir, le choix à été fait en faveur de l'ASCII... Pour des raisons de simplicité.

Credits
-----------------------
[Julien David](https://lipn.univ-paris13.fr/~david/)

Licence
-------
This program is free software: you can redistribut it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

You should have received a copy of the GNU General Public License along with this program. If not, see : [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/)
