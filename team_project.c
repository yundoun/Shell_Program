#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
    char buf[256];
    char *argv[50];
    int narg;
    pid_t pid;
    int background = 0; // 백그라운드 실행 여부를 나타내는 플래그

    while (1)
    {
        printf("shell> ");
        fgets(buf, sizeof(buf), stdin);

        // 개행 문자 제거
        buf[strcspn(buf, "\n")] = '\0';

        // "exit" 입력 시 프로그램 종료
        if (strcmp(buf, "exit") == 0)
        {
            printf("Goodbye!\n");
            break;
        }

        // 백그라운드 실행 여부 확인
        if (buf[strlen(buf) - 1] == '&')
        {
            background = 1;
            buf[strlen(buf) - 1] = '\0'; // '&' 제거
        }
        else
        {
            background = 0;
        }

        narg = getargs(buf, argv);
        pid = fork();

        if (pid == 0)
        {
            // 자식 프로세스에서 명령 실행
            execvp(argv[0], argv);
            perror("execvp"); // execvp 실행에 실패한 경우
            exit(1);
        }
        else if (pid > 0)
        {
            // 부모 프로세스에서 대기
            if (background == 0)
            {
                wait(NULL);
            }
        }
        else
        {
            perror("fork failed"); // fork 실패한 경우
        }
    }

    return 0;
}

int getargs(char *cmd, char **argv)
{
    int narg = 0;
    while (*cmd)
    {
        if (*cmd == ' ' || *cmd == '\t')
            *cmd++ = '\0';
        else
        {
            argv[narg++] = cmd++;
            while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t')
                cmd++;
        }
    }
    argv[narg] = NULL;
    return narg;
}
