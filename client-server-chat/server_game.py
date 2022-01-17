from socket import *

host = "127.0.0.1"    # localhost
port = 7777
guessed = ""
  
# create TCP socket and associate port with socket
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
serverSocket.bind((host, port))

# listen for TCP connection requests
serverSocket.listen(1)
print("Server listening on: localhost on port:", port)

# create new socket
connectionSocket, addr = serverSocket.accept()
print("Connected by ('", host, "', ", port, ")\n")

# some instructions
print("Welcome to hangman! Your opponent will enter a word and you will guess letters to figure out what the word is.")
print("Letter case (upper vs. lower) will be ignored in this game.")
print("Good luck!")

firstPass = True
# continue to receive/send messages until '/q' send 
while 1:
  if firstPass:
    print("Waiting for client to enter a word to guess...")

  # get message from client
  message = connectionSocket.recv(2048)
  messageDecoded = message.decode()
  if messageDecoded == "/q":      # client ending communication, close socket
    break
  elif messageDecoded[0] == "0":     # no lives remaining, end of game, close socket
    print("\n", messageDecoded[1:])
    print("YOU RAN OUT OF LIVES! GAME OVER!")
    break
  elif messageDecoded[0] == "1":  # winner!, print word, end of game, close socket 
    print("YOU WON!")
    print("The word was:", messageDecoded[1:])
    break
  else:                           # game still going, print message sent
    print("\n", messageDecoded)

  # print instructions if first time through
  if firstPass:
    print("Type /q to quit")
    firstPass = False

  # get letter guess and send to client
  print("Enter a letter to guess...")
  toSend = input(">")
  connectionSocket.send(toSend.encode())
  if toSend == "/q":
    break

connectionSocket.close()