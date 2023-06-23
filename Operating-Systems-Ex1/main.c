
#include "Ex1.h"

//return all the files where you now work
void dirFunc()
{
    DIR *dir;
    struct dirent *sd;
    dir = opendir(".");
    if (dir == NULL)
    {
        printf("Error! Unable to open directory.\n");
        exit(1);
    }
    while ((sd = readdir(dir)) != NULL)
    {
        printf(">> %s\n", sd->d_name);
    }
    closedir(dir);
    return;
}

//return the path where you now work
void pathFunc()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    else
    {
        perror("getcwd() error");
        exit(1);
    }
}
//copy file1 to file2 with fread, fwrite, fopen 
void copySrcToDst(char *src, char *dst)
{
    FILE * filer, * filew;
	int numr,numw;
	char buffer[100];
	
	if((filer=fopen(src,"rb"))==NULL){
		fprintf(stderr, "open read file error.\n");
		exit(1);
	}

	if((filew=fopen(dst,"wb"))==NULL){
		fprintf(stderr,"open write file error.\n");
		exit(1);
	}
	while(feof(filer)==0){	
	if((numr=fread(buffer,1,100,filer))!=100){
		if(ferror(filer)!=0){
		fprintf(stderr,"read file error.\n");
		exit(1);
		}
		else if(feof(filer)!=0);
	}
	if((numw=fwrite(buffer,1,numr,filew))!=numr){
		fprintf(stderr,"write file error.\n");
		exit(1);
	}
	}	
	
	fclose(filer);
	fclose(filew);
    printf("\nThe file copy sucsessfuly\n");
}

// the main work in simple way like switch case 
int main()
{
    char input[50] = {0};
    pathFunc();
    if (!scanf("%[^\n]%*c", input))
    {
        while (getchar() != '\n');
    }
    int len = strlen(input);
    for (int i = 0; i < len; i++)
    {
        printf("%c", input[i]);
    }
    // printf("\n");
    while (strcmp(input, "EXIT"))
    {
        pathFunc();
        if (strncmp(input, "ECHO", 4) == 0)
        {
            printf(">>");
            // printf("%d\n", len);
            for (int i = 4; i < 50; i++)
            {
                printf("%c", input[i]);
            }

            printf("\n");
        }
        else if (strncmp(input, "DIR", 3) == 0)
        {
            dirFunc();
        }
        else if (strncmp(input, "TCP PORT", 8) == 0)
        {
            powerClient();
            dup2(1, 480);
            dup2(sock, 1);
        }
        else if (strncmp(input, "LOCAL", 5) == 0)
        {
            close(sock);
            dup2(480, 1);
            printf("return to the local shell and the client disconnect\n");
        }
        /*
         chdir() changes the current working directory
         of the calling process to the directory specified in path
         so it is system call.
        */
        else if (strncmp(input, "CD", 2) == 0)
        {
            char path[50];
            bzero(path, 50);
            int j = 0;
            for (int i = 3; i < strlen(input); i++, j++)
            {
                path[j] = input[i];
            }
            chdir(path);
            pathFunc();
        }
        /*
        fopen, fread, fwrite is libary function.
        */
        else if (strncmp(input, "COPY", 4) == 0)
        {
            char srcFile[50];
            char dstFile[50];
            bzero(srcFile, 50);
            bzero(dstFile, 50);
            int j = 5;
            int i = 0;
            int flag1 = 1;
            while (input[j] != ' ')
            {

                srcFile[i] = input[j];
                i++;
                j++;
            }
            j++;
            i = 0;
            while (input[j] != '\0')
            {
                dstFile[i] = input[j];
                i++;
                j++;
            }
            copySrcToDst(srcFile, dstFile);
        }
        /*
        unlink is system function.
        */
        else if (strncmp(input, "DELETE", 6) == 0)
        {
            char path[50];
            bzero(path, 50);
            int j = 0;
            for (int i = 7; i < strlen(input); i++, j++)
            {
                path[j] = input[i];
            }
            unlink(path);
        }
        else if (strncmp(input, "EXIT", 4) == 0)
        {
            break;
        }
        /*
        The system() is library function
        */
        else
        {
            /* fork a child process */
            int pid = fork();
            if (pid < 0)
            {
                /* error occurred */
                return 1;
            }
            else if (pid == 0)
            {
                /* child process */
                char p[50] = "/bin/";
                strcat(p, input);
                execlp(p, input, NULL); // A new program(ls executable is loaded into memory and executed
            }
            else
            {
                /* parent process */
                /* parent will wait for the child to complete */
                wait(NULL);
                printf("Child Complete\n");
            }
            // system(input);
        }
        bzero(input, 50);
        if (!scanf("%[^\n]%*c", input))
        {
            while (getchar() != '\n')
                ;
        }
    }
    printf("good bye\n");

    return 0;
}
