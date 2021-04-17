/*******************************************************************
 *   Made by : 2019147505 김호진
 *   Simple Unix Mini Shell 구현.
 *******************************************************************/

#include <stdio.h>    // For: printf(), fprintf(), getchar(), perror(), stderr
#include <stdlib.h>   // For: malloc(), realloc(), free(), exit(), execvp(), EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>   // For: strtok(), strcmp(), strcat(), strcpy()
#include <unistd.h>   // For: chdir(), fork(), exec(), pid_t, getcwd()
#include <sys/wait.h> // For: waitpid()
#include <fcntl.h>    // For: File creation modes
#include <signal.h>
#include <sys/types.h>
#include <pwd.h> // getpwuid
#include <sys/mman.h>
#include <errno.h>
#include <time.h>

#define NUM_OF_SUP 14 // 표준 유닉스 프로그램 갯수를 설정

char *standardUnixProgram[] = {"pwd", "ls", "echo", "mkdir", "head", "tail", "rm", "time", "cat", "vi", "find", "clear", "rmdir", "ps"};

void shell_loop(void);                    // exit명령 전까지 miniShell을 반복한다.
void PathDisplay(void);                   // [시간:분:초]아이디@경로> 출력
char *read_command_line(void);            // 들어온 입력을 전부 받아 단순 문자열로 저장한다.
char **split_command_line(char *command); // 받은 전체 입력을 space를 기준으로 문자열로 나눠준다.
int shell_execute(char **args);           // 입력된 명령어에 해당하는 동작을 수행한다.
int shell_execute_sup(char **args);       // 표준 유닉스 프로그램을 실행한다.
int shell_execute_program(char **args);   // .가 입력되었을 때 해당 파일을 실행하도록 해준다
int sep_background(char **args);          //
int sup_background(char **args);          //

// 백그라운드 동작 명령이 들어왔는지 체크한다.
int background_exec = 0;

int main(int argc, char **argv)
{
    printf("\n");
    printf("******************************************************************************\n");
    printf("*                                                                            *\n");
    printf("*                  minishell by 2017147505 김호진 start!                     *\n");
    printf("*                                                                            *\n");
    printf("*  구현 기능 목록 : cd, exit, Standard Unix Program, redirectioin( < , > )   *\n");
    printf("*                   프로그램 실행(.), 백그라운드 동작(&) 기타 등등           *\n");
    printf("******************************************************************************\n");
    printf("\n");

    // exit될때까지 계속반복하는 loop
    shell_loop();

    return 0;
}

/*
main loop of the Mini-Shell
 */
void shell_loop(void)
{
    // Display help at startup
    int status;
    char *command_line;
    char **arguments;
    status = 1;

    while (status)
    {
        // background 명령어인지 알려주는 변수를 초기화한다.
        background_exec = 0;
        PathDisplay();
        command_line = read_command_line();
        if (strcmp(command_line, "") == 0)
            continue;
        arguments = split_command_line(command_line);
        //arguments 는 그냥 키워드들 배열. 키워드들을 parameter로 전달하고 커맨드를 수행하도록 한다.
        status = shell_execute(arguments);
        //shell_execute 의 reuturn이 0 이면 종료.
    }
}

/* 
[시간:분:초]아이디@경로> 출력 
*/
void PathDisplay()
{
    //-------------------show the path-----------------------------
    long size;
    char *buf;
    char *road;

    size = pathconf(".", _PC_PATH_MAX);

    if ((buf = (char *)malloc((size_t)size)) != NULL)
        road = getcwd(buf, (size_t)size);

    //----------show the user name root------------------------

    struct passwd *getpwuid(uid_t uid); // 아이디를 가져온다.
    struct passwd *user;
    uid_t uid = 0;

    time_t timer;
    struct tm *t;
    timer = time(NULL);
    t = localtime(&timer);

    if ((user = getpwuid(uid)) == NULL)
        perror("getpwuid() error");
    else
    {
        printf("[%d:%d:%d]%s@%s$", t->tm_hour, t->tm_min, t->tm_sec, user->pw_name, road);
    }
    free(buf);
}

/* 입력을 해석할 수 있게 키워드 단위(space로 구분)로 나눈다 */
char *read_command_line(void)
{
    int position = 0;
    int buffer_size = 30;
    char *command = (char *)malloc(sizeof(char) * 30);
    char ch;

    // command line을 char by char로 읽어서 command로 반환한다.
    ch = getchar();
    while (ch != EOF && ch != '\n')
    {
        // &가 명령어에 포함되어있으면 백그라운드에서 동작하라고 알린 후 없애준다.
        if (ch == '&')
        {
            background_exec = 1;
            ch = ' ';
        }
        command[position] = ch;

        // 버퍼를 필요한 경우에 따라 재할당해서 크기를 늘려준다.
        if (position >= buffer_size)
        {
            buffer_size += 8;
            command = realloc(command, sizeof(char) * buffer_size);
        }
        position++;
        ch = getchar();
    }
    // 자꾸 마지막에 space가 없으면 키워드로 구분짓지 않는 문제가 발생해서 임의로 마지막에 space를 넣음.
    command[position] = ' ';
    return command;
}

