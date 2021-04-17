
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

void mergeSort(int data[], int left, int right)
void merge(int data[], int left, int middle, int right)

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

    //병합정렬 시작
    mergeSort(num_list, 0, numOfNumbers - 1);

    printf("%d", num_list[0]);
    for (int i = 1; i < numOfNumbers; i++)
    {
        printf(" %d", num_list[i]);
    }
    printf("\n");
    // sort end

    return 0;
}

void mergeSort(int data[], int left, int right)
{
    int middle;
    if (left < right)
    {
        middle = (left + right) / 2;
        mergeSort(data, left, middle);
        mergeSort(data, middle + 1, right);
        merge(data, left, middle, right);
    }
}

void merge(int data[], int left, int middle, int right)
{
    int le = left, mi = middle + 1, count = left;
    int temp[right]; // 새 배열

    while (le <= middle && mi <= right)
    {
        if (data[le] <= data[mi])
            temp[count++] = data[le++];
        else
            temp[count++] = data[mi++];
    }

    while (le <= middle)
        temp[count++] = data[le++];

    while (mi <= right)
        temp[count++] = data[mi++];

    for (int i = left; i <= right; i++)
        data[i] = temp[i];
}