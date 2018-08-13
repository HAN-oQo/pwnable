#mistake walkthrough

hint : operator priority

##fd 
if(fd=open("/home/mistake/password",O_RDONLY,0400) < 0){
		printf("can't open password %d\n", fd);
		return 0;
	}

fd=open("/home/mistake/password",O_RDONLY,0400) < 0
priority
1. open("/home/mistake/password",O_RDONLY,0400) < 0
2. fd

open을 했는데 잘열렸을 거니까 open의 리턴값은 양수
그런데 0보다는 크니까 false, 즉 0이된다.

fd = 0 -> stdin 표준입력상태가 된다. 
그러므로 pw_buf, pw_buf2를 모두 우리가 쓸 수 있게된다. 

strncmp(pw_buf, pw_buf2, PW_LEN)이것은 뒤에 바이트 수만큼 문자열을 비교하겠다는 뜻 같으면 0을 반환한다. 

xor 연산을 하면 1과 하는 것이므로 결국 not 연산과 같다고 생각하면 된다. 

password1을 0000000000으로 설정하고
password2를 1111111111으로 설정하면 완료.


