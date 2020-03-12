#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    char *word = nextWord(file);

    while(word)
    {
        hashMapPut( map, word, 0);
        free(word);
        word = nextWord(file);
    }
}
/**
 * Converts provided ascii characters to lower case equivalent using character arithmetic.
 * Parameters:  a single char to be converted
 * @param ch
 * @return ch
 */
char toLowerCase(char ch){

    if(ch >= 65 && ch <= 90) {
        ch = ch + 32;
    }
    return ch;
}
/**
 * Helper function to determine the length of a raw char array in C.
 * Parameters:  char pointer to char array in memory
 * @param word
 * @return
 */
int stringLength(char* word) {
    int count = 0;

    if(word != NULL) {

        while (isprint(word[count]))
        {
            count++;
        }
    }
    return count;
}
/**
 * Validates if all characters are only alpha chars (letters), otherwise returns false.
 * Parameters:  char pointer to char array in memory
 * @param word
 * @return 1 (true) for all valid chars otherwise 0 (false).
 */
int isAlphaChars(char* word) {

    int len = stringLength(word);

    int valid = 1;

    for(int i = 0; i < len; i++) {

        if(!isalpha(word[i] )) {

            valid = 0;
        }
    }
    return valid;
}
/**
 * Helper function to iterate through char* array and converts each char to its lower case equivalent.
 * Parameters:  char pointer to char array in memory
 * @param word
 * @return
 */
void convertCaseToLower(char* word) {

    int index = 0;

    while(word[index] != '\0') {

        word[index] = toLowerCase(word[index]);

        index++;
    }

}
/**
 * Function to ensure string is properly formatted given the constraints of a single word with no space or
 * special characters.  If input is correct, it calls a helper function to convert all letters to lowercase and
 * returns a 1 (true) otherwise provides feedback to the user and returns 0 (false).
 * Parameters:  char pointer to char array in memory
 * @param word
 * @return 1 (true) or 0 (false)
 */
int validate(char* word) {

    if(isAlphaChars(word)) {

        convertCaseToLower(word);
        return 1;

    } else {

        printf("\nNot a valid format, see rules!\n");
        printf("\nRules for the program success: \n"
               "1. single words only\n"
               "2. upper and lowercase chars\n"
               "3. no spaces or special chars\n");
        return 0;
    }
}
/**
 * Source code credit:  https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C
 * Levenshtein word edit distance algorithm.  Calculates a value which is the number of single letter edits to
 * convert the first provided word (s1) into the provided second word (s2), the originally desired word.
 * @param map
 * @param word
 * @return
 */
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
int levenshtein(char *s1, char *s2) {
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len+1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return(column[s1len]);
}
/**
 * Helper function to iterate through the hashMap and update the values for the Levenshtein distances.
 * Calls the levenshtein algorithm on each dictionary key compared to provided word.
 * @param map
 * @param word
 * @return
 */
void updateDistances(HashMap * map, char* word) {

    for(int i = 0; i < map->capacity; i++)
    {
        HashLink * curr = map->table[i];

        while (curr != NULL)
        {
            curr->value = levenshtein(curr->key, word);
            curr = curr->next;
        }
    }
}
/**
 * Generates an array of the 5 keys (words) with the lowest levenshstein distance values in the hashmap.
 * Iterates through the hashMap checking distance values versus value of keys in lowestKeys array.  Finally,
 * prints the top 5 possible alternative words with the lowest edit word distances.
 * @param map
 * @param word
 * @return
 */
void wordOptions(HashMap * map, char* word)
{
    updateDistances(map, word);

    // initialize with 5 non-NULL keys that are not words and will return a value of NULL with hashMapGet
    char* lowestKeys [5] = {"abcd", "efgh", "ijkl", "mnop", "qrst"};

    // iterate through the entire dictionary to find 5 lowest Levenshstein values;
    for(int i = 0; i < map->capacity; i++)
    {
        HashLink * curr = map->table[i];

        while (curr != NULL)
        {
            for(int j = 0; j < 5; j++)
            {
                int * testValue = hashMapGet(map, lowestKeys[j]);
                //populates first 5 if testValue returns null due to passing in a null param
                if(testValue == NULL) {
                    lowestKeys[j] = curr->key;
                } else {
                    //replace key in lowestKeys array is less than current value, else do nothing
                    if(curr->value < *testValue){
                        lowestKeys[j] = curr->key;
                        break;
                    }
                }
            }
            curr = curr->next;
        }
    }

    for (int k = 0; k < 5; k++)
    {
        printf("%s\n", lowestKeys[k]);
    }


}
/**
 * Checks the spelling of the word provided by the user. If the word is spelled incorrectly,
 * print the 5 closest words as determined by a metric like the Levenshtein distance.
 * Otherwise, indicate that the provided word is spelled correctly. Use dictionary.txt to
 * create the dictionary.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    HashMap* map = hashMapNew(1000);

    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);

    char inputBuffer[256];
    int quit = 0;

    printf("\nWelcome to my spell checker program, written in ANSI C!");
    printf("\n\nRules for the program success: \n"
           "1. single words only\n"
           "2. upper and lowercase chars\n"
           "3. no spaces or special chars\n");

    while (!quit)
    {
        printf("\nEnter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);

        //checks for only alpha chars and converts input to all lowercase
        if (validate(inputBuffer))
        {
            if (strcmp(inputBuffer, "quit") == 0)
            {
                quit = 1;
            }
            else if (hashMapContainsKey(map, inputBuffer))
            {
                printf("\nThe inputted word was spelled correctly!");

            } else {

                printf("\nThat's not spelled right, did you mean:\n");
                wordOptions(map, inputBuffer);
            }
        }
    }
    hashMapDelete(map);
    return 0;
}
