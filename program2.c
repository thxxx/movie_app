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
void forkmerge(int nums[], int maxlen, int end);
int fm2(int nums[], int maxlen, int number);
void firfork(int nums[], int maxlen, int number);

int nowChilderen = 0;

int main(int argc, char *argv[])

{
    int std_in, std_out, std_err;
    std_in = dup(0);
    std_out = dup(1);
    std_err = dup(2);

    // get input start

    printf("문자열을 입력하세요 : ");

    int number;

    scanf("%d", &number);

    printf("%d \n", number);
    int no_of_tokens = 64;

    int num_list[number];

    int numbers;
    int maxlen = *argv[1] - '0';

    for (int i = 0; i < number; i++)
    {
        scanf("%d", &numbers);
        num_list[i] = numbers;
        printf("%d ", num_list[i]);
    }
    printf("\n");
    num_list[number] = (int)0;

    // get input and store in num_list end.

    // mergeSort start

    struct timeval stop, start_time;
    gettimeofday(&start_time, NULL);

    //do stuff
    printf("시작! \n");

    forkmerge(num_list, maxlen, number);
    //do your thang
    gettimeofday(&stop, NULL);
    int ms = stop.tv_usec - start_time.tv_usec;

    dup2(std_in, 0);
    dup2(std_out, 1);
    dup2(std_err, 2);

    printf("\n정렬 후\n");
    for (int i = 0; i < number; i++)
    {
        printf("%d ", num_list[i]);
    }
    printf("\n 걸린 시간(단위 ms) : %d ", ms);
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
    int tmp[8]; // 새 배열
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

int fm2(int nums[], int maxlen, int number)
{

    pid_t pid, pidr;

    int status;
    pid = fork();

    if (pid == 0)
    { //child process
        printf("\n child %d \n", nowChilderen);
        if (maxlen <= nowChilderen)
        {
            int in, out;
            char *args[] = {"program1", nums, 0, number, NULL}; // merge sort로 넘겨줄 인자들. program1의 redirection입력으로 안넘어간다.
            // open input and output files
            in = open("iiin.txt", O_RDONLY);
            out = open("ooout.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            // replace standard input with input file
            dup2(in, 0);
            // replace standard output with output file
            dup2(out, 1);
            // close unused file descriptors
            close(in);
            close(out);
            // execute program 1
            //mergeSort(nums, 0, number);
            execv("./program1", args);

            exit(0);
        }
        else
        {
            forkmerge(nums, maxlen, number);
        }
        exit(0);
    }
    else if (pid > 0) // parent process
    {
        pid = wait(&status); // wait child

        char buffer[5] = {
            0,
        }; // 문자열 데이터 4바이트 NULL 1바이트. 4 + 1 = 5
        int count = 0;
        int total = 0;

        FILE *fp = fopen("ooout.txt", "r"); // hello.txt 파일을 읽기 모드(r)로 열기.
                                            // 파일 포인터를 반환

        while (feof(fp) == 0) // 파일 포인터가 파일의 끝이 아닐 때 계속 반복
        {
            count = fread(buffer, sizeof(char), 4, fp); // 1바이트씩 4번(4바이트) 읽기
            nums[(int)(total / 4)] = buffer;
            //printf("%s", buffer);                       // 읽은 내용 출력
            memset(buffer, 0, 5); // 버퍼를 0으로 초기화
            total += count;       // 읽은 크기 누적
        }
        fclose(fp); // 파일 포인터 닫기

        // input, output file 삭제해야함.

        return 1;
    }
    else
    { // Error in forking
        perror("error");
    }

    return 1;
}

/*
두개의 프로세스로 나누어 실행하게 한다.

*/
void forkmerge(int nums[], int maxlen, int end)
{
    nowChilderen += 2;

    int middle = (int)(end / 2);

    int left[middle];
    int right[end - middle];

    for (int i = 0; i < middle; i++)
    {
        left[i] = nums[i];
    }
    for (int i = 0; i < end - middle; i++)
    {
        right[i] = nums[middle + i];
    }

    fm2(left, maxlen, middle);
    fm2(right, maxlen, end - middle);

    for (int i = 0; i < middle; i++)
    {
        nums[i] = left[i];
    }
    for (int i = 0; i < end - middle; i++)
    {
        nums[middle + i] = right[i];
    }
}

void firfork(int nums[], int maxlen, int number)
{
    int i = 0;
    while (nums[i] != (int)0)
    {
        printf("%d ", nums[i]);
        i++;
    }

    nowChilderen += 2;

    forkmerge(nums, maxlen, number);
}