
/*******************************************************************
 *   Made by : 2019147505 김호진
 *   숫자들을 입력받아 멀티프로세싱방법으로 병합정렬(Merge Sort)을 한 후 출력하는 프로그램
 *******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>

void workProcess(int *data, int end);
int *split_command_int(char *command);
void merge(int data[], int p, int q, int r);
void merge_after_merge(int nums[], int nop, int accelerator);

#define READ 0
#define WRITE 1

int numOfProcess = 1;
int numOfNumbers = 0;
int numPerProcess = 0;

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
        numOfProcess = atoi(argv[1]); // argv[1] is the number of processes
    }

    // ---------------------입력받기 완료------------------

    int data[numOfNumbers + 1];

    struct timeval stop, start_time;
    gettimeofday(&start_time, NULL);

    numPerProcess = numOfNumbers / numOfProcess;
    int remaind = numOfNumbers % numOfProcess;

    // 지금 부터 내 출력이 입력으로 들어옴
    int out = open("temp_out_os.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    dup2(out, 1);
    close(out);

    for (int j = 0; j < numOfProcess; j++)
    {
        if (j == numOfProcess - 1)
        {
            for (int i = 0; i < numOfProcess - 1; i++)
            {
                int status;
                wait(&status); // 자식 프로세스가 종료될때까지 기다린다.
            }
            for (int i = 0; i < (numPerProcess + remaind); i++)
            {
                data[i] = num_list[numPerProcess * j + i];
            }
            pid_t pid;

            int fds[2]; // 2개의 fd를 담을 배열을 선언한다.
            pipe(fds);  // pipe를 호출해 두 개의 fd로 배열을 채워준다.

            pid = fork();

            if (pid == 0)
            {                                     // if pid == 0, 자식 프로세스
                dup2(fds[READ], STDIN_FILENO);    // fds[0]으로 표준입력을 넘겨준다.
                close(fds[READ]);                 // fds[0]은 자식 프로세스에서 더이상 필요하지 않기 떄문에 닫아준다. 복사본이기 때문에(?)
                close(fds[WRITE]);                // 원래부터 필요없었던 fd라 닫아준다.
                char *cmd[] = {"program1", NULL}; // 명령어 인자를 만들어준다.
                if (execvp("./program1", cmd) < 0)
                    exit(0); // 명령어 실행하고 문제있으면 exit
            }
            else if (pid > 0)
            {
                // 부모 프로세스 코드 시작
                close(fds[READ]);                                      // 쓰기만 하면되는 부모 프로세스에서는 필요 없는 fd라 닫아준다.
                dprintf(fds[WRITE], "%d \n", numPerProcess + remaind); // fds[1]에 출력을 쓴다.

                for (int i = 0; i < numPerProcess + remaind; i++)
                {
                    dprintf(fds[WRITE], "%d \n", data[i]); // fds[1]에 출력을 쓴다.
                }
                close(fds[WRITE]);
            }
        }
        else
        {
            for (int i = 0; i < numPerProcess; i++)
            {
                data[i] = num_list[numPerProcess * j + i];
            }
            pid_t pid;

            int fds[2]; // 2개의 fd를 담을 배열을 선언한다.
            pipe(fds);  // pipe를 호출해 두 개의 fd로 배열을 채워준다.

            pid = fork();

            if (pid == 0)
            {                                     // if pid == 0, 자식 프로세스
                dup2(fds[READ], STDIN_FILENO);    // fds[0]으로 표준입력을 넘겨준다.
                close(fds[READ]);                 // fds[0]은 자식 프로세스에서 더이상 필요하지 않기 떄문에 닫아준다. 복사본이기 때문에(?)
                close(fds[WRITE]);                // 원래부터 필요없었던 fd라 닫아준다.
                char *cmd[] = {"program1", NULL}; // 명령어 인자를 만들어준다.
                if (execvp("./program1", cmd) < 0)
                    exit(0); // 명령어 실행하고 문제있으면 exit
            }
            else if (pid > 0)
            {
                // 부모 프로세스 코드 시작
                close(fds[READ]);                            // 쓰기만 하면되는 부모 프로세스에서는 필요 없는 fd라 닫아준다.
                dprintf(fds[WRITE], "%d \n", numPerProcess); // fds[1]에 출력을 쓴다.

                for (int i = 0; i < numPerProcess; i++)
                {
                    dprintf(fds[WRITE], "%d \n", data[i]); // fds[1]에 출력을 쓴다.
                }
                close(fds[WRITE]);
            }
        }
    }
    printf("\n");
    // 마무리 작업들

    int status;
    wait(&status); // 자식 프로세스가 종료될때까지 기다린다.
    //이제 아님
    dup2(std_in, 0);
    dup2(std_out, 1);
    dup2(std_err, 2);
    //
    //

    gettimeofday(&stop, NULL);
    int ms = stop.tv_usec - start_time.tv_usec;
    printf("ms : %d\n", ms);

    //각 프로세스의 출력값들을 따로 받아서 저장해야된다.

    FILE *fp = fopen("temp_out_os.txt", "r"); // hello.txt 파일을 읽기 모드(r)로 열기.
                                              // 파일 포인터를 반환
    char read[100] = {
        0,
    };

    fread(read, 1, 200, fp); //전체 읽기

    int *outs;
    outs = split_command_int(read);
    fclose(fp); // 파일 포인터 닫기
    //remove("temp_out_os.txt"); // 읽은 임시파일 삭제
    // 최종적으로 합쳐준다.
    merge_after_merge(outs, numOfProcess, 1);

    for (int i = 0; i < numOfNumbers; i++)
    {
        printf("%d ", outs[i]);
    }

    printf("\n");

    return 0;
}

void workProcess(int *data, int end)
{

    pid_t pid;

    int fds[2]; // 2개의 fd를 담을 배열을 선언한다.
    pipe(fds);  // pipe를 호출해 두 개의 fd로 배열을 채워준다.

    pid = fork();

    if (pid == 0)
    {                                     // if pid == 0, 자식 프로세스
        dup2(fds[READ], STDIN_FILENO);    // fds[0]으로 표준입력을 넘겨준다.
        close(fds[READ]);                 // fds[0]은 자식 프로세스에서 더이상 필요하지 않기 떄문에 닫아준다. 복사본이기 때문에(?)
        close(fds[WRITE]);                // 원래부터 필요없었던 fd라 닫아준다.
        char *cmd[] = {"program1", NULL}; // 명령어 인자를 만들어준다.
        if (execvp("./program1", cmd) < 0)
            exit(0); // 명령어 실행하고 문제있으면 exit
    }
    else if (pid > 0)
    {
        // 부모 프로세스 코드 시작
        close(fds[READ]);                  // 쓰기만 하면되는 부모 프로세스에서는 필요 없는 fd라 닫아준다.
        dprintf(fds[WRITE], "%d \n", end); // fds[1]에 출력을 쓴다.

        for (int i = 0; i < end; i++)
        {
            dprintf(fds[WRITE], "%d \n", data[i]); // fds[1]에 출력을 쓴다.
        }
        close(fds[WRITE]);
    }
    int status;

    waitpid(pid, &status, 0); // 자식 프로세스가 종료될때까지 기다린다.
}

// 입력받은 command를 delimeter로 구분하여 나누어서 tokens에 담아 반환한다. int로 반환한다.
int *split_command_int(char *command)
{
    int position = 0;
    int no_of_tokens = 64;
    int *tokens = malloc(sizeof(int *) * no_of_tokens);
    char delim[3] = " \n";

    char *token = strtok(command, delim);

    while (token != NULL)
    {
        tokens[position] = atoi(token);
        position++;
        token = strtok(NULL, delim);
    }

    return tokens;
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

/* 각각 mergesort된 부분들을 합친다. */
void merge_after_merge(int nums[], int nop, int accelerator)
{
    for (int i = 0; i < nop; i += 2)
    {
        int left = i * (numPerProcess * accelerator);
        int right = ((i + 2) * numPerProcess * accelerator) - 1;
        int middle = left + (numPerProcess * accelerator) - 1;
        if (right >= numOfNumbers)
        {
            right = numOfNumbers - 1;
        }
        merge(nums, left, middle, right);
    }
    if (nop / 2 >= 1)
    {
        merge_after_merge(nums, nop / 2, accelerator * 2);
    }
}
