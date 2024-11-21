#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME_MAX_LENGTH 50
#define TITLE_MAX_LENGTH 256
#define IDENTIFIER_MAX_LENGTH 20

void createSongFile(const char *username, const char *iswc, const char *songInfo) {
    char filename[FILENAME_MAX_LENGTH];

    snprintf(filename, sizeof(filename), "Server_%s_%s.txt", username, iswc);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error creating file");
        return;
    }

    fprintf(file, "%s\n%s\n", iswc, songInfo);
    
    fclose(file);

    printf("Created file: %s\n", filename);
}

int main() {
    char input[IDENTIFIER_MAX_LENGTH];
    char username[FILENAME_MAX_LENGTH];
    char iswc[FILENAME_MAX_LENGTH];
    char songInfo[TITLE_MAX_LENGTH];

    while (1) {
        printf("Enter [Username]_T-XXX.XXX.XXX, where [Username] is replaced by a username and each X is replaced with a digit 0-9. (or type 'exit' to quit): ");
        scanf("%s", input);

        if (strcmp(input, "exit") == 0) {
            break;
        }

        char *underscore = strchr(input, '_');
        if (underscore == NULL) {
            printf("Invalid format. Please use [Username]_T-XXX.XXX.XXX.\n");
            continue;
        }

        //parse string by underscore
        *underscore = '\0';
        strncpy(username, input, sizeof(username));
        strcpy(iswc, underscore + 1);

        printf("Enter song title and artist (format: 'Song Title - Artist'): ");
        scanf(" %[^\n]s", songInfo); 

        createSongFile(username, iswc, songInfo);
    }

    printf("Exiting program. Goodbye!\n");
    return 0;
}