#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#include "raylib.h"
#include "raymath.h"

#include "fonction.h"

int menuactuelle = 0;
int selectedsommet = 0;
int selectedVille = -1;
int screenWidth = 1920;
int screenHeight = 1080;
char inputtextSommet[10];
char inputtextCoef[10];
int selectedbus = 0;
char inputtextbus[10];
int selectedsecour = 0;
char inputtextsecour[10];
int selectedambulance = 0;
char inputtextambulance[10];
int selectedcamion = 0;
char inputtextcamion[10];

char inputtexttt[10];
int selectedtt = 100;

Color couleurpv(int pv) { // Fonction pour déterminer la couleur en fonction des point de vie pour une route ou un sommet
    int pointsMax = 100;

    if (pv > pointsMax) pv = pointsMax;
    if (pv < 0) pv = 0;

    int rouge = (int)((1 - (float)pv / pointsMax) * 255);
    int vert = (int)((float)pv / pointsMax * 255);

    return (Color){rouge, vert, 0, 255};
}

void afficher_arcs() { // fonction pour afficher les arcs
    for (int i = 0; i < nbarc; i++) {
        Arc arc = arcs[i];
        int sommet1_id = arc.sommet1;
        int sommet2_id = arc.sommet2;

        int x1, y1, x2, y2;

        switch (sommets[sommet1_id].type) {
            case typeville:
                x1 = sommets[sommet1_id].data.ville.x;
                y1 = sommets[sommet1_id].data.ville.y;
                break;
            case typehopital:
                x1 = sommets[sommet1_id].data.hopital.x;
                y1 = sommets[sommet1_id].data.hopital.y;
                break;
            case typeentrepot:
                x1 = sommets[sommet1_id].data.entrepot.x;
                y1 = sommets[sommet1_id].data.entrepot.y;
                break;
        }

        switch (sommets[sommet2_id].type) {
            case typeville:
                x2 = sommets[sommet2_id].data.ville.x;
                y2 = sommets[sommet2_id].data.ville.y;
                break;
            case typehopital:
                x2 = sommets[sommet2_id].data.hopital.x;
                y2 = sommets[sommet2_id].data.hopital.y;
                break;
            case typeentrepot:
                x2 = sommets[sommet2_id].data.entrepot.x;
                y2 = sommets[sommet2_id].data.entrepot.y;
                break;
        }

        int pv = arc.pointdevie / 2;
        DrawLine(x1, y1, x2, y2, couleurpv(pv));
    }
}

void afficher_sommets() { // fonction pour afficher les sommets avec leurs infos
    for (int i = 0; i < nbsommets; i++) {
        int x = 0, y = 0;

        switch (sommets[i].type) {
            case typeville:
                x = sommets[i].data.ville.x;
                y = sommets[i].data.ville.y;

                int pv = sommets[i].data.ville.pointdevie / 2;

                DrawCircle(x, y, 10, couleurpv(pv));
                DrawText(TextFormat("Ville %d (%dhp):\n %dnour/%dmedic", sommets[i].id, sommets[i].data.ville.pointdevie, sommets[i].data.ville.stocknourriture, sommets[i].data.ville.stockmedicament), x + 12, y - 8, 10, DARKBLUE);
                if (sommets[i].data.ville.aRecuNourriture) {
                    DrawText("Reçu Nourriture", x - 30, y - 30, 10, GREEN);
                }
                if (sommets[i].data.ville.aRecuMedicament) {
                    DrawText("Reçu Medicament", x - 30, y - 20, 10, BLUE);
                }

                break;
            case typeentrepot:
                x = sommets[i].data.entrepot.x;
                y = sommets[i].data.entrepot.y;
                DrawCircle(x, y, 10, BLUE);
                DrawText(TextFormat("Entrepot %d (%dhp):\n %dnour/%dmedic)", sommets[i].id, sommets[i].data.entrepot.pointdevie, sommets[i].data.entrepot.stocknourriture, sommets[i].data.entrepot.stockmedicament), x + 12, y - 8, 10, MAROON);
                if (sommets[i].data.entrepot.aRecuNourriture) {
                    DrawText("Reçu Nourriture", x - 30, y - 30, 10, GREEN);
                }
                if (sommets[i].data.entrepot.aRecuMedicament) {
                    DrawText("Reçu Medicament", x - 30, y - 20, 10, BLUE);
                }

                break;
            case typehopital:
                x = sommets[i].data.hopital.x;
                y = sommets[i].data.hopital.y;
                DrawCircle(x, y, 10, PINK);
                DrawText(TextFormat("Hopital %d (%dhp):\n %dmedic", sommets[i].id, sommets[i].data.hopital.pointdevie, sommets[i].data.hopital.stockmedicament), x + 12, y - 8, 10, MAROON);
                if (sommets[i].data.hopital.aRecuMedicament) {
                    DrawText("Reçu Medicament", x - 30, y - 20, 10, BLUE);
                }

                break;
            default:
                break;
        }
    }
}

void resetarecu(int nbsommets) { // Reset les notification si un sommet a recu des items
    for (int i=0;i<nbsommets;i++) {
        switch (sommets[i].type) {
            case typeville:
                sommets[i].data.ville.aRecuMedicament = false;
                sommets[i].data.ville.aRecuNourriture = false;
                break;
            case typehopital:
                sommets[i].data.hopital.aRecuMedicament = false;
                sommets[i].data.hopital.aRecuNourriture = false;
                break;
            case typeentrepot:
                sommets[i].data.entrepot.aRecuMedicament = false;
                sommets[i].data.entrepot.aRecuNourriture = false;
                break;
        }
    }
}

