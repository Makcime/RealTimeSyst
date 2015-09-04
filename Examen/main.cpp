//
//  main.cpp
//  Created by Gwendal Le Vaillant on 21/05/2015
//

#include <iostream>

#include <cstdlib>

#include <ctime> // time_t time()
#include <cmath>

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // entre autres : usleep

#include "MA1INFO_TpsReel_Examen2015.h"



// Déclarations réservées à cette partie de code
#define TAILLE_TABLEAUX 1000

#define PAUSE_EVENEMENTS_MIN_S (0.5)
#define PAUSE_EVENEMENTS_MAX_S (1.5)


// C'est ici qu'est effectivement déclarée notre variable globale
int ressourceGlobale;
pthread_mutex_t mutConsole; 
pthread_mutex_t mutRessourceGlobale;

int main(int argc, const char * argv[]) {
    
    
    // Initialisations
    std::cout << "Examen Tps-Réel 2015 : problème des lecteurs/rédacteurs" << std::endl;
    ressourceGlobale = -1;
    srand((unsigned int)time(NULL)); // graine pour rand()
    
    // Fonction utilisateur d'initialisation
    initialisationProgramme();
    
    /* Les références aux thread seront dans un tableau de taille fixe,
     * et on fera un join de tous ces thread à la toute fin du programme.
     *
     * La durée d'exécution du programme doit rester assez courte pour
     * que cela ne pose pas de problème de dépassement mémoire.
     */
    pthread_t threadsLecteurs[TAILLE_TABLEAUX];
    lecteur lecteurs[TAILLE_TABLEAUX];
    pthread_t threadsRedacteurs[TAILLE_TABLEAUX];
    redacteur redacteurs[TAILLE_TABLEAUX];
    
    // On crée aléatoirement des lecteurs et des rédacteurs
    // sur une période fixée
    int idLecteur=0, idRedacteur=0;
    time_t instantDebut = time(NULL);
    while ( difftime(time(NULL), instantDebut) < DUREE_EXECUTION_S )
    {
        // Tirage 1 : de 0,00 à 0,25 -> rédacteur
        //            de 0,25 à 1,00 -> lecteur
        double tirageChoix = (double)(rand())/(double)(RAND_MAX);
        // Rédacteur
        if (tirageChoix < 0.25) {
            redacteurs[idRedacteur].id = idRedacteur;
            redacteurs[idRedacteur].valeurRessource = rand();
            pthread_create(threadsRedacteurs+idRedacteur, NULL,
                           evenementNouveauRedacteur, redacteurs+idRedacteur);
            idRedacteur++;
        }
        // Lecteur
        else {
            lecteurs[idLecteur].id = idLecteur;
            pthread_create(threadsLecteurs+idLecteur, NULL,
                           evenementNouveauLecteur, lecteurs+idLecteur);
            idLecteur++;
        }
        
        // Tirage 2 : durée avant arrive d'un nouveau rédacteur ou lecteur
        double dureePause = PAUSE_EVENEMENTS_MIN_S +
                            (PAUSE_EVENEMENTS_MAX_S-PAUSE_EVENEMENTS_MIN_S)
                                *((double)(rand())/(double)(RAND_MAX));
        usleep( ceil(1000000.0*dureePause) );
    }
    
    
    // Fonction utilisateur pour le nettoyage
    fermetureProgramme();
    
      
    // Join de tous les threads
    for (int i=0; i<idLecteur; i++)
        pthread_join(threadsLecteurs[i], NULL);
    for (int i=0; i<idRedacteur; i++)
        pthread_join(threadsRedacteurs[i], NULL);
    std::cout << "Programme lecteurs/rédacteurs terminé" << std::endl;
       
    
    return EXIT_SUCCESS;
}
