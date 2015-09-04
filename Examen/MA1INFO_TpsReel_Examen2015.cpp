

#include <iostream>
#include <cstdlib> // entre autres : rand()

#include "MA1INFO_TpsReel_Examen2015.h"


/* Fonction appelée au lancement du programme
 */
void initialisationProgramme() {
    // Implémenter ici l'initialisation de vos variables, la création
    // de vos objets système, etc.

    /* initialisation du scheduler en mode FIFO et a la priorité max*/ 
    int priority = 0;
    int pid = getpid();
    priority = sched_get_priority_max(SCHED_FIFO);
    struct sched_param param;
    param.sched_priority = priority;
    if (sched_setscheduler(pid, SCHED_FIFO, & param) != 0) {
        perror("sched_setscheduler");
    exit(EXIT_FAILURE);  
    }

    /* creation et initialisation des mutex pour la console et la ressource*/
    pthread_mutexattr_t attributsMutex;
    pthread_mutexattr_init(&attributsMutex);
    pthread_mutexattr_settype(&attributsMutex, PTHREAD_MUTEX_DEFAULT);
    pthread_mutex_init(&mutConsole, &attributsMutex);
    pthread_mutex_init(&mutRessourceGlobale, &attributsMutex);
    pthread_mutexattr_destroy(&attributsMutex); 

}

/* Fonction appelée juste avant la fin du programme, après que le
 * join() de tous les threads (lecteurs et rédacteurs) ait été correctement
 * effectué.
 * Ne sera jamais appelée s'il y a un blocage dans un lecteur ou un rédacteur.
 */
void fermetureProgramme(){
    // Implémenter ici le nettoyage nécessaire
    pthread_mutex_destroy(&mutConsole);
    pthread_mutex_destroy(&mutRessourceGlobale);
}


/* Un nouveau lecteur arrive dans un thread séparé
 */
void* evenementNouveauLecteur(void* nouveauLecteurVoid) {
    
    int priority = 0;
    pthread_t self = pthread_self();
    priority = sched_get_priority_max(SCHED_FIFO);
    struct sched_param param;
    param.sched_priority = priority;

    // modification de la priorité du lecteur a la priorité max
    pthread_setschedparam(self, SCHED_FIFO, &param);


    lecteur* nouveauLecteur = (lecteur*) nouveauLecteurVoid;
    pthread_mutex_lock(&mutConsole); // console protégée par un mutex
    std::cout << "Lecteur " << nouveauLecteur->id << " demande la ressource" << std::endl;
    pthread_mutex_unlock(&mutConsole);

    lireRessource(nouveauLecteur);   

    pthread_exit(NULL);
    return NULL; // on n'utilisera pas le paramètre de retour
}

/* Un nouveau rédacteur arrive dans un thread séparé
 */
void* evenementNouveauRedacteur(void* nouveauRedacteurVoid) {
    int priority = 0;
    pthread_t self = pthread_self();
    priority = sched_get_priority_min(SCHED_FIFO);
    struct sched_param param;
    param.sched_priority = priority;

    // modification de la priorité du lecteur a la priorité min
    pthread_setschedparam(self, SCHED_FIFO, &param);

    redacteur* nouveauRedacteur = (redacteur*) nouveauRedacteurVoid;
    pthread_mutex_lock(&mutConsole); // console protégée par un mutex
    std::cout << "Redacteur " << nouveauRedacteur->id << " demande la ressource" << std::endl;
    pthread_mutex_unlock(&mutConsole);
    ecrireRessource(nouveauRedacteur);

    pthread_exit(NULL);
    return NULL; // on n'utilisera pas le paramètre de retour
}

/* Fonction qui exécute effectivement la lecture de la ressource,
 * et qui devra simuler un temps de lecture non-négligeable.
 * À compléter comme indiqué dans le sujet d'examen.
 */
void lireRessource(lecteur* lecteurActuel) {
    
    // Durée de pause calculée pour vous, en secondes
    double dureePause = TEMPS_LECTURE_MIN_S +
    (TEMPS_LECTURE_MAX_S-TEMPS_ECRITURE_MIN_S)
        *((double)(rand())/(double)(RAND_MAX));
    
    pthread_mutex_lock(&mutRessourceGlobale); // console protégée par un mutex
    // Pour simuler le temps nécessaire à la lecture, donner ici au
    // système d'exploitation une commande de pause en nanosecondes
    // The value of the nanoseconds field must be in the range 0 to 999999999.
    struct timespec nanot = {.tv_sec = ceil(dureePause), .tv_nsec = 0};
    nanosleep(&nanot, NULL);

    // La lecture effective est réalisée juste après la pause
    pthread_mutex_lock(&mutConsole); // console protégée par un mutex
    std::cout << "    Lecteur " << lecteurActuel->id << " lit " << ressourceGlobale << std::endl;
    pthread_mutex_unlock(&mutConsole);
    
    pthread_mutex_unlock(&mutRessourceGlobale);
}


/* Fonction qui exécute effectivement l'écriture de la ressource,
 * et qui devra simuler un temps d'écriture non-négligeable.
 * À compléter comme indiqué dans le sujet d'examen.
 */
void ecrireRessource(redacteur* redacteurActuel) {
    
    // Durée de pause calculée pour vous, en secondes
    double dureePause = TEMPS_ECRITURE_MIN_S +
    (TEMPS_ECRITURE_MAX_S-TEMPS_ECRITURE_MIN_S)
        *((double)(rand())/(double)(RAND_MAX));
    
    pthread_mutex_lock(&mutRessourceGlobale); // console protégée par un mutex
    // Pour simuler le temps nécessaire à l'écriture, donner ici au
    // système d'exploitation une commande de pause en nanosecondes

    // The value of the nanoseconds field must be in the range 0 to 999999999.
    struct timespec nanot = {.tv_sec = ceil(dureePause), .tv_nsec = 0};
    nanosleep(&nanot, NULL);
    
    // L'écriture effective est réalisée juste après la pause
    ressourceGlobale = redacteurActuel->valeurRessource;
    pthread_mutex_lock(&mutConsole); // console protégée par un mutex
    std::cout << "    Redacteur " << redacteurActuel->id << " écrit " << ressourceGlobale << std::endl;
    pthread_mutex_unlock(&mutConsole);

    pthread_mutex_unlock(&mutRessourceGlobale);
}