void afficher_stats(int nbsommets) { // afficher les stats de la simulation
    int x_offset = 20;

    int y_offset = GetScreenHeight() - 300;

    DrawText(TextFormat("Nombre de sommets: %d", nbsommets), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Nombre d'arcs: %d", nbarc), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Nombre de vehicule: %d", idvehicule), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Population globale: %d", population), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Nombre de morts: %d", nbmort), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Nombre de bus: %d", nbbus), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Nombre d'ambulance: %d", nbambulance), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Nombre de camions: %d", nbcamion), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Nombre de secours: %d", nbsecour), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Nombre de villes: %d", nbville), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Nombre d'hopitaux: %d", nbhopital), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Nombre d'entrepots: %d", nbentrepot), x_offset, y_offset, 10, BLACK);
    y_offset += 15;

    DrawText(TextFormat("Taux de destruction: %d%%", taux_destruction(sommets, nbsommets, arcs, nbarc)), x_offset, y_offset, 10, BLACK);
}

void afficher_sommet(int sommet_id) { // afficher les stats des sommets
    int x_offset = 25;
    int y_offset = 100;


    if (sommets[sommet_id].type == typeville) {
        DrawText(TextFormat("%s (Ville %d)", sommets[sommet_id].nom ,sommet_id), x_offset, y_offset, 20, BLACK);
        y_offset += 20;
        DrawText(TextFormat("HP: %d", sommets[sommet_id].data.ville.pointdevie), x_offset, y_offset, 20, BLACK);
        y_offset += 20;
        DrawText(TextFormat("Nourriture: %d", sommets[sommet_id].data.ville.stocknourriture), x_offset, y_offset, 20, BLACK);
        y_offset += 20;
        DrawText(TextFormat("Medicaments: %d", sommets[sommet_id].data.ville.stockmedicament), x_offset, y_offset, 20, BLACK);
        y_offset += 20;
        DrawText(TextFormat("Habitants: %d", sommets[sommet_id].data.ville.nbhabitant), x_offset, y_offset, 20, BLACK);
    }else if (sommets[sommet_id].type == typehopital) {
        DrawText(TextFormat("%s (Hopital %d):", sommets[sommet_id].nom, sommet_id), x_offset, y_offset, 20, BLACK);
        y_offset += 20;
        DrawText(TextFormat("HP: %d", sommets[sommet_id].data.hopital.pointdevie), x_offset, y_offset, 20, BLACK);
        y_offset += 20;
        DrawText(TextFormat("Medicament: %d", sommets[sommet_id].data.hopital.stockmedicament), x_offset, y_offset, 20, BLACK);
        y_offset += 20;
        DrawText(TextFormat("Patients: %d/%d", sommets[sommet_id].data.hopital.nbpatient, sommets[sommet_id].data.hopital.nbpatientmax), x_offset, y_offset, 20, BLACK);
    }else if (sommets[sommet_id].type == typeentrepot) {
        DrawText(TextFormat("%s (Entrepot %d):", sommets[sommet_id].nom, sommet_id), x_offset, y_offset, 20, BLACK);
        y_offset += 20;
        DrawText(TextFormat("HP: %d", sommets[sommet_id].data.entrepot.pointdevie), x_offset, y_offset, 20, BLACK);
        y_offset += 20;
        DrawText(TextFormat("Medicament: %d", sommets[sommet_id].data.entrepot.stockmedicament), x_offset, y_offset, 20, BLACK);
        y_offset += 20;
        DrawText(TextFormat("Nourriture: %d", sommets[sommet_id].data.entrepot.stocknourriture), x_offset, y_offset, 20, BLACK);
    }
}

void afficherbus(int selectedbus) { // afficher les stat des bus
    int x_offset = 25;
    int y_offset = 100;
    DrawText(TextFormat("Bus %d:", listebus[selectedbus].id), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Vitesse: %d", listebus[selectedbus].vitesse), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Tout terrain: %d", listebus[selectedbus].toutterrain), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Humaine: %d/%d", listebus[selectedbus].capacitehumaine, listebus[selectedbus].capacitehumainemax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Nourriture: %d/%d", listebus[selectedbus].capacitenourriture, listebus[selectedbus].capacitenourrituremax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Medicament: %d/%d", listebus[selectedbus].capacitemedicament, listebus[selectedbus].capacitemedicamentmax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Sommet Actuel: %d", listebus[selectedbus].sommetactuel), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Avancement: %d/%d", listebus[selectedbus].avancementroute, listebus[selectedbus].longueur), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Destination: %d/%d", listebus[selectedbus].destination), x_offset, y_offset, 20, BLACK);
}

void affichersecour(int selectedsecour) { // afficher les stat des secours
    int x_offset = 725;
    int y_offset = 100;
    DrawText(TextFormat("Secour %d:", listesecour[selectedsecour].id), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Vitesse: %d", listesecour[selectedsecour].vitesse), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Tout terrain: %d", listesecour[selectedsecour].toutterrain), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Humaine: %d/%d", listesecour[selectedsecour].capacitehumaine, listesecour[selectedsecour].capacitehumainemax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Nourriture: %d/%d", listesecour[selectedsecour].capacitenourriture, listesecour[selectedsecour].capacitenourrituremax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Medicament: %d/%d", listesecour[selectedsecour].capacitemedicament, listesecour[selectedsecour].capacitemedicamentmax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Sommet Actuel: %d", listesecour[selectedsecour].sommetactuel), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Avancement: %d/%d", listesecour[selectedsecour].avancementroute, listesecour[selectedsecour].longueur), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Destination: %d/%d", listesecour[selectedsecour].destination), x_offset, y_offset, 20, BLACK);
}

