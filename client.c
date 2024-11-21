#include <stdio.h>
#include <stdlib.h>
#include "ClientHandler.h"
#include "Macros.h"

int main() {
    printf("Welcome (Back) to UFMyMusic!\n");
    int clientSocket = createClientSocket();

    // Reading in the Username
    char usernameBuffer[USERNAME_LENGTH+10] = "";
    char username[USERNAME_LENGTH] = "";

    while (1) {
        printf("Enter Username: ");
        fgets(usernameBuffer, sizeof(usernameBuffer), stdin);
        
        // Reading the Possibly Leftover Bits in Stream
        char ch = '\0';
        if (strlen(usernameBuffer) == USERNAME_LENGTH+9)
            while((ch = getchar()) != '\n' && ch != EOF);
        usernameBuffer[strlen(usernameBuffer)-1] = '\0';

        // Checking Username's Validity
        if (isAlphaNumeric(usernameBuffer) == 1 && strlen(usernameBuffer) < USERNAME_LENGTH)
            break;

        // Handling Invalid Usernames
        printf(ANSI_COLOR_RED "\nInvalid Username.\n" ANSI_COLOR_RESET);
        printf("Usernames must\n");
        printf("\t(1) have a length equal to or less than 200 characters; and\n");
        printf("\t(2) only have letters and/or numbers.\n");
        printf("Try again.\n\n");
    }
    memcpy(username, usernameBuffer, sizeof(username));
    sendBytes(clientSocket, username, USERNAME_LENGTH);

    // Communicating with the Server
    char actionBuffer[4] = "";
    int action = 0;

    while (1) {
        // Printing the Menu
        printf("Actions:\n");
        printf("1.\tLIST:\n\tShow the files you currently have on server.\n");
        printf("2.\tDIFF:\n\tShow the files that you don't have.\n");
        printf("3.\tPULL:\n\tPull the files that you don't have.\n");
        printf("4.\tLEAVE\n\n");

        // Scanning the User's Input
        printf("Enter Action: ");
        if (fgets(actionBuffer, sizeof(actionBuffer), stdin) == NULL)
            killProcess("Failed to Read Action");

        // Reading the Possibly Leftover Bits in Stream
        char ch = '\0';
        if (strlen(actionBuffer) == 3)
            while((ch = getchar()) != '\n' && ch != EOF);
        actionBuffer[strlen(actionBuffer)-1] = '\0';

        action = strtoul(actionBuffer, NULL, 0);
        printf("\n");

        // Received Invalid Action
        if (action < 1 || action > 4) {
            printf("Invalid Action. Try Again.\n\n");
            continue;
        }

        // Redirecting Control
        switch (action) {
            case 1:
                clientList(clientSocket);
                break;
            case 2:
                clientDiff(username, clientSocket);
                break;
            case 3:
                clientPull(username, clientSocket);
                break;
            case 4:
            default:
                clientLeave(clientSocket);
                printf("Thank you for using UFMyMusic.\n");
                printf("Have a great day, %s!\n", username);
                close(clientSocket);
                return 0;
        }
        printf("\n");
    }
}