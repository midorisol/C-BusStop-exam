#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_SIZE 10
#define MAX_NAME 50

typedef struct {
    char name[MAX_NAME];
    int x;
    int y;
} BusStop;

typedef struct {
    BusStop* stops;
    int capacity;
    int count;
} BusSystem;


BusSystem* init_bus_system(int initial_capacity) {
    BusSystem* system = (BusSystem*)malloc(sizeof(BusSystem));
    if (system == NULL) {
        return NULL;
    }
    system->stops = (BusStop*)malloc(initial_capacity * sizeof(BusStop));
    if (system->stops == NULL) {
        free(system);
        return NULL;
    }
    system->capacity = initial_capacity;
    system->count = 0;
    return system;
}

void free_bus_system(BusSystem* system) {
    if (system != NULL) {
        free(system->stops);
        free(system);
    }
}

// Növeli a tömb méretét, ha szükséges
int expand_if_needed(BusSystem* system) {
    if (system->count >= system->capacity) {
        int new_capacity = system->capacity + 1;  // Csak eggyel növeljük
        BusStop* new_stops = (BusStop*)realloc(system->stops, new_capacity * sizeof(BusStop));
        if (new_stops == NULL) {
            return 0;
        }
        system->stops = new_stops;
        system->capacity = new_capacity;
    }
    return 1;
}

void print_bus_stops(const BusSystem* system) {
    if(system->count == 0) {
        printf("Nincsenek buszmegallok a rendszerben.\n");
        return;
    }

    for(int i = 0; i < system->count; i++) {
        printf("%d. %s (%d, %d)\n", i+1, system->stops[i].name,
               system->stops[i].x, system->stops[i].y);
    }
}

void delete_bus_stop(BusSystem* system, char map[MAP_SIZE][MAP_SIZE]) {
    if(system->count == 0) {
        printf("Nincs torolheto megallo!\n");
        return;
    }

    print_bus_stops(system);

    int index;
    printf("Kerem adja meg a torlendo megallo sorszamat: ");
    scanf("%d", &index);

    if(index < 1 || index > system->count) {
        printf("Hibas sorszam!\n");
        return;
    }

    index--;
    map[system->stops[index].x][system->stops[index].y] = ' ';

    for(int i = index; i < system->count - 1; i++) {
        system->stops[i] = system->stops[i + 1];
    }

    system->count--;
    printf("Megallo sikeresen torolve!\n");
}

void save_list(BusSystem* system) {
    if(system->count == 0) {
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

    for(int i = 0; i < system->count; i++) {
        fprintf(file, "%s\n%d %d\n", system->stops[i].name,
                system->stops[i].x, system->stops[i].y);
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

void create_bus_stop(BusSystem* system, char map[MAP_SIZE][MAP_SIZE]) {
    if (!expand_if_needed(system)) {
        printf("Memoria foglalasi hiba!\n");
        return;
    }

    BusStop newStop;
    printf("Kerem adja meg a megallo nevet: ");
    scanf(" %[^\n]s", newStop.name);

    printf("Kerem adja meg a koordinatakat (0-9 kozott):\n");
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

    system->stops[system->count] = newStop;
    system->count++;
    map[newStop.x][newStop.y] = 'B';
    printf("Megallo sikeresen letrehozva!\n");
}

void load_list(BusSystem* system, char map[MAP_SIZE][MAP_SIZE]) {
    char filename[100];
    printf("Kerem adja meg a fajl nevet: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        printf("Hiba a fajl megnyitasa soran!\n");
        return;
    }

    for(int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            map[i][j] = ' ';
        }
    }
    system->count = 0;

    char name[MAX_NAME];
    int x, y;

    while(fscanf(file, "%[^\n]\n%d %d\n", name, &x, &y) == 3) {
        if(x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE) {
            printf("Hibas koordinatak a fajlban: %s (%d,%d)\n", name, x, y);
            continue;
        }

        if (!expand_if_needed(system)) {
            printf("Memoria foglalasi hiba!\n");
            break;
        }

        strcpy(system->stops[system->count].name, name);
        system->stops[system->count].x = x;
        system->stops[system->count].y = y;
        map[x][y] = 'B';
        system->count++;
    }

    fclose(file);
    printf("Lista sikeresen betoltve! Osszesen %d megallo.\n", system->count);
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
   BusSystem* system = init_bus_system(1);  // Kezdeti kapacitás: 1

    if (system == NULL) {
        printf("Hiba: Nem sikerult inicializalni a rendszert!\n");
        return 1;
    }

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
                if(system->count == 0) {
                    printf("Nincs megjeleníthető buszmegálló!\n");
                } else {
                    show_map(map);
                }
                break;
            case 2:
                print_bus_stops(system);
                break;
            case 3:
                create_bus_stop(system, map);
                break;
            case 4:
                delete_bus_stop(system, map);
                break;
            case 5:
                save_list(system);
                break;
            case 6:
                load_list(system, map);
                break;
            case 7:
                printf("Utvonal tervezes\n");
                break;
            case 8:
                free_bus_system(system);
                printf("Viszontlatasra!\n");
                return 0;
            default:
                printf("Ervenytelen menupont! Kerem valasszon 1-8 kozott.\n");
        }
    }

    free_bus_system(system);
    return 0;
}
