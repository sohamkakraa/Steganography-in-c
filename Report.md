# Who I am

- Name: Soham Kakra
- Student ID: H00360439
- Student username: sk2015

# Program design

The struct called PPM holds the details of PPM images in the original order.

getPPM() takes in a ppm file as an argument and scans its details part by part. The comment after the format of the PPM file is stored into a Linked List and the pixel data is scaned as a 2d dynamic array.

showComments() prints the comments.

showPPM() takes a ppm struct as an agrument and writes the data of it into the standard output file.

loadPPMFromFile() takes in the filename as an argument, Loads the file and returns it with the help of getPPM().

encode() takes in the text to be encoded and the ppm struct. It hides the text into the red successive pixels of the 2d dynamic pixels array, by converting the text characters into its respected ASCII values and adding a random number between 0..9 as a key to the encoded message, which is saved in the first blue pixel.

decode() takes in two ppm structs (i1 for comparission and i2 for encodedPPM) and checks if they have the same dimensions. If true, then it compares their red pixels values in the 2d dynamic pixels array. When two pixels at same position do not match, it appends the character value of the pixels into a char* after subtracting it from the key placed in the first blue pixel. 

freeLinkedList() deallocate the memory of Comments' nodes.

# Choice of data structures and algorithms

All structures are contructed using **typedef** : 

typedef struct Pixel{
  ..
} Pixel;

typedef struct PPM {
    ..
    Pixel ** pixels;
} PPM;


The **comments** in ppm file has been stored as a linked list with a recursive data structure. This is because there may be mutiple comments in one PPM file. Each node in the Linked List stores a commment as a whole. 

typedef struct Comment { 
    ..
    struct Comment* next; 
} Comment;


To **encode** a text into a ppm file, the row and column values for the red pixel are chosen at random in such a way that the text is encoded in successive pixels. If the ASCII value of the text already exists at that particular location then an increment is done moving to the next red pixel. The random number is chosen by using the rand() to return a random number in the range from 0 to width or height divded by length of the text.


The **decode**() used for extracting the encoded mesaage in a ppm file, first checks if the file to be compared with and the file in which the encoded message is present have the same dimensions. Then uses a linear search with Big-O Notation of O(N) to search for red pixels that are not equal. If a red pixel at the same location is not equal, then it is appended into the char*.
    
