#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_INVENTORY 3
#define MAX_DESCRIPTION 256

typedef struct Room Room;

typedef struct {
    char name[MAX_DESCRIPTION];
    int health;
    int strength;
} Creature;

struct Room {
    char description[MAX_DESCRIPTION];
    char *items[MAX_INVENTORY];
    Creature *creature;
    Room *connections[4];
};

typedef struct {
    int health;
    int strength;
    char *inventory[MAX_INVENTORY];
    Room *currentRoom;
} Player;

void look(Player *player);
void move(Player *player, const char *direction);
void pickup(Player *player, const char *item);
void attack(Player *player);
void inventory(Player *player);
void stats(Player *player);
void saveGame(Player *player, const char *filepath);
void loadGame(Player *player, Room *rooms, const char *filepath);
void listSaves(const char *directory);
void freeRooms(Room *rooms, int roomCount);
void controls();  // Yeni eklenen kontrol fonksiyonu

Room *createRoom(const char *description, Creature *creature);
Creature *createCreature(const char *name, int health, int strength);
void connectRooms(Room *room1, Room *room2, int direction);
int findEmptyInventorySlot(Player *player);

enum { UP = 0, DOWN, LEFT, RIGHT };

int main() {
    Creature *goblin = createCreature("Goblin", 20, 5);
    Creature *orc = createCreature("Orc", 40, 10);
    Creature *troll = createCreature("Troll", 60, 15);
    Creature *dragon = createCreature("Dragon", 100, 20);

    Room *room1 = createRoom("A dark, damp dungeon room.", NULL);
    Room *room2 = createRoom("A narrow corridor with flickering torches.", goblin);
    Room *room3 = createRoom("A treasure room with a golden chest.", NULL);
    Room *room4 = createRoom("A spooky forest with thick fog.", orc);
    Room *room5 = createRoom("A cavern filled with echoing roars.", troll);
    Room *room6 = createRoom("A fiery pit with molten lava.", dragon);

    room1->items[0] = "wooden sword";
    room4->items[0] = "iron shield";
    room5->items[0] = "energy potion";
    room6->items[0] = "fireproof cloak";

    connectRooms(room1, room2, RIGHT);
    connectRooms(room2, room1, LEFT);
    connectRooms(room2, room3, UP);
    connectRooms(room3, room2, DOWN);
    connectRooms(room3, room4, RIGHT);
    connectRooms(room4, room3, LEFT);
    connectRooms(room4, room5, DOWN);
    connectRooms(room5, room4, UP);
    connectRooms(room5, room6, LEFT);
    connectRooms(room6, room5, RIGHT);

    Player player = {100, 10, {NULL}, room1};

    char command[50];
    char arg[50];
    printf("Welcome to the DUNGEON CRAWLER player!\n\n");
    printf("Type controls for all commands.\n");
    printf("Type list for saved games.\n");

    while (1) {
        printf("\n> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        char *cmd = strtok(command, " ");
        char *param = strtok(NULL, "\n");

        if (cmd) {
            if (strcmp(cmd, "look") == 0) {
                look(&player);
            } else if (strcmp(cmd, "move") == 0 && param) {
                move(&player, param);
            } else if (strcmp(cmd, "pickup") == 0 && param) {
                pickup(&player, param);
            } else if (strcmp(cmd, "attack") == 0) {
                attack(&player);
            } else if (strcmp(cmd, "inventory") == 0) {
                inventory(&player);
            } else if (strcmp(cmd, "stats") == 0) { 
                stats(&player);
            } else if (strcmp(cmd, "save") == 0 && param) {
                saveGame(&player, param);
            } else if (strcmp(cmd, "load") == 0 && param) {
                loadGame(&player, room1, param);
            } else if (strcmp(cmd, "list") == 0) {
                listSaves("."); 
            } else if (strcmp(cmd, "controls") == 0) {  // Yeni komut
                controls();
            } else if (strcmp(cmd, "exit") == 0) {
                printf("Goodbye!\n");
                break;
            } else {
                printf("Invalid command. Try again.\n");
            }
        }
    }

    freeRooms(room1, 6);
    return 0;
}

void look(Player *player) {
    printf("%s\n", player->currentRoom->description);
    if (player->currentRoom->creature) {
        printf("You see a %s here.\n", player->currentRoom->creature->name);
    }
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (player->currentRoom->items[i]) {
            printf("You see a %s on the ground.\n", player->currentRoom->items[i]);
        }
    }
}

void move(Player *player, const char *direction) {
    int dir;
    if (strcmp(direction, "up") == 0) dir = UP;
    else if (strcmp(direction, "down") == 0) dir = DOWN;
    else if (strcmp(direction, "left") == 0) dir = LEFT;
    else if (strcmp(direction, "right") == 0) dir = RIGHT;
    else {
        printf("Invalid direction.\n");
        return;
    }

    if (player->currentRoom->connections[dir]) {
        player->currentRoom = player->currentRoom->connections[dir];
        printf("You moved %s.\n", direction);
    } else {
        printf("You can't go that way.\n");
    }
}

void pickup(Player *player, const char *item) {
    int slot = findEmptyInventorySlot(player);
    if (slot == -1) {
        printf("Your inventory is full!\n");
        return;
    }

    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (player->currentRoom->items[i] && strcmp(player->currentRoom->items[i], item) == 0) {
            player->inventory[slot] = player->currentRoom->items[i];
            player->currentRoom->items[i] = NULL;
            printf("You picked up the %s.\n", item);
            return;
        }
    }

    printf("Item not found in this room.\n");
}

