
/*******************************************************************
 *       Made by : 2019147505 김호진
 *   숫자들을 입력받아 병합정렬(Merge Sort)을 한 후 출력하는 프로그램
 *******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>

void merge_sort(int data[], int left, int right);
void merge(int data[], int left, int middle, int right);

int main(int argc, char *argv[])

{
    int numOfNumbers;

    scanf("%d", &numOfNumbers); // 첫 입력은 전체 숫자의 갯수로 따로 저장한다.

    int num_list[numOfNumbers + 1];

    int numbers;

    for (int i = 0; i < numOfNumbers; i++)
    {
        scanf("%d", &numbers);
        num_list[i] = numbers;
    }
    // ---------------------입력받기 완료------------------ 받은 입력은 num_list에 저장

    struct timeval stop, start_time;
    gettimeofday(&start_time, NULL);

    //병합정렬 시작
    merge_sort(num_list, 0, numOfNumbers - 1);

    gettimeofday(&stop, NULL);
    int ms = stop.tv_usec - start_time.tv_usec;

    for (int i = 0; i < numOfNumbers; i++)
    {
        printf("%d ", num_list[i]);
    }
    printf("\n");
    // sort end
    return 0;
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