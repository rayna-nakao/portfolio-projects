# Client-Server Chat
### Oreogon State University, CS 372, Portfolio Project
####Functions of the chat from assignment:<br />
This chat client-server is fairly simple in design. The server doesn’t handle multiple clients, and there is only one socket connection made. You will reuse this socket for the life of the program.

Specifications:<br />
Server
1. The server creates a socket and binds to ‘localhost’ and port xxxx
2. The server then listens for a connection
3. When connected, the server calls recv to receive data
4. The server prints the data, then prompts for a reply
5. If the reply is /q, the server quits
6. Otherwise, the server sends the reply
7. Back to step 3
8. Sockets are closed (can use with in python3)

Client
1. The client creates a socket and connects to ‘localhost’ and port xxxx
2. When connected, the client prompts for a message to send
3. If the message is /q, the client quits
4. Otherwise, the client sends the message
5. The client calls recv to receive data
6. The client prints the data
7. Back to step 2
8. Sockets are closed (can use with in python3)
