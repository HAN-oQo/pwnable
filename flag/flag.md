 >>wget http://pwnable.kr/bin/flag

 이후 ./flag를 해보았더니 permission denied란다
 sudo ./flag도 마찬가지

 chmod 사용. 
 chmod 755 myfile -> 절대모드를 사오ㅛㅇ하여 소유자에게는 읽기, 쓰기 및 실행 권한을 부여하고 그룹 및 기타사용자에게는 읽기 및 실행 권한을 부여하십시오.

 https://www.ibm.com/support/knowledgecenter/ko/ssw_ibm_i_73/rzahz/rzahzchmod.htmㅣ -> chmod 사용법.

lldb flag를 해보았는데 의미가 없었음
사용법 http://nickdesaulniers.github.io/blog/2016/01/20/debugging-x86-64-assembly-with-lldb-and-dtrace/

file flag를 해본 결과 linux, gnu를 위한 거였음. ,mac에서는 안열리는 듯 했다

I will malloc() and strcpy the flag there. take it.>> 한음이 실행

https://cpuu.postype.com/post/838572
radare2를 이용해서 reverse 하려고

r2 flag

%ebp가