void attack(Player *player) {
    if (!player->currentRoom->creature) {
        printf("There's nothing to attack here!\n");
        return;
    }

    Creature *creature = player->currentRoom->creature;
    while (player->health > 0 && creature->health > 0) {
        printf("You hit the %s for %d damage.\n", creature->name, player->strength);
        creature->health -= player->strength;
        if (creature->health <= 0) {
            printf("You defeated the %s!\n", creature->name);
            player->currentRoom->creature = NULL;
            free(creature);
            return;
        }

        printf("The %s hits you for %d damage.\n", creature->name, creature->strength);
        player->health -= creature->strength;
        if (player->health <= 0) {
            printf("You have been defeated. Game over!\n");
            exit(0);
        }
    }
}

void inventory(Player *player) {
    printf("Your inventory:\n");
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (player->inventory[i]) {
            printf("- %s\n", player->inventory[i]);
        }
        else {
            printf("- [empty]\n");  // Eðer envanterde boþ bir alan varsa, [empty] yazdýrýlýr
        }
    }
}

void stats(Player *player) {
    printf("Player Stats:\n");
    printf("Health: %d\n", player->health);
    printf("Strength: %d\n", player->strength);

    if (player->currentRoom->creature) {
        Creature *creature = player->currentRoom->creature;
        printf("\nCreature Stats:\n");
        printf("Name: %s\n", creature->name);
        printf("Health: %d\n", creature->health);
        printf("Strength: %d\n", creature->strength);
    } else {
        printf("\nNo creature in this room.\n");
    }
}

Room *createRoom(const char *description, Creature *creature) {
    Room *room = malloc(sizeof(Room));
    strncpy(room->description, description, MAX_DESCRIPTION);
    room->creature = creature;
    for (int i = 0; i < 4; i++) room->connections[i] = NULL;
    for (int i = 0; i < MAX_INVENTORY; i++) room->items[i] = NULL;
    return room;
}

Creature *createCreature(const char *name, int health, int strength) {
    Creature *creature = malloc(sizeof(Creature));
    strncpy(creature->name, name, MAX_DESCRIPTION);
    creature->health = health;
    creature->strength = strength;
    return creature;
}

void connectRooms(Room *room1, Room *room2, int direction) {
    room1->connections[direction] = room2;
    room2->connections[(direction + 2) % 4] = room1;
}

int findEmptyInventorySlot(Player *player) {
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (!player->inventory[i]) return i;
    }
    return -1;
}

void freeRooms(Room *rooms, int roomCount) {
    for (int i = 0; i < roomCount; i++) {
        free(&rooms[i]);
    }
}

void saveGame(Player *player, const char *filepath) {
    FILE *file = fopen(filepath, "w");
    if (file) {
        fprintf(file, "Player Health: %d\n", player->health);
        fprintf(file, "Player Strength: %d\n", player->strength);
        
        // Envanterdeki eþyalarý kaydet
        fprintf(file, "Inventory:\n");
        for (int i = 0; i < MAX_INVENTORY; i++) {
            if (player->inventory[i]) {
                fprintf(file, "%s\n", player->inventory[i]);
            } else {
                fprintf(file, "[empty]\n");  // Boþ slotlarý belirt
            }
        }

        fclose(file);
        printf("Game saved!\n");
    } else {
        printf("Error saving game!\n");
    }
}

void loadGame(Player *player, Room *rooms, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (file) {
        fscanf(file, "Player Health: %d\n", &player->health);
        fscanf(file, "Player Strength: %d\n", &player->strength);
        
        // Envanteri yükle
        char line[MAX_DESCRIPTION];
        fscanf(file, "Inventory:\n");
        for (int i = 0; i < MAX_INVENTORY; i++) {
            if (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\n")] = 0;  // '\n' karakterini temizle
                if (strcmp(line, "[empty]") != 0) {
                    player->inventory[i] = strdup(line);  // Eþyayý envantere ekle
                }
            }
        }

        fclose(file);
        printf("Game loaded!\n");
    } else {
        printf("Error loading game!\n");
    }
}

void listSaves(const char *directory) {
    DIR *dir = opendir(directory);
    if (!dir) {
        printf("Error: Could not open directory '%s'.\n", directory);
        return;
    }

    printf("Saved games:\n");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".save")) {
            printf("- %s\n", entry->d_name);
        }
    }

    closedir(dir);
}

void controls() {
    printf("Dungeon Crawler - Controls:\n");
    printf("look - Describes the current room and its contents.\n");
    printf("move [direction] - Moves the player in the specified direction (up, down, left, right).\n");
    printf("pickup [item] - Picks up an item from the room and adds it to your inventory.\n");
    printf("attack - Attacks the creature in the current room (if any).\n");
    printf("inventory - Shows the items in your inventory and indicates empty slots with [empty].\n");
    printf("stats - Displays the player's health, strength, and the creature's stats in the current room.\n");
    printf("save [filename] - Saves the current game state to the specified file.\n");
    printf("load [filename] - Loads the game state from the specified save file.\n");
    printf("list - Lists all available save files in the current directory.\n");
    printf("exit - Exits the game.\n");
}

