#include "fonction.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

int nbarc = 0;
Sommet sommets[1000];
int nbsommets = 0;
Sommet listeville[1000];
Sommet listehopital[1000];
Sommet listeentrepot[1000];
int nbville = 0;
int nbhopital = 0;
int nbentrepot = 0;
Arc* arcs = NULL;
bool visite[1000];
int idvehicule = -1;
bool atteignable[1000] = {false};
bool visites[1000];
int adjacence[1000][1000];
int tailleadj[1000];
int groupe[1000];
Vehicule listebus[1000];
int nbbus = 0;
Vehicule listecamion[1000];
int nbcamion = 0;
Vehicule listeambulance[1000];
int nbambulance = 0;
Vehicule listesecour[1000];
int nbsecour = 0;
int population = 0;
int nbmort = 0;
bool msg = false;
int tour = 1;
int currentsommet = 0;
int newx;
int newy;

Arc* ajouterarc(Arc ajout) { // ajout de l'arc dans le tab
    arcs = realloc(arcs, (nbarc + 1) * sizeof(Arc));
    arcs[nbarc] = ajout;
    return arcs;
}

void affichervehicule(int id) { // afficher les stats d'un véhicule
    if (id >= idvehicule) {
        printf("Vehicule inexistant\n");
        return;
    }
    Vehicule v;
    v.id = -1;

    for (int i=0;i<nbbus;i++) {
        if (listebus[i].id == id) {
            v = listebus[i];
        }
    }
    if (v.id == -1) {
        for (int i=0;i<nbambulance;i++) {
            if (listeambulance[i].id == id) {
                v = listeambulance[i];
            }
        }
    }
    if (v.id == -1) {
        for (int i=0;i<nbcamion;i++) {
            if (listecamion[i].id == id) {
                v = listecamion[i];
            }
        }
    }
    if (v.id == -1) {
        for (int i=0;i<nbsecour;i++) {
            if (listesecour[i].id == id) {
                v = listesecour[i];
            }
        }
    }

    switch (v.type) {
        case 0:
            printf("Vehicule %d (Bus):\n", v.id);
        break;
        case 1:
            printf("Vehicule %d (Camion):\n", v.id);
        break;
        case 2:
            printf("Vehicule %d (Ambulance):\n", v.id);
        break;
        case 3:
            printf("Vehicule %d (Secour):\n", v.id);
        break;
        default:
            printf("Vehicule %d (Type inconnu):\n", v.id);
        break;
    }

    printf("  Vitesse : %d\n", v.vitesse);
    printf("  Tout terrain : %d\n", v.toutterrain);
    printf("  Capacite humaine : %d / %d\n", v.capacitehumaine, v.capacitehumainemax);
    printf("  Capacite nourriture : %d / %d\n", v.capacitenourriture, v.capacitenourrituremax);
    printf("  Capacite medicament : %d / %d\n", v.capacitemedicament, v.capacitemedicamentmax);
    printf("  Sommet actuel : %d\n", v.sommetactuel);
    printf("  Avancement sur la route : %d\n", v.avancementroute);
    printf("  Destination : %d\n", v.destination);
}

Vehicule creervehicule(int type) { // créer un véhicule en fonction de son type
    Vehicule retour;
    idvehicule++;
    retour.type = type;
    retour.sommetactuel = -1;
    retour.avancementroute = -1;
    retour.destination = -1;
    retour.reparation1 = -1;
    retour.reparation2 = -1;
    retour.longueur = -1;
    switch (type) {
        case 0:
            retour.id = idvehicule;
            retour.vitesse = 7;
            retour.toutterrain = 40;
            retour.capacitehumaine = 0;
            retour.capacitehumainemax = 70;
            retour.capacitenourriture = 0;
            retour.capacitenourrituremax = 0;
            retour.capacitemedicament = 0;
            retour.capacitemedicamentmax = 0;
            break;
        case 1:
            retour.id = idvehicule;
            retour.vitesse = 10 + rand() % 5;
            retour.toutterrain = 65;
            retour.capacitehumaine = 0;
            retour.capacitehumainemax = 0;
            retour.capacitenourriture = 0;
            retour.capacitenourrituremax = 10000;
            retour.capacitemedicament = 0;
            retour.capacitemedicamentmax = 2500;
            break;
        case 2:
            retour.id = idvehicule;
            retour.vitesse = 17;
            retour.toutterrain = 75;
            retour.capacitehumaine = 0;
            retour.capacitehumainemax = 3;
            retour.capacitenourriture = 0;
            retour.capacitenourrituremax = 0;
            retour.capacitemedicament = 0;
            retour.capacitemedicamentmax = 50;
            break;
        case 3:
            retour.id = idvehicule;
            retour.vitesse = 13;
            retour.toutterrain = 85;
            retour.capacitehumaine = 0;
            retour.capacitehumainemax = 0;
            retour.capacitenourriture = 0;
            retour.capacitenourrituremax = 0;
            retour.capacitemedicament = 0;
            retour.capacitemedicamentmax = 0;
            break;
    }
    return retour;
}

void spawnvehicule(int nbsommets) { // spawn les véhicules dans les villes au lancement
    for (int i=0;i<nbsommets;i++) {
        int boucle = 0;
        Vehicule nouveau;
        switch (sommets[i].type) {
            case typeville:
                boucle = sommets[i].data.ville.nbhabitant / 100;
                for (int j=0;j<boucle;j++) {
                    nouveau = creervehicule(0);
                    nouveau.sommetactuel = i;
                    listebus[nbbus] = nouveau;
                    nbbus++;
                }
                break;

            case typehopital:
                for (int j=0;j<3;j++) {
                    nouveau = creervehicule(2);
                    nouveau.sommetactuel = i;
                    listeambulance[nbambulance] = nouveau;
                    nbambulance++;
                }
                for (int j=0;j<2;j++) {
                    nouveau = creervehicule(3);
                    nouveau.sommetactuel = i;
                    listesecour[nbsecour] = nouveau;
                    nbsecour++;
                }
                break;

            case typeentrepot:
                for (int j=0;j<3;j++) {
                    nouveau = creervehicule(1);
                    nouveau.sommetactuel = i;
                    listecamion[nbcamion] = nouveau;
                    nbcamion++;
                }
                for (int j=0;j<3;j++) {
                    nouveau = creervehicule(0);
                    nouveau.sommetactuel = i;
                    listebus[nbbus] = nouveau;
                    nbbus++;
                }
                break;
        }
    }
}

