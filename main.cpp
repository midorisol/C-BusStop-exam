#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_SIZE 10
#define MAX_STOPS 20
#define MAX_NAME 50

typedef struct {
    char name[MAX_NAME];
    int x;
    int y;
} BusStop;

BusStop stops[MAX_STOPS];
int stopCount = 0;

void print_bus_stops() {
    if(stopCount == 0) {
        printf("Nincsenek buszmegallok a rendszerben.\n");
        return;
    }

    for(int i = 0; i < stopCount; i++) {
        printf("%d. %s (%d, %d)\n", i+1, stops[i].name, stops[i].x, stops[i].y);
    }
}

void delete_bus_stop(char map[MAP_SIZE][MAP_SIZE]) {
    if(stopCount == 0) {
        printf("Nincs torolheto megallo!\n");
        return;
    }

    print_bus_stops();

    int index;
    printf("Kerem adja meg a torlendo megallo sorszamat: ");
    scanf("%d", &index);

    if(index < 1 || index > stopCount) {
        printf("Hibas sorszam!\n");
        return;
    }

    index--; // A felhasználó 1-től számoz, mi 0-tól

    // Töröljük a megállót a térképről
    map[stops[index].x][stops[index].y] = ' ';

    // Tömörítjük a tömböt
    for(int i = index; i < stopCount - 1; i++) {
        stops[i] = stops[i + 1];
    }

    stopCount--;
    printf("Megallo sikeresen torolve!\n");
}


void save_list() {
    if(stopCount == 0) {
        printf("Nincs mentheto lista!\n");
        return;
    }

    char filename[100];
    printf("Kerem adja meg a fajl nevet: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "w");
    if(file == NULL) {
        printf("Hiba a fajl letrehozasa soran!\n");
        return;
    }

    for(int i = 0; i < stopCount; i++) {
        fprintf(file, "%s\n%d %d\n", stops[i].name, stops[i].x, stops[i].y);
    }

    fclose(file);
    printf("Lista sikeresen mentve!\n");
}

void show_map(char map[MAP_SIZE][MAP_SIZE]) {
    printf("  ");
    for(int i = 0; i < MAP_SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for(int i = 0; i < MAP_SIZE; i++) {
        printf("%d ", i);
        for(int j = 0; j < MAP_SIZE; j++) {
            if(map[i][j] == 'B') {
                printf("B ");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
}



void create_bus_stop(char map[MAP_SIZE][MAP_SIZE]) {
    if(stopCount >= MAX_STOPS) {
        printf("Nem lehet tobb megallot letrehozni, a memoria megtelt!\n");
        return;
    }

    BusStop newStop;
    printf("Kerem adja meg a megallo nevet: ");
    scanf(" %[^\n]s", newStop.name);

    printf("Kerem adja meg a koordinatakat (1-10 kozott):\n");
    printf("X koordinata: ");
    scanf("%d", &newStop.x);
    printf("Y koordinata: ");
    scanf("%d", &newStop.y);

    if(newStop.x < 0 || newStop.x >= MAP_SIZE ||
       newStop.y < 0 || newStop.y >= MAP_SIZE) {
        printf("Hibas koordinatak! A koordinataknak 0-9 kozott kell lenniuk.\n");
        return;
    }

    if(map[newStop.x][newStop.y] == 'B') {
        printf("Ezen a helyen mar letezik megallo!\n");
        return;
    }

    stops[stopCount] = newStop;
    stopCount++;
    map[newStop.x][newStop.y] = 'B';
    printf("Megallo sikeresen letrehozva!\n");
}

void displayMenu() {
    printf("\nBuszmegallo kezelo program\n");
    printf("1) Terkep - az aktualis varos buszmegalloiank megjelenítese\n");
    printf("2) Lista - az aktualis varos buszmegalloiank felsorolasa\n");
    printf("3) Uj megallo - uj megallo letrehozasa\n");
    printf("4) Megallo torles - megallo torlese\n");
    printf("5) Mentes - mentes fajlba\n");
    printf("6) Betoltes - betoltes fajlbol\n");
    printf("7) Utvonal - ket allomas kozti utvonal\n");
    printf("8) Kilepes\n");
    printf("\nValasszon menupontot: ");
}

int main() {
    char map[MAP_SIZE][MAP_SIZE];
    int choice;

    for(int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            map[i][j] = ' ';
        }
    }

    printf("Udvozoljuk a buszmegallo kezelo programban!\n");
    printf("A program segitsegevel kezelheti egy varos buszmegalloit.\n");

    while(1) {
        displayMenu();
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                if(stopCount == 0) {
                    printf("Nincs megjeleníthető buszmegálló!\n");
                } else {
                    show_map(map);
                }
                break;
            case 2:
                print_bus_stops();
                break;
            case 3:
                create_bus_stop(map);
                break;
            case 4:
                delete_bus_stop(map);
                break;
            case 5:
                save_list();
                break;
            case 6:
                printf("Adatok betoltese fajlbol\n");
                break;
            case 7:
                printf("Utvonal tervezes\n");
                break;
            case 8:
                printf("Viszontlatasra!\n");
                return 0;
            default:
                printf("Ervenytelen menupont! Kerem valasszon 1-8 kozott.\n");
        }
    }
    return 0;
}
