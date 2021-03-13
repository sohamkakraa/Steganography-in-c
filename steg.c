#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
  Stores the comments in a PPM file in the form of Linked List
*/
typedef struct Comment { 
    char * data; 
    struct Comment* next; 
} Comment;

/*
  Stores the RGB values of a pixel
*/
typedef struct Pixel{
  int red;
  int green;
  int blue;
} Pixel;

/*
  Stores the contents of a PPM file
*/
typedef struct PPM{
  /* TODO: Question 1 */
  char format[3];
  Comment * comments;
  int width, height, max;
  Pixel **pixels;
} PPM;

/*
  Returns the PPM struct from the file fd
*/
PPM* getPPM(FILE* fd){	
  PPM* ppm = (PPM*) malloc(sizeof(PPM));	

  /* TODO: Question 2a */
  //Getting the format of the PPM file
  fscanf(fd, "%s", ppm->format);
  if (strcmp(ppm->format, "P3") != 0) {
    printf("Invalid Image Type");
    exit(0);
  }

  //Getting the comments in the PPM file 
  char c = getc(fd);
  ppm->comments = (Comment *) malloc(sizeof(Comment));
  Comment *temp = ppm->comments;
  while ((c = getc(fd)) == '#') {
    fseek(fd, -1, SEEK_CUR);  //Moving the cursor one line behind its current position
    char str[50];
    fgets(str, 50, fd);
    temp->data = (char *) malloc(strnlen(str, 50));
    strcpy(temp->data, str);
    temp->data[strlen(temp->data) - 1] = 0;
    temp->next = (Comment *) malloc(sizeof(Comment));
    temp = temp->next;
    temp->next = NULL;
  }

  fseek(fd, -1, SEEK_CUR); //Moving the cursor one line behind its current position
  //Getting the width, height and max RGB value from the PPM file
  fscanf(fd, "%d", &ppm->width);
  fscanf(fd, "%d", &ppm->height);
  fscanf(fd, "%d", &ppm->max);

  //Allocating RGB pixels in memory as a 2D dynamic array
  ppm->pixels = (Pixel **) malloc(ppm->width * sizeof(Pixel *));
  for (int i = 0; i < ppm->width; i++)
    ppm->pixels[i] = (Pixel *) malloc(ppm->height * sizeof(Pixel));

  //Getting the RGB pixels values from the PPM file
  for(int i = 0; i < ppm->width; i++)
    for(int j = 0; j < ppm->height; j++)
      fscanf(fd, "%d %d %d", &ppm->pixels[i][j].red, &ppm->pixels[i][j].green, &ppm->pixels[i][j].blue);

  return ppm;
}

/*
  Prints the PPM comments
*/
void showComments(Comment *comment) {
  while (comment->next != NULL) {
    printf("%s\n", comment->data);
    comment = comment->next;
  }
}

/*
  Prints a PPM struct in the format of a PPM file
*/
void showPPM(PPM* ppm){

  /* TODO: Question 2b */
  /* write the contents of the ppm to output file*/

  /* TODO: write the headers first */
  printf("%s\n", ppm->format);
  showComments(ppm->comments);
  printf("%d %d\n", ppm->width, ppm->height);
  printf("%d\n", ppm->max);
  /* TODO: then write the R G B pixel values */
  for (int i = 0; i < ppm->width; i++)
    for (int j = 0; j < ppm->height; j++) 
      printf("%d %d %d\n", ppm->pixels[i][j].red, ppm->pixels[i][j].green, ppm->pixels[i][j].blue);
}


/*
  Loads a file and returns it
  Prints error message and returns NULL if file can't be found or isn't a valid PPM
*/
PPM* loadPPMFromFile(char* filename){
  FILE* file;

  /* TODO: step 1, open the file */
  file = fopen(filename, "r");
  /* step 2: Check that file has been loaded correctly; show error otherwise*/
  if(file != NULL){
    PPM* ppm;
    /* TODO: step 3, get the PPM data from the file with the getPPM function */
    ppm = getPPM(file);

    if(ppm == NULL) {
      /* TODO: step 4, display error if file cannot been parsed into a PPM struct*/
      printf("Error: File cannot be parsed into a PPM struct");
    }

    /* TODO: step 5, close the file */
    fclose(file);
    /* step 6: return the PPM */
    return ppm;
  } else {
    fclose(file);
    return NULL;
  }
}


/*
  Encodes text into red field of PPM
  Returns the encoded PPM
*/
struct PPM * encode(char * text, PPM * img)
{
  srand((unsigned) time(NULL));
  int randomRow = rand() % (img->width / (strlen(text) - 1));
  int randomCol = rand() % (img->height  / (strlen(text) - 1)); 
  int randomKey = rand() % 9;

  if(randomRow < 1) 
    randomRow = 1;  
  if(randomCol < 1) 
    randomCol = 1;  

