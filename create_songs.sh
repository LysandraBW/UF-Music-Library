#!/bin/bash

filename1="Server_Lily_T-123.456.789.txt"
echo "T-123.456.789\nStarstruck - Nicki Minaj\n" > "$filename1"
echo "Created file: $filename1"

#adding more content to this filename so that you'll
#be able to see (1) how the file with the correct filename will be pulled
#and (2) that you can add more information to a song file, although you still
# need the ISWC code and the song title in the first two lines.
filename2="Server_Lily_T-987.654.321.txt"
echo "T-987.654.321\nSomebody That I Used to Know - Gotye\nReleased in 2011\nAlbum: Making Mirrors\nFeatured Artist(s): Kimbra\n" > "$filename2"
echo "Created file: $filename2"

#filename3 represents a duplicate of the contents in filename2, 
#but with a different ISWP in the title.
filename3="Server_Lily_T-111.111.111.txt"
echo "T-987.654.321\nSomebody That I Used to Know - Gotye\n" > "$filename3"
echo "Created file: $filename3"

filename4="Server_Lily_T-456.123.789.txt"
echo "T-456.123.789\nStacy's Mom - Fountains of Wayne\n" > "$filename4"
echo "Created file: $filename4"

echo "All songs created successfully!"