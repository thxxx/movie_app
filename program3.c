#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For: malloc(), realloc(), free(), exit(), execvp(), EXIT_SUCCESS, EXIT_FAILURE
#include <time.h>
#include <sys/time.h>

#include <sys/timeb.h>

#include <unistd.h>   // For: chdir(), fork(), exec(), pid_t, getcwd()
#include <sys/wait.h> // For: waitpid()
#include <fcntl.h>    // For: File creation modes
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>

void mergeTwo(char **firstData, char **secondData, char **arguments);
int numOfThread = 1;
int nowChilderen = 1;
void mergeSort(int data[], int p, int r);
void *thread_merge_sort(void *arg);
void merge_sections_of_array(int arr[], int number, int aggregation);

void merge(int data[], int p, int q, int r);

int depth = 0;

int LENGTH;
int NUMBERS_PER_THREAD;
int OFFSET;
int arr[100];
int mynum = 0;

char **split_command_line(char *command)
{
    int position = 0;
    int no_of_tokens = 64;
    char **tokens = malloc(sizeof(char *) * no_of_tokens);
    char delim[3] = " \n";

    // Split the command line into tokens with space as delimiter
    char *token = strtok(command, delim);

    //tokens[0] = NULL;

    while (token != NULL)
    {
        tokens[position] = token;
        position++;
        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    return tokens;
}

char *read_command_line(void)
{
    int position = 0;
    int buf_size = 30;
    char *command = (char *)malloc(sizeof(char) * 30);
    char c;

    // Read the command line character by character
    c = getchar();
    while (c != EOF && c != '\n')
    {
        command[position] = c;

        // Reallocate buffer as and when needed
        if (position >= buf_size)
        {
            buf_size += 8;
            command = realloc(command, sizeof(char) * buf_size);
        }

        position++;
        c = getchar();
    }
    return command;
}

int main(int argc, char *argv[])

{
    int std_in, std_out, std_err;
    std_in = dup(0);
    std_out = dup(1);
    std_err = dup(2);

    // get input start

    int number;
    int first;
    char *command_line;
    char **arguments;

    scanf("%d ", &number);

    printf(" 입력받은 숫자의 개수 : %d \n", number);

    if (argv[1] != NULL)
    {
        numOfThread = atoi(argv[1]);
    }

    command_line = read_command_line();
    arguments = split_command_line(command_line); //arguments 는 그냥 키워드들 배열
    int *dda = (int *)malloc(sizeof(int) * 30);

    for (int i = 0; i < number; i++)
    {
        arr[i] = atoi(arguments[i]);
        printf("%d ", atoi(arguments[i]));
    }

    LENGTH = number;
    NUMBERS_PER_THREAD = LENGTH / numOfThread;
    OFFSET = LENGTH % numOfThread;

    struct timeval stop, start_time;
    gettimeofday(&start_time, NULL);

    pthread_t threads[numOfThread];

    for (long i = 0; i < numOfThread; i++)
    {
        int rc = pthread_create(&threads[i], NULL, thread_merge_sort, (void *)i);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (long i = 0; i < numOfThread; i++)
    {
        pthread_join(threads[i], NULL);
    }
    merge_sections_of_array(arr, numOfThread, 1);

    //do your thang
    gettimeofday(&stop, NULL);
    int ms = stop.tv_usec - start_time.tv_usec;

    //do stuff
    printf(" 멀티프로세스 병합정렬 시작! \n");

    // 정렬 프로세스. 가장 중요
    /*****************************/
    // num_list 는 입력받은 숫자들, numOfThread은 리디렉션으로 들어온 생성하는 프로세스 수 , nnumber은 숫자 개수

    // 전체 입력 데이터를 total_process_num 만큼 적절히 나눈다.

    /*****************************/

    dup2(std_in, 0);
    dup2(std_out, 1);
    dup2(std_err, 2);

    printf("\n정렬 후\n");
    for (int i = 0; i < LENGTH; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n 걸린 시간(단위 ms) : %d ", ms);
    // sort end
    free(command_line);
    free(arguments);

    return 0;
}

void *thread_merge_sort(void *arg)
{
    mynum += 1;
    int thread_id = (long)arg;
    int left = thread_id * (NUMBERS_PER_THREAD);
    int right = (thread_id + 1) * (NUMBERS_PER_THREAD)-1;

    printf("\nI am working in %d thread\n", mynum);

    if (thread_id == numOfThread - 1)
    {
        right += OFFSET;
    }
    int middle = left + (right - left) / 2;
    if (left < right)
    {
        mergeSort(arr, left, right);
        mergeSort(arr, left + 1, right);
        merge(arr, left, middle, right);
    }
    return NULL;
}

void mergeTwo(char **firstData, char **secondData, char **arguments)
{
    int i = 0;
    int j = 0;
    int count = 0;

    while (firstData[i] != NULL && secondData[j] != NULL)
    {
        if (atoi(firstData[i]) <= atoi(secondData[j]))
        {
            arguments[count] = firstData[i];
            i++;
        }
        else
        {
            arguments[count] = secondData[j];
            j++;
        }
        count++;
    }
    if (firstData[i] == NULL)
    {
        while (secondData[j] != NULL)
        {
            arguments[count] = secondData[j];
            j++;
            count++;
        }
    }
    else if (secondData[j] == NULL)
    {
        while (firstData[i] != NULL)
        {
            arguments[count] = firstData[i];
            i++;
            count++;
        }
    }
}

void mergeSort(int data[], int p, int r)
{
    int q;
    if (p < r)
    {
        q = (p + r) / 2;
        mergeSort(data, p, q);
        mergeSort(data, q + 1, r);
        merge(data, p, q, r);
    }
}
void merge(int data[], int p, int q, int r)
{
    int i = p, j = q + 1, k = p;
    int tmp[r]; // 새 배열
    while (i <= q && j <= r)
    {
        if (data[i] <= data[j])
            tmp[k++] = data[i++];
        else
            tmp[k++] = data[j++];
    }
    while (i <= q)
        tmp[k++] = data[i++];
    while (j <= r)
        tmp[k++] = data[j++];
    for (int a = p; a <= r; a++)
        data[a] = tmp[a];
}

/* merge locally sorted sections */
void merge_sections_of_array(int arr[], int number, int aggregation)
{
    for (int i = 0; i < number; i = i + 2)
    {
        int left = i * (NUMBERS_PER_THREAD * aggregation);
        int right = ((i + 2) * NUMBERS_PER_THREAD * aggregation) - 1;
        int middle = left + (NUMBERS_PER_THREAD * aggregation) - 1;
        if (right >= LENGTH)
        {
            right = LENGTH - 1;
        }
        merge(arr, left, middle, right);
    }
    if (number / 2 >= 1)
    {
        merge_sections_of_array(arr, number / 2, aggregation * 2);
    }
}