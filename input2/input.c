#include<stdio.h>
#include<stdlib.h>  //atoi
#include<string.h>  //memset, memcpy,
// 프로그래밍 언어는 표준 라이브러리에 문자열 관련 명령을 구현하는 여러 함수들이 존재한다. 복사, 결합, 토큰화, 검색과 같은 다양한 명령이 지원된다. 문자열의 경우 표준 라이브러리는 문자열이 널 종단된다는 규칙을 사용한다: n개의 문자의 문자열은 n + 1 요소의 배열로 표현되며, 끝은 "NUL" 문자로 끝난다.
#include<unistd.h>
//On Unix-like systems, the interface defined by unistd.h is typically made up largely of system call wrapper functions such as fork, pipe and I/O primitives (read, write, close, etc.).
#include<sys/types.h>
//size_t
#include<sys/socket.h>
#include<netinet/in.h>
//sockaddr_in, addr_t
#include <arpa/inet.h>
 //htonl(uint32_t hostlong);
 //uint16_t htons(uint16_t hostshort);
 
int main()
{	
	char *env[2] = {"\xde\xad\xbe\xef = \xca\xfe\xba\xbe", NULL}; 
	char *argv[100];
	int i = 0;
	for (i=0; i<99; i++){
		argv[i] = "A";
	}
	argv['A']="\x00";
	argv['B']= "\x20\x0a\x0d";
	argv[100]= NULL;
	argv['C']= "1234"; //portnum

	int fd1[2], fd2[2];

	if(pipe(fd1)== -1){
		printf("pipe() error");
		exit(1);
	}
	if(pipe(fd2)== -1){
		printf("pipe() error");
		exit(1);
	}

	pid_t pid;

	if((pid = fork()) == -1){
		printf("fork() error");
		exit(1);
	}

	if(pid == 0){
		sleep(2);

		close(fd1[1]);
		close(fd2[1]);
		dup2(fd1[0], 0);
		dup2(fd2[0], 2);
		close(fd1[0]);
		close(fd2[0]);

		FILE *fp = fopen("\x0a", "w");
		fwrite("\x00\x00\x00\x00",4,1,fp);
		fclose(fp);

		execve("/home/input2/input", argv, env);  //server

	}else{
		close(fd1[0]);
		close(fd1[0]); 
		write(fd1[1], "\x00\x0a\x00\xff", 4); 
		write(fd2[1], "\x00\x0a\x02\xff", 4);
		
		int sockfd, port;

		struct sockaddr_in server_addr;
		 
		sockfd = socket(AF_INET, SOCK_STREAM, 0); //client socket 생성
		 
		memset(&server_addr, 0x00, sizeof(server_addr)); // server_addr을 일단 초기화
		 
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		server_addr.sin_port = htons(atoi(argv['C']));
		 
		if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))){
		printf("connect() error");
		return 1;
		}

		write(sockfd, "\xde\xad\xbe\xef", 4);
		close(sockfd);	
		//client 


	}


}