bool coord() {
    for (int i=0;i<currentsommet;i++) { //gérer les coordonnées pour éviter les superpositions
        int x = 0;
        int y = 0;
        switch (sommets[i].type) {
            case typeville:
                x = sommets[i].data.ville.x;
                y = sommets[i].data.ville.y;
                break;
            case typehopital:
                x = sommets[i].data.hopital.x;
                y = sommets[i].data.hopital.y;
                break;
            case typeentrepot:
                x = sommets[i].data.entrepot.x;
                y = sommets[i].data.entrepot.y;
                break;
        }
        if (x-25 > newx && x+25 < newx) {
            if (y-25 > newy && y+25 < newy) {
                return true;
            }
        }
    }
    return false;
}

void creercord() { // création des coordonnés pour éviter la superposition
    newx = 20 + rand() % 760;
    newy = 20 + rand() % 660;
    while (coord() == true) {
        newx = 20 + rand() % 760;
        newy = 20 + rand() % 660;
    }
}

void creergraphe(int nbsommets) { // création du graphee entier (arc, sommet et véhicule)
    srand(time(NULL));
    if (choixMode == 1) {
        for (int i = 0; i < nbsommets; i++) {
            sommets[i].id = i;
            char *listenom[50] = {"Velmor","Tirandel","Braywick","Solthar","Nimrovia","Drelmoor","Kalthorn","Zarquel","Arkenfell","Myriath","Eronde","Valcira","Drastel","Fynmoor","Obrith","Zenthra","Calverin","Tarnhelm","Quelmorra","Mirthane","Norvalis","Sildora","Urmenth","Halgrin","Tharil","Vorneth","Iskendril","Luthmere","Gryndel","Azmorra","Krelwind","Verinth","Orlian","Dalkros","Nareth","Ulgrin","Fenmire","Thralden","Kymbray","Zevrana","Aldevor","Molkar","Yrenth","Drakspire","Emberfell","Ostelan","Vyroneth","Wynthor","Krimvale","Elyndar"};
            int random = rand() % 50 + 1;
            if (i <= 50) {
                sommets[i].nom = listenom[i];
            }else {
                sommets[i].nom = "Zoin";
            }
            creercord();
            if (random <= 10) {
                sommets[i].type = 1;
                sommets[i].data.hopital.nbpatient = 10 + rand() % 20;
                sommets[i].data.hopital.nbpatientmax = 200;
                sommets[i].data.hopital.stockmedicament = 2000 + rand() % 3000;
                sommets[i].data.hopital.pointdevie = 100;
                sommets[i].data.hopital.x = newx;
                sommets[i].data.hopital.y = newy;
                listehopital[nbhopital] = sommets[i];
                nbhopital++;
            }else if (random <= 20) {
                sommets[i].type = 2;
                sommets[i].data.entrepot.stocknourriture = 50000 + rand() % 10000;
                sommets[i].data.entrepot.stockmedicament = 25000 + rand() % 3000;
                sommets[i].data.entrepot.pointdevie = 100;
                sommets[i].data.entrepot.x = newx;
                sommets[i].data.entrepot.y = newy;
                listeentrepot[nbentrepot] = sommets[i];
                nbentrepot++;
            }else {
                sommets[i].type = 0;
                sommets[i].data.ville.nbhabitant = rand() % 1000 + 100;
                sommets[i].data.ville.arrivage = 0;
                sommets[i].data.ville.evacuation = 0;
                sommets[i].data.ville.stocknourriture = sommets[i].data.ville.nbhabitant * 15;
                sommets[i].data.ville.stockmedicament = sommets[i].data.ville.nbhabitant * 8;
                sommets[i].data.ville.pointdevie = 100;
                population += sommets[i].data.ville.nbhabitant;
                sommets[i].data.ville.direction = 0;
                sommets[i].data.ville.x = newx;
                sommets[i].data.ville.y = newy;
                listeville[nbville] = sommets[i];
                nbville++;
            }
        }
    }else {
        int cols = (int)sqrt(nbsommets);
        int rows = (nbsommets + cols - 1) / cols;

        int spacing_x = (int)(800 / (cols + 1) * 1.8);
        int spacing_y = (int)(700 / (rows + 1) * 1.5);

        for (int i = 0; i < nbsommets; i++) {
            sommets[i].id = i;
            int col = i % cols;
            int row = i / cols;
            int pos_x = spacing_x * (col + 1);
            int pos_y = spacing_y * (row + 1);

            int random = rand() % 50 + 1;
            if (random <= 10) {
                sommets[i].type = 1;
                sommets[i].data.hopital.nbpatient = 10 + rand() % 20;
                sommets[i].data.hopital.nbpatientmax = 200;
                sommets[i].data.hopital.stockmedicament = 2000 + rand() % 3000;
                sommets[i].data.hopital.pointdevie = 100;
                sommets[i].data.hopital.x = pos_x;
                sommets[i].data.hopital.y = pos_y;
                listehopital[nbhopital++] = sommets[i];
            } else if (random <= 20) {
                sommets[i].type = 2;
                sommets[i].data.entrepot.stocknourriture = 50000 + rand() % 10000;
                sommets[i].data.entrepot.stockmedicament = 25000 + rand() % 3000;
                sommets[i].data.entrepot.pointdevie = 100;
                sommets[i].data.entrepot.x = pos_x;
                sommets[i].data.entrepot.y = pos_y;
                listeentrepot[nbentrepot++] = sommets[i];
            } else {
                sommets[i].type = 0;
                sommets[i].data.ville.nbhabitant = rand() % 1000 + 100;
                sommets[i].data.ville.arrivage = 0;
                sommets[i].data.ville.evacuation = 0;
                sommets[i].data.ville.stocknourriture = sommets[i].data.ville.nbhabitant * 15;
                sommets[i].data.ville.stockmedicament = sommets[i].data.ville.nbhabitant * 8;
                sommets[i].data.ville.pointdevie = 100;
                sommets[i].data.ville.direction = 0;
                population += sommets[i].data.ville.nbhabitant;
                sommets[i].data.ville.x = pos_x;
                sommets[i].data.ville.y = pos_y;
                listeville[nbville++] = sommets[i];
            }
        }
    }
    currentsommet++;
    for (int i = 0; i < nbsommets; i++) {
        for (int j = i + 1; j < nbsommets; j++) {
            if (rand() % coefarc == 0) {
                Arc nouveau;
                nouveau.id = nbarc;
                nouveau.sommet1 = i;
                nouveau.sommet2 = j;
                nouveau.pointdevie = 100;
                nouveau.longueur = 10 + rand() % 20 + 1;
                nouveau.intervention = false;
                ajouterarc(nouveau);
                nbarc++;
            }
        }
    }
}

