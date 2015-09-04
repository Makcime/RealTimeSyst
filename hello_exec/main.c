#include <stdio.h>


int main(void)
{
    /* Tableau de char contenant les arguments (là aucun : le nom du
       programme et NULL sont obligatoires)  */
    char *arguments[] = { "ls", NULL };

    /* Lancement de la commande */
    execv("/bin/ls",arguments);
}