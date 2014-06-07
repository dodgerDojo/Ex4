//TODO:
//1. name and stuff
//2. return error check
/********************************/

// Includes:

#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <pthread.h>

/********************************/

// Defines:

#define NUM_OF_FILES        (5)

#define CREATION_COST_FILE  ("CreationCost.txt")
#define DELIVERY_COST_FILE  ("DeliveryCost.txt")
#define PACKAGING_COST_FILE ("PackagingCost.txt")
#define PAINT_COST_FILE     ("PaintCost.txt")
#define QA_COST_FILE        ("QaCost.txt")

#define MAX_FLOAT_DIGITS_LENGTH (256)

#define EXIT_ERROR_CODE     (-1)
#define EXIT_OK_CODE        (0)

#define NOF_INPUTS_ERROR     ("wrong number of inputs.\n")
#define FOPEN_ERROR          ("fopen() failed.\n")
#define FCLOSE_ERROR         ("fclose() failed.\n")
#define PTHREAD_CREATE_ERROR ("pthread_create() failed.\n")

/********************************/

// Defines:

typedef struct
{
  const char *file_name;
  int line_number;
} FILENAME_AND_LINE_NO;

/********************************/

// Static Variables:

static char *FILE_LIST[] = {CREATION_COST_FILE, \
                            DELIVERY_COST_FILE, \
                            PACKAGING_COST_FILE, \
                            PAINT_COST_FILE, \
                            QA_COST_FILE};

static pthread_t Thread_Ids[NUM_OF_FILES];
static float product_total_cost;

/********************************/

// Static Declarations:

static float readFloat(char const *p_file_name, int num_of_line);

static void* readCostAndUpdateTotal(void *arg);

static void initFileList(FILENAME_AND_LINE_NO *p_file_datas);

static float getProductTotalCost(int line_number);

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

static void* readCostAndUpdateTotal(void *arg)
{
    FILENAME_AND_LINE_NO *p_data = (FILENAME_AND_LINE_NO *)arg;
    int line_number = p_data->line_number;
    const char *p_file_name = p_data->file_name;

    printf("got %s at line %d\n Float read: %f \n", p_file_name, line_number, readFloat(p_file_name, line_number));

    /*pthread_mutex_lock(&lock);

    for(i=0; i<1000000;i++)
        counter++;

    pthread_mutex_unlock(&lock);*/

    return NULL;
}

static void initFileList(FILENAME_AND_LINE_NO *p_file_datas)
{
    int file_num = 0;
    for(file_num = 0; file_num < NUM_OF_FILES; file_num++)
    {
        p_file_datas[file_num].file_name = FILE_LIST[file_num];
    }
}

static float getProductTotalCost(int line_number)
{
    int i = 0;
    FILENAME_AND_LINE_NO file_datas[NUM_OF_FILES] = {0};

    initFileList(file_datas);

    for(i = 0; i < NUM_OF_FILES; i++)
    {
        file_datas[i].line_number = line_number;

        if(pthread_create(&(Thread_Ids[i]), NULL, &readCostAndUpdateTotal, (void *)&(file_datas[i])) != 0)
        {
            write(STDERR_FILENO, PTHREAD_CREATE_ERROR, sizeof(PTHREAD_CREATE_ERROR));
            exit(EXIT_ERROR_CODE);
        }
    }
    
    for(i = 0; i < NUM_OF_FILES; i++)
    {
        pthread_join(Thread_Ids[i], NULL);
    }
}

/********************************/

// Main:

int main(int argc, char *argv[])
{
    const unsigned int EXPECTED_ARGC = 2;
    const unsigned int ARGV_INDEX_NUM_OF_PRODECTS = 1;

    int num_of_products = 0, product_line = 0;

    // Check the amount of arguments.
    if(argc != EXPECTED_ARGC)
    {
        // No checking needed, exits with error code.
        write(STDERR_FILENO, NOF_INPUTS_ERROR, sizeof(NOF_INPUTS_ERROR));
        exit(EXIT_ERROR_CODE);
    }

    num_of_products = atoi(argv[ARGV_INDEX_NUM_OF_PRODECTS]);

    for(product_line = 1; product_line <= num_of_products; ++product_line)
    {
        getProductTotalCost(product_line);
        //float product_price = getProductTotalCost(product_line);
        //printf("Total Price: %f\n");
    }

    printf("ended.\n");

    return EXIT_OK_CODE;
}
