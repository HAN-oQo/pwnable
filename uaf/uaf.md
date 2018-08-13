#uaf walkthrough

solution을 보니 gdb helper로 사용되는 peda를 사용했다. 
lldb에도 그런것이 있다면, mac에도 그런 것이 있다면 설치를 하여 사용하도록 하자. 

Cpp파일이다. 

gdb에

>>source/usr/share/peda/peda.py

이것으로 peda를 실행.

##human() class의 virtual function
1. give_shell()
2. introduce()

두가지가 있다. 

Man(), Woman() class는 human()을 상속받는다. 

Main()에서는 human class pointer m, w를 사용한다. 이때 
Human* m = new Man()
Human* w = new Woman()

new를 통해서 이것은 heap에 동적할당해서 가리키도 있다. 

c++에서는 파생클래스는 기본클레스를 오버라이드하더라도 기본클래스와 다른 자료형으로 인식된다. 
하지만 위와 같이 기본클래스의 포인터나 참조객체를 통해서 파생클래스의 객체를 참조랄 수 있게 하면서, 기본클래스, 파생클래스 랠체를 모두 처리할 수 있기 때문에 위와같은 표현이 가능하다. 

##main()의 3가지 메뉴
1. use
	Human class pointer m,w을 통해서 Man, Woman객체의 오버라이딩 된 introduce()를 실행한다. 아마 이름, 나이를 소개를 하고 (베이스 클래스), 그리고 나서 im nice guy, cute girl을 호출할 것이다. 
2. after
	argv[1]의 크기만큼 메모리를 동적할당 받고, argv[2]의 파일명에 해당하는 데이터를 읽어서 동적할당받은 메모리에 저장을 한다. 
3. Free 
	m, w객체를 차례대로 delete해준다. 


##virtual function 동작원리
같은 클래스의 객체끼리 virtual function을 vtable에 모두 모아놓고 사용을 한다. 
