[![Build Status](https://api.travis-ci.org/samogwas/samogwas.png)](https://travis-ci.org/samogwas/samogwas)

# samogwas
samogwas

**Description du contenu:**

* ./apps/               : le dossier générant les exécutables avec les options adaptées(i.e. création des exécutables FLTM et GWAS)
* ./core/               : le coeur de l'application (information mutuelle, EM, tests statistiques)
* ./dependencies/       : les dépendances externes à l'application (Boost et ProBT)
* ./tests/              : les tests de l'application
* ./cmake/
* ./examples/           : dossier contenant des données test pour l'application
* ./CMakeLists.txt      : permet de générer les Makefiles

**Comment compiler?**

Lancer le script ./compile, générant les exécutables dans un nouveau dossier build/

**Comment exécuter l'application?**

Lancer l'exécutable ./fltm avec les paramètres adaptés

**Exemple:**

```sh
$ ./fltm -d ../example/inputs/data/dat_300.csv -l ../example/inputs/labels/lab_300.csv
```
Vous pouvez trouver des exemples des fichiers dat.csv, lab.csv et clustering.cfg dans le dossier ./examples/