  int k = 0, i = randomRow, j = randomCol;
  //Red fields swapped with ASCII int
  img->pixels[0][0].blue = randomKey;
  while (k < strlen(text)) {
    if(img->pixels[i][j].red == text[k] + randomKey) {
      i++;
      j++;
    } else {
      img->pixels[i][j].red = text[k] + randomKey; 
      k++;
      i += randomRow;
      j += randomCol;
    }
  }
  return img;
}


/*
  Compares 2 PPM structs and decodes the message hidden within
  Returns the decoded message if the images have the same dimensions
  Returns NULL otherwise
*/
char* decode(PPM* i1, PPM* i2){
  char* decodedMsg = malloc(sizeof(char));

  /* TODO: Question 2d */
  int i, j, k = 0;
  int randomKey = i2->pixels[0][0].blue;
  if (i1->width * i1->height == i2->width * i2 ->height) {
    for (i = 0; i < i1->width; i++)
      for (j = 0; j < i1->height; j++)
        if(i1->pixels[i][j].red != i2->pixels[i][j].red) {
          decodedMsg[k] = i2->pixels[i][j].red - randomKey;
          k++;
        }
  } else {
    fprintf(stderr, "The two PPM files do not have the same dimensions\n");
    exit(0);
  }
  return decodedMsg;
}

/*
  Frees the Comments Linked List
*/
void freeLinkedList(Comment* head)
{
  Comment* temp;
   while (head != NULL) {
       temp = head;
       head = head->next;
       free(temp);
    }
}

/* TODO: Question 3 */
int main(int argc, char* argv[]){
  /* check arguments */
  switch(argc) {
  case 2:
    /* TODO: not enough arguments, print an error message */
    fprintf(stderr, "Error: Not enough Arguments\n");
    break;
  case 3:
    if(strcmp(argv[1], "e") == 0){ //Argument "e" - encode PPM
      PPM* ppm = loadPPMFromFile(argv[2]);

       /*Check that PPM is valid; continue if it is, exit otherwise */
      if(ppm != NULL){
        PPM* encodedPPM;
        /* TODO: encode the ppm with the text and assign to encodedPPM */
        char *text = (char*) malloc(ppm->max * sizeof(char));
        fprintf(stderr, "Enter your secret Message: [Max Limit %d]\n", ppm->max);
        fgets(text, ppm->max, stdin);
        encodedPPM = encode(text, ppm);
        /*Check that no error occured*/
        if(encodedPPM == NULL){
            return EXIT_FAILURE;
        } else {
          /* TODO: print a confirmation that encoding was successful */
          fprintf(stderr, "Encoding was successful\n");
          /* TODO: print the PPM to standard output with showPPM */
					showPPM(encodedPPM);
          /*Freeing all mallocs */
          freeLinkedList(ppm->comments);
          free(ppm->pixels);
          free(text);
          return EXIT_SUCCESS;				
        }
      } else {	
        return EXIT_FAILURE;
      }
    } else {
      fprintf(stderr, "Unrecognised or incomplete command line.\n");
      return EXIT_FAILURE;			
    }
    break;
  case 4:
    if(strcmp(argv[1], "d") == 0){ //Argument "d" - decode PPM
      PPM* comparisonPPM;
      PPM* encodedPPM;

      /* TODO: get comparison file filename from argv, load it with
         loadPPMFromFile then assign to comparisonPPM */
      comparisonPPM = loadPPMFromFile(argv[2]);
      /* TODO: get encoded file filename from argv, load it with
         loadPPMFromFile then assign to encodedPPM */
			encodedPPM = loadPPMFromFile(argv[3]);
      /*Check both PPMs are valid; continue if so, exit otherwise */
      if(comparisonPPM != NULL && encodedPPM != NULL){
        char* decodedMsg;
        
        /* TODO: decode the encodedPPM with the comparisonPPM and assign to decodedMsg */ 
        decodedMsg =	decode(comparisonPPM, encodedPPM);
        /*Check that the message has been decoded*/
        if(decodedMsg != NULL){
          /* TODO: print a confirmation message that the message was decoded */ 
          fprintf(stderr, "Decoded message: ");

          /* TODO: print the decoded message */
          printf("%s", decodedMsg);
          /*Freeing all mallocs */
          freeLinkedList(comparisonPPM->comments);
          freeLinkedList(encodedPPM->comments);
          free(comparisonPPM->pixels);
          free(encodedPPM->pixels);
          free(decodedMsg);
          return EXIT_SUCCESS;
        } else {
          return EXIT_FAILURE;
        }				
      } else {
        return EXIT_FAILURE;
      }			
    } else {
      fprintf(stderr, "Unrecognised or incomplete command line.\n\n");
      return EXIT_FAILURE;			
    }
    break;
  default:
    fprintf(stderr, "Unrecognised or incomplete command line.\n\n");
    return EXIT_FAILURE;
  }
}