void afficherambulance(int selectedambulance) { // afficher les stat des ambulances
    int x_offset = 725;
    int y_offset = 300;
    DrawText(TextFormat("Ambulance %d:", listeambulance[selectedambulance].id), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Vitesse: %d", listeambulance[selectedambulance].vitesse), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Tout terrain: %d", listeambulance[selectedambulance].toutterrain), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Humaine: %d/%d", listeambulance[selectedambulance].capacitehumaine, listeambulance[selectedambulance].capacitehumainemax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Nourriture: %d/%d", listeambulance[selectedambulance].capacitenourriture, listeambulance[selectedambulance].capacitenourrituremax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Medicament: %d/%d", listeambulance[selectedambulance].capacitemedicament, listeambulance[selectedambulance].capacitemedicamentmax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Sommet Actuel: %d", listeambulance[selectedambulance].sommetactuel), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Avancement: %d/%d", listeambulance[selectedambulance].avancementroute, listeambulance[selectedambulance].longueur), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Destination: %d/%d", listeambulance[selectedambulance].destination), x_offset, y_offset, 20, BLACK);
}

void affichercamion(int selectedcamion) { // afficher les stat des camions
    int x_offset = 25;
    int y_offset = 300;
    DrawText(TextFormat("Camion %d:", listecamion[selectedcamion].id), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Vitesse: %d", listecamion[selectedcamion].vitesse), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Tout terrain: %d", listecamion[selectedcamion].toutterrain), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Humaine: %d/%d", listecamion[selectedcamion].capacitehumaine, listecamion[selectedcamion].capacitehumainemax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Nourriture: %d/%d", listecamion[selectedcamion].capacitenourriture, listecamion[selectedcamion].capacitenourrituremax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Capacite Medicament: %d/%d", listecamion[selectedcamion].capacitemedicament, listecamion[selectedcamion].capacitemedicamentmax), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Sommet Actuel: %d", listecamion[selectedcamion].sommetactuel), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Avancement: %d/%d", listecamion[selectedcamion].avancementroute, listecamion[selectedcamion].longueur), x_offset, y_offset, 20, BLACK);
    y_offset += 20;
    DrawText(TextFormat("Destination: %d/%d", listecamion[selectedcamion].destination), x_offset, y_offset, 20, BLACK);
}

void boutonsuivant() { // bouton suivant pour les menus
    Rectangle suivant = {screenWidth - 120, screenHeight - 50, 100, 40};
    DrawRectangleRec(suivant, LIGHTGRAY);
    DrawText("Suivant (G)", suivant.x -10, suivant.y + 10, 20, BLACK);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), suivant)) {
        menuactuelle++;
    }

    if (IsKeyPressed(KEY_G)) {
        menuactuelle++;
    }
}

void boutonprecedent() { // bouton précedent pour les menus
    Rectangle precedent = {screenWidth - 270, screenHeight - 50, 100, 40};
    DrawRectangleRec(precedent, LIGHTGRAY);
    DrawText("Precedent (F)", precedent.x, precedent.y + 10, 20, BLACK);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), precedent)) {
        menuactuelle--;
    }
    if (IsKeyPressed(KEY_F)) {
        menuactuelle--;
    }
}

