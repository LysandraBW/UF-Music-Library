# Overview
UFmyMusic combines music across machines by syncing files from the server to the client. The server acts as a catch-all storage unit, holding all songs saved across multiple machines for a given user. These songs may not be accessible from your local machine, but are quickly downloadable with the PULL function. UFmyMusic supports concurrent client requests due to the multithreaded server.

## Actions
UFmyMusic provides four different actions for users: (1) LIST, (2) DIFF, (3) PULL, and (4) EXIT. The user will be prompted to choose from any of the four options at the start of the program and after actions (1)-(3).

(1) LIST lists all the files the server has stored for a client. Thus, all files displayed will be present in the server. They may also exist in the client, but will never solely exist in the client.

(2) DIFF displays the files that are (1) present in both the client and server (represented by a plus symbol "+"); and (2) only present in the server (represented by a minus symbol "-"). This command must be called before PULL (if PULL is to do anything useful).

(3) PULL pulls the files that the client doesn't have from the server. This function creates a copy of the server's file. However, to upkeep with the organization of files (i.e. who owns what file), the filename will have "Client" instead of "Server". If there are no files to pull, nothing happens.

(4) LEAVE closes the client connection. On the receiving side, the server will also free any memory it was using for that client.

## Compiling and Running UFmyMusic
1. Run "make". After executing this command, you should have three executables: server, client, and create_songs.

2. To create song files, you can either use method 1 (using a program to easily create the files, prevents human-made errors), method 2 (manually creating the file by yourself), or method 3 (running a script, also prevents human-made errors). All methods are explained below. 
WARNING: If ANY filename changes are made to test duplicates (files with different names, but the same content), ONLY change the ISWC (i.e. the Xs). For example, you could have two files, Server_Alice_T-222.222.222.txt and Server_Alice_T-111.111.111.txt, where both contain contents "T-222.222.222\nForever Young - Alphaville". The second mismatched duplicate can only be created via method 2 (manually), as method 1 automatically writes the first content line to match the title ISWC. We ask that you only change the ISWC code as the naming of the file is really important to the organization of the directory. Changing anything else may mean that the server or client will be unable to find locate their files.
    
    **1. Method 1: Creating Song Files with a Program**
    1. Run "./create_songs". Follow the prompts, first entering a username and an ISWC in the format "[Username]_T-XXX.XXX.XXX", where [Username] is replaced with the username you'd like to use to identify a client and Xs are replaced by any digit 0-9 (i.e. "Alice_T-123.456.789"). However, for this program, keep the username to less than 20 (preferably 10) alphanumeric characters to prevent any errors. You will also need to make sure to enter in the ISWC code correctly as we don't check the formatting of the ISWC code.
    2. Next, enter a song title and artist in the format "[Song Title] - [Song Artist]". This **must** be less than 255 characters.
    3. You can continue to create as many files as desired, or quit the program by typing "exit".

    **Method 2: Manually Creating a Song File**
    1. First, you'll need to create a text file in the format “Server_[Username]_T-XXX.XXX.XXX.txt”. As aforementioned, the username you enter must be less than 200 characters and alphanumeric, and the X's must be replaced with a digit (of your choice) 0-9. For example, you could create a blank song/text file with the following command: "nano Server_Alice_T-123.456.789.txt".
    2. The first line of the text file should replicate the exact T-XXX.XXX.XXX in the filename. The second line corresponds to the song title and artist. For example, this could be the body of a song file: "T-123.456.789\nCome As You Are - Nirvana". As a side note, you can add more text to the song file (like if you wanted to store the song's producer or the album it was a part of) -- it's just not required.
    3. Repeat this step for as many files as needed.

    **Method 3: Running a Shell Script**
    1. Run sh create_songs.sh
    2. After executing this command, you should see that songs are created for the user "Lily". Thus, when running the client, enter the username "Lily" (without the quotations, naturally) to see that user's files. Look inside the shell script for more information.

3. Start the server by running "./server". Naturally, you'll have to leave it running while communicating with the client.

4. Start the client(s) by running "./client". Each client should corrrespond to its own terminal, separate from each other and the server.

5. As the client, you can now follow the prompts (refer to the Actions section for more information about the actions). First, you will be prompted to enter a username. If you want to test the song files you made, make sure to use the same exact username (with the exact casing) that you used to create said song files. If not, you can just enter in any username. After, you'll just be using the displayed commands until you decide to leave.

## Ending Notes
Please be careful of the file name. If you've created a file and you're like "Where is it? It's not showing up!", it could be that you named it incorrectly; this could happen as it can sometimes be hard to see where the typo occurred. So, again, the correct formatting of the filename is exactly like so: Server_ClientUsername_T-XXX.XXX.XXX.txt or Client_ClientUsername_T-XXX.XXX.XXX.txt. Alas, check whether you've misused a "_" or a "-", and that everything is in the right place. Furthermore, reference the exact username you used in the filename when using the client's program. Meaning, if the filename is "Server_Bobby_T-111.111.111.txt", you need to enter "Bobby" as the username (to view Bobby's files). Please make sure that you're typing in everything correctly! Thank you and we hope that everything works out!!