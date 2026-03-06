#ifndef FONCTION_H
#define FONCTION_H
#include <stdbool.h>


enum typevehicule {
    Bus,       // 0
    Camion,    // 1
    Ambulance,  // 2
    Secour     // 3
};

typedef struct Vehicule {
    int id;
    enum typevehicule type;
    int vitesse;
    int toutterrain;
    int capacitehumaine;
    int capacitehumainemax;
    int capacitenourriture;
    int capacitenourrituremax;
    int capacitemedicament;
    int capacitemedicamentmax;
    int sommetactuel;
    int avancementroute;
    int longueur;
    int destination;
    int reparation1;
    int reparation2;
} Vehicule;

enum typesommet {
    typeville,     // 0
    typehopital,   // 1
    typeentrepot   // 2
};

typedef struct Ville {
    int nbhabitant;
    int arrivage;
    int arrivagenourriture;
    int arrivagemedicament;
    int evacuation;
    int stocknourriture;
    int stockmedicament;
    int pointdevie;
    int direction;
    int x;
    int y;
    bool aRecuNourriture;
    bool aRecuMedicament;
} Ville;

typedef struct Hopital {
    int nbpatient;
    int nbpatientmax;
    int arrivagemedicament;
    int stockmedicament;
    int pointdevie;
    int x;
    int y;
    bool aRecuNourriture;
    bool aRecuMedicament;
} Hopital;

typedef struct Entrepot {
    int stocknourriture;
    int stockmedicament;
    int pointdevie;
    int x;
    int y;
    bool aRecuNourriture;
    bool aRecuMedicament;
} Entrepot;

typedef struct Sommet { 
    int id;
    enum typesommet type;
    char* nom;
    union {
        Ville ville;
        Hopital hopital;
        Entrepot entrepot;
    } data;

} Sommet;

typedef struct Arc { //routes
    int id;
    int sommet1;
    int sommet2;
    int pointdevie;
    int longueur;
    int nbvehicule;
    bool intervention;
} Arc;

typedef struct dijkstra{ // pour le parcour dijkstra uniquement
    int sommet;
    int precedent;
    int distance;
    bool visite;
} dijkstra;

void creergraphe(int nbsommets);
void jeu(int nbsommets);
void seisme(int nbsommets);
void spawnvehicule(int nbsommets);
void afficher_map();
int detectergroupes(int nbsommets);
int trouverchemin(Vehicule vehicule, int destination, int nbsommets, int *predecesseurs);
void accessible_oriente(int depart, bool* visite);
bool sommets_accessibles_depuis(int source, int cible);
int taux_destruction(const Sommet* sommets, int nbsommets, const Arc* arcs, int nbarc);
void identifierRoutesCritiques(int nbsommets);

extern Sommet sommets[];
extern int nbsommets;
extern Arc* arcs;
extern int nbarc;
extern int idvehicule;
extern int population;
extern int nbmort;
extern int nbbus;
extern int nbambulance;
extern int nbcamion;
extern int nbsecour;
extern int nbville;
extern int nbhopital;
extern int nbentrepot;
extern int coefarc;
extern int choixMode;
extern int groupe[];
extern Vehicule listebus[];
extern Vehicule listeambulance[];
extern Vehicule listesecour[];
extern Vehicule listecamion[];
extern int nbbus;
extern int nbsecour;
extern int nbcamion;
extern int nbambulance;
extern int routesecur[];
extern int nbArcsSelectionnes;

#endif