void afficher_menuvehicule() { // menu pour afficher les véhicule avec les 4 types différents
    const int menuWidth = 200;
    const int menuHeight = 200;
    int menuXbus = 350;
    int menuYbus = 90;
    int menuXsecour = 1050;
    int menuYsecour = 90;
    int menuXambulance = 1050;
    int menuYambulance = 300;
    int menuXcamion = 350;
    int menuYcamion = 300;

    static char inputtextbus[10] = "";
    static char inputtextsecour[10] = "";
    static char inputtextambulance[10] = "";
    static char inputtextcamion[10] = "";
    static int selectedbus = 0;
    static int selectedsecour = 0;
    static int selectedambulance = 0;
    static int selectedcamion = 0;

    Rectangle inputBoxbus = { menuXbus + 10, menuYbus + 40, 180, 40 };
    Rectangle buttonBoxbus = { menuXbus + 50, menuYbus + 100, 100, 40 };
    Rectangle inputBoxsecour = { menuXsecour + 10, menuYsecour + 40, 180, 40 };
    Rectangle buttonBoxsecour = { menuXsecour + 50, menuYsecour + 100, 100, 40 };
    Rectangle inputBoxambulance = { menuXambulance + 10, menuYambulance + 40, 180, 40 };
    Rectangle buttonBoxambulance = { menuXambulance + 50, menuYambulance + 100, 100, 40 };
    Rectangle inputBoxcamion = { menuXcamion + 10, menuYcamion + 40, 180, 40 };
    Rectangle buttonBoxcamion = { menuXcamion + 50, menuYcamion + 100, 100, 40 };
    Rectangle boutonplusbus = { menuXbus - 10 , menuYbus + 40, 20, 20 };
    Rectangle boutonmoinsbus = { menuXbus - 10 , menuYbus + 60, 20, 20 };
    Rectangle boutonplussecour = { menuXsecour - 10 , menuYsecour + 40, 20, 20 };
    Rectangle boutonmoinssecour = { menuXsecour - 10 , menuYsecour + 60, 20, 20 };
    Rectangle boutonplusambulance = { menuXambulance - 10 , menuYambulance + 40, 20, 20 };
    Rectangle boutonmoinsambulance = { menuXambulance - 10 , menuYambulance + 60, 20, 20 };
    Rectangle boutonpluscamion = { menuXcamion - 10 , menuYcamion + 40, 20, 20 };
    Rectangle boutonmoinscamion = { menuXcamion - 10 , menuYcamion + 60, 20, 20 };

    Vector2 mouse = GetMousePosition();
    boutonsuivant();
    boutonprecedent();

    DrawRectangle(menuXbus, menuYbus, menuWidth, menuHeight, LIGHTGRAY);
    DrawText(TextFormat("ID Bus (%d):", nbbus), inputBoxbus.x, inputBoxbus.y - 25, 18, DARKGRAY);
    DrawRectangleRec(inputBoxbus, WHITE);
    DrawText(inputtextbus, inputBoxbus.x + 10, inputBoxbus.y + 10, 20, BLACK);
    DrawRectangleLinesEx(inputBoxbus, 2, GRAY);
    int key = GetCharPressed();
    if (key >= 32 && key <= 125 && strlen(inputtextbus) < 9) {
        int len = strlen(inputtextbus);
        inputtextbus[len] = (char)key;
        inputtextbus[len + 1] = '\0';
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(inputtextbus);
        if (len > 0) inputtextbus[len - 1] = '\0';
    }
    DrawRectangleRec(boutonplusbus, SKYBLUE);
    DrawText("+", boutonplusbus.x + 5, boutonplusbus.y + 1, 20, DARKBLUE);
    DrawRectangleRec(boutonmoinsbus, SKYBLUE);
    DrawText("-", boutonmoinsbus.x + 5, boutonmoinsbus.y + 1, 20, DARKBLUE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonplusbus)) {
        selectedbus++;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonmoinsbus)) {
        selectedbus--;
    }
    if (selectedbus < 0) {
        selectedbus = nbbus-1;
    }
    if (selectedbus >= nbbus) {
        selectedbus = 0;
    }
    DrawRectangleRec(buttonBoxbus, SKYBLUE);
    DrawText("Valider", buttonBoxbus.x + 20, buttonBoxbus.y + 10, 20, DARKBLUE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, buttonBoxbus)) {
        int id = atoi(inputtextbus);
        selectedbus = id;
    }
    afficherbus(selectedbus);



    DrawRectangle(menuXsecour, menuYsecour, menuWidth, menuHeight, LIGHTGRAY);
    DrawText(TextFormat("ID Secour (%d):", nbsecour), inputBoxsecour.x, inputBoxsecour.y - 25, 18, DARKGRAY);
    DrawRectangleRec(inputBoxsecour, WHITE);
    DrawText(inputtextsecour, inputBoxsecour.x + 10, inputBoxsecour.y + 10, 20, BLACK);
    DrawRectangleLinesEx(inputBoxsecour, 2, GRAY);
    key = GetCharPressed();
    if (key >= 32 && key <= 125 && strlen(inputtextsecour) < 9) {
        int len = strlen(inputtextsecour);
        inputtextsecour[len] = (char)key;
        inputtextsecour[len + 1] = '\0';
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(inputtextsecour);
        if (len > 0) inputtextsecour[len - 1] = '\0';
    }
    DrawRectangleRec(boutonplussecour, SKYBLUE);
    DrawText("+", boutonplussecour.x + 5, boutonplussecour.y + 1, 20, DARKBLUE);
    DrawRectangleRec(boutonmoinssecour, SKYBLUE);
    DrawText("-", boutonmoinssecour.x + 5, boutonmoinssecour.y + 1, 20, DARKBLUE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonplussecour)) {
        selectedsecour++;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonmoinssecour)) {
        selectedsecour--;
    }
    if (selectedsecour < 0) {
        selectedsecour = nbsecour-1;
    }
    if (selectedsecour >= nbsecour) {
        selectedsecour = 0;
    }
    DrawRectangleRec(buttonBoxsecour, SKYBLUE);
    DrawText("Valider", buttonBoxsecour.x + 20, buttonBoxsecour.y + 10, 20, DARKBLUE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, buttonBoxsecour)) {
        int id = atoi(inputtextsecour);
        selectedsecour = id;
    }
    affichersecour(selectedsecour);



    DrawRectangle(menuXambulance, menuYambulance, menuWidth, menuHeight, LIGHTGRAY);
    DrawText(TextFormat("ID Ambulance (%d):", nbambulance), inputBoxambulance.x, inputBoxambulance.y - 25, 18, DARKGRAY);
    DrawRectangleRec(inputBoxambulance, WHITE);
    DrawText(inputtextambulance, inputBoxambulance.x + 10, inputBoxambulance.y + 10, 20, BLACK);
    DrawRectangleLinesEx(inputBoxambulance, 2, GRAY);
    key = GetCharPressed();
    if (key >= 32 && key <= 125 && strlen(inputtextambulance) < 9) {
        int len = strlen(inputtextambulance);
        inputtextambulance[len] = (char)key;
        inputtextambulance[len + 1] = '\0';
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(inputtextambulance);
        if (len > 0) inputtextambulance[len - 1] = '\0';
    }
    DrawRectangleRec(boutonplusambulance, SKYBLUE);
    DrawText("+", boutonplusambulance.x + 5, boutonplusambulance.y + 1, 20, DARKBLUE);
    DrawRectangleRec(boutonmoinsambulance, SKYBLUE);
    DrawText("-", boutonmoinsambulance.x + 5, boutonmoinsambulance.y + 1, 20, DARKBLUE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonplusambulance)) {
        selectedambulance++;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonmoinsambulance)) {
        selectedambulance--;
    }
    if (selectedambulance < 0) {
        selectedambulance = nbambulance-1;
    }
    if (selectedambulance >= nbambulance) {
        selectedambulance = 0;
    }
    DrawRectangleRec(buttonBoxambulance, SKYBLUE);
    DrawText("Valider", buttonBoxambulance.x + 20, buttonBoxambulance.y + 10, 20, DARKBLUE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, buttonBoxambulance)) {
        int id = atoi(inputtextambulance);
        selectedambulance = id;
    }
    afficherambulance(selectedambulance);



    DrawRectangle(menuXcamion, menuYcamion, menuWidth, menuHeight, LIGHTGRAY);
    DrawText(TextFormat("ID Camion (%d):", nbcamion), inputBoxcamion.x, inputBoxcamion.y - 25, 18, DARKGRAY);
    DrawRectangleRec(inputBoxcamion, WHITE);
    DrawText(inputtextcamion, inputBoxcamion.x + 10, inputBoxcamion.y + 10, 20, BLACK);
    DrawRectangleLinesEx(inputBoxcamion, 2, GRAY);
    key = GetCharPressed();
    if (key >= 32 && key <= 125 && strlen(inputtextcamion) < 9) {
        int len = strlen(inputtextcamion);
        inputtextcamion[len] = (char)key;
        inputtextcamion[len + 1] = '\0';
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(inputtextcamion);
        if (len > 0) inputtextcamion[len - 1] = '\0';
    }
    DrawRectangleRec(boutonpluscamion, SKYBLUE);
    DrawText("+", boutonpluscamion.x + 5, boutonpluscamion.y + 1, 20, DARKBLUE);
    DrawRectangleRec(boutonmoinscamion, SKYBLUE);
    DrawText("-", boutonmoinscamion.x + 5, boutonmoinscamion.y + 1, 20, DARKBLUE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonpluscamion)) {
        selectedcamion++;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonmoinscamion)) {
        selectedcamion--;
    }
    if (selectedcamion < 0) {
        selectedcamion = nbcamion-1;
    }
    if (selectedcamion >= nbcamion) {
        selectedcamion = 0;
    }
    DrawRectangleRec(buttonBoxcamion, SKYBLUE);
    DrawText("Valider", buttonBoxcamion.x + 20, buttonBoxcamion.y + 10, 20, DARKBLUE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, buttonBoxcamion)) {
        int id = atoi(inputtextcamion);
        selectedcamion = id;
    }
    affichercamion(selectedcamion);
}


