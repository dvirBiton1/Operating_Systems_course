# Operating-Systems-Ex5

Dvir Biton

Ohad Shirazi

how to run:

1.make clean (to be sure there is no foo.txt file open)

2. make

3.run server by the command ./server

4.open another terminal(you need one terminal for the server and another one for each client)

5.run each client with the command ./client 127.0.0.1

6. now you can send PUSH,TOP,POP from each client terminal to control your shared stack

## Remarks:

* You can see your TOP in each client terminal

* We have sleep command in push method in order to check the synchronized ( of fcntl )
  (now it's in comment, you can delete the "\\" in oreder to check our code)

## Testing:

* run the server by the command: ./server
* run the test file by the command: ./test 127.0.0.1
* for each client open a new terminal and the run the command above again

(Credit: https://www.informit.com/articles/article.aspx?p=23618&seqNum=4).