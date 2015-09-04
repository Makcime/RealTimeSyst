//
//  MA1INFO_TpsReel_Examen2015.h
//
//
//
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <cmath>


#ifndef Semaphores_LecteurRedacteur_MA1INFO_TpsReel_Examen2015_h
#define Semaphores_LecteurRedacteur_MA1INFO_TpsReel_Examen2015_h



// Constantes de temps

#define DUREE_EXECUTION_S       20

#define TEMPS_LECTURE_MIN_S     (0.2)
#define TEMPS_LECTURE_MAX_S     4

#define TEMPS_ECRITURE_MIN_S    (0.4)
#define TEMPS_ECRITURE_MAX_S    (0.5)


// Structures de description des lecteurs et des rédacteurs
typedef struct redacteur {
    int id;
    int valeurRessource;
} redacteur;

typedef struct lecteur {
    int id;
} lecteur;


// Fonctions exécutées au début, et à la fin du programme
void initialisationProgramme();
void fermetureProgramme();


// Fonctions appelées dans des threads séparés (depuis main.cpp)
void* evenementNouveauLecteur(void* nouveauLecteurVoid);
void* evenementNouveauRedacteur(void* nouveauRedacteurVoid);

// La lecture et l'écriture doivent être effectuées à partir de ces fonctions
void lireRessource(lecteur* lecteurActuel);
void ecrireRessource(redacteur* redacteurActuel);

// Ressource accessible par tous, à protéger
extern int ressourceGlobale;
extern pthread_mutex_t mutConsole; 
extern pthread_mutex_t mutRessourceGlobale;

#endif