void afficher_menusommet() { // menu pour afficher les sommets 
    const int menuWidth = 200;
    const int menuHeight = 200;
    int menuX = GetScreenWidth() - menuWidth - 50;
    int menuY = 70;

    static char inputtextSommet[10] = "";
    static int selectedsommet = 0;

    Rectangle inputBoxSommet = { menuX + 10, menuY + 40, 180, 40 };
    Rectangle buttonBox = { menuX + 50, menuY + 100, 100, 40 };
    Rectangle boutonplus = { menuX - 10 , menuY + 40, 20, 20 };
    Rectangle boutonmoins = { menuX - 10 , menuY + 60, 20, 20 };

    Vector2 mouse = GetMousePosition();

    DrawRectangle(menuX, menuY, menuWidth, menuHeight, LIGHTGRAY);

    DrawText("ID Sommet:", inputBoxSommet.x, inputBoxSommet.y - 25, 18, DARKGRAY);
    DrawRectangleRec(inputBoxSommet, WHITE);
    DrawText(inputtextSommet, inputBoxSommet.x + 10, inputBoxSommet.y + 10, 20, BLACK);
    DrawRectangleLinesEx(inputBoxSommet, 2, GRAY);

    int key = GetCharPressed();
    if (key >= 32 && key <= 125 && strlen(inputtextSommet) < 9) {
        int len = strlen(inputtextSommet);
        inputtextSommet[len] = (char)key;
        inputtextSommet[len + 1] = '\0';
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(inputtextSommet);
        if (len > 0) inputtextSommet[len - 1] = '\0';
    }

    boutonsuivant();
    boutonprecedent();

    DrawRectangleRec(boutonplus, SKYBLUE);
    DrawText("+", boutonplus.x + 5, boutonplus.y + 1, 20, DARKBLUE);
    DrawRectangleRec(boutonmoins, SKYBLUE);
    DrawText("-", boutonmoins.x + 5, boutonmoins.y + 1, 20, DARKBLUE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonplus)) {
        selectedsommet++;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonmoins)) {
        selectedsommet--;
    }

    if (selectedsommet < 0) {
        selectedsommet = nbsommets-1;
    }
    if (selectedsommet >= nbsommets) {
        selectedsommet = 0;
    }

    DrawRectangleRec(buttonBox, SKYBLUE);
    DrawText("Valider", buttonBox.x + 20, buttonBox.y + 10, 20, DARKBLUE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, buttonBox)) {
        int id = atoi(inputtextSommet);
        selectedsommet = id;
    }
    afficher_sommet(selectedsommet);
}

void afficher_groupe(int idgroupeaffiche, int nbgroupes) { //afficher les différents groupes
    int x_offset = 50;
    int y_offset = 100;

    DrawText(TextFormat("Groupes détectés: %d", nbgroupes), x_offset, y_offset, 20, BLACK);
    y_offset += 30;
    DrawText(TextFormat("Groupe sélectionné: %d", idgroupeaffiche), x_offset, y_offset, 20, BLACK);
    y_offset += 30;

    DrawText("Sommets dans ce groupe :", x_offset, y_offset, 20, BLACK);
    y_offset += 30;

    for (int i = 0; i < nbsommets; i++) {
        if (groupe[i] == idgroupeaffiche) {
            DrawText(TextFormat("Sommet %d", i), x_offset + 20, y_offset, 18, DARKGRAY);
            y_offset += 20;
        }
    }
}


