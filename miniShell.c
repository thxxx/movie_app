/*******************************************************************
 *           File:  miniShell.c
 *         Author:  Ashwitha Yadav T
 *   Date Started:  September 22, 2017
 *    Description:  A simple implementation of the Unix Shell in the
 *                      C Programming language.
 *******************************************************************/

#include <stdio.h>	  // For: printf(), fprintf(), getchar(), perror(), stderr
#include <stdlib.h>	  // For: malloc(), realloc(), free(), exit(), execvp(), EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>	  // For: strtok(), strcmp(), strcat(), strcpy()
#include <unistd.h>	  // For: chdir(), fork(), exec(), pid_t, getcwd()
#include <sys/wait.h> // For: waitpid()
#include <fcntl.h>	  // For: File creation modes
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/mman.h>
#include <errno.h>
#include <time.h>

#define NUM_OF_SUP 11 // Number of builtin commands defined

char *standardUnixProgram[] = {"ls", "exit", "help", "pwd", "echo", "mkdir", "head", "tail", "rm", "time", "cat", "vi"};

void DisplayPrompt(void);
char *read_command_line(void);
char **split_command_line(char *command);
int shell_execute(char **args);
void shell_loop(void);
int shell_execute_sup(char **args);
int shell_execute_program(char **args);
int sep_background(char **args);
int sup_background(char **args);

int background_exec = 0;

int main(int argc, char **argv)
{
	printf("minishell by Daniel start! \n");
	// Main loop of the shell
	shell_loop();

	return 0;
}

void DisplayPrompt()
{

	//-------------------show the path-----------------------------

	long size;
	char *buf;
	char *ptr;

	size = pathconf(".", _PC_PATH_MAX);

	if ((buf = (char *)malloc((size_t)size)) != NULL)
		ptr = getcwd(buf, (size_t)size);

	//----------show the user name root------------------------

	struct passwd *getpwuid(uid_t uid);
	struct passwd *p;
	uid_t uid = 0;

	time_t timer;
	struct tm *t;
	timer = time(NULL);
	t = localtime(&timer);

	if ((p = getpwuid(uid)) == NULL)
		perror("getpwuid() error");
	else
	{
		printf("[%d:%d:%d]%s@%s>", t->tm_hour, t->tm_min, t->tm_sec, p->pw_name, ptr);
	}
	free(buf);
}

/*
 * Built-in command functions
 */

