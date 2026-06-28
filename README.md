# Analyse_Cache_Misses L1/L2 entre 2 threads vs 2 processus
Il s'agit d'un projet academique en systeme d'exploitation dont l'intitulé est <<: Analyse de l'impact des Threads vs Processus sur la Consommation de Cache L1/L2>>.   
Ce projet réaliseé sur Linux tente de repondre fidelement a la problematique suivante : Dans quelle mesure les changements de contexte influencent-ils les taux de défaut des caches L1 et L2, et comment cet impact varie-t-il selon que l'exécution repose sur 2 processus ou sur 2 threads ?  
Pour le realiser, nous avons successivement executé des codes qui créent de la charge de travail au processeur (1 cas pour l'utilisation de 2 processus, 1 autre cas pour l'utilisation de 2 thread) et nous avons consigé dans un dataset, l'ensemble des variables pertinentes a l'etude. Il s'agit de : 
-	Nombre de tour ( avec la boucle for() lié à la constante YIELD_FREQ)
-	Cache_Load_L1 : Nombre de requête du processeur dans la cache L1
-	Cache_Miss_L1 : Nombre de caches misses Sur le cache L1
-	Taux d’échec L1 : Pourcentage de perte calculé en divisant le Cache_Load_L1 et le Cache_Miss_L1
-	Cache_Load_L2 : Nombre de requête du processeur dans la cache L2
-	Cache_Miss_L2 : Nombre de caches misses Sur le cache L2
-	Taux d’échec L2 : Pourcentage de perte calculé en divisant le Cache_Load_L2 et le Cache_Miss_L2
-	Temps d’exécution
-	Nbre_Changement de contexte

# :clipboard: Structure du projet
```
DALIL/
├── IMAGE_ANALYSE/
|   ├── Vue_Dataframe.png
|   ├── scaterplot_L1.png
|   ├── scaterplot_L2.png
|   ├── Taux_echec.png
|   ├── Box_plot_Thread.png
|   ├── Box_plot_Processus.png
|   ├── Vue_control_changement_contexte.png
|   ├── heatmap_processus.png
|   └── heatmap_thread.png
|
├── IMAGE_EXECUTION_CODE/
|   ├── screen1.png
|   ├── screen2.png
|   ├── screen3.png
|   └── screen4.png
|
|
├── process_cache.c
├── thread_cache.c
├── note_book_etude.ipynb
├── Donnée_excel.xls
├── Rapport.pdf
└── README-PROJET
```
# 🚀 PRE-REQUIS DE DEMARRAGE

- une machine avec Linux(Ubuntu) installé et mis a jour
- LE compilateur GCC
- Les autorisations Root

⚠️ Important : Il est fortement déconseillé d'utiliser une Machine Virtuelle ceci pour eviter de biaiser l'etude sur les données a travers le passage par l'hyperviseur   
Pour installer GCC et mettre a jour votre machine : 
```
sudo apt update
sudo apt upgrade
sudo apt install gcc

```  
# 📚 DOCUMENTATION PAR PARTIE
## Partie 1 Methodologie experimentale
Ici nous nous interessont au vif du projet. On prepare les outils de monitoring et d'analyse : 
- La commande taskset() pour forcer l’utilisation d’un seul c ur lors de la capture
- La commande perf() necessaire pour voir les statistiques de traitement des caches
- La commande fork() pour dupliquer le processus parent a n de creer plusieurs processus enfants
- Le procede de creation de thread selon la norme POSIX comme vus plus haut
- La commande sched_yield() pour provoquer les changements de contexte
- Python pour une analyse statistique exploratoire
### Objectif : 
- Preparer l'environnement et les outils de monitoring
- presenter les etapes de deroulement
- Faire la mise en oeuvre

## Partie 2 - Resultat et discussion
![alt text](<Capture d'écran 2026-06-27 121149.png>)
![alt text](./"image analyse"/Taux_echec.png)
![alt text](heatmap_processus.png)
![alt text](heatmap_thread.png)
![alt text](scaterplot_L1.png)
 Il s'agira de discuter sur les resultats obtenus ainsi que sur les constats fait.
## Partie 3 - Conclusio
### Objectif : 
Faire une conclusion de l'etude. 
