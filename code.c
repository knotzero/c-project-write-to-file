#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALBUMS 10
#define MAX_PHOTOS 50
#define FILENAME "albums.txt"

typedef struct {
    char name[50];
    int size;
} Photo;

typedef struct {
    char name[50];
    Photo photos[MAX_PHOTOS];
    int photoCount;
} Album;

Album albums[MAX_ALBUMS];
int albumCount = 0;

void clearScreen() {
    printf("\033[H\033[J");
}

void header() {
    printf("--------------\n");
    printf("1 - Create Album\n");
    printf("2 - Add Photo\n");
    printf("3 - Display Albums\n");
    printf("4 - Delete Album\n");
    printf("0 - Exit\n");
    printf("--------------\n");
}

void loadAlbums() {
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) {
        while (fgets(albums[albumCount].name, 50, file) != NULL) {
            albums[albumCount].name[strcspn(albums[albumCount].name, "\n")] = '\0';
            fscanf(file, "%d", &albums[albumCount].photoCount);
            fgetc(file); 
            for (int i = 0; i < albums[albumCount].photoCount; i++) {
                fgets(albums[albumCount].photos[i].name, 50, file);
                albums[albumCount].photos[i].name[strcspn(albums[albumCount].photos[i].name, "\n")] = '\0';
                fscanf(file, "%d", &albums[albumCount].photos[i].size);
                fgetc(file);
            }
            albumCount++;
        }
        fclose(file);
    }
}

void saveAlbums() {
    FILE *file = fopen(FILENAME, "w");
    if (file != NULL) {
        for (int i = 0; i < albumCount; i++) {
            fprintf(file, "%s\n", albums[i].name);
            fprintf(file, "%d\n", albums[i].photoCount);
            for (int j = 0; j < albums[i].photoCount; j++) {
                fprintf(file, "%s\n", albums[i].photos[j].name);
                fprintf(file, "%d\n", albums[i].photos[j].size);
            }
        }
        fclose(file);
    }
}

void createAlbum() {
    clearScreen();
    if (albumCount >= MAX_ALBUMS) {
        printf("Album limit reached!\n");
        return;
    }
    printf("Enter album name: ");
    fgets(albums[albumCount].name, 50, stdin);
    albums[albumCount].name[strcspn(albums[albumCount].name, "\n")] = '\0';
    albums[albumCount].photoCount = 0;
    albumCount++;
    saveAlbums();
    printf("Album created successfully!\n");
}

void addPhoto() {
    clearScreen();
    if (albumCount == 0) {
        printf("No albums available. Create one first.\n");
        return;
    }

    char albumName[50];
    printf("Enter album name: ");
    fgets(albumName, 50, stdin);
    albumName[strcspn(albumName, "\n")] = '\0';
    
    int index = -1;
    for (int i = 0; i < albumCount; i++) {
        if (strcmp(albums[i].name, albumName) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Album not found! Returning to menu.\n");
        return;
    }
    
    if (albums[index].photoCount >= MAX_PHOTOS) {
        printf("Photo limit reached in this album!\n");
        return;
    }
    
    printf("Enter photo name: ");
    fgets(albums[index].photos[albums[index].photoCount].name, 50, stdin);
    albums[index].photos[albums[index].photoCount].name[strcspn(albums[index].photos[albums[index].photoCount].name, "\n")] = '\0';
    printf("Enter photo size (in KB): ");
    scanf("%d", &albums[index].photos[albums[index].photoCount].size);
    getchar();
    albums[index].photoCount++;
    saveAlbums();
    printf("Photo added successfully!\n");
}

void displayAlbums() {
    clearScreen();
    if (albumCount == 0) {
        printf("No albums available!\n");
        return;
    }
    for (int i = 0; i < albumCount; i++) {
        printf("Album [%d]: %s\n", i, albums[i].name);
        int totalSize = 0;
        for (int j = 0; j < albums[i].photoCount; j++) {
            printf("Photo: %s (%d KB)\n", albums[i].photos[j].name, albums[i].photos[j].size);
            totalSize += albums[i].photos[j].size;
        }
        printf("Total Album Size: %d KB\n", totalSize);
    }
}

void deleteAlbum() {
    clearScreen();
    if (albumCount == 0) {
        printf("No albums to delete!\n");
        return;
    }

    char albumName[50];
    printf("Enter album name to delete: ");
    fgets(albumName, 50, stdin);
    albumName[strcspn(albumName, "\n")] = '\0';

    int index = -1;
    for (int i = 0; i < albumCount; i++) {
        if (strcmp(albums[i].name, albumName) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Album not found! Returning to menu.\n");
        return;
    }

    for (int i = index; i < albumCount - 1; i++) {
        albums[i] = albums[i + 1];
    }
    albumCount--;
    saveAlbums();
    printf("Album \"%s\" deleted successfully!\n", albumName);
}

void menu(int op) {
    switch (op) {
        case 1:
            createAlbum();
            break;
        case 2:
            addPhoto();
            break;
        case 3:
            displayAlbums();
            break; 
        case 4:
            deleteAlbum();
            break;
        default:
            printf("Exiting application.\n");
    }
}

int main() {
    loadAlbums();
    
    int option;
    do {
        clearScreen();
        header();
        printf("Enter option: ");
        scanf("%d", &option);
        getchar();
        menu(option);
        printf("Press Enter to continue...");
        getchar();
    } while (option > 0 && option <= 4);

    return 0;
}