void afficher_chemin_interactif() { // menu pour afficher les chemins d'un point A à B
    static int sommet_selectionne1 = -1;
    static int sommet_selectionne2 = -1;
    static int chemin[1000];
    static int chemin_length = 0;
    static bool chemin_calcule = false;

    ClearBackground(WHITE);

    static int predecesseurs[1000];

    const int menuWidth = 200;
    const int menuHeight = 200;
    int menuX = 1200;
    int menuY = 90;


    static char inputtexttt[10] = "";

    Rectangle inputBoxtt = { menuX + 10, menuY + 40, 180, 40 };
    Rectangle buttonBox = { menuX + 50, menuY + 100, 100, 40 };
    Rectangle boutonplus = { menuX - 10 , menuY + 40, 20, 20 };
    Rectangle boutonmoins = { menuX - 10 , menuY + 60, 20, 20 };

    static bool afficherInaccessibles = false;
    Rectangle boutonInaccessibles = { menuX + 10, menuY + 250, 180, 30 };


    Vector2 mouse = GetMousePosition();

    static bool atteignable[1000];
    bool sommets_inaccessibles[1000] = { false };

    if (afficherInaccessibles) {
        memset(atteignable, 0, sizeof(atteignable));
        accessible_oriente(0, atteignable);

        for (int i = 0; i < nbsommets; i++) {
            if (!atteignable[i]) {
                sommets_inaccessibles[i] = true;
            }
        }
    }


    DrawRectangleRec(boutonInaccessibles, afficherInaccessibles ? RED : SKYBLUE);
    DrawText("Sommets inaccessibles", boutonInaccessibles.x + 10, boutonInaccessibles.y + 5, 16, BLACK);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonInaccessibles)) {
        afficherInaccessibles = !afficherInaccessibles;
    }



    DrawRectangle(menuX, menuY, menuWidth, menuHeight, LIGHTGRAY);
    DrawText(TextFormat("Tout terrain (%d):", selectedtt), inputBoxtt.x, inputBoxtt.y - 25, 18, DARKGRAY);
    DrawRectangleRec(inputBoxtt, WHITE);
    DrawText(inputtexttt, inputBoxtt.x + 10, inputBoxtt.y + 10, 20, BLACK);
    DrawRectangleLinesEx(inputBoxtt, 2, GRAY);
    int key = GetCharPressed();
    if (key >= 32 && key <= 125 && strlen(inputtexttt) < 9) {
        int len = strlen(inputtexttt);
        inputtexttt[len] = (char)key;
        inputtexttt[len + 1] = '\0';
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(inputtexttt);
        if (len > 0) inputtexttt[len - 1] = '\0';
    }
    DrawRectangleRec(boutonplus, SKYBLUE);
    DrawText("+", boutonplus.x + 5, boutonplus.y + 1, 20, DARKBLUE);
    DrawRectangleRec(boutonmoins, SKYBLUE);
    DrawText("-", boutonmoins.x + 5, boutonmoins.y + 1, 20, DARKBLUE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonplus)) {
        selectedtt++;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, boutonmoins)) {
        selectedtt--;
    }
    if (selectedtt < 0) {
        selectedtt = 100;
    }
    if (selectedtt > 100) {
        selectedtt = 0;
    }
    DrawRectangleRec(buttonBox, SKYBLUE);
    DrawText("Valider", buttonBox.x + 20, buttonBox.y + 10, 20, DARKBLUE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, buttonBox)) {
        int id = atoi(inputtexttt);
        selectedtt = id;
    }

    boutonsuivant();
    boutonprecedent();
    for (int i = 0; i < nbarc; i++) {
        int x1, y1, x2, y2;

        int s1 = arcs[i].sommet1;
        int s2 = arcs[i].sommet2;

        switch (sommets[s1].type) {
            case typeville: x1 = sommets[s1].data.ville.x; y1 = sommets[s1].data.ville.y; break;
            case typehopital: x1 = sommets[s1].data.hopital.x; y1 = sommets[s1].data.hopital.y; break;
            case typeentrepot: x1 = sommets[s1].data.entrepot.x; y1 = sommets[s1].data.entrepot.y; break;
        }

        switch (sommets[s2].type) {
            case typeville: x2 = sommets[s2].data.ville.x; y2 = sommets[s2].data.ville.y; break;
            case typehopital: x2 = sommets[s2].data.hopital.x; y2 = sommets[s2].data.hopital.y; break;
            case typeentrepot: x2 = sommets[s2].data.entrepot.x; y2 = sommets[s2].data.entrepot.y; break;
        }

        DrawLine(x1, y1, x2, y2, DARKGRAY);
    }

    for (int i = 0; i < nbsommets; i++) {
        int x = 100, y = 100;
        switch (sommets[i].type) {
            case typeville: x = sommets[i].data.ville.x; y = sommets[i].data.ville.y; break;
            case typehopital: x = sommets[i].data.hopital.x; y = sommets[i].data.hopital.y; break;
            case typeentrepot: x = sommets[i].data.entrepot.x; y = sommets[i].data.entrepot.y; break;
        }

        Color c = BLACK;
        if (i == sommet_selectionne1 || i == sommet_selectionne2) {
            c = RED;
        } else if (afficherInaccessibles && sommets_inaccessibles[i]) {
            c = MAROON; 
        }

        DrawCircle(x, y, 10, c);

        if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){x, y}, 10) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (i == sommet_selectionne1) {
                sommet_selectionne1 = -1;
                chemin_calcule = false;
                chemin_length = 0;
            } else if (i == sommet_selectionne2) {
                sommet_selectionne2 = -1;
                chemin_calcule = false;
                chemin_length = 0;
            } else if (sommet_selectionne1 == -1) {
                sommet_selectionne1 = i;
                chemin_calcule = false;
            } else if (sommet_selectionne2 == -1) {
                sommet_selectionne2 = i;
                chemin_calcule = false;
            }
        }
    }



    if (!chemin_calcule && sommet_selectionne1 != -1 && sommet_selectionne2 != -1) {
        Vehicule vehicule;
        vehicule.sommetactuel = sommet_selectionne1;
        vehicule.toutterrain = selectedtt;
        chemin_length = 0;

        int distance = trouverchemin(vehicule, sommet_selectionne2, nbsommets, predecesseurs);

        if (distance == -1) {
            chemin_length = 0;
        } else {
            int courant = sommet_selectionne2;
            while (courant != -1) {
                chemin[chemin_length++] = courant;
                courant = predecesseurs[courant];
            }
        }


        chemin_calcule = true;
    }


    for (int i = chemin_length - 1; i > 0; i--) {
    int a = chemin[i];
    int b = chemin[i - 1];
    int x1, y1, x2, y2;

    switch (sommets[a].type) {
        case typeville: x1 = sommets[a].data.ville.x; y1 = sommets[a].data.ville.y; break;
        case typehopital: x1 = sommets[a].data.hopital.x; y1 = sommets[a].data.hopital.y; break;
        case typeentrepot: x1 = sommets[a].data.entrepot.x; y1 = sommets[a].data.entrepot.y; break;
    }

    switch (sommets[b].type) {
        case typeville: x2 = sommets[b].data.ville.x; y2 = sommets[b].data.ville.y; break;
        case typehopital: x2 = sommets[b].data.hopital.x; y2 = sommets[b].data.hopital.y; break;
        case typeentrepot: x2 = sommets[b].data.entrepot.x; y2 = sommets[b].data.entrepot.y; break;
    }

    DrawLine(x1, y1, x2, y2, DARKGRAY);

    int longueur_arc = -1;
    for (int j = 0; j < nbarc; j++) {
        if ((arcs[j].sommet1 == a && arcs[j].sommet2 == b) || (arcs[j].sommet1 == b && arcs[j].sommet2 == a)) {
            longueur_arc = arcs[j].longueur;
            break;
        }
    }

    if (longueur_arc != -1) {
        int midx = (x1 + x2) / 2;
        int midy = (y1 + y2) / 2;
        DrawText(TextFormat("%d", longueur_arc), midx, midy - 10, 14, DARKGRAY);
    }

    DrawLine(x1, y1, x2, y2, GREEN);
    }


    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        sommet_selectionne1 = -1;
        sommet_selectionne2 = -1;
        chemin_length = 0;
        chemin_calcule = false;
    }
    int total_distance = 0;
    for (int i = chemin_length - 1; i > 0; i--) {
        int a = chemin[i];
        int b = chemin[i - 1];
        int x1, y1, x2, y2;

        switch (sommets[a].type) {
            case typeville: x1 = sommets[a].data.ville.x; y1 = sommets[a].data.ville.y; break;
            case typehopital: x1 = sommets[a].data.hopital.x; y1 = sommets[a].data.hopital.y; break;
            case typeentrepot: x1 = sommets[a].data.entrepot.x; y1 = sommets[a].data.entrepot.y; break;
        }

        switch (sommets[b].type) {
            case typeville: x2 = sommets[b].data.ville.x; y2 = sommets[b].data.ville.y; break;
            case typehopital: x2 = sommets[b].data.hopital.x; y2 = sommets[b].data.hopital.y; break;
            case typeentrepot: x2 = sommets[b].data.entrepot.x; y2 = sommets[b].data.entrepot.y; break;
        }

        DrawLine(x1, y1, x2, y2, GREEN);

        for (int j = 0; j < nbarc; j++) {
            if ((arcs[j].sommet1 == a && arcs[j].sommet2 == b) || (arcs[j].sommet1 == b && arcs[j].sommet2 == a)) {
                total_distance += arcs[j].longueur;
                break;
            }
        }
    }

    if (chemin_length == 0 && sommet_selectionne1 != -1 && sommet_selectionne2 != -1) {
        DrawText("Pas de chemin disponible", 20, GetScreenHeight() - 30, 20, RED);
    } else {
        DrawText(TextFormat("Longueur du chemin : %d", total_distance), 20, GetScreenHeight() - 70, 20, BLACK);
    }
    if (afficherInaccessibles) {
        int y_offset = menuY + 290;
        DrawText("Sommets inaccessibles :", menuX, menuY + 290, 18, RED);
        y_offset += 25;
        for (int i = 0; i < nbsommets; i++) {
            if (sommets_inaccessibles[i]) {
                DrawText(TextFormat("- Sommet %d", i), menuX, y_offset, 16, DARKGRAY);
                y_offset += 20;
            }
        }
    }



}


