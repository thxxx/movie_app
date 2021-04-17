
/*******************************************************************
 *   Made by : 2017147505 김호진
 *   숫자들을 입력받아 멀티 쓰레딩 방법으로 병합정렬(Merge Sort)을 한 후 출력하는 프로그램
 *******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>

void mergeSort(int data[], int p, int r);
void *thread_merge_sort(void *arg);
void merge_sections_of_array(int arr[], int number, int aggregation);
void merge(int data[], int p, int q, int r);
char **split_command_line(char *command);
char *read_command_line(void);

int depth = 0;
int numOfThread = 1;
int nowChilderen = 1;
int LENGTH;
int NUMTO_THREAD;
int OFFSET;
int arr[100];
int mynum = 0;

int main(int argc, char *argv[])

{
    int std_in, std_out, std_err;
    std_in = dup(0);
    std_out = dup(1);
    std_err = dup(2);

    // get input start

    int numOfNumbers;

    scanf("%d", &numOfNumbers);
    printf(" 입력받은 숫자의 개수 : %d \n", numOfNumbers); // 첫 입력은 전체 숫자의 갯수로 따로 저장한다.

    int num_list[numOfNumbers + 1];

    int numbers;

    for (int i = 0; i < numOfNumbers; i++)
    {
        scanf("%d", &numbers);
        num_list[i] = numbers;
    }

    if (argv[1] != NULL)
    {
        numOfThread = atoi(argv[1]);
    }

    // ---------------------입력받기 완료------------------

    // 글로벌 변수로 만들어 줌으로써 쓰레드간에 공유가 되도록 한다.
    LENGTH = numOfNumbers;
    NUMTO_THREAD = LENGTH / numOfThread;
    OFFSET = LENGTH % numOfThread;

    struct timeval stop, start_time;
    gettimeofday(&start_time, NULL);

    // 쓰레드를 받은 입력의 개수만큼 생성한다.
    pthread_t threads[numOfThread];

    for (long i = 0; i < numOfThread; i++)
    {
        // 각각의 쓰레드에 thread_merge_sort함수를 실행하도록 한다. 공유하는 데이터는 인자로 넘겨줄 필요가 없다. 함수에게는 i를 전달한다.
        int rc = pthread_create(&threads[i], NULL, thread_merge_sort, (void *)i);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (long i = 0; i < numOfThread; i++)
    {
        //쓰레드의 종료를 기다린다.
        pthread_join(threads[i], NULL);
    }
    merge_sections_of_array(num_list, numOfThread, 1);

    // 마무리 작업들
    gettimeofday(&stop, NULL);
    int ms = stop.tv_usec - start_time.tv_usec;

    dup2(std_in, 0);
    dup2(std_out, 1);
    dup2(std_err, 2);

    printf("\n정렬 후\n");
    for (int i = 0; i < LENGTH; i++)
    {
        printf("%d ", num_list[i]);
    }
    printf("\n 걸린 시간: %dms", ms);
    // sort end

    return 0;
}

void *thread_merge_sort(void *arg)
{
    mynum += 1;
    int thread_id = (long)arg;
    int left = thread_id * (NUMTO_THREAD);
    int right = (thread_id + 1) * (NUMTO_THREAD)-1;

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

/* 각각 mergesort된 부분들을 합친다. */
void merge_sections_of_array(int arr[], int number, int aggregation)
{
    for (int i = 0; i < number; i = i + 2)
    {
        int left = i * (NUMTO_THREAD * aggregation);
        int right = ((i + 2) * NUMTO_THREAD * aggregation) - 1;
        int middle = left + (NUMTO_THREAD * aggregation) - 1;
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

/* 입력나눠서 저장하는 func들 */
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
