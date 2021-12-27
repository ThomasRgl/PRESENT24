
Le but de ce projet est double. Il consiste tout d'abord en l'implémentation d'une version affaiblie de l'algorithme de chiffrement par bloc PRESENT, 
et appelé PRESENT24 pour sa version à 10 tours chiffrant des blocs de 24 bits.
Le second objectif de ce projet est d'implémenter une attaque par le milieu sur un message de 24 bits doublement chiffré.


Compilation: 

le fichier makefile présent à la racine permet de compiler le projet.
Il est possible de choisir la version que l'on souhaite utiliser (la version v1 semble plus performante )

la compilation avec make produit 2 fichier executables, present.out et fastPresent.out.
present.out est une version basique sans threads, et fastPresent est une version multithreadées plus rapide.


Benchmark:

Il est possible d'acceder au code ayant permi les tests de performances, il s'agit simplement d'une copie des codes du projet originel. 
Il se trouve dans le dossier src/benchmark
On y retrouve les tests pour la Sbox, la Pbox, le scheduleur de clef et l'attaque.
./launch compile,lance, et exectute le script gnuplot.

lien du git:
https://github.com/ThomasRgl/PRESENT24

références : 

https://tinycrypt.wordpress.com/2017/03/15/asmcodes-present/
https://iacr.org/archive/ches2007/47270450/47270450.pdf
https://github.com/yaspr/ycrypt/tree/master/present
