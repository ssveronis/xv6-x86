#include "types.h"
#include "set.h"
#include "user.h"

//TODO: Να μην είναι περιορισμένο σε int

Set* createRoot(){
    //Αρχικοποίηση του Set
    Set *set = malloc(sizeof(Set));
    set->size = 0;
    set->root = NULL;
    return set;
}

void createNode(int i, Set *set){
    /*
    Η συνάρτηση αυτή δημιουργεί ένα νέο SetNode με την τιμή i και εφόσον δεν υπάρχει στο Set το προσθέτει στο τέλος του συνόλου.
    Σημείωση: Ίσως υπάρχει καλύτερος τρόπος για την υλοποίηση.
    */

    //Αρχικοποίηση του SetNode
    SetNode *temp = malloc(sizeof(SetNode));
    temp->i = i;
    temp->next = NULL;

    //Έλεγχος ύπαρξης του i
    SetNode *curr = set->root;//Ξεκινάει από την root
    if(curr != NULL) { //Εφόσον το Set δεν είναι άδειο
        while (curr->next != NULL){ //Εφόσον υπάρχει επόμενο node
            if (curr->i == i){ //Αν το i υπάρχει στο σύνολο
                free(temp); 
                return; //Δεν εκτελούμε την υπόλοιπη συνάρτηση
            }
            curr = curr->next; //Επόμενο SetNode
        }
    }
    /*
    Ο έλεγχος στην if είναι απαραίτητος για να ελεγχθεί το τελευταίο SetNode.
    */
    if (curr->i != i) attachNode(set, curr, temp); 
    else free(temp);
}

void attachNode(Set *set, SetNode *curr, SetNode *temp){
    //Βάζουμε το temp στο τέλος του Set
    if(set->size == 0) set->root = temp;
    else curr->next = temp;
    set->size += 1;
}

void deleteSet(Set *set){
    if (set == NULL) return; //Αν ο χρήστης είναι ΜΛΚ!
    SetNode *temp;
    SetNode *curr = set->root; //Ξεκινάμε από το root
    while (curr != NULL){ 
        temp = curr->next; //Αναφορά στο επόμενο SetNode
        free(curr); //Απελευθέρωση της curr
        curr = temp;
    }
    free(set); //Διαγραφή του Set
}

SetNode* getNodeAtPosition(Set *set, int i){
    if (set == NULL || set->root == NULL) return NULL; //Αν ο χρήστης είναι ΜΛΚ!

    SetNode *curr = set->root;
    int n;

    for(n = 0; n<i && curr->next != NULL; n++) curr = curr->next; //Ο έλεγχος εδω είναι: n<i && curr->next != NULL
    return curr;
}