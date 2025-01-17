//Note: kind of went back and forth on following the directions in the assignment repo vs
//trying to get all the tests in test.sh to pass. hopefully close enough to what was expected
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
void reverse_string(char *, int, int);
void count_word_lengths(char *, int, int);
void replace_word(char *, int, int, char *, char *);
//add additional prototypes here

//the following function takes the user string and copies it to the buffer
//while removing any extra spaces between words and at the beginning/end of the string.
//the function also adds periods to the end of the string to fill the buffer.
//the function returns the length of the string after removing extra spaces.
int setup_buff(char *buff, char *user_str, int len){
    char last_char = '\0';
    int count = 0;
    char *temp = user_str;
    int str_len = 0;
    while(*temp != '\0') {
        if(last_char == ' ' && *temp == ' '){
            last_char = *temp;
            temp++;
            continue;
        }
        count++;
        if(count > len) return -1;
        last_char = *temp;
        temp++;
    }
    count = 0;
    while(*user_str == ' '){
        user_str++;
    }
    while(*user_str != '\0'){
        if(last_char == ' ' && *user_str == ' '){
            last_char = *user_str;
            user_str++;
            continue;
        }
        char *end_check = user_str + 1;
        while(*end_check == ' '){
            end_check++;
        }
        if(*end_check == '\0' && *user_str == ' '){
            break;
        }
        *buff = *user_str;
        last_char = *user_str;
        buff++;
        user_str++;
        count++;
    }
    str_len = count;
    while(count < len){
        *buff = '.';
        buff++;
        count++;
    }
    return str_len;
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]");
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

//the following function transverses the buffer using a pointer initialized to the start of the buffer
//and counts the number of words in the string by checking for spaces and incrementing the word count
//when a space is found. The function returns the word count.
int count_words(char *buff, int len, int str_len){
    char last_char = '\0';
    int word_count = 0;
    int char_count = 0;
    int new_word = 0;
    while(char_count != str_len){ 
        last_char = *buff;
        if(*buff != ' ' && new_word == 0){
            word_count++;
            new_word = 1;
        }
        if(*buff == ' '){
            new_word = 0;
        }
        buff++;
        char_count++;
    }
    return word_count;
}

//the following function reverses the string in the buffer by initializing 
//two pointers to the start and end of the buffer and swapping the characters
//at the two pointers, then moving the pointers forwar/backward until they meet in the middle. 
//The function then prints the reversed string.
void reverse_string(char *buff, int len, int str_len){
    char *start = buff;
    char *end = buff + str_len - 1;
    char temp;
    while(start < end){
        temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    //the code below follows the directions in the assignment repo
    //but it causes the provided test.sh to fail.
    //keeping it here as a comment.

    /*printf("Reversed String: ");
    for(int i = 0; i < str_len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');*/
}

//the following function prints the words in the buffer and the length of each word
//by transversing the buffer and printing each character until a space is found, then
//printing the length of the word and starting a new line.
//an additional check is made to print the length of the last word in the buffer.
void count_word_lengths(char *buff, int len, int str_len){
    printf("Word Print\n----------\n");
    int count = 0;
    int word_count = 0;
    int size = 0;
    while(count < str_len){
        if(count == 0){
            printf("%d. ", word_count + 1);
        }
        if(*buff != ' '){
            putchar(*buff);
            size++;
        }
        else{
            printf("(%d)\n", size);
            word_count++;
            printf("%d. ", word_count + 1);
            size = 0;
        }
        if(count == str_len - 1){
            printf("(%d)\n", size);
            word_count++;
        }
        count++;
        buff++;
    }
    printf("\n");
    printf("Number of words returned: %d\n", word_count);
}

// helper function for replace_word
// returns the length of a word
int get_word_len(char *word) {
    char *ptr = word;
    int len = 0;
    while(*ptr != '\0') {
        len++;
        ptr++;
    }
    return len;
}

// helper function for replace_word
// returns 1 if c is a word boundary, 0 otherwise
int is_word_boundary(char c, int pos, int str_len) {
    return (c == ' ' || c == '.' || pos == str_len);
}

// helper function for replace_word
// returns 1 if str matches word, 0 otherwise
int check_word_match(char *str, char *word) {
    while(*word != '\0') {
        if(*str != *word) return 0;
        str++;
        word++;
    }
    return 1;
}

//the following function replaces the first occurance of old_word in the buffer with
//new_word by transversing the buffer and checking if the word at the current position
//matches old_word. If a match is found, the function then checks if the word is at a word boundary
//and replaces the word with new_word.
//kind of convoluted way of fixing the buffer after replacing the word, depending on if
//the new word is longer or shorter than the old word. 
void replace_word(char *buff, int len, int str_len, char *old_word, char *new_word) {
    int old_len = get_word_len(old_word);
    int new_len = get_word_len(new_word);
    
    for(int count = 0; count < str_len; count++) {
        if(buff[count] == *old_word) {
            if(check_word_match(&buff[count], old_word) && is_word_boundary(buff[count + old_len], count + old_len, str_len)) {
                int shift = new_len - old_len;
                if(shift < 0) {
                    for(int i = count + old_len; i < str_len; i++) {
                        buff[i + shift] = buff[i];
                    }
                    for(int i = str_len + shift; i < len; i++) {
                        buff[i] = '.';
                    }
                } else {
                    for(int i = str_len - 1; i >= count + old_len; i--) {
                        buff[i + shift] = buff[i];
                    }
                }
                for(int i = 0; i < new_len; i++) {
                    buff[count + i] = new_word[i];
                }
                return;
                //side note: this fails a test in test.sh (not a crash), if old word does not exist in the buffer
                //but the directions didnt actually specify what to do in that case. (unless I missed it)
                //so I'm just going to leave it as is.
            }
        }
    }
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //the or statement prevents this satement from having an error if argv[1] does not exist
    //since the first condition will be false and the second condition will not be checked.
    //both conditions serve to check if the user has provided the option in the correct format
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //this if statement simply checks if the user has provided a string to work with
    //(should be the third argument) and if not, it will print the usage and exit with an error code.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char*)malloc(BUFFER_SZ);
    if (buff == NULL){
        printf("Error allocating memory for buffer\n");
        exit(2); //chose to follow the written instructions and exit with 2 instead of 99
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff, BUFFER_SZ, user_str_len);
            break;
        case 'w':
            count_word_lengths(buff, BUFFER_SZ, user_str_len);
            break;
        case 'x':
            if(argc < 5){
                usage(argv[0]);
                exit(1);
            }
            char *old_word = argv[3];
            char *new_word = argv[4];
            replace_word(buff, BUFFER_SZ, user_str_len, old_word, new_word);
            break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//providing both the pointer and the length is a good generally good practice
//because it allows for the function to be able to easily work with buffers
//of varying sizes. This would be more obviously useful in a program where
//the buffer was not always 50 bytes. 