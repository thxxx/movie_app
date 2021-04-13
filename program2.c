
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
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>

int workProcess(char **args, int maxlen, int number);
void merge(char **firstData, char **secondData, char **arguments);
void dividehalf(char **arguments, int end, int currentdepth);
char *read_command_line(void);
char **split_command_line(char *command);

int numOfProcess = 1;
int mynum = 0;
int depth = 0;

int main(int argc, char *argv[])

{
    int std_in, std_out, std_err;
    std_in = dup(0);
    std_out = dup(1);
    std_err = dup(2);

    // get input start

    int numOfNumbers;
    int first;
    char *command_line;
    char **arguments;

    scanf("%d ", &numOfNumbers);

    printf(" 입력받은 숫자의 개수 : %d \n", numOfNumbers);

    if (argv[1] != NULL)
    {
        numOfProcess = atoi(argv[1]);
    }

    command_line = read_command_line();
    arguments = split_command_line(command_line); //arguments 는 그냥 키워드들 배열

    for (int i = 0; i < numOfNumbers; i++)
    {
        printf("%s ", arguments[i]);
    }
    // ---------------------입력받기 완료------------------

    int i = 1;
    while (i < numOfProcess)
    {
        i *= 2;
        depth += 1;
    }
    //do stuff
    printf(" 멀티프로세스 병합정렬 시작 \n");

    // 정렬 프로세스. 가장 중요
    /*****************************/
    // num_list 는 입력받은 숫자들, maxlen은 리디렉션으로 들어온 생성하는 프로세스 수 , nnumber은 숫자 개수

    // 전체 입력 데이터를 total_process_num 만큼 적절히 나눈다.
    dividehalf(arguments, numOfNumbers, 0);

    /*****************************/

    dup2(std_in, 0);
    dup2(std_out, 1);
    dup2(std_err, 2);

    printf("\n정렬 후\n");
    for (int i = 0; i < numOfNumbers; i++)
    {
        printf("%s ", arguments[i]);
    }

    // 이렇게 메모리 해제를 시켜줄 수 있는것인지 잘 모르겠습니다.
    free(command_line);
    free(arguments);

    return 0;
}

void dividehalf(char **arguments, int end, int currentdepth)
{
    currentdepth += 1;
    int middle = (int)(end / 2);

    char **left = malloc(sizeof(char *) * middle);
    char **right = malloc(sizeof(char *) * (end - middle));

    for (int i = 0; i < middle; i++)
    {
        left[i] = arguments[i];
    }
    for (int i = 0; i < end - middle; i++)
    {
        right[i] = arguments[middle + i];
    }

    if (depth > currentdepth)
    { // 아직 수행할 프로세스의 수보다 나누어진 영역이 더 적어 더 잘게 나누어서 실행가능하다.
        dividehalf(left, middle, currentdepth);
        dividehalf(right, end - middle, currentdepth);
        merge(left, right, arguments);
    }
    else if (mynum < numOfProcess - 1)
    {
        workProcess(left, numOfProcess, middle);
        workProcess(right, numOfProcess, end - middle);
        merge(left, right, arguments); // left와 right로 나누어져 병합정렬된 후 다시 합쳐서 arguments를 수정한다.
    }
    else
    {
        workProcess(arguments, numOfProcess, end);
    }

    free(left);
    free(right);
}

int workProcess(char **args, int maxlen, int number)
{
    pid_t pid, pidr;
    mynum += 1;

    int status;
    pid = fork();

    int in, out;

    if (pid == 0)
    { //child process
        printf("i am process number %d \n", mynum);

        // 자식프로세스의 프로그램으로 넘겨줄 값들을 임시파일에 작성해서 넘겨준다.
        out = open("tempout.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        dup2(out, 1);
        printf("%d\n", number);
        for (int k = 0; k < number; k++)
        {
            printf("%s ", args[k]);
        }
        printf("\n");
        close(out);

        char *argv[] = {"program1", (char *)0};
        // 넘겨줄 파일과 결과를 쓸 파일을 open한다.
        in = open("tempout.txt", O_RDONLY);
        out = open("ooout.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

        dup2(in, 0);
        dup2(out, 1);

        close(in);
        close(out);

        //program1을 실행해 병합정렬한다.
        execvp("./program1", args);
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
        int co = 0;
        char *command = (char *)malloc(sizeof(char) * 100);
        char **outs;

        while (feof(fp) == 0) // 파일 포인터가 파일의 끝이 아닐 때 계속 반복
        {
            count = fread(buffer, sizeof(char), 4, fp); // 1바이트씩 4번(4바이트) 읽기
            //printf("%s", buffer);
            command[co] = buffer[0];
            command[co + 1] = buffer[1];
            command[co + 2] = buffer[2];
            command[co + 3] = buffer[3];
            //printf("%s", buffer);                       // 읽은 내용 출력
            memset(buffer, 0, 5); // 버퍼를 0으로 초기화
            total += count;       // 읽은 크기 누적
            co += count;
        }

        outs = split_command_line(command);

        for (int i = 0; i < number; i++)
        {
            args[i] = outs[i];
        }

        fclose(fp); // 파일 포인터 닫기

        // 임시로 작성한 input, output file 삭제해야함.
        remove("tempout.txt");
        remove("ooout.txt");

        return 1;
    }
    else
    { // Error in forking
        perror("error");
    }

    return 1;
}

void merge(char **firstData, char **secondData, char **arguments)
{
    // firstData와 secondData를 합쳐서 arguments로 반환.
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

char *read_command_line(void)
{
    int position = 0;
    int buf_size = 30;
    char *command = (char *)malloc(sizeof(char) * 30);
    char c;

    // command line을 char by char로 읽어서 command로 반환한다.
    c = getchar();
    while (c != EOF && c != '\n')
    {
        command[position] = c;

        // 버퍼를 필요한 경우에 따라 재할당해서 크기를 늘려준다.
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

char **split_command_line(char *command)
{
    int position = 0;
    int no_of_tokens = 64;
    char **tokens = malloc(sizeof(char *) * no_of_tokens);
    char delim[3] = " \n";

    // 입력받은 command를 delimeter로 구분하여 나누어서 tokens에 담는다.
    char *token = strtok(command, delim);

    while (token != NULL)
    {
        tokens[position] = token;
        position++;
        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    return tokens;
}