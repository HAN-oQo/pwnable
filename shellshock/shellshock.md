#bash shellshock

##bash 환경변수 세팅

x라는 변수에 3을 대입하고 싶으면 직관적으로 x=3을 한 후 $x로 변수를 확인하면 된다. 변수가 제대로 setting 되었는지 확인하기 위해 env 명령어를 사용해본다. 확인해보면 x가 setting되어있지 않는 것을 확인할 수 있을 것이다. 아직까지 완벽하게 setting이 된 것이 아니다. 완전히 setting 하기 위해서 export 함수를 통해 해당 변수를 export 해주어야 한다.



출처: http://operatingsystems.tistory.com/entry/Shellshock-CVE20146271 [Maybe]


>> export x=3
>> env

를하면 x=3이 환경변수로 들어가 있음을 확인할 수 있다. 

환경변수로 함수를 선언하는 것도 가능한다. 

>>foo() { echo hello; }
*띄어쓰기에 주의하자..

이방법을 환경변수 등록 가능
>>foo 
>>> hello

>> export -f foo
함수도 역기 export를 해줘야 env에서 정의를 확인이 가능 

>> export -nf foo
이렇게 해서 env에서 함수의 정의를 삭제 가능하다

>> export foo='(){.... }; .....' 이렇게 한다면
물론 문자열이지만 그것이 subshell에서는 함수로 저장이되는 취약점을 이용을 한다. 
그것이 초기화되는데 그것을 함수로 env로 초기화를 한다. 그리고 그 뒤에 것은 명령어로 인식을 한다. 


출처: http://operatingsystems.tistory.com/entry/Shellshock-CVE20146271

### Bash Process 동작 순서
a. Bash 실행
b. Bash 환경변수 초기화
c. Bash shell prompt 출력
d. 명령어 기다림
e. (명령어를 수행할 경우) 명령어를 문자열로 저장하여 해당 문자열 parsing 수행
f. parsing된 구조체를 이용하여 명령어 수행

출처: http://operatingsystems.tistory.com/entry/Shellshock-CVE20146271 [Maybe]


초기화 -> 함수 선언을 초기와. 원래의 환경변수를 가져오는 것


/bin/: 기본명령어가 모여있는 디렉토리
새로생긴 bash에는 cat이라는 


##아직 이해하지 못한 부분.  
export foo6='() { echo hi; };/bin/ls;'
export foo='() { echo hi; }; echo bbb;' // shellshock test
*'='양여ㅠ은 띄어쓰기 ㄴㄴ () {}사이는 띄어쓰기 {} 안의 값도 띄어쓰기 ㄲ
env로 등록되었는지 확인을 해보자


** 권한이 중요 **

./shellshock -> 됨 
권한 때문에 foo=() { echo hi; }; /bin/ls;이렇게 그대로 들어간다
./bash.      -> 됨
foo=() {  echo hi }이렇게 들어간다. 

export foo6='() { echo hi; };/bin/ls;'
./bash       -> 됨
./shellshock -> 안됨

