#random walkthrough

>>cat random.c

unsingned int random = rand();

rand()자체가 취약점. 
rand()를 통해서 생성되는 난수는 난수이긴한데 값은 고정된 값이기 때문에 다시 난수가 생성이 되어도 고정된 값이 생성이된다. 
그러니까 srand()가 아니므로 rand()의 return값만 찾아주면 되는 문제인 듯하다. 