void afficher_menugroupe() { // menu pour afficher les différents groupes avec bouton valider
    static int nbgroupes = 0;
    static int groupeaffiche = 0;

    Rectangle boutonplus = {screenWidth - 140 , 170, 40, 40};
    Rectangle boutonmoins = {screenWidth - 180 , 170, 40, 40};
    Rectangle boutonvalider = {screenWidth - 180, 220, 100, 40};

    Vector2 mouse = GetMousePosition();

    boutonsuivant();
    boutonprecedent();

    DrawRectangleRec(boutonplus, SKYBLUE);
    DrawText("+", boutonplus.x + 12, boutonplus.y + 10, 20, DARKBLUE);

    DrawRectangleRec(boutonmoins, SKYBLUE);
    DrawText("-", boutonmoins.x + 12, boutonmoins.y + 10, 20, DARKBLUE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, boutonplus)) {
            groupeaffiche++;
            if (groupeaffiche >= nbgroupes) groupeaffiche = 0;
        } else if (CheckCollisionPointRec(mouse, boutonmoins)) {
            groupeaffiche--;
            if (groupeaffiche < 0) groupeaffiche = nbgroupes - 1;
        } else if (CheckCollisionPointRec(mouse, boutonvalider)) {
            nbgroupes = detectergroupes(nbsommets);
            groupeaffiche = 0;
        }
    }

    DrawRectangleRec(boutonvalider, GREEN);
    DrawText("Valider", boutonvalider.x + 10, boutonvalider.y + 10, 20, WHITE);

    afficher_groupe(groupeaffiche, nbgroupes);
}



