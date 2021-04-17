
/*******************************************************************
 *                  Made by : 2017147505 김호진
 *      숫자들을 입력받아 병합정렬(Merge Sort)을 한 후 출력하는 프로그램
 *******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>

void mergeSort(int data[], int p, int r);
void merge(int data[], int p, int q, int r);

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

    for (int i = numOfNumbers - 1; i >= 0; i--)
    {
        printf(" %d", num_list[i]);
    }
    printf("\n");
    // sort end

    return 0;
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
