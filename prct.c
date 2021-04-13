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

    int j = 1;
    while (j < maxlen)
    {
        j *= 2;
        depth += 1;
    }
    //do stuff
    printf(" 멀티프로세스 병합정렬 시작! \n");

    // 정렬 프로세스. 가장 중요
    /*****************************/
    // num_list 는 입력받은 숫자들, maxlen은 리디렉션으로 들어온 생성하는 프로세스 수 , nnumber은 숫자 개수

    // 전체 입력 데이터를 total_process_num 만큼 적절히 나눈다.

    pid_t pids[maxlen];
    char num[10] = "123456789";
    char *args;
    int i;
    int n = 4;
    int status;

    for (i = 0; i < n; i++)
    {
        if ((pids[i] = fork()) < 0)
        {
            perror("fork");
            return -1;
        }
        else if (pids[i] == 0)
        {
            printf("%c\n", num[i]);
            args = (char *)malloc(sizeof(char));
            args[0] = num[i];
            execl("./pp", "pp", args, NULL);
            exit(0);
        }
        else
        {
            pids[i] = wait(&status);
        }
    }
    for (int i = 0; i < n; i++)
    {
    }
}