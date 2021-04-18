
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
void merge_sections_of_array(int gloNumList[], int number, int aggregation);
void merge(int data[], int p, int q, int r);

int numOfThread = 1;
int gloNumList[100];
int mynum = 0;

int numOfNumbers = 0;
int numPerThread = 0;
int remaind;

int main(int argc, char *argv[])

{
    int std_in, std_out, std_err;
    std_in = dup(0);
    std_out = dup(1);
    std_err = dup(2);

    // get input start

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

    for (int i = 0; i < numOfNumbers; i++)
    {
        gloNumList[i] = num_list[i];
        printf("%d ", num_list[i]);
    }

    // ---------------------입력받기 완료------------------

    // 글로벌 변수로 만들어 줌으로써 쓰레드간에 공유가 되도록 한다.

    numPerThread = numOfNumbers / numOfThread;
    remaind = numOfNumbers % numOfThread;

    struct timeval stop, start_time;
    gettimeofday(&start_time, NULL);

    // 쓰레드를 받은 입력의 개수만큼 생성한다.
    pthread_t threads[numOfThread];

    for (int i = 0; i < numOfThread; i++)
    {
        // 각각의 쓰레드에 thread_merge_sort함수를 실행하도록 한다. 공유하는 데이터는 인자로 넘겨줄 필요가 없다. 함수에게는 i를 전달한다.
        int res = pthread_create(&threads[i], NULL, thread_merge_sort, (void *)(long)i);
        if (res)
        {
            printf("ERROR; return code from pthread_create() is %d\n", res);
            exit(-1);
        }
    }

    for (int i = 0; i < numOfThread; i++)
    {
        //쓰레드의 종료를 기다린다.
        pthread_join(threads[i], NULL);
    }
    merge_sections_of_array(gloNumList, numOfThread, 1);

    // 마무리 작업들
    gettimeofday(&stop, NULL);
    int ms = stop.tv_usec - start_time.tv_usec;

    dup2(std_in, 0);
    dup2(std_out, 1);
    dup2(std_err, 2);

    printf("\n정렬 후\n");
    for (int i = numOfNumbers - 1; i >= 0; i--)
    {
        printf("%d ", gloNumList[i]);
    }
    printf("\n 걸린 시간: %dms", ms);
    // sort end

    return 0;
}

void *thread_merge_sort(void *arg)
{
    mynum += 1;
    int thread_id = (long)arg;
    int left = thread_id * (numPerThread);
    int right = (thread_id + 1) * (numPerThread)-1;

    printf("\nI am working in %d thread\n", mynum);

    if (thread_id == numOfThread - 1)
    {
        right += remaind;
    }
    int middle = left + (right - left) / 2;
    if (left < right)
    {
        mergeSort(gloNumList, left, right);
        mergeSort(gloNumList, left + 1, right);
        merge(gloNumList, left, middle, right);
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

void merge(int data[], int left, int middle, int right)
{
    int le = left, mi = middle + 1, count = left;
    int tmp[right]; // 새 배열

    while (le <= middle && mi <= right)
    {
        if (data[le] <= data[mi])
            tmp[count++] = data[le++];
        else
            tmp[count++] = data[mi++];
    }

    while (le <= middle)
        tmp[count++] = data[le++];

    while (mi <= right)
        tmp[count++] = data[mi++];

    for (int i = left; i <= right; i++)
        data[i] = tmp[i];
}

/* 각각 mergesort된 부분들을 합친다. */
void merge_sections_of_array(int arr[], int number, int aggregation)
{
    for (int i = 0; i < number; i = i + 2)
    {
        int left = i * (numPerThread * aggregation);
        int right = ((i + 2) * numPerThread * aggregation) - 1;
        int middle = left + (numPerThread * aggregation) - 1;
        if (right >= numOfNumbers)
        {
            right = numOfNumbers - 1;
        }
        merge(arr, left, middle, right);
    }
    if (number / 2 >= 1)
    {
        merge_sections_of_array(arr, number / 2, aggregation * 2);
    }
}
