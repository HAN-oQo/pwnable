#leg walkthrough

intel 기반 아키텍쳐의 어셈블리가 아닌 arm architecture의 assembly

main함수는 보니
key1,2,3()의 리턴값을 확인해서 다 더한 값을 입력을 하면된다. 


ssh leg@pwnable.kr -p2222을 했을 때 이상한 것이 떠도 그냥 하자. 

wget으로 leg.asm, leg.c를 다운 받는다. 


>>cat leg.c
>>cat leg.asm 

	0x00008d68 <+44>:	bl	0x8cd4 <key1>
   0x00008d6c <+48>:	mov	r4, r0
   0x00008d70 <+52>:	bl	0x8cf0 <key2>
   0x00008d74 <+56>:	mov	r3, r0
   0x00008d78 <+60>:	add	r4, r4, r3
   0x00008d7c <+64>:	bl	0x8d20 <key3>
   0x00008d80 <+68>:	mov	r3, r0
   0x00008d84 <+72>:	add	r2, r4, r3
   0x00008d88 <+76>:	ldr	r3, [r11, #-16]
   0x00008d8c <+80>:	cmp	r2, r3

r0가 return 값이라는 것을 알 수 있다. 


##key1()
   0x00008cdc <+8>:	mov	r3, pc
   0x00008ce0 <+12>:	mov	r0, r3


###pc란?
reference: http://mandu-mandu.tistory.com/77
cpu가 명령어 하나를 수행할 때 fetch > decode > execute 의 과정을 거친다고 합니다.

2개의 opcode를 실행하려면 fetch > decode > execute > fetch > decode > execute 총 6번의 작업이 필요하죠.

하지만 pipe line라는 것을 이용해서


1->    2->    3->      4

fetch->decode->execute
	   fetch ->decode ->execute
			   fetch  ->decode ->execute

 
이렇게 병렬적으로 단계를 수행하면 2개의 opcode를 실행할때 6번의 작업이 필요했던것을 4번으로 줄일 수 있습니다.

직렬보다 효율적이죠.



pc는 fetch할 주소를 담고 있습니다.

현재 명령어가 execute단계라면, 다음 명령어는 decode단계, 그 다음 명령어는 fetch 단계이겠죠.

그래서 pc는 다다음번째 명령어의 주소를 담고 있게 됩니다.

그렇다면 key1의 리턴값은 0x00008ce4이다.


##key2()
   0x00008d04 <+20>:	mov	r3, pc
   0x00008d06 <+22>:	adds	r3, #4
   0x00008d08 <+24>:	push	{r3}3
   0x00008d0a <+26>:	pop	{pc}
   0x00008d0c <+28>:	pop	{r6}		; (ldr r6, [sp], #4)
   0x00008d10 <+32>:	mov	r0, r3

pc = 0x00008d08
r3 = 0x00008d08+4 = 0x00008d0c

key2의 리턴값은 0x00008d0c


##key3()
   0x00008d28 <+8>:	mov	r3, lr
   0x00008d2c <+12>:	mov	r0, r3

###lr?
함수 호출 후에 다시 되돌아가 실행할 주소

그렇다면 
   0x00008d7c <+64>:	bl	0x8d20 <key3>
   0x00008d80 <+68>:	mov	r3, r0

lr = 0x00008d80
r0 = 0x00008d80


key3의 리턴 값은 0x00008d80이다. 


>>ssh leg@pwnable.kr -p2222
>>./leg 
>>key1+key2+key3의 십진법으로 나타낸수를 입력

flag를 얻을 수 있다.
