# Operating-Systems-Ex4

Dvir Biton 

Ohad Shirazi 

how to run:

1.make

2.open other terminal(you need one terminal to server and other for the clients)

3.run server by with command ./server

4.run client with command ./client 127.0.0.1

5.you can send PUSH,TOP,POP from the client terminal

## Remarks:

* PUSH and POP you see in server terminal.(for my debugging).

* TOP you see in the client terminal.

* the sleep in func push is just for the first time(it's for the test), do not delete that.

* for the test(i have problem with the linker so instead of use doctest i craete test.cpp and send eror if the test fail).

* in the push func i put sleep intentionally to see if relly is work in parallel and synchronize.

* my test cheack if really it's work in parallel and synchronize(the main test is to see if the sleep in the func push in fisrt time dont hart the synchronize), to see that more clearly i suggest to use teminal and write more tests.

* if you want for the 'moss' all the code you can copy from Ex4.h to server(i write them there for the order).

* we did the bonus to see that you need remove from files Ex4.hpp, client.cpp, server.cpp the note (remove '//' in the place write "for the bonus...until here").

(I got help in malloc and free with stackoverflow).






