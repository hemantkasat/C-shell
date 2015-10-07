#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
void readcommand ( char *command )
{
	char *c = command;
	while ((*c = getchar()) != '\n')
		c++;
	*c = 0;
}
void runcommand(char **arg,int bg,int nopipes,char *user,char *host,char *dir)
{
	pid_t pid;
	int status;
	int fd[2][2],i,k;
	char *argredir[1000];
	char *newarg[1000];
	char *inf[1000],*outf[1000];
	char *token;
	int file,redir=0;
	int len_argredir,j,l,m=0;
	int redirno=0;
	int middleisinput=0;
	int indexinput=0,indexoutput=0;
//		printf("%s\n",arg[0]);
	pipe(fd[0]);pipe(fd[1]);
//	printf("no of pipes %d\n",nopipes-1);
//		printf("%s@%s:%s$ ",user,host,dir);  //prints prompts
	for(i=0;i<nopipes-1;i++)
	{
		bg=0;
		int  infile;
		int  outfile;
		int in=0;
		int out=0;
	//	else if(strcmp(newarg[k-2],"&")==0)
		if(arg[i][strlen(arg[i])-1]=='&')
		{
			arg[i][strlen(arg[i])-1]='\0';
		//	runcommand(arg,1,nopipes,user,host,dir);//runs command
			bg=1;
			setpgid(0,0);
		}
		printf("%d\n",bg);

		for(j=0;j<strlen(arg[i]);j++)
		{
			if(arg[i][j]=='<')
			{
				indexinput=j;
				
				redirno++;
				in=1;
			}
			if(arg[i][j]=='>')
			{
				indexoutput=j;
				redirno++;
				out=1;
			}
		}
//		printf("%s\n",arg[i]);
		token=strtok(arg[i],"<>");
		newarg[0]=token;
		k=1;
		while(token!=NULL)
		{
//			printf("in %dth command\n",i);
//			printf("%s\n",token);
			newarg[k]=strtok(NULL,"<>");
			token=newarg[k];
			k++;
		}
		newarg[1]=strtok(newarg[1]," ");
		newarg[2]=strtok(newarg[2]," ");
		if(indexinput<indexoutput)
		{
				if(in==1){
					printf("in file is %s\n",newarg[1]);
					infile=open(newarg[1],O_RDONLY,0);
					}
				if(out==1)
					{if(in==1){
					outfile=open(newarg[2],O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				//	printf("out file is %s\n",newarg[2]);
					}
					if(in==0)
					{
					outfile=open(newarg[1],O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				//	printf("out file is %s\n",newarg[2]);
					}
					}
		}
		if(indexinput>indexoutput)
		{		if(in==1){
					if(out==1){
					infile=open(newarg[2],O_RDONLY,0);	
					printf("in file is %s\n",newarg[2]);
					}
					if(out==0)
					{
					infile=open(newarg[1],O_RDONLY,0);	
		//			printf("in file is %s\n",newarg[2]);
					}
					}
					if(out==1)
					{
						outfile=open(newarg[1],O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		//			printf("out file is %s\n",newarg[1]);
					}
		}
			
			
		token=strtok(newarg[0]," ");
		newarg[0]=token;
		k=1;
		while(token!=NULL)
		{
		//	printf("in %dth command\n",i);
	//		printf("%s\n",token);
			newarg[k]=strtok(NULL," ");
			token=newarg[k];
			k++;
		}
		char dr[1024];
		dr[0]='/';
		dr[1]='h';
		dr[2]='o';
		dr[3]='m';
		dr[4]='e';
		dr[5]='/';
				for(l=6,j=0;user[m]!='\0';l++,m++)				{
					dr[l]=user[m];
				}
				dr[l]='\0';
//				printf("%s\n",dr);
		
		

		if(strcmp(newarg[0],"cd") == 0){
			if(newarg[1]=='\0'){
		//		printf("1\n");
		//		printf("%s\n",user);
					
				l=chdir(dr);
		//		printf("%d\n",l);
			}
			else
			{
				if(newarg[1][0]=='~'){
		//			printf("~ working\n");

					chdir(dr);
				}
				else{

				l=chdir(newarg[1]);
				if(l!=0)
					printf("%s directory doesn't exist\n",newarg[1]);}//if you input a non-existing directory
		}}
		else if(strcmp(newarg[0],"echo")==0){
					printf("%s\n",newarg[1]);

				}
		else if(strcmp(newarg[0],"pwd")==0)
		{
			printf("%s\n",dir);
		}
		else if(strcmp(newarg[0],"exit")==0)
		{
			exit(0);//exit
		}
	/*	else if(strcmp(newarg[k-2],"&")==0)
		{
			printf("%s\n",newarg[k-2]);
			newarg[k-2]='\0';
			printf("%s %s\n",newarg[0],newarg[1]);
			setpgid(0,0);
			if(execvp(*newarg,newarg)<0)//command not executed successfully
			{
			printf("executing command\n");
			printf("%s command not found\n",newarg[0]);
			exit(-1);
			}
//			runcommand(newarg,1,k,user,host,dir);
			}*/
		
			
		else{	
//		printf("%s\n",newarg[0]);
//		printf("%s\n",newarg[1]);
		pid=fork();//creates child process
//		printf("%s\n",newarg[0]);
		if(pid<0)
		{
			perror("child proc. not created\n");
			exit(-1);
		}
		else if(pid==0)
		{
		//	printf("%d %d\n",in,out);
		//printf("%s\n",newarg[0]);
	//		printf("%s\n",newarg[1]);
			if(i==0)
			{

			
	//			printf("at start\n");
		//		if(len_argredir==1)
				if(i!=nopipes-2)
				{	if(out==0){
				//		printf("writing to file d\n");
						dup2(fd[i][1],1);
						}
				if(in!=0)
				{
					dup2(infile,0);
					close(infile);
					dup2(fd[i][1],1);
				}
				if(out!=0)
				{
					dup2(outfile,1);
					close(outfile);
				}
				}
				if(i==nopipes-2)
				{
				//	printf("%d %d\n",in,out);
				//	printf("also at last\n");
					if(in!=0){
						dup2(infile,0);//printf("also at last and and infile is present\n");
						close(infile);
						}
					if(out!=0)
					{
					//	printf("writing to outfile\n");
						dup2(outfile,1);
						close(outfile);
					}
				}
		//		else 
		//			dup2(file,1);
		}
			else if(i==nopipes-2)
			{
		//		printf("%d %d\n",in,out);
				if(i!=0)
				{
					
		//			printf("at  end\n");
					if(in==0){
		//				printf("reading from pipe\n");
						
						
						dup2(fd[i-1][0],0);
						}
					if(in!=0)
					{
		//				printf("reading from file\n");
						dup2(infile,0);
						close(infile);
					}
					if(out!=0)
					{
		//				printf("writing to file\n");
						
						dup2(outfile,1);
						close(outfile);
					}
				}
				if(i==0)
				{
					if(in!=0){
						dup2(infile,0);
						close(infile);
						}
					if(out!=0)
					{
						dup2(outfile,1);
						close(outfile);
					}
				}
					
			}
			else
			{
		//		printf("in b/w\n");
				dup2(fd[i-1][0],0);
				dup2(fd[i][1],1);
	//			printf("%d\n",fd[i][1]);
			}
	//		printf("%s %s\n",newarg[0],newarg[1]);
			if(execvp(*newarg,newarg)<0)//command not executed successfully
			{
	//		printf("executing command\n");
			printf("%s command not found\n",newarg[0]);
			exit(-1);
			}
		}
		
	else{
	//	while(wait(&status)!=pid);
		if(bg==0)
		{
			printf("hi\n");
			waitpid(pid,&status,WUNTRACED);
		}
	}
	}
}
}

int main(int argc,char **argv,char **envp)
{
	char command[1000];
	char *token;
	char *arg[1000];
	char *built[1000];
	char user[1000];
	char host[20];
	char dir[100];
	int i,j,k,bg=0,len_arg,spaceparse;
	while (1)
	{
		j=getlogin_r(user,1000);//gets username
	//	printf("%s , %s\n",user,host);
		j=gethostname(host,20);//gets hostname
		getcwd(dir,sizeof(dir));//gets directory
		printf("%s@%s:%s$ ",user,host,dir);  //prints prompts
		readcommand(command);//reads command
		//printf("%s\n",command);
		token=strtok(command,"|");
		arg[0]=token;
		k=1;
		while(token!=NULL)
		{
//			printf("%s\n",token);
			arg[k]=strtok(NULL,"|");
			token=arg[k];
			k++;
		}
		len_arg=k;
		//printf("%s\n",arg[0]);


				
//		parse(command,arg);//differentiate b/w command and argument 
	
		//	printf("in else\n");
//			printf("%s and %d\n",arg[0],len_arg);
			runcommand(arg,0,len_arg,user,host,dir);//runs command
	}
	return 0;
}
