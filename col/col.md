col, col.c, flag가 들어있다. 

col.c를 확인
hashcode와 checkpassword의 return 값이 일치하면 되는 문제

checkpassword를 확인해보니 string을 인수로 받아서 인트형 포인터로 캐스팅을 한다. 그리고 나서 그것을 모두 더하였을 때의 값을 return 한다.

우선 해쉬코드를 분석

>>python
>>0x21DD09EC //hashcode. python에서는 0x로 hex를 표현하면 이것을 십진법으로 계산을 하여준다. 그리고 /x로 hex를 표현한다면 그것은 string, 문자로 바꾸어준다.

코드를 확인해보니 4바이트를 5번 받는 것을 확인할수있다.

>> hex(0x21DD09EC/5)
>> hex(0x21DD09EC/5 +4)


>>./col `python -c 'print"\xc8\xce\xc5\x06"*4+"\xcc\xce\xc5\x06"'`    //python -c 옵션은 커맨드 옵션. 해_결