// 입력받은 command를 delimeter로 구분하여 나누어서 tokens에 담는다.
char **split_command_line(char *command)
{
    int position = 0;
    char **tokens = malloc(sizeof(char *) * 60);
    char delim[] = " \n";

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

/*
입력된 명령어에 해당하는 동작을 찾아서 수행한다. 
*/
int shell_execute(char **args)
{

    if (args[0] == NULL)
    { // Empty command 라면 loop의 status가 1이되어 while문을 빠져나감
        return 1;
    }

    if (strcmp(args[0], "exit") == 0)
    {
        //exit이 입력이 되면 loop의 status가 0이되어 while문을 빠져나감. 미니쉘 종료
        return 0;
    }

    // 현재의 Standard Input and Output file descriptors를 복사해둬서
    // 커맨드 수행 후 다시 복구될 수 있게 한다.
    int std_in, std_out, std_err;
    std_in = dup(0);
    std_out = dup(1);
    std_err = dup(2);

    int i = 1;
    int num_of_token = (int)(sizeof(args) / sizeof(args[0]));

    // 프로그램의 실행 (.)
    if (strncmp(*args, ".", 1) == 0)
    {
        if (background_exec == 0)
        {
            //백그라운드 동작이 아닌경우
            shell_execute_program(args);
        }
        else if (background_exec == 1)
        {
            //백그라운드 동작인 경우
            sep_background(args);
        }

        // 빠져나가기전 복구해준다.
        dup2(std_in, 0);
        dup2(std_out, 1);
        dup2(std_err, 2);

        return 1; // 이후를 체크하지 않고 바로 빠져나간다.
    }

    // redirection 체크
    while (args[i] != NULL)
    {
        // input redirection인 경우 < 뒤의 파일을 열어서 입력으로 받는다.
        if (strcmp(args[i], "<") == 0)
        {
            int in = open(args[i + 1], O_RDONLY);
            if (in < 0)
            {
                perror("os-kim - error");
                return 1;
            }

            if (dup2(in, 0) < 0)
            {
                perror("os-kim - error");
                return 1;
            }
            close(in);
            args[i] = NULL;
            args[i + 1] = NULL;
            i += 2;
        }
        // output redirection인 경우 출력값들을 > 다음에 오는 파일에 작성되도록 한다.
        else if (strcmp(args[i], ">") == 0)
        {
            int out = open(args[i + 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            if (out < 0)
            {
                perror("os-kim - error");
                return 1;
            }

            if (dup2(out, 1) < 0)
            {
                perror("os-kim - error");
                return 1;
            }
            close(out);
            args[i] = NULL;
            args[i + 1] = NULL;
            i += 2;
        }

        else
        {
            i++;
        }
    }
    // 리디렉션이면 위에서 실행된 후 넘어온다.

    //
    // cd 명령어 : 구현기능 - "cd", "cd /path", "cd ..", "cd /",
    //
    if (strcmp(args[0], "cd") == 0)
    {
        int chdir_return;

        char path[500];
        if (args[1] == NULL)
        {
            args[1] = "/";
        }

        if (strncmp(args[1], "/", 1) != 0)
        {
            getcwd(path, sizeof(path)); // 현재 디렉토리를 path에 저장한다. 최대 크기는 sizeof(path) = 500
            strcpy(path, "/");          // 현재 디렉토리 뒤에 / 를 붙이고
            strcpy(path, args[1]);      // 두번째 인자로 들어온 이동할 디렉토리도 붙여준다
            chdir_return = chdir(path); // 현재의 작업 디렉토리를 path로 바꿔준다. 에러발생했다면 음수값 리턴.
        }
        else if (strcmp(args[1], "..") == 0) // 두번째 인자가 .. 라면 이전 디렉토리로 간다!
        {
            getcwd(path, sizeof(path));
            char *path1 = strstr(path, "/"); // path에서 / 를 검색해서 뒤에서 부터 가장 먼저 나타나는 곳 까지 반환. / 도 포함한다
            int s = strlen(path1);           // 길이 s에 저장
            //strcpy(path, path1);
            path[s] = (char)0; // 없애버린다.
            chdir_return = chdir(path);
        }
        else if (strcmp(args[1], "/") == 0)
        {
            chdir_return = chdir(args[1]);
        }
        else
        {
            printf("cd: %s: No such file or directory\n", args[1]);
        }

        //error handling
        if (chdir_return < 0)
        {
            printf("Error while changing the directory, error is : %s \n", strerror(errno));
        }
    }
    // 앞에 해당하는게 있으면 리턴되어서 여기까지 안온다.

    /*
    * 
    * 표준 유닉스 프로그램 명령어인지 체크한다.
    * 
    */
    for (int j = 0; j < NUM_OF_SUP; j++)
    {
        if (strcmp(args[0], standardUnixProgram[j]) == 0)
        {
            if (background_exec == 0)
            {
                // 백그라운드 동작이 아니라면
                shell_execute_sup(args);
            }
            else if (background_exec == 1)
            {
                // 백그라운드 동작이라면
                sup_background(args);
            }

            dup2(std_in, 0);
            dup2(std_out, 1);
            dup2(std_err, 2);

            return 1;
        }
    }

    dup2(std_in, 0);
    dup2(std_out, 1);
    dup2(std_err, 2);

    //return ret_status;
    return 1;
}

/*
스탠다드 유닉스 프로그램을 실행하는 함수
Standard Unix Program
*/
int shell_execute_sup(char **args)
{
    char **argv = malloc(sizeof(char *) * 50);
    int i = 0;
    while (args[i] != NULL)
    {
        argv[i] = args[i];
        i++;
    }
    argv[i] = (char *)0; // 마지막엔 null 포인터를 넣어준다.

    pid_t pid = fork(); // 부모 프로세스의 메모리를 복제한 자식 프로세스 생성

    if (pid == 0)
    {
        // child process
        if (execvp(argv[0], argv) < 0)
            exit(0); // 명령어 실행하고 문제있으면 exit
    }
    else if (pid > 0)
    {
        // 부모 프로세스 코드 시작
        int status;
        pid_t wpid = waitpid(pid, &status, 0); // 인자로 받은 pid값을 가진 자식프로세스의 종료를 기다린다.
        free(argv);
        return 1;
    }
    else
    { // pid가 음수값이라면 fork시 error가 발생한 것.
        free(argv);
        perror(" forling error");
    }
    return 1;
}

/*
.가 입력되었을 때 해당 프로그램을 실행하도록 해준다.
shell_execute_sup와 비슷한 부분은 주석 최소화
*/
int shell_execute_program(char **args)
{
    pid_t pid = fork();

    int k = 0;
    char *argv[20];
    while (args[k] != NULL)
    {
        argv[k] = (char *)args[k];
        k++;
    }
    argv[k] = NULL;

    int status;

    if (pid == 0)
    {
        int in, out;
        int i = 0;

        while (args[i] != NULL)
        {
            if (strcmp(args[i], "<") == 0)
            { // open input and output files
                in = open(args[i + 1], O_RDONLY);
                dup2(in, 0);
                close(in);

                while (args[i] != NULL) // what if redirection is double!
                {
                    if (strcmp(args[i], ">") == 0)
                    {
                        out = open(args[i + 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

                        dup2(out, 1);

                        close(out);
                    }
                    i++;
                }

                if (execvp(argv[0], argv) < 0)
                    exit(0);
            }
            else if (strcmp(args[i], ">") == 0)
            { // open input and output files
                out = open(args[i + 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

                dup2(out, 1);

                close(out);

                if (execvp(argv[0], argv) < 0)
                    exit(0);
            }
            i++;
        }

        execvp(argv[0], argv);
        exit(1);
    }
    else if (pid > 0)
    {
        // parent process
        pid = wait(&status); // wait child. 이번엔 wait를 사용
        return 1;
    }
    else
    {
        perror("forking error");
    }
    return 1;
}

// 백그라운드
int sep_background(char **args)
{
    int fdp[2];         // 2개의 fd를 담을 배열을 정의한다.
    pipe(fdp);          // pipe를 호출해 두 개의 fd로 배열을 채워준다.
    pid_t pid = fork(); // 부모 프로세스의 fd와 메모리를 복제한 자식 프로세스 생성한다.

    int status;

    if (pid == 0)
    {                               // if pid == 0, 자식 프로세스
        dup2(fdp[0], STDIN_FILENO); // fdp[0]으로 표준입력을 넘겨준다.
        close(fdp[0]);
        int k = 0;     // fdp[0]은 자식 프로세스에서 더이상 필요하지 않기 떄문에 닫아준다.
        char *cmd[10]; // 명령어 인자를 만들어준다.
        while (args[k] != NULL)
        {
            cmd[k] = (char *)args[k]; // 하나씩 넣음.
            k++;
        }
        cmd[k] = NULL;
        int in, out;
        int i = 0;

        close(fdp[1]);
        while (args[i] != NULL)
        {
            if (strcmp(args[i], "<") == 0)
            { // open input and output files

                in = open(args[i + 1], O_RDONLY);
                dup2(in, 0);
                close(in);

                while (args[i] != NULL) // what if redirection is double!
                {
                    if (strcmp(args[i], ">") == 0)
                    {
                        out = open(args[i + 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

                        dup2(out, 1);

                        close(out);
                    }
                    i++;
                }

                if (execvp(cmd[0], cmd) < 0)
                    exit(0); // 명령어 실행하고 문제있으면 exit
            }
            else if (strcmp(args[i], ">") == 0)
            { // open input and output files
                out = open(args[i + 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

                dup2(out, 1);

                close(out);

                if (execvp(cmd[0], cmd) < 0)
                    exit(0); // 명령어 실행하고 문제있으면 exit
            }

            i++;
        }

        execvp(args[0], args);
        exit(1);
    }
    else if (pid > 0) // parent process
    {
        printf("[1] %d\n", pid);
        return 1;
    }
    else
    { // Error in forking
        perror("error");
    }
    return 1;
}

int sup_background(char **args)
{
    return 1;
}