void afficher_menu_accessibilite() { // menu pour afficher les sommets accessible a partir d'un sommet et les routes critiques 
    ClearBackground(WHITE);

    Vector2 mouse = GetMousePosition();

    boutonprecedent();
    boutonsuivant();

    static int sommet_selectionne = -1;         
    static bool atteignable[1000] = { false }; 

    identifierRoutesCritiques(nbsommets);

    for (int i = 0; i < nbsommets; i++) {
        int x = 0, y = 0;
        switch (sommets[i].type) {
            case typeville: x = sommets[i].data.ville.x; y = sommets[i].data.ville.y; break;
            case typehopital: x = sommets[i].data.hopital.x; y = sommets[i].data.hopital.y; break;
            case typeentrepot: x = sommets[i].data.entrepot.x; y = sommets[i].data.entrepot.y; break;
        }

        if (CheckCollisionPointCircle(mouse, (Vector2){x, y}, 10) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (sommet_selectionne == i) {
                sommet_selectionne = -1;
                memset(atteignable, 0, sizeof(atteignable));
            } else {
                sommet_selectionne = i;
                memset(atteignable, 0, sizeof(atteignable));
                accessible_oriente(sommet_selectionne, atteignable); 
            }
        }
    }

    for (int i = 0; i < nbarc; i++) {
        int s1 = arcs[i].sommet1;
        int s2 = arcs[i].sommet2;
        int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

        switch (sommets[s1].type) {
            case typeville: x1 = sommets[s1].data.ville.x; y1 = sommets[s1].data.ville.y; break;
            case typehopital: x1 = sommets[s1].data.hopital.x; y1 = sommets[s1].data.hopital.y; break;
            case typeentrepot: x1 = sommets[s1].data.entrepot.x; y1 = sommets[s1].data.entrepot.y; break;
        }

        switch (sommets[s2].type) {
            case typeville: x2 = sommets[s2].data.ville.x; y2 = sommets[s2].data.ville.y; break;
            case typehopital: x2 = sommets[s2].data.hopital.x; y2 = sommets[s2].data.hopital.y; break;
            case typeentrepot: x2 = sommets[s2].data.entrepot.x; y2 = sommets[s2].data.entrepot.y; break;
        }

        Color arcColor = DARKGRAY; 

        for (int j = 0; j < nbArcsSelectionnes; j++) {
            if (routesecur[j] == arcs[i].id) {
                arcColor = GREEN;
                break;
            }
        }

        DrawLine(x1, y1, x2, y2, arcColor);
    }

    for (int i = 0; i < nbsommets; i++) {
        int x = 0, y = 0;
        switch (sommets[i].type) {
            case typeville: x = sommets[i].data.ville.x; y = sommets[i].data.ville.y; break;
            case typehopital: x = sommets[i].data.hopital.x; y = sommets[i].data.hopital.y; break;
            case typeentrepot: x = sommets[i].data.entrepot.x; y = sommets[i].data.entrepot.y; break;
        }

        Color c = BLACK;
        if (i == sommet_selectionne) {
            c = RED;             
        } else if (sommet_selectionne != -1 && atteignable[i]) {
            c = GREEN;            
        }

        DrawCircle(x, y, 10, c);
        DrawText(TextFormat("%d", i), x - 5, y - 25, 16, DARKGRAY);
    }
}





void afficher_map() { // "main", et menu pour la map principal et la simulation
    InitWindow(screenWidth, screenHeight, "Système de gestion d’une catastrophe naturelle");

    Rectangle boutonNext = {screenWidth - 120, 20, 100, 30};
    SetTargetFPS(60);

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 950.0f, 530.0f };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    bool is_panning = false;
    Vector2 mouse_pan_start_pos = { 0 };


    while (!WindowShouldClose()) {
        if(menuactuelle >= 6){
        menuactuelle = 0;
        }
        if(menuactuelle < 0){
            menuactuelle = 5;
        }

        ClearBackground(RAYWHITE);
        if (menuactuelle == 0) {

            float wheel_move = GetMouseWheelMove();
            if (wheel_move != 0) {
                Vector2 mouse_world_pos_before_zoom = GetScreenToWorld2D(GetMousePosition(), camera);
                camera.zoom += wheel_move * 0.05f;
                if (camera.zoom < 0.2f) camera.zoom = 0.2f;
                if (camera.zoom > 3.0f) camera.zoom = 3.0f;
                Vector2 mouse_world_pos_after_zoom = GetScreenToWorld2D(GetMousePosition(), camera);
                camera.target = Vector2Add(camera.target, Vector2Subtract(mouse_world_pos_before_zoom, mouse_world_pos_after_zoom));
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
                if (!is_panning) { is_panning = true; mouse_pan_start_pos = GetMousePosition(); }
            }

            if (is_panning) {
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
                    Vector2 current_mouse_pos = GetMousePosition();
                    Vector2 mouse_delta = Vector2Subtract(mouse_pan_start_pos, current_mouse_pos);
                    camera.target = Vector2Add(camera.target, Vector2Scale(mouse_delta, 1.0f / camera.zoom));
                    mouse_pan_start_pos = current_mouse_pos;
                } else { is_panning = false; }
            }

            BeginDrawing();
            BeginMode2D(camera);

            afficher_arcs();
            afficher_sommets();
            afficher_stats(nbsommets);

            DrawRectangleRec(boutonNext, LIGHTGRAY);
            DrawText("Next (N)", boutonNext.x + 25, boutonNext.y + 8, 16, BLACK);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), boutonNext)) {
                resetarecu(nbsommets);
                jeu(nbsommets);
            }
            if (IsKeyPressed(KEY_N)) {
                resetarecu(nbsommets);
                jeu(nbsommets);
            }

            boutonsuivant();
            boutonprecedent();
        } else if (menuactuelle == 1){
            afficher_menusommet();
        } else if (menuactuelle == 2) {
            afficher_menuvehicule();
        } else if (menuactuelle == 3) {
            afficher_menugroupe();
        } else if (menuactuelle == 4) {
            afficher_chemin_interactif();
        }else if (menuactuelle == 5) {
            afficher_menu_accessibilite();
        }


        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
}
