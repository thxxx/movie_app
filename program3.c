
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

void merge_sort(int data[], int p, int r);
void *merge_sort_at_thread(void *tid);
void merge_after_merge(int gloNumList[], int number, int aggregation);
void merge(int data[], int p, int q, int r);

int numOfThread = 1;
int gloNumList[100];

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

    scanf("%d", &numOfNumbers); // 첫 입력은 전체 숫자의 갯수로 따로 저장한다.

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
        // 각각의 쓰레드에 merge_sort_at_thread함수를 실행하도록 한다. 공유하는 데이터는 인자로 넘겨줄 필요가 없다. 함수에게는 i를 전달한다.
        int res = pthread_create(&threads[i], NULL, merge_sort_at_thread, (void *)(long)i);
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
    merge_after_merge(gloNumList, numOfThread, 1);

    // 마무리 작업들
    gettimeofday(&stop, NULL);
    int ms = stop.tv_usec - start_time.tv_usec;

    dup2(std_in, 0);
    dup2(std_out, 1);
    dup2(std_err, 2);

    printf("\n");
    for (int i = numOfNumbers - 1; i >= 0; i--)
    {
        printf("%d ", gloNumList[i]);
    }
    printf("\n%d\n", ms);
    // sort end

    return 0;
}

void *merge_sort_at_thread(void *tid)
{
    int threadId = (long)tid;
    int left = threadId * (numPerThread);
    int right = (threadId + 1) * (numPerThread)-1;

    if (threadId == numOfThread - 1)
    {
        right += remaind;
    }
    int middle;
    if (left < right)
    {
        middle = (right + left) / 2;
        merge_sort(gloNumList, left, right);
        merge_sort(gloNumList, left + 1, right);
        merge(gloNumList, left, middle, right);
    }
    return NULL;
}

void merge_sort(int data[], int left, int right)
{
    int middle;
    if (left < right)
    {
        middle = (left + right) / 2;
        merge_sort(data, left, middle);
        merge_sort(data, middle + 1, right);
        merge(data, left, middle, right);
    }
}

void merge(int data[], int left, int middle, int right)
{
    int le = left, mi = middle + 1, count = left;
    int temp[right]; // 새 임시 배열

    while (le <= middle && mi <= right) // 한쪽이 비기전까지(범위끝에 다다르기 전까지) 계속 비교한다.
    {
        if (data[le] <= data[mi]) // 작은값을 넣어준다.
            temp[count++] = data[le++];
        else
            temp[count++] = data[mi++];
    }

    while (le <= middle)
        temp[count++] = data[le++];

    while (mi <= right)
        temp[count++] = data[mi++];

    for (int i = left; i <= right; i++)
        data[i] = temp[i]; // 범위에 해당하는 만큼만 배열을 바꿔준다.
}

/* 각각 mergesort된 부분들을 합친다. */
void merge_after_merge(int arr[], int number, int aggregation)
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
        merge_after_merge(arr, number / 2, aggregation * 2);
    }
}
