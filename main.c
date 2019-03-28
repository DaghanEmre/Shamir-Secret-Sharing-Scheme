#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct x_y{
    int x;
    int y;
    struct x_y *ptr;
}x_y;


typedef struct poly_node{
    int coeff;
    int expo;
    struct poly_node *ptr;
}poly_node;

char lineG[150];
char lineCopy[150];

void checking_FileExist(FILE* fileName);
void encryption_Func(char* plainTextFile, char* cipherTextFile, FILE* keysFile);
void list_Sub_Poly_Func(char* subPolynomialFile, FILE* keysFile,x_y **x_y_head);
void parsing_KeysFile(char** line, FILE *subPolyFile, x_y **x_y_head);
void x_y_insert(int x, int y,FILE* keysFile, x_y **x_y_head);
void display_x_y(x_y **x_y_head, FILE *subPolyFile, int *x_y_counter);
void poly_node_insert(poly_node **poly_head, int coeff, int expo);

static char * readline(FILE* keysFile)
{
    if(keysFile != NULL)
    {
        if(fgets(lineG,sizeof(lineG),keysFile) != NULL) /*I got the line from keysFile, and put to lineG until NULL comes*/
            return lineG;
        else
            fclose(keysFile);
    }
    else exit(-1);
    return NULL;
}

int main(int argc, char*argv[])
{
    static FILE *keysFile; /*producing a keysFile*/
    if(keysFile == NULL)
        keysFile = fopen(argv[2], "r");
    checking_FileExist(keysFile);

    x_y *x_y_head = NULL; /*Head of x_y linked list*/

    if(strcmp("-e",argv[1]) == 0){ /*ENCRYTYON */
        encryption_Func(argv[3], argv[4], keysFile);
    }else if(strcmp("-l", argv[1]) == 0){ /*LISTING */
        list_Sub_Poly_Func(argv[3], keysFile,&x_y_head);
    }
    return 0;

}

void display_x_y(x_y **x_y_head, FILE *subPolyFile, int *x_y_counter){
    x_y *temp = *x_y_head;
    while( *temp->ptr != NULL)
    {
        fprintf(subPolyFile,"x%d : %d, y%d : %d", *x_y_counter, *temp->x, *x_y_counter, *temp->y);
        (*temp) = (*temp)->ptr;
        (*x_y_counter)++;
        fprintf( subPolyFile , "\n" ) ;
    }

}

void create_poly_list(int *x_y_counter, x_y **x_y_head){
    poly_node *poly_node_list[(*x_y_counter)*((*x_y_counter)-1)];
    int list_counter = 0;
    int i = 0;

    while(list_counter <= (*x_y_counter) ){
        for(int i; i<=*x_y_counter; i++){
            if(i != list_counter){
                poly_node *poly_head = NULL;

                poly_node_insert(&poly_head, 1 , 1);

                poly_node_insert(&poly_head, -(*x_y_head->x),0);

                poly_node_list[list_counter]=poly_head;
            }
            (*x_y_head)=(*x_y_head->ptr);
        }
        list_counter++;
    }

}

/*adds a node to a polynomial*/
void x_y_insert(int x, int y,FILE* keysFile, x_y **x_y_head){
    x_y *current;
    current = *x_y_head;

    /*Creates a new node if the list is empty*/
    if(*x_y_head == NULL)
    {
        *x_y_head =(x_y*) malloc (sizeof(x_y));
        current = *x_y_head;
    }
    else
    {
        /* traverse the entire linked list */
        while(current->ptr != NULL)
                current = current->ptr;

        /* create new nodes at intermediate stages */

        current->ptr = (x_y*)malloc(sizeof(x_y));
        current = current->ptr;
    }

    /* assign coefficient and exponent */
    current->x = x;
    current->y = y;
    current->ptr = NULL;
}

void parsing_KeysFile(char** line, FILE *subPolyFile, x_y **x_y_head){
    char *token, *token1,*token2;
    char *current = *line;

    if(line == NULL)
        fputs("Empty File!",subPolyFile);

    /*Parsing keysFile*/
    token = strtok(current, " "); /*That gets x*/
    token1 = strtok(NULL, " ");
    token2 = strtok(NULL, "\0"); /*That gets y*/
    free(token1);

    x_y_insert(atoi(token),atoi(token2),subPolyFile,&(*x_y_head) );
}

void list_Sub_Poly_Func(char* subPolynomialFile, FILE* keysFile,x_y **x_y_head){

    /*to open a sub polynomial file*/
    static FILE *subPolyFile;
    if(subPolyFile == NULL)
        subPolyFile = fopen(subPolynomialFile, "w");

    char * line;
    int i=0;

    checking_FileExist(subPolyFile);
    /*I made a for loop to get lines from keysFile until NULL*/
    for(i=0; i<1000; i++)
    {
        line = readline(keysFile);
        if(line != NULL)
            parsing_KeysFile(&line, keysFile, &(*x_y_head) );
        else
             break;
    }
    int x_y_counter=0;
    display_x_y(&(*x_y_head),subPolyFile,&x_y_counter);
    create_poly_list(&x_y_counter,&(*x_y_head));
    fclose(subPolyFile);
    return;


}

void poly_node_insert(poly_node **poly_head, int coeff, int expo){
    poly_node *current;
    current = *poly_head;

    /* create a new node if the list is empty */
    if(*poly_head == NULL)
    {
        *poly_head = malloc(sizeof(poly_node));
        current = *poly_head;
    }else{
         /* traverse the entire linked list */
         while(current->ptr != NULL)
            current = current->ptr;

         /* create new nodes at intermediate stages */
         current->ptr=malloc(sizeof(poly_node));
         current = current->ptr;
    }

    /* assign coefficient and exponent */
    current->coeff = coeff;
    current->expo = expo;
    current->ptr = NULL;
}

void encryption_Func(char* plainTextFile, char* cipherTextFile, FILE* keysFile){
    FILE* plainFile = fopen(plainTextFile, "r");
    FILE* cipherFile = fopen(cipherTextFile, "w");

    checking_FileExist(plainFile);
    checking_FileExist(cipherFile);

}

void checking_FileExist(FILE* fileName){
    if (fileName == NULL)
        {
            printf("Error! Could not open file\n");
            exit(-1);
        }

}

