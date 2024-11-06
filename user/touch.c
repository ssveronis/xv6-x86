#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define ASCII_a 97
#define ASCII_z 122
#define ASCII_A 65
#define ASCII_Z 90
#define ASCII_0 48
#define ASCII_9 57
#define ASCII__ 95
#define ASCII_DOT 46

int validateFileName(char* s);
char* getFileName(char *argv[]);
char* manageFileNameInput(char* argv[]);
int getFlagInt(char *flag);
void cFlagHandler(char* filename, char *argv[], int i);

int main(int argc, char *argv[]){
    char* filename;

    if (argc <= 1) { //Αν δεν υπάρχει όνομα για το αρχείο
        printf(1, "touch: file name not defined\n");
        exit();
    }
    filename = manageFileNameInput(argv); //Validate το όνομα του αρχείου
    mknod(filename, 0, 0); // Δημηουργία του αρχείου
    free(filename);
    exit();
}

char* manageFileNameInput(char* argv[]){
    char* filename = getFileName(argv);
    if(validateFileName(filename) == 0){ //Input validation fail
        printf(1, "touch: file name invalid\n");
        exit();
    }
    return filename;
}

char* getFileName(char *argv[]){
    char* filename = malloc(strlen(argv[1])+1); //Δεσμεύουμε μνήμη για το όνομα του αρχείο
    strcpy(filename, argv[1]); //Αντιγρφή του ονόματος στη θέση μνήμης της filename
    return filename;
}

int validateFileName(char* s){
    int i, match = 1;
    for(i=0; s[i] != '\0'; i++){ //Για κάθε χαρακτήρα του ονόματος
        int d = (unsigned char) s[i]; //ASCII code
        int charmatch = ((d >= ASCII_a && d <= ASCII_z) || //validation
                         (d >= ASCII_A && d <= ASCII_Z) ||
                         (d >= ASCII_0 && d <= ASCII_9) ||
                          d == ASCII__ || d == ASCII_DOT);
        match = match & charmatch; // bitwise AND
    }
    return match;
}
