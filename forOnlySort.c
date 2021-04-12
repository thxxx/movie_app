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

void mergeSort(int data[], int p, int r);
void merge(int data[], int p, int q, int r);

int main(int argc, char *argv[])

{
    int number;

    scanf("%d", &number); // 첫 입력은 전체 갯수

    int num_list[number + 1];

    int numbers;

    for (int i = 0; i < number; i++)
    {
        scanf("%d", &numbers);
        num_list[i] = numbers;
    }

    mergeSort(num_list, 0, number - 1);

    printf("%d", num_list[0]);
    for (int i = 1; i < number; i++)
    {
        printf(" %d", num_list[i]);
    }
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
