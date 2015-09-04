
#include <iostream>
#include <ctime>		// time_t time()
#include <unistd.h>		// entre autres : usleep
#include <semaphore.h>

#include <fcntl.h>

#define P_PENSE 0
#define P_FAIM  1
#define P_MANGE 2

#define NB_PHILOSOPHES 5

#define DUREE_REPAS_S  10


char *etatsPhilosophes;		// on va considérer que les opérations lecture/écriture sur char sont *atomiques*
sem_t **semFourchettes;
sem_t **semPhilosophes;		// va nous permettre de contrôler les philosophes mal polis

void *vieDuPhilosophe(void *idPtr);



int main(int argc, const char *argv[])
{

		int debug(0);

	std::
	    cout << "Problème des philosophes - durée du repas " <<
	    DUREE_REPAS_S << " secondes" << std::endl << std::endl;

	// Philosphes
	etatsPhilosophes = (char *) malloc(sizeof(char) * NB_PHILOSOPHES);
	semPhilosophes =
	    (sem_t **) malloc(sizeof(sem_t *) * NB_PHILOSOPHES);
	std::string nomSemaphore("/philosophe");
	for (int i = 0; i < NB_PHILOSOPHES; i++) {
		etatsPhilosophes[i] = P_FAIM;
		semPhilosophes[i] = sem_open((nomSemaphore + std::to_string(i)).c_str(), O_CREAT,	// par d'erreur si existait déjà
					     0,	// mode d'ouverture
					     0);	// valeur initiale du sémaphore à compte (ici = bloqué)
	}

	// Fourchettes
	// représentées par des sémaphores : fourchette en utilisation <=> sémaphore acquis
	semFourchettes =
	    (sem_t **) malloc(sizeof(sem_t *) * NB_PHILOSOPHES);
	nomSemaphore = "/fourchette";
	for (int i = 0; i < NB_PHILOSOPHES; i++) {
		semFourchettes[i] = sem_open((nomSemaphore + std::to_string(i)).c_str(), O_CREAT,	// par d'erreur si existait déjà
					     0,	// mode d'ouverture
					     1);	// valeur initiale du sémaphore à compte (ici = libre)
	}

	// Lancement des threads pour les philosophes
	pthread_t threadsPhilosophes[NB_PHILOSOPHES];
	int idPhilosophes[NB_PHILOSOPHES];	// pour garder des variables en mémoire,
	// car on passe un pointeur à un thread
	for (int i = 0; i < NB_PHILOSOPHES; i++) {
		idPhilosophes[i] = i;
		pthread_create(threadsPhilosophes + i, NULL,
			       vieDuPhilosophe, idPhilosophes + i);
	}





	// Boucle de contrôle
	// Dans un premier temps : on va fonctionner par scrutation des états des philosophes
	// et analyse des temps associés
	sem_post(semPhilosophes[2]);
	time_t instantDebut = time(NULL);
	while (difftime(time(NULL), instantDebut) < DUREE_REPAS_S) {
		// ***** mécanisme d'ordonnancement entre philosophes à implémenter ****


		// Pause définissant la fréquence de scrutation (< 1kHz ici)
		usleep(1000);
	}




	// Arrêt du repas/apéro/discussion endiablée
	// Après un certain temps fixé
	for (int i = 0; i < NB_PHILOSOPHES; i++) {
		pthread_cancel(threadsPhilosophes[i]);
		std::
		    cout << "Fin du repas pour le philosophe " << i <<
		    std::endl;
	}

	// Nettoyage
	for (int i = 0; i < NB_PHILOSOPHES; i++) {
		sem_close(semPhilosophes[i]);
		nomSemaphore = "/philosophe";
		sem_unlink((nomSemaphore + std::to_string(i)).c_str());
		sem_close(semFourchettes[i]);
		nomSemaphore = "/fourchette";
		sem_unlink((nomSemaphore + std::to_string(i)).c_str());
	}
	free(semFourchettes);
	free(etatsPhilosophes);

	return 0;
}





void *vieDuPhilosophe(void *idPtr)
{
	int id = *((int *) idPtr);

	// Configuration du thread : il sera annulable à partir de n'importe quel point de préemption
	// (appel bloquant, appel système, etc...)
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	while (1) {
		// ***** À implémenter : *****
		// - structure permettant le contrôle du philosphe par main() grâce aux semPhilosophes
		// - prise/relâchement de fourchette
		// - changement d'état


		pthread_testcancel();	// point où l'annulation du thread est permise
	}

	return NULL;
}
