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

void makeTwoProcess(char **arguments, int maxlen, int end);
int fm2(char **args, int maxlen, int number);
void merge(char **firstData, char **secondData, char **arguments);
void dividehalf(char **arguments, int end, int currentdepth);
int maxlen = 1;
int nowChilderen = 1;
int mynum = 0;

int depth = 0;

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

int main(int argc, char *argv[])

{
    int std_in, std_out, std_err;
    std_in = dup(0);
    std_out = dup(1);
    std_err = dup(2);

    // get input start

    int number;
    int first;
    char *command_line;
    char **arguments;

    scanf("%d ", &number);

    printf(" 입력받은 숫자의 개수 : %d \n", number);

    if (argv[1] != NULL)
    {
        maxlen = atoi(argv[1]);
    }

    command_line = read_command_line();
    arguments = split_command_line(command_line); //arguments 는 그냥 키워드들 배열

    for (int i = 0; i < number; i++)
    {
        printf("%s ", arguments[i]);
    }

    int i = 1;
    while (i < maxlen)
    {
        i *= 2;
        depth += 1;
    }
    //do stuff
    printf(" 멀티프로세스 병합정렬 시작! \n");

    // 정렬 프로세스. 가장 중요
    /*****************************/
    // num_list 는 입력받은 숫자들, maxlen은 리디렉션으로 들어온 생성하는 프로세스 수 , nnumber은 숫자 개수

    // 전체 입력 데이터를 total_process_num 만큼 적절히 나눈다.
    dividehalf(arguments, number, 0);

    /*****************************/

    dup2(std_in, 0);
    dup2(std_out, 1);
    dup2(std_err, 2);

    printf("\n정렬 후\n");
    for (int i = 0; i < number; i++)
    {
        printf("%s ", arguments[i]);
    }
    // sort end
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
    {
        dividehalf(left, middle, currentdepth);
        dividehalf(right, end - middle, currentdepth);
        merge(left, right, arguments);
    }
    else if (mynum < maxlen - 1)
    {
        printf("left: ");

        for (int i = 0; i < middle; i++)
        {
            printf("%s ", left[i]);
        }
        printf("\n");

        printf("right: ");
        for (int i = 0; i < end - middle; i++)
        {
            printf("%s ", right[i]);
        }
        printf("\n");

        fm2(left, maxlen, middle);
        fm2(right, maxlen, end - middle);
        merge(left, right, arguments);

        printf("after merg \n");

        printf("nums : %d left: ", middle);

        for (int i = 0; i < middle; i++)
        {
            printf("%s ", left[i]);
        }
        printf("\n");

        printf("nums : %d right: ", end - middle);
        for (int i = 0; i < end - middle; i++)
        {
            printf("%s ", right[i]);
        }
        printf("\n");
    }
    else
    {

        printf("args: ");
        for (int i = 0; i < end; i++)
        {
            printf("%s ", arguments[i]);
        }
        printf("\n");
        fm2(arguments, maxlen, end);

        printf("after merg \n");

        for (int i = 0; i < end; i++)
        {
            printf("%s ", arguments[i]);
        }
        printf("\n");
    }

    free(left);
    free(right);
}

int fm2(char **args, int maxlen, int number)
{
    pid_t pid, pidr;
    mynum += 1;

    int status;
    pid = fork();

    int in, out;

    if (pid == 0)
    { //child process
        printf("i am process number %d \n", mynum);

        out = open("tempout.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        dup2(out, 1);
        printf("%d\n", number);
        for (int k = 0; k < number; k++)
        {
            printf("%s ", args[k]);
        }
        printf("\n");

        close(out);
        char *argv[] = {"program1", (char *)0}; // merge sort로 넘겨줄 인자들. program1의 redirection입력으로 안넘어간다.
        // open input and output files
        in = open("tempout.txt", O_RDONLY);
        out = open("ooout.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        // replace standard input with input file
        dup2(in, 0);
        // replace standard output with output file
        dup2(out, 1);
        // close unused file descriptors
        close(in);
        close(out);
        //execute program 1

        // 여기 program1의 인자로 argv가 전달이 안되고 있다.
        // 여기서 args는 내가 원하는대로 자른 후 병합정렬에 넘겨주려는 값임.
        // 근데 in으로 받은게 무조건 들어간다?
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

        outs = split_command_line(command); //arguments 는 그냥 키워드들 배열

        for (int i = 0; i < number; i++)
        {
            args[i] = outs[i];
        }

        fclose(fp); // 파일 포인터 닫기

        // input, output file 삭제해야함.
        //remove("tempout.txt");
        //remove("ooout.txt");

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
void makeTwoProcess(char **arguments, int maxlen, int end)
{
    mynum += 1;

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

    // 받은 숫자들을 두그룹으로 나눴다. left, right. right가ㅏ left보다 하나 크거나 같음.

    // 프로세스를 두개 만들어서 각각 병합 정렬을 시도한다.
    // 근데 두개로 나눠서 해버리면 병합정렬된 각각을 또 합쳐야하는거 아닌가?

    printf("\n left: ");
    for (int i = 0; i < middle; i++)
    {
        printf("%s ", left[i]);
    }
    printf("\n");

    printf("\n right: ");
    for (int i = 0; i < end - middle; i++)
    {
        printf("%s ", right[i]);
    }
    printf("\n");

    //
    //

    fm2(left, maxlen, middle);
    fm2(right, maxlen, end - middle);

    merge(left, right, arguments);

    //
    //
    //
    free(left);
    free(right);
}

void merge(char **firstData, char **secondData, char **arguments)
{
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
