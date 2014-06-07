//TODO:
//1. name and stuff

/********************************/

// Includes:

#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <stdlib.h>

/********************************/

// Defines:

#define CREATION_COST_FILE ("CreationCost.txt")

#define MAX_FLOAT_DIGITS_LENGTH (1000)

#define EXIT_ERROR_CODE     (-1)
#define EXIT_OK_CODE        (0)

#define NOF_INPUTS_ERROR    ("wrong number of inputs.\n")
#define FOPEN_ERROR         ("fopen() failed.\n")
#define FCLOSE_ERROR        ("fclose() failed.\n")

/********************************/

// Static Variables:

/********************************/

// Static Declarations:

static float readFloat(char const *p_file_name, int num_of_line);

/********************************/

// Functions:

static float readFloat(char const *p_file_name, int num_of_line)
{
    int current_line_number = 0;
    char line[MAX_FLOAT_DIGITS_LENGTH] = {0};

    FILE *file = fopen(p_file_name, "r");

    if(NULL == file)
    {
        // No checking needed, exits with error code.
        write(STDERR_FILENO, FOPEN_ERROR, sizeof(FOPEN_ERROR) - 1);
        exit(EXIT_ERROR_CODE);
    }

    while(fgets(line, sizeof(line), file) != NULL)
    {
        current_line_number++;
        if(current_line_number == num_of_line)
        {
            break;
        }
    }

    if(EOF == fclose(file))
    {
        // No checking needed, exits with error code.
        write(STDERR_FILENO, FCLOSE_ERROR, sizeof(FCLOSE_ERROR) - 1);
        exit(EXIT_ERROR_CODE);   
    }

    return atof(line);
}

/********************************/

// Main:

int main(int argc, char *argv[])
{
    const unsigned int EXPECTED_ARGC = 2;
    const unsigned int ARGV_INDEX_NUM_OF_PRODECTS = 1;

    int num_of_products = 0, i = 0;

    // Check the amount of arguments.
    if(argc != EXPECTED_ARGC)
    {
        // No checking needed, exits with error code.
        write(STDERR_FILENO, NOF_INPUTS_ERROR, sizeof(NOF_INPUTS_ERROR));
        exit(EXIT_ERROR_CODE);
    }

    num_of_products =  atoi(argv[ARGV_INDEX_NUM_OF_PRODECTS]);

    for(i = 0; i < num_of_products; ++i)
    {
        printf("%f\n", readFloat(CREATION_COST_FILE, i+1));
    }

    return EXIT_OK_CODE;
}