void affichersommets(int nbsommets) { // afficher les sommets sans stat précise
    printf("Sommets:\n");
    for (int i = 0; i < nbsommets; i++) {
        switch (sommets[i].type) {
            case typeville:
                printf("Sommet %d, Ville, HP: %d\n", sommets[i].id, sommets[i].data.ville.pointdevie);
                break;
            case typehopital:
                printf("Sommet %d, Hopital, HP: %d\n", sommets[i].id, sommets[i].data.hopital.pointdevie);
                break;
            case typeentrepot:
                printf("Sommet %d, Entrepot, HP: %d\n", sommets[i].id, sommets[i].data.entrepot.pointdevie);
                break;
        }
    }
}

void afficherarcs() { // afficher les arcs
    printf("\nArcs:\n");
    for (int i = 0; i < nbarc; i++) {
        printf("Arc %d: %d -> %d | Longueur: %d | PV: %d\n",
               arcs[i].id, arcs[i].sommet1, arcs[i].sommet2,
               arcs[i].longueur, arcs[i].pointdevie);
    }
}

void damagesursommet(int sommetid, int puissance) { // calcul des dommages sur les sommets (rayon d'impact)
    switch (sommets[sommetid].type) {
        case typeville:
            sommets[sommetid].data.ville.pointdevie -= 10 * puissance;
            sommets[sommetid].data.ville.evacuation = sommets[sommetid].data.ville.nbhabitant / 100 * sommets[sommetid].data.ville.pointdevie;
        break;
        case typehopital:
            sommets[sommetid].data.hopital.pointdevie -= 10 * puissance;
        break;
        case typeentrepot:
            sommets[sommetid].data.entrepot.pointdevie -= 10 * puissance;
        break;
    }
}

void damagesurroute(int debut, int puissance) { // calcul des dommages sur les routes (rayon d'impact)
    for (int i = 0; i < nbarc; i++) {
        if (arcs[i].sommet1 == debut || arcs[i].sommet2 == debut) {
            int degats = 0;
            if (puissance == 1) {
                degats = 60 + rand() % 25 + 1;
            } else if (puissance == 2) {
                degats = 40 + rand() % 20 + 1;
            } else if (puissance == 3) {
                degats = 20 + rand() % 10 + 1;
            }
            arcs[i].pointdevie -= degats;
            if (arcs[i].pointdevie < 0) {
                arcs[i].pointdevie = 0;
            }
        }
    }
}


void propageronde(int sommet, int puissanceactu, int puissancemax) { // propagation du rayon d'impact
    if (puissanceactu > puissancemax || visite[sommet]) {
        return;
    }
    visite[sommet] = true;
    damagesursommet(sommet, puissanceactu);
    damagesurroute(sommet, puissanceactu);
    for (int i = 0; i < nbarc; i++) {
        int voisin = -1;
        if (arcs[i].sommet1 == sommet)
            voisin = arcs[i].sommet2;
        else if (arcs[i].sommet2 == sommet)
            voisin = arcs[i].sommet1;
        if (voisin != -1 && !visite[voisin]) {
            propageronde(voisin, puissanceactu + 1, puissancemax);
        }
    }
}


void seisme(int nbsommets) { // création du seisme et applicatin des dégats
    int centre = rand() % nbsommets;
    while (sommets[centre].type != typeville) {
        centre = rand() % nbsommets;
    }
    printf("Seisme declenche au sommet %d\n", centre);
    switch (sommets[centre].type) {
        case typeville:
            sommets[centre].data.ville.pointdevie = 20;
            sommets[centre].data.ville.evacuation = sommets[centre].data.ville.nbhabitant / 100 * sommets[centre].data.ville.pointdevie;
        break;
        case typehopital:
            sommets[centre].data.hopital.pointdevie = 20;
        break;
        case typeentrepot:
            sommets[centre].data.entrepot.pointdevie = 20;
        break;
    }
    for (int i = 0; i < nbsommets; i++) {
        visite[i] = false;
    }
    propageronde(centre, 1, 3);
}


void accessible_oriente(int depart, bool* visite) {
    visite[depart] = true;

    for (int i = 0; i < nbarc; i++) {
        if (arcs[i].sommet1 == depart && !visite[arcs[i].sommet2]) {
            accessible_oriente(arcs[i].sommet2, visite);
        }
    }
}

bool sommets_accessibles_depuis(int source, int cible) {
    bool atteignable[1000] = { false };
    accessible_oriente(source, atteignable);
    return atteignable[cible];
}


void inaccessible(int nbsommets) { // pour trouver les sommets inaccessibles
    memset(atteignable, 0, sizeof(bool) * nbsommets); 

    accessible_oriente(0, atteignable);

    printf("Sommets inaccessibles (orienté):\n");
    bool tousAccessibles = true;
    for (int i = 0; i < nbsommets; i++) {
        if (!atteignable[i]) {
            printf("Sommet %d\n", i);
            tousAccessibles = false;
        }
    }

    if (tousAccessibles) {
        printf("Aucun sommet est inaccessible\n");
    }
}

void infosommet(int nbSomets, int sommet) { //info complete sur un sommets
    if (sommet > nbSomets) {
        printf("Ville inexsistante");
    }
    else {
        switch (sommets[sommet].type) {
            case typeville:
                printf("Info de la ville %d:\n", sommet);
                printf("Habitant: %d\n", sommets[sommet].data.ville.nbhabitant);
                printf("Nourriture: %d\n", sommets[sommet].data.ville.stocknourriture);
                printf("Medicament: %d\n", sommets[sommet].data.ville.stockmedicament);
                printf("Point de vie: %d\n", sommets[sommet].data.ville.pointdevie);
                printf("Evacuation: %d\n", sommets[sommet].data.ville.evacuation);
                break;
            case typehopital:
                printf("Info de l'hopital %d:\n", sommet);
                printf("Patient: %d\n", sommets[sommet].data.hopital.nbpatient);
                printf("Patient Max: %d\n", sommets[sommet].data.hopital.nbpatientmax);
                printf("Medicament: %d\n", sommets[sommet].data.hopital.stockmedicament);
                printf("Point de vie: %d\n", sommets[sommet].data.hopital.pointdevie);
                break;
            case typeentrepot:
                printf("Info de l'entrepot %d:\n", sommet);
                printf("Nourriture: %d\n", sommets[sommet].data.entrepot.stocknourriture);
                printf("Medicament: %d\n", sommets[sommet].data.entrepot.stockmedicament);
                printf("Point de vie: %d\n", sommets[sommet].data.entrepot.pointdevie);
                break;
        }
    }
}

