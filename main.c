#include "raylib.h"
#include "fonction.h"
#include <string.h>
#include <stdlib.h>

int coefarc = 7;
int choixMode = 1;

int main() { // menu de séléction des parametres de la simulation
    const int screenWidth = 600;
    const int screenHeight = 550;

    InitWindow(screenWidth, screenHeight, "Configuration");

    char inputTextSommet[10] = "50";
    char inputTextCoef[10] = "7";
    int nbsommetsTemp = 0;
    int coefarcTemp = 0;
    bool enterPressed = false;

    Rectangle inputBoxSommet = { screenWidth / 2 - 100, 120, 200, 40 };
    Rectangle inputBoxCoef = { screenWidth / 2 - 100, 210, 200, 40 };
    Rectangle buttonBox = { screenWidth / 2 - 60, 430, 120, 40 };
    Rectangle dropdownBox = { screenWidth / 2 - 100, 300, 200, 40 };

    const char *options[] = {"Mode 1", "Mode 2"};
    int dropdownActive = 0;
    int selectedOption = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose() && !enterPressed) {
        ClearBackground(RAYWHITE);
        Vector2 mouse = GetMousePosition();

        BeginDrawing();

        DrawText("Configuration initiale", screenWidth / 2 - 120, 30, 24, DARKGRAY);
        DrawText("Nombre de sommets :", inputBoxSommet.x, inputBoxSommet.y - 25, 18, DARKGRAY);
        DrawRectangleRec(inputBoxSommet, LIGHTGRAY);
        DrawText(inputTextSommet, inputBoxSommet.x + 10, inputBoxSommet.y + 10, 20, BLACK);
        DrawRectangleLinesEx(inputBoxSommet, 2, GRAY);

        DrawText("Coefficient arc : (bas => + d'arcs)", inputBoxCoef.x, inputBoxCoef.y - 25, 18, DARKGRAY);
        DrawRectangleRec(inputBoxCoef, LIGHTGRAY);
        DrawText(inputTextCoef, inputBoxCoef.x + 10, inputBoxCoef.y + 10, 20, BLACK);
        DrawRectangleLinesEx(inputBoxCoef, 2, GRAY);

        DrawText("Choisir un mode :", dropdownBox.x, dropdownBox.y - 25, 18, DARKGRAY);
        DrawRectangleRec(dropdownBox, LIGHTGRAY);
        DrawText(options[selectedOption], dropdownBox.x + 10, dropdownBox.y + 10, 20, BLACK);
        DrawRectangleLinesEx(dropdownBox, 2, GRAY);

        if (dropdownActive) {
            for (int i = 0; i < 2; i++) {
                Rectangle optRect = { dropdownBox.x, dropdownBox.y + (i + 1) * 40, dropdownBox.width, 40 };
                DrawRectangleRec(optRect, (i == selectedOption) ? SKYBLUE : LIGHTGRAY);
                DrawText(options[i], optRect.x + 10, optRect.y + 10, 20, BLACK);
                DrawRectangleLinesEx(optRect, 1, GRAY);

                if (CheckCollisionPointRec(mouse, optRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedOption = i;
                    dropdownActive = 0;
                }
            }
        }

        if (CheckCollisionPointRec(mouse, dropdownBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            dropdownActive = !dropdownActive;
        }

        DrawRectangleRec(buttonBox, SKYBLUE);
        DrawText("Valider", buttonBox.x + 20, buttonBox.y + 10, 20, DARKBLUE);

        int key = GetCharPressed();
        if (key >= 32 && key <= 125) {
            if (CheckCollisionPointRec(mouse, inputBoxSommet) && strlen(inputTextSommet) < 9) {
                int len = strlen(inputTextSommet);
                inputTextSommet[len] = (char)key;
                inputTextSommet[len + 1] = '\0';
            }
            if (CheckCollisionPointRec(mouse, inputBoxCoef) && strlen(inputTextCoef) < 9) {
                int len = strlen(inputTextCoef);
                inputTextCoef[len] = (char)key;
                inputTextCoef[len + 1] = '\0';
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (CheckCollisionPointRec(mouse, inputBoxSommet)) {
                int len = strlen(inputTextSommet);
                if (len > 0) inputTextSommet[len - 1] = '\0';
            }
            if (CheckCollisionPointRec(mouse, inputBoxCoef)) {
                int len = strlen(inputTextCoef);
                if (len > 0) inputTextCoef[len - 1] = '\0';
            }
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, buttonBox)) {
            nbsommetsTemp = atoi(inputTextSommet);
            coefarcTemp = atoi(inputTextCoef);
            if (nbsommetsTemp > 0 && coefarcTemp > 0) {
                choixMode = selectedOption + 1;
                nbsommets = nbsommetsTemp;
                coefarc = coefarcTemp;
                enterPressed = true;
            }
        }

        EndDrawing();
    }

    CloseWindow();

    if (enterPressed) {
        creergraphe(nbsommets);
        spawnvehicule(nbsommets);
        seisme(nbsommets);
        afficher_map();
    }

    return 0;
}