char **split_command_line(char *command)
{
	int position = 0;
	int no_of_tokens = 64;
	char **tokens = malloc(sizeof(char *) * no_of_tokens);
	char delim[2] = " ";

	// Split the command line into tokens with space as delimiter
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
		if (c == '&')
		{
			background_exec = 1;
			position++;
			c = getchar();
			continue;
		}
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

/*

 */
int shell_execute(char **args)
{

	if (args[0] == NULL)
	{ // Empty command
		return 1;
	}

	if (strcmp(args[0], "exit") == 0)
	{

		return 0;
	}

	// Copy the current Standard Input and Output file descriptors
	// so they can be restored after executing the current command
	int std_in, std_out, std_err;
	std_in = dup(0);
	std_out = dup(1);
	std_err = dup(2);

	// Check if redirection operators are present
	int i = 1;
	int args_length = (int)(sizeof(args) / sizeof(args[0]));

	int chars = (int)(sizeof(args) / sizeof(*args[0]));

	// . 실행 시작

	if (strncmp(*args, ".", 1) == 0)
	{
		if (background_exec == 0)
		{
			shell_execute_program(args);
		}
		else if (background_exec == 1)
		{
			sep_background(args);
		}

		dup2(std_in, 0);
		dup2(std_out, 1);
		dup2(std_err, 2);

		return 1;
	}

	while (args[i] != NULL)
	{
		if (strcmp(args[i], "<") == 0)
		{										   // Input redirection
			int inp = open(args[i + 1], O_RDONLY); // open after <
			if (inp < 0)
			{
				perror("0 : os-kim");
				return 1;
			}

			if (dup2(inp, 0) < 0)
			{
				perror("dup : os-kim");
				return 1;
			}
			close(inp);
			args[i] = NULL;
			args[i + 1] = NULL;
			i += 2;
		}

		else if (strcmp(args[i], ">") == 0)
		{ // Output redirection

			int out = open(args[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0755);
			if (out < 0)
			{
				perror("os-kim");
				return 1;
			}

			if (dup2(out, 1) < 0)
			{
				perror("os-kim");
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
	// 리디렉션이면 위에서 실행됨

	if (strcmp(args[0], "cd") == 0)
	{
		int chdir_rtrn;
		char path[500];

		if (strncmp(args[1], "/", 1) != 0)
		{
			getcwd(path, sizeof(path)); // 현제 디렉토리를 path에 저장한다. 최대 크기는 sizeof(path) = 500
			strcpy(path, "/");			// 현재 디렉토리 뒤에 / 를 붙이고
			strcpy(path, args[1]);		// 두번째 인자로 들어온 이동할 디렉토리도 붙여준다
			chdir_rtrn = chdir(path);	// 현재의 작업 디렉토리를 path로 바꿔준다. 에러발생했다면 음수값 리턴.
		}
		else if (strcmp(args[1], "..") == 0) // 두번째 인자가 .. 라면 이전 디렉토리로 간다!
		{
			getcwd(path, sizeof(path));
			char *path1 = strstr(path, "/"); // path에서 / 를 검색해서 뒤에서 부터 가장 먼저 나타나는 곳 까지 반환. / 도 포함한다
			int s = strlen(path1);			 // 길이 s에 저장
			//strcpy(path, path1);
			path[s] = (char)0; // 없애버린다.
			chdir_rtrn = chdir(path);
		}
		else if (strncmp(args[1], "/", strlen("/")) == 0)
		{
			chdir_rtrn = chdir(args[1]);
		}
		else if (strcmp(args[1], "/") == 0)
		{
			chdir_rtrn = chdir(args[1]);
		}
		else
		{
			printf("incorrect directory");
		}

		//error handling
		if (chdir_rtrn < 0)
			printf("Error while changing the directory, error is : %s \n", strerror(errno));
	}
	// 앞에 해당하는게 있으면 리턴되어서 여기까지 안온다.

	for (int j = 0; j < NUM_OF_SUP; j++)
	{
		if (strcmp(args[0], standardUnixProgram[j]) == 0)
		{
			if (background_exec == 0)
			{
				shell_execute_sup(args);
			}
			else if (background_exec == 1)
			{
				sup_background(args);
			}

			dup2(std_in, 0);
			dup2(std_out, 1);
			dup2(std_err, 2);

			return 1;
		}
	}
	// For other commands, execute a child process
	// int ret_status = start_process(args);

	// Restore the Standard Input and Output file descriptors
	dup2(std_in, 0);
	dup2(std_out, 1);
	dup2(std_err, 2);

	//return ret_status;
	return 1;
}

/*
 * Function:  shell_loop
 * ---------------------
 *  main loop of the Mini-Shell
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
		background_exec = 0;
		DisplayPrompt();
		command_line = read_command_line();
		if (strcmp(command_line, "") == 0)
		{
			continue;
		}
		arguments = split_command_line(command_line); //arguments 는 그냥 키워드들 배열

		status = shell_execute(arguments);
		//shell_execute 의 reuturn이 0 이면 종료.
	}
}

/*

스탠다드 유닉스 프로그램 실행 함수

*/

int shell_execute_sup(char **args)
{
	{
		pid_t pid;

		int status;
		pid = fork();

		char temp_out[] = "temp_out.txt";

		if (pid == 0)
		{ //child process
			int in, out;

			// argv에 args를 넣어줘야한다.
			char *argv[] = {args[0], (char *)0};

			// open input and output files
			in = open("input.txt", O_RDONLY);
			out = open(temp_out, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			// replace standard input with input file
			dup2(in, 0);
			// replace standard output with output file
			dup2(out, 1);

			// close unused file descriptors
			close(in);
			close(out);

			execvp(argv[0], args); // ls -l  -> argv[0] = "ls"

			exit(1); // 위에서 exec이 실행되면 이건 실행이 안돼야하는게 맞다. 따라서 정상이면 0, 비정상이면 1
		}
		else if (pid > 0) // parent process
		{

			pid = wait(&status); // wait child

			char buffer[5] = {
				0,
			}; // 문자열 데이터 4바이트 NULL 1바이트. 4 + 1 = 5
			int count = 0;
			int total = 0;

			FILE *fp = fopen(temp_out, "r"); // hello.txt 파일을 읽기 모드(r)로 열기.
											 // 파일 포인터를 반환

			while (feof(fp) == 0) // 파일 포인터가 파일의 끝이 아닐 때 계속 반복
			{
				count = fread(buffer, sizeof(char), 4, fp); // 1바이트씩 4번(4바이트) 읽기
				printf("%s", buffer);						// 읽은 내용 출력
				memset(buffer, 0, 5);						// 버퍼를 0으로 초기화
				total += count;								// 읽은 크기 누적
			}
			fclose(fp); // 파일 포인터 닫기

			// input, output file 삭제해야함.
			remove(temp_out);
			// print

			return 1;
		}
		else
		{ // Error in forking
			perror("error");
		}

		return 1;
	}
}

int shell_execute_program(char **args)
{
	pid_t pid;
	printf(" . 로 파일 실행 \n");

	int status;
	pid = fork();

	if (pid == 0)
	{ //child process

		int in, out;
		char *argv[] = {args[0], (char *)0};
		if (strcmp(args[1], "<") == 0)
		{ // open input and output files
			printf("args[2] : %s \n", args[2]);
			in = open(args[2], O_RDONLY);
			//out = open("output.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

			dup2(in, 0);
			//dup2(out, 1);

			close(in);
			//close(out);

			execvp(args[0], args);
		}
		else if (strcmp(args[1], ">") == 0)
		{ // open input and output files
			printf("args[2] : %s \n", args[2]);
			//in = open(args[2], O_RDONLY);
			out = open(args[2], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

			//dup2(in, 0);
			dup2(out, 1);

			//close(in);
			close(out);

			execvp(args[0], args);
		}

		execv(args[0], argv);
		exit(1);
	}
	else if (pid > 0) // parent process
	{

		pid = wait(&status); // wait child
		int returnValue;
		if (WIFEXITED(status))
		{
			returnValue = WEXITSTATUS(status);
		}

		//remove("output.txt");
		return 1;
	}
	else
	{ // Error in forking
		perror("error");
	}
	return 1;
}

int sep_background(char **args)
{
	pid_t pid;
	printf(" . 로 파일 실행 \n");

	int status;
	pid = fork();

	if (pid == 0)
	{ //child process

		int in, out;
		char *argv[] = {args[0], (char *)0};
		printf("%s", args[0]);

		// open input and output files
		in = open("input.txt", O_RDONLY);
		//out = open("output.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		/*
		dup2(in, 0);
		dup2(out, 1);

		close(in);
		close(out);
		*/

		sleep(3);

		execv(args[0], argv);
		exit(1);
	}
	else if (pid > 0) // parent process
	{

		pid = wait(&status); // wait child
		int returnValue;
		if (WIFEXITED(status))
		{
			returnValue = WEXITSTATUS(status);
		}

		//remove("output.txt");
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
	printf("vv\n");
	{
		pid_t pid;

		int status;
		pid = fork();

		char temp_out[] = "temp_out.txt";

		if (pid == 0)
		{ //child process
			int in, out;

			// argv에 args를 넣어줘야한다.
			char *argv[] = {args[0], (char *)0};

			// open input and output files
			in = open("input.txt", O_RDONLY);
			out = open(temp_out, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			// replace standard input with input file
			dup2(in, 0);
			// replace standard output with output file
			dup2(out, 1);

			// close unused file descriptors
			close(in);
			close(out);

			execvp(argv[0], argv); // ls -l  -> argv[0] = "ls"

			exit(1); // 위에서 exec이 실행되면 이건 실행이 안돼야하는게 맞다. 따라서 정상이면 0, 비정상이면 1
		}
		else if (pid > 0) // parent process
		{

			waitpid(pid, &status, WNOHANG); // wait child

			if (WIFEXITED(status))
			{
				printf("정상종료\n");
				char buffer[5] = {
					0,
				}; // 문자열 데이터 4바이트 NULL 1바이트. 4 + 1 = 5
				int count = 0;
				int total = 0;

				FILE *fp = fopen(temp_out, "r"); // hello.txt 파일을 읽기 모드(r)로 열기.
												 // 파일 포인터를 반환

				while (feof(fp) == 0) // 파일 포인터가 파일의 끝이 아닐 때 계속 반복
				{
					count = fread(buffer, sizeof(char), 4, fp); // 1바이트씩 4번(4바이트) 읽기
					printf("%s", buffer);						// 읽은 내용 출력
					memset(buffer, 0, 5);						// 버퍼를 0으로 초기화
					total += count;								// 읽은 크기 누적
				}
				fclose(fp); // 파일 포인터 닫기

				// input, output file 삭제해야함.
				remove(temp_out);
				// print
				printf("리턴값 %d\n", WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status))
			{
				printf("신호받았음\n");
				printf("신호번호 %d\n", WTERMSIG(status));
			}

			return 1;
		}
		else
		{ // Error in forking
			perror("error");
		}

		return 1;
	}

	return 1;
}