int trouverchemin(Vehicule vehicule, int destination, int nbsommets, int *predecesseurs){ // algo dijstra pour trouver le chemin le plus court entre A et B en fonction de l'état de la route et du vehicule
    dijkstra noeuds[1000];

    for (int i = 0; i < nbsommets; i++) {
        noeuds[i].sommet = i;
        noeuds[i].precedent = -1;
        noeuds[i].distance = 1000000;
        noeuds[i].visite = false;
    }

    noeuds[vehicule.sommetactuel].distance = 0;

    while (true) {
        int min = 1000000;
        int var = -1;

        for (int i = 0; i < nbsommets; i++) {
            if (!noeuds[i].visite && noeuds[i].distance < min) {
                min = noeuds[i].distance;
                var = i;
            }
        }

        if (var == -1) {
            break;
        }
        noeuds[var].visite = true;

        for (int i = 0; i < nbarc; i++) {
            int voisin = -1;
            if (arcs[i].sommet1 == var) {
                int voisin = arcs[i].sommet2;
                if (!noeuds[voisin].visite) {
                    if (arcs[i].pointdevie <= 0) continue;
                    if (vehicule.toutterrain < arcs[i].pointdevie) continue;

                    int newdistance = noeuds[var].distance + arcs[i].longueur;
                    if (newdistance < noeuds[voisin].distance) {
                        noeuds[voisin].distance = newdistance;
                        noeuds[voisin].precedent = var;
                    }
                }
            }

            if (voisin != -1 && !noeuds[voisin].visite) {
                if (arcs[i].pointdevie <= 0) {
                    continue;
                }
                if (vehicule.toutterrain < arcs[i].pointdevie) {
                    continue;
                }

                int newdistance = noeuds[var].distance + arcs[i].longueur;
                if (newdistance < noeuds[voisin].distance) {
                    noeuds[voisin].distance = newdistance;
                    noeuds[voisin].precedent = var;
                }
            }
        }
    }

    if (noeuds[destination].distance == 1000000) {
        printf("Pas de chemin dispo entre %d et %d\n", vehicule.sommetactuel, destination);
        return -1;
    }

    printf("Chemin le plus court (distance %d) :\n", noeuds[destination].distance);

    int chemin[1000];
    int count = 0;
    int courant = destination;
    while (courant != -1) {
        chemin[count++] = courant;
        courant = noeuds[courant].precedent;
    }

    for (int i = count - 1; i >= 0; i--) {
        printf("%d", chemin[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");

    for (int i = 0; i < nbsommets; i++) {
        predecesseurs[i] = noeuds[i].precedent;
    }

    return noeuds[destination].distance;
}


Vehicule nouvellesroutes(int nbsommets, Vehicule vehicule) { // création de nouvelle routes 
    if (nbsommets < 2) {
        return vehicule;
    }

    int tentative = 0;
    const int maxe = 1000;

    while (tentative < maxe) {
        int s1 = rand() % nbsommets;
        int s2 = rand() % nbsommets;
        if (s1 == s2) {
            tentative++;
            continue;
        }

        bool dejaConnecte = false;
        for (int i = 0; i < nbarc; i++) {
            if ((arcs[i].sommet1 == s1 && arcs[i].sommet2 == s2) ||
                (arcs[i].sommet1 == s2 && arcs[i].sommet2 == s1)) {
                dejaConnecte = true;
                break;
                }
        }

        if (!dejaConnecte) {
            vehicule.reparation1 = s1;
            vehicule.reparation2 = s2;
            return vehicule;
        }
        tentative++;
    }
    return vehicule;
}

void construireadjacence(int nbsommets) { //constuction de la matrice d'adjacence
    for (int i = 0; i < nbsommets; i++) {
        tailleadj[i] = 0;
    }
    for (int i = 0; i < nbarc; i++) {
        int u = arcs[i].sommet1;
        int v = arcs[i].sommet2;

        if (arcs[i].pointdevie > 50 && arcs[i].intervention == false) {
            adjacence[u][tailleadj[u]++] = v; 
        }
    }
}


void profondeurrecursive(int sommet) { //parcour en profondeur du graphe
    visites[sommet] = true; 
    printf("Visite du sommet %d\n", sommet);
    for (int i = 0; i < tailleadj[sommet]; i++) {
        int voisin = adjacence[sommet][i];
        if (!visites[voisin]) {
            profondeurrecursive(voisin);
        }
    }
}

void parcourgroupe(int sommet, int idgroupe) { // parcour des groupes
    visites[sommet] = true;
    groupe[sommet] = idgroupe;
    for (int i = 0; i < tailleadj[sommet]; i++) {
        int voisin = adjacence[sommet][i];
        if (!visites[voisin]) {
            parcourgroupe(voisin, idgroupe);
        }
    }
}

int detectergroupes(int nbsommets) { //detection des diffréetns groupes
    for (int i = 0; i < nbsommets; i++) {
        visites[i] = false;
        groupe[i] = -1;
    }
    construireadjacence(nbsommets);
    int nbgroupes = 0;
    for (int i = 0; i < nbsommets; i++) {
        if (!visites[i]) {
            parcourgroupe(i, nbgroupes);
            nbgroupes++;
        }
    }
    return nbgroupes;
}

Vehicule reunirgroupes(int nbsommets, Vehicule vehicule) { // algo pour réunir les groupes (nouvelle routes ou réparation)
    vehicule.reparation1 = -1;
    vehicule.reparation2 = -1;
    int nbgroupes = detectergroupes(nbsommets);
    if (nbgroupes <= 1) {
        //printf("Tous les sommets sont deja connectes\n");
        return vehicule;
    }
    for (int i = 0; i < nbsommets; i++) {
        for (int j = i + 1; j < nbsommets; j++) {
            if (groupe[i] != groupe[j]) {
                int dejaPris = 0;
                for (int s = 0; s < nbsecour; s++) {
                    if ((listesecour[s].reparation1 == i && listesecour[s].reparation2 == j) ||
                        (listesecour[s].reparation1 == j && listesecour[s].reparation2 == i)) {
                        dejaPris = 1;
                        break;
                        }
                }
                if (dejaPris) {
                    continue;
                }
                vehicule.reparation1 = i;
                vehicule.reparation2 = j;
                for (int k = 0; k < nbarc; k++) {
                    if ((arcs[k].sommet1 == i && arcs[k].sommet2 == j) ||
                        (arcs[k].sommet1 == j && arcs[k].sommet2 == i)) {
                        arcs[k].intervention = true;
                        break;
                        }
                }
                printf("Route %d a %d\n", i, j);
                return vehicule;
            }
        }
    }
    vehicule = nouvellesroutes(nbsommets, vehicule);
    return vehicule;
}

int taux_destruction(const Sommet* sommets, int nbsommets, const Arc* arcs, int nbarc) { // calcul du taux de destruction de la simulation
    int total_pv_actuel = 0;
    int total_pv_max = 0;

    for (int i = 0; i < nbsommets; i++) {
        int pv = 0;
        switch (sommets[i].type) {
            case typeville:
                pv = sommets[i].data.ville.pointdevie;
                break;
            case typehopital:
                pv = sommets[i].data.hopital.pointdevie;
                break;
            case typeentrepot:
                pv = sommets[i].data.entrepot.pointdevie;
                break;
        }
        total_pv_actuel += pv;
        total_pv_max += 100;
    }

    for (int i = 0; i < nbarc; i++) {
        total_pv_actuel += arcs[i].pointdevie;
        total_pv_max += 100; 
    }

    if (total_pv_max == 0) return 0; 

    int pourcentage_vie_restante = (total_pv_actuel * 100) / total_pv_max;
    int taux_destruction = 100 - pourcentage_vie_restante;

    return taux_destruction;
}


void stat(int nbsommets) { // stat de la simulation
    printf("Nombre de sommets: %d\n", nbsommets);
    printf("Nombre d'arcs: %d\n", nbarc);
    printf("Nombre de vehicule: %d\n", idvehicule);
    printf("Population globale: %d\n", population);
    printf("Nombre de mort: %d\n", nbmort);
    printf("Nombre bus: %d\n", nbbus);
    printf("Nombre ambulance: %d\n", nbambulance);
    printf("Nombre camiom: %d\n", nbcamion);
    printf("Nombre secour: %d\n", nbsecour);
    printf("Nombe de ville: %d\n", nbville);
    printf("Nombe d'hopital: %d\n", nbhopital);
    printf("Nombe d'entrepot: %d\n", nbentrepot);
}


int gethp(Sommet s) { // avoir les pv d'un sommet peut impotre son type
    switch (s.type) {
        case typeville:
            return s.data.ville.pointdevie;
        case typehopital:
            return s.data.hopital.pointdevie;
        case typeentrepot:
            return s.data.entrepot.pointdevie;
        default:
            return 0;
    }
}

void trierville() { //trier les villes en fonction des pv
    for (int i = 0; i < nbville - 1; i++) {
        for (int j = 0; j < nbville - i - 1; j++) {
            if (gethp(listeville[j]) > gethp(listeville[j + 1])) {
                Sommet tmp = listeville[j];
                listeville[j] = listeville[j + 1];
                listeville[j + 1] = tmp;
            }
        }
    }
}

void gerersecour(int nbsommets) { // parcour pour l'ia des véhicules de secour
    static int predecesseurs[1000];
    for (int i=0;i<nbsecour;i++) {
        if (listesecour[i].reparation1 == -1) {
            listesecour[i] = reunirgroupes(nbsommets, listesecour[i]);
            int longueur1 = trouverchemin(listesecour[i], listesecour[i].reparation1, nbsommets, predecesseurs);
            int longueur2 = trouverchemin(listesecour[i], listesecour[i].reparation2, nbsommets, predecesseurs);
            if (longueur1 < longueur2) {
                listesecour[i].destination = listesecour[i].reparation1;
                listesecour[i].longueur = longueur1;
            }else {
                listesecour[i].destination = listesecour[i].reparation2;
                listesecour[i].longueur = longueur2;
            }
            if (listesecour[i].reparation1 != -1) {
                printf("Secour %d va vers %d !\n", listesecour[i].id, listesecour[i].destination);
            }else if (sommets[listesecour[i].sommetactuel].type != typehopital){
                listesecour[i].destination = listehopital[rand() % nbhopital].id;
                listesecour[i].longueur = trouverchemin(listesecour[i], listesecour[i].destination, nbsommets, predecesseurs);
                listesecour[i].reparation1 = -2;
                printf("Secour %d rentre a l'hopital %d!\n", listesecour[i].id, listesecour[i].destination);
            }
        }else if (listesecour[i].avancementroute < listesecour[i].longueur){
            listesecour[i].avancementroute += listesecour[i].vitesse;
            if (msg == true) {
                printf("Secour %d avance...\n", listesecour[i].id);
            }
        }else if (listesecour[i].avancementroute >= listesecour[i].longueur){
            if (listesecour[i].reparation1 == -2) {
                printf("Secour %d est bien rentre a l'hopital %d!\n", listesecour[i].id, listesecour[i].destination);
                listesecour[i].sommetactuel = listesecour[i].destination;
                listesecour[i].destination = -1;
                listesecour[i].avancementroute = -1;
                listesecour[i].reparation1 = -1;
                listesecour[i].reparation2 = -1;
            }else {
                bool trouve = false;
                for (int boucle = 0; boucle < nbarc; boucle++) {
                    if ((arcs[boucle].sommet1 == listesecour[i].reparation1 && arcs[boucle].sommet2 == listesecour[i].reparation2) ||
                        (arcs[boucle].sommet2 == listesecour[i].reparation1 && arcs[boucle].sommet1 == listesecour[i].reparation2)) {
                        arcs[boucle].pointdevie = 75;
                        printf("Secour %d a repare la route entre %d et %d !\n",
                               listesecour[i].id, listesecour[i].reparation1, listesecour[i].reparation2);
                        trouve = true;
                        break;
                        }
                }

                if (!trouve) {
                    Arc nouveau;
                    nouveau.id = nbarc;
                    nouveau.sommet1 = listesecour[i].reparation1;
                    nouveau.sommet2 = listesecour[i].reparation2;
                    nouveau.pointdevie = 75;
                    nouveau.longueur = 10 + rand() % 20 + 1;
                    nouveau.intervention = false;
                    ajouterarc(nouveau);
                    nbarc++;
                    printf("Secour %d a construit la route entre %d et %d !\n",
                           listesecour[i].id, listesecour[i].reparation1, listesecour[i].reparation2);
                }

                listesecour[i].sommetactuel = listesecour[i].destination;
                listesecour[i].destination = -1;
                listesecour[i].avancementroute = -1;
                listesecour[i].reparation1 = -1;
                listesecour[i].reparation2 = -1;
            }

        }
    }
}

void trierhopitaux() { // trier les hopitaux en fonction de leurs nombre de patients
    for (int i = 0; i < nbhopital - 1; i++) {
        for (int j = 0; j < nbhopital - i - 1; j++) {
            if (sommets[listehopital[j].id].data.hopital.nbpatient > sommets[listehopital[j + 1].id].data.hopital.nbpatient) {
                Sommet tmp = listehopital[j];
                listehopital[j] = listehopital[j + 1];
                listehopital[j + 1] = tmp;
            }
        }
    }
}

void gererambulance(int nbsommets) { // parcour pour l'ia des véhicules d'ambulance
    static int predecesseurs[1000];
    for (int i=0;i<nbambulance;i++) {
        if (listeambulance[i].destination == -1) {
            if (sommets[listeambulance[i].sommetactuel].type == typeville) {
                int trouveHopital = 0;
                for (int j = 0; j < nbhopital && !trouveHopital; j++) {
                    if (sommets[listehopital[j].id].data.hopital.nbpatient + listeambulance[i].capacitehumaine <= sommets[listehopital[j].id].data.hopital.nbpatientmax) {
                        listeambulance[i].longueur = trouverchemin(listeambulance[i], listehopital[j].id, nbsommets, predecesseurs);
                        if (listeambulance[i].longueur != -1) {
                            listeambulance[i].destination = listehopital[j].id;
                            printf("Ambulance %d va vers l'hopital %d\n", sommets[i].id, listehopital[j].id);
                            trouveHopital = 1;
                        }
                    }
                }
                if (!trouveHopital) {
                    printf("Plus de place dans les hopitaux\n");
                }
            } else {
                int trouveVille = 0;
                for (int j = 0; j < nbville && !trouveVille; j++) {
                    if (sommets[listeville[j].id].data.ville.nbhabitant - sommets[listeville[j].id].data.ville.arrivage > sommets[listeville[j].id].data.ville.evacuation) {
                        listeambulance[i].destination = listeville[j].id;
                        listeambulance[i].longueur = trouverchemin(listeambulance[i], listeville[j].id, nbsommets, predecesseurs);
                        sommets[listeville[j].id].data.ville.arrivage += listeambulance[i].capacitehumainemax;
                        printf("Ambulance %d va vers la ville %d\n", listeambulance[i].id, listeville[j].id);
                        trouveVille = 1;
                    }
                }
                if (!trouveVille) {
                    printf("TOUTES les victimes ont ete degage des villes\n");
                }
            }
        }else if (listeambulance[i].avancementroute < listeambulance[i].longueur){
            listeambulance[i].avancementroute += listeambulance[i].vitesse;
            if (msg == true) {
                printf("Ambulance %d avance...\n", listeambulance[i].id);
            }
        }else if (listeambulance[i].avancementroute >= listeambulance[i].longueur){
            if (sommets[listeambulance[i].destination].type == typeville) {
                printf("Ambulance %d arrive a %d\n", listeambulance[i].id, listeambulance[i].destination);
                listeambulance[i].sommetactuel = listeambulance[i].destination;
                sommets[listeambulance[i].destination].data.ville.arrivage -= listeambulance[i].capacitehumainemax;
                sommets[listeambulance[i].destination].data.ville.nbhabitant -= listeambulance[i].capacitehumainemax;
                listeambulance[i].destination = -1;
                listeambulance[i].longueur = -1;
                listeambulance[i].capacitehumaine = listeambulance[i].capacitehumainemax;
            }else {
                printf("Ambulance %d arrive a l'hopital %d\n", listeambulance[i].id, listeambulance[i].destination);
                listeambulance[i].sommetactuel = listeambulance[i].destination;
                sommets[listeambulance[i].destination].data.hopital.nbpatient += listeambulance[i].capacitehumainemax;
                listeambulance[i].destination = -1;
                listeambulance[i].longueur = -1;
                listeambulance[i].capacitehumaine = 0;
            }
        }
    }
}

void gererbus(int nbsommets) { // parcour pour l'ia des véhicules de bus
    static int predecesseurs[1000];
    for (int i = 0; i < nbbus; i++) {
        if (listebus[i].destination == -1) {
            if (sommets[listebus[i].sommetactuel].type == typeville) {
                int trouveVilleDestination = 0;
                for (int j = 0; j < nbville && !trouveVilleDestination; j++) {
                    if (sommets[listeville[j].id].data.ville.nbhabitant - sommets[listeville[j].id].data.ville.arrivage > sommets[listeville[j].id].data.ville.evacuation) {
                        listebus[i].destination = listeville[j].id;
                        listebus[i].longueur = trouverchemin(listebus[i], listeville[j].id, nbsommets, predecesseurs);
                        sommets[listeville[j].id].data.ville.arrivage += listebus[i].capacitehumainemax;
                        printf("Bus %d va evacuer la ville %d\n", listebus[i].id, listeville[j].id);
                        trouveVilleDestination = 1;
                    }
                }
                if (!trouveVilleDestination) {
                    if (msg == true) {
                        printf("Aucune ville a evacuer\n");
                    }
                }
            } else {
                int trouveVilleSaine = 0;
                for (int j = nbville - 1; j >= 0 && !trouveVilleSaine; j--) {
                    if (sommets[listeville[j].id].data.ville.arrivage + listebus[i].capacitehumainemax <= sommets[listeville[j].id].data.ville.nbhabitant) {
                        listebus[i].destination = listeville[j].id;
                        listebus[i].longueur = trouverchemin(listebus[i], listeville[j].id, nbsommets, predecesseurs);
                        printf("Bus %d retourne vers la ville %d\n", listebus[i].id, listeville[j].id);
                        trouveVilleSaine = 1;
                    }
                }
                if (!trouveVilleSaine) {
                    printf("Aucune ville saine pour retour de bus %d\n", listebus[i].id);
                }
            }
        } else if (listebus[i].avancementroute < listebus[i].longueur) {
            listebus[i].avancementroute += listebus[i].vitesse;
            if (msg == true) {
                printf("Bus %d avance...\n", listebus[i].id);
            }
        } else if (listebus[i].avancementroute >= listebus[i].longueur) {
            if (sommets[listebus[i].destination].type == typeville) {
                printf("Bus %d est arrive a la ville %d\n", listebus[i].id, listebus[i].destination);
                listebus[i].sommetactuel = listebus[i].destination;

                if (sommets[listebus[i].destination].data.ville.arrivage >= listebus[i].capacitehumainemax) {
                    sommets[listebus[i].destination].data.ville.arrivage -= listebus[i].capacitehumainemax;
                }

                if (sommets[listebus[i].destination].data.ville.nbhabitant >= listebus[i].capacitehumainemax) {
                    sommets[listebus[i].destination].data.ville.nbhabitant -= listebus[i].capacitehumainemax;
                    listebus[i].capacitehumaine = listebus[i].capacitehumainemax;
                } else {
                    listebus[i].capacitehumaine = sommets[listebus[i].destination].data.ville.nbhabitant;
                    sommets[listebus[i].destination].data.ville.nbhabitant = 0;
                }

                listebus[i].destination = -1;
                listebus[i].longueur = -1;
            } else {
                printf("Bus %d arrive dans la ville %d pour decharger %d personnes\n", listebus[i].id, listebus[i].destination, listebus[i].capacitehumaine);
                listebus[i].sommetactuel = listebus[i].destination;
                sommets[listebus[i].destination].data.ville.nbhabitant += listebus[i].capacitehumaine;
                listebus[i].capacitehumaine = 0;
                listebus[i].destination = -1;
                listebus[i].longueur = -1;
            }
        }
    }
}

void gerercamion(int nbsommets) { // parcour pour l'ia des véhicules de secourcamion
    static int predecesseurs[1000];
    for (int i = 0; i < nbcamion; i++) {
        if (listecamion[i].destination == -1) {
            if (sommets[listecamion[i].sommetactuel].type == typeentrepot) {
                listecamion[i].capacitenourriture = listecamion[i].capacitenourrituremax;
                listecamion[i].capacitemedicament = listecamion[i].capacitemedicamentmax;
                int maxBesoin = 0;
                int idvilleChoisie = -1;
                for (int j = 0; j < nbsommets; j++) {
                    if (sommets[j].type == typeville) {
                        int besoinNourriture = (sommets[j].data.ville.nbhabitant * 10) -
                                               (sommets[j].data.ville.stocknourriture + sommets[j].data.ville.arrivagenourriture);

                        int besoinMedicament = ((sommets[j].data.ville.nbhabitant / 100) *
                                                (100 - sommets[j].data.ville.pointdevie) * 10) -
                                                sommets[j].data.ville.stockmedicament - sommets[j].data.ville.arrivagemedicament;

                        int totalBesoin = besoinNourriture + besoinMedicament;

                        if (totalBesoin > maxBesoin) {
                            maxBesoin = totalBesoin;
                            idvilleChoisie = j;
                        }
                    }
                }
                if (idvilleChoisie != -1) {
                    listecamion[i].destination = idvilleChoisie;
                    listecamion[i].longueur = trouverchemin(listecamion[i], idvilleChoisie, nbsommets,predecesseurs);
                    printf("Camion %d part livrer la ville %d\n", listecamion[i].id, idvilleChoisie);
                    sommets[idvilleChoisie].data.ville.arrivagenourriture += listecamion[i].capacitenourriture;
                    sommets[idvilleChoisie].data.ville.arrivagemedicament += listecamion[i].capacitemedicament;
                } else {
                    if (msg == true) {
                        printf("Aucune ville n'a besoin de ravitaillement\n");
                    }
                }
            } else {
                int identrepot = listeentrepot[rand() % nbentrepot].id;
                listecamion[i].destination = identrepot;
                listecamion[i].longueur = trouverchemin(listecamion[i], identrepot, nbsommets, predecesseurs);
                printf("Camion %d retourne a l'entrepot %d\n", listecamion[i].id, identrepot);
            }
        } else if (listecamion[i].avancementroute < listecamion[i].longueur) {
            listecamion[i].avancementroute += listecamion[i].vitesse;
            printf("Camion %d avance...\n", listecamion[i].id);
        } else {
            int dest = listecamion[i].destination;
            listecamion[i].sommetactuel = dest;

            if (sommets[dest].type == typeville) {
                printf("Camion %d arrive a la ville %d (+%d nourriture / +%d medicament)\n", listecamion[i].id, dest, listecamion[i].capacitenourriture, listecamion[i].capacitemedicament);

                sommets[dest].data.ville.stocknourriture += listecamion[i].capacitenourriture;
                sommets[dest].data.ville.arrivagenourriture -= listecamion[i].capacitenourriture;
                if (listecamion[i].capacitenourriture > 0) {
                    sommets[dest].data.ville.aRecuNourriture = true;
                }

                sommets[dest].data.ville.stockmedicament += listecamion[i].capacitemedicament;
                sommets[dest].data.ville.arrivagemedicament -= listecamion[i].capacitemedicament;
                if (listecamion[i].capacitemedicament > 0) {
                    sommets[dest].data.ville.aRecuMedicament = true;
                }

                listecamion[i].capacitenourriture = 0;
                listecamion[i].capacitemedicament = 0;
            } else {
                printf("Camion %d arrive a l'entrepot %d pour se ravitailler\n", listecamion[i].id, dest);
            }
            listecamion[i].destination = -1;
            listecamion[i].longueur = -1;
            listecamion[i].avancementroute = 0;
        }
    }
}



void gerersommet(int nbsommets) { // faire vivre les sommets (consomation et mort)
    for (int i = 0; i < nbsommets; i++) {
        int besoinmedic = 0;
        int nbmorts = 0;
        switch (sommets[i].type) {
            case typeville:
                if (msg == true) {
                    printf("La ville %d consomme %d nourritures et %d medicaments\n", i, sommets[i].data.ville.nbhabitant, sommets[i].data.ville.nbhabitant / 100 * (100 - sommets[i].data.ville.pointdevie));
                }
                if (sommets[i].data.ville.stocknourriture - sommets[i].data.ville.nbhabitant >= 0) {
                    sommets[i].data.ville.stocknourriture -= sommets[i].data.ville.nbhabitant;
                } else {
                    int manquenour = sommets[i].data.ville.nbhabitant - sommets[i].data.ville.stocknourriture;
                    sommets[i].data.ville.nbhabitant -= manquenour;
                    nbmorts += manquenour;
                    sommets[i].data.ville.stocknourriture = 0;
                }
                besoinmedic = sommets[i].data.ville.nbhabitant / 100 * (100 - sommets[i].data.ville.pointdevie);
                if (sommets[i].data.ville.stockmedicament - besoinmedic >= 0) {
                    sommets[i].data.ville.stockmedicament -= besoinmedic;
                } else {
                    int manquemedic = besoinmedic - sommets[i].data.ville.stockmedicament;
                    sommets[i].data.ville.stockmedicament = 0;
                    int mortsmedic = manquemedic;
                    if (mortsmedic > sommets[i].data.ville.nbhabitant) {
                        mortsmedic = sommets[i].data.ville.nbhabitant;
                    }
                    sommets[i].data.ville.nbhabitant -= mortsmedic;
                    nbmorts += mortsmedic;
                }
                break;
            case typehopital:
                if (msg == true) {
                    printf("L'hopital %d consomme %d medicaments\n", i, sommets[i].data.hopital.nbpatient / 2);
                }
                besoinmedic = sommets[i].data.hopital.nbpatient / 2;
                if (sommets[i].data.hopital.stockmedicament - besoinmedic >= 0) {
                    sommets[i].data.hopital.stockmedicament -= besoinmedic;
                } else {
                    int manquemedichopital = besoinmedic - sommets[i].data.hopital.stockmedicament;
                    sommets[i].data.hopital.stockmedicament = 0;
                    int mortshopital = manquemedichopital;
                    if (mortshopital > sommets[i].data.hopital.nbpatient) {
                        mortshopital = sommets[i].data.hopital.nbpatient;
                    }
                    sommets[i].data.hopital.nbpatient -= mortshopital;
                    nbmorts += mortshopital;
                }
                break;

            case typeentrepot:
                break;
        }
        if (nbmorts > 0) {
            nbmort += nbmorts;
        }
    }
}


void afficherlisteville() { // afficher toutes les villes
    for (int i=0;i<nbville;i++) {
        printf("Ville %d: %dhp, %d habitants,%d nourriture, %d medicament\n", listeville[i].id, sommets[listeville[i].id].data.ville.pointdevie, sommets[listeville[i].id].data.ville.nbhabitant, sommets[listeville[i].id].data.ville.stocknourriture, sommets[listeville[i].id].data.ville.stockmedicament);
    }
}

void maj(int nbsommets) { // faire la maj sur les sous listes trier
    int var = 0;
    for (int i = 0; i < nbsommets; i++) {
        if (sommets[i].type == typeville) {
            listeville[var] = sommets[i];
            var++;
        }
    }
    var = 0;
    for (int i = 0; i < nbsommets; i++) {
        if (sommets[i].type == typehopital) {
            listehopital[var] = sommets[i];
            var++;
        }
    }
    var = 0;
    for (int i = 0; i < nbsommets; i++) {
        if (sommets[i].type == typeentrepot) {
            listeentrepot[var] = sommets[i];
            var++;
        }
    }
}

void console(int nbsommets) { // console pour le debug et l'accessibilité dans la console
    char input[100];
    while(true) {
        printf(">");
        scanf(" %99s", input);
        if (strcmp(input, "route") == 0 || strcmp(input, "routes") == 0) {
            afficherarcs();
        }
        else if (strcmp(input, "map") == 0) {
            affichersommets(nbsommets);
        }
        else if (strcmp(input, "seisme") == 0) {
            seisme(nbsommets);
        }
        else if (strcmp(input, "quit") == 0) {
            return;
        }
        else if (strcmp(input, "inaccessible") == 0) {
            inaccessible(nbsommets);
        }
        else if (strcmp(input, "groupe") == 0) {
            int nbgroupes = detectergroupes(nbsommets);
            printf("Nombre de groupes : %d\n", nbgroupes);
            for (int i = 0; i < nbsommets; i++) {
                printf("Sommet %d est dans le groupe %d\n", i, groupe[i]);
            }
        }
        else if (strcmp(input, "sommet") == 0 || strcmp(input, "sommets") == 0) {
            int sommet;
            printf("Quelle sommet voulez vous inspecter?\n");
            scanf("%d", &sommet);
            infosommet(nbsommets, sommet);
        }
        else if (strcmp(input, "vehicule") == 0 || strcmp(input, "vehicules") == 0) {
            int vehicule;
            printf("Quel vehicule voulez vous inspecter?\n");
            scanf("%d", &vehicule);
            scanf("%d", &vehicule);
            affichervehicule(vehicule);
        }
        else if (strcmp(input, "stat") == 0) {
            stat(nbsommets);
        }
        else if (strcmp(input, "msg") == 0) {
            if (msg == false) {
                msg = true;
                printf("Message active\n");
            }else {
                msg = false;
                printf("Message desactive\n");
            }
        }
        else if (strcmp(input, "next") == 0|| strcmp(input, "n") == 0) {
            printf("Tour %d:\n", tour);
            tour++;
            maj(nbsommets);
            gerersecour(nbsommets);
            gererambulance(nbsommets);
            gerersommet(nbsommets);
            gererbus(nbsommets);
            gerercamion(nbsommets);
        }
        else if (strcmp(input, "listeville") == 0) {
            afficherlisteville();
        }
        else if (strcmp(input, "help") == 0) {
            printf("Commandes disponibles :\n route / routes\n stat\n listeville\n map\n seisme\n inaccessible\n groupe\n sommet / sommets\n nouvelleroute\n reliergroupe \n vehicule / vehicules\n help\n quit\n");
        }
        else {
            printf("Commande inconnue\n");
        }
    }
}

void jeu(int nbsommets) { // appel des fonction de la simulation
    maj(nbsommets);
    trierville();
    trierhopitaux();
    gerersecour(nbsommets);
    gererambulance(nbsommets);
    gerersommet(nbsommets);
    gererbus(nbsommets);
    gerercamion(nbsommets);
    //console(nbsommets); POUR DEBUG
}

int comparerArcs(const void* a, const void* b){
    Arc* arcA = (Arc*)a;
    Arc* arcB = (Arc*)b;
    return arcA->longueur - arcB->longueur;
}

int trouver(int parent[], int i){
    if (parent[i] != i)
        parent[i] = trouver(parent, parent[i]);
    return parent[i];
}

void unir(int parent[], int rang[], int x, int y){
    int xroot = trouver(parent, x);
    int yroot = trouver(parent, y);

    if (rang[xroot] < rang[yroot]){
        parent[xroot] = yroot;
    } else if (rang[xroot] > rang[yroot]){
        parent[yroot] = xroot;
    } else{
        parent[yroot] = xroot;
        rang[xroot]++;
    }
}

int routesecur[1000];
int nbArcsSelectionnes = 0;

void identifierRoutesCritiques(int nbsommets){ // indentification des routes prioritaire 
    qsort(arcs, nbarc, sizeof(Arc), comparerArcs);
    int parent[nbsommets];
    int rang[nbsommets];

    for (int i = 0; i < nbsommets; i++){
        parent[i] = i;
        rang[i] = 0;
    }

    nbArcsSelectionnes = 0;

    for (int i = 0; i < nbarc && nbArcsSelectionnes < nbsommets - 1; i++){
        Arc arc = arcs[i];
        int x = trouver(parent, arc.sommet1);
        int y = trouver(parent, arc.sommet2);

        if (x != y){
            unir(parent, rang, x, y);
            routesecur[nbArcsSelectionnes++] = arc.id;
        }
    }
}
