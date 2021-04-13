/*******************************************************************
 *   Made by : 2019147505 김호진
 *   Simple Unix Mini Shell 구현.
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

#define NUM_OF_SUP 15 // 표준 유닉스 프로그램 갯수

char *standardUnixProgram[] = {"ls", "exit", "help", "pwd", "echo", "mkdir", "head", "tail", "rm", "time", "cat", "vi", "find", "clear", "rmdir", "make"};

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
	printf("minishell by 김호진 start! \n");
	// exit될때까지 계속반복하는 loop
	shell_loop();

	return 0;
}

/* [시간:분:초]아이디@경로> 출력 */
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

/* 입력을 해석할 수 있게 키워드 단위(space로 구분)로 나눈다 */
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
	// 자꾸 마지막에 space가 없으면 키워드로 구분짓지 않는 문제가 발생해서 임의로 마지막에 space를 넣음.
	command[position] = ' ';
	return command;
}

char **split_command_line(char *command)
{
	int position = 0;
	int no_of_tokens = 64;
	char **tokens = malloc(sizeof(char *) * no_of_tokens);
	char delim[] = " \n";

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

/*
쉘에 들어온 명령어를 수행한다.
 */
int shell_execute(char **args)
{

	if (args[0] == NULL)
	{ // Empty command
		return 1;
	}

	if (strcmp(args[0], "exit") == 0)
	{
		// loop의 status가 0이되어 while문을 빠져나감
		return 0;
	}

	// 현재의 Standard Input and Output file descriptors를 복사해둬서
	// 커맨드 수행 후 다시 복구될 수 있게 한다.
	int std_in, std_out, std_err;
	std_in = dup(0);
	std_out = dup(1);
	std_err = dup(2);

	int i = 1;
	int args_length = (int)(sizeof(args) / sizeof(args[0]));

	int chars = (int)(sizeof(args) / sizeof(*args[0]));

	// 프로그램의 실행 (.)
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

	// redirection 체크
	while (args[i] != NULL)
	{
		if (strcmp(args[i], "<") == 0)
		{										   // Input redirection
			int inp = open(args[i + 1], O_RDONLY); // open after <
			if (inp < 0)
			{
				perror("os-kim - open 실패");
				return 1;
			}

			if (dup2(inp, 0) < 0)
			{
				perror("os-kim - write 실패");
				return 1;
			}
			close(inp);
			args[i] = NULL;
			args[i + 1] = NULL;
			i += 2;
		}

		else if (strcmp(args[i], ">") == 0)
		{ // Output redirection
			int out = open(args[i + 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			if (out < 0)
			{
				perror("os-kim - open 실패");
				return 1;
			}

			if (dup2(out, 1) < 0)
			{
				perror("os-kim - write 실패");
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
		background_exec = 0;
		DisplayPrompt();
		command_line = read_command_line();
		if (strcmp(command_line, "") == 0)
		{
			continue;
		}
		arguments = split_command_line(command_line);

		//arguments 는 그냥 키워드들 배열. 키워드들을 parameter로 전달하고 커맨드를 수행하도록 한다.
		status = shell_execute(arguments);
		//shell_execute 의 reuturn이 0 이면 종료.
	}
}

/*

스탠다드 유닉스 프로그램을 실행하는 함수

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

			char **argv = malloc(sizeof(char *) * 50);
			int i = 0;
			while (args[i] != NULL)
			{
				argv[i] = args[i];
				i++;
			}
			argv[i] = (char *)0; // 마지막엔 null 포인터를 넣어준다.

			// open input and output files
			out = open(temp_out, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

			// replace standard output with output file
			dup2(out, 1);

			// close unused file descriptors
			close(out);

			execvp(argv[0], argv);

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

/*

.가 입력되었을 때 해당 파일을 실행하도록 해준다

*/
int shell_execute_program(char **args)
{
	pid_t pid;

	int status;
	pid = fork();

	if (pid == 0)
	{ //child process

		int in, out;
		char *argv[] = {args[0], (char *)0};
		if (strcmp(args[1], "<") == 0)
		{ // open input and output files
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