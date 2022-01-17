from socket import *

host = "127.0.0.1"    # localhost
port = 7777
livesRemaining = 3    # number of lives
  
# create client's socket, TCP
clientSocket = socket(AF_INET, SOCK_STREAM)

# initiate TCP connection
clientSocket.connect((host, port))
print("Connected to:", host, "on port:", port, "\n")

# some instructions
print("Welcome to hangman! You will enter a word and your opponent will guess letters to figure out what your word is.")
print("Letter case (upper vs. lower) will be ignored in this game.")
print("Good luck!")

opponentProgress = ""
hangmanWord = ""
firstPass = True
while 1:
  # print instructions if first pass
  if firstPass:
    # get hangman word to guess
    print("Type /q to quit")
    hangmanWord = input("Enter the word for your opponent to guess...\n").lower()

    # make 'gameboard'
    for x in range(0, len(hangmanWord)):
      opponentProgress += "_"
    firstPass = False

  # to keep some of the program required "chat" functionality, require client to enter an input
  # client can quit can or continue
  toSend = input("Type /q to quit or press enter to continue the game\n")
  if toSend == "/q":      # end chat
    clientSocket.send(toSend.encode())
    break
  else:                   # continue game, send 'gameboard' and remaining lives
    toSend = opponentProgress + "   Guesses remaining: " + str(livesRemaining)
    clientSocket.send(toSend.encode())

  # receive letter guess from server
  message = clientSocket.recv(2048)
  messageDecoded = message.decode().lower()
  if messageDecoded == "/q":      # end chat if they quit the game
    break
  else:                           # see if letter guess is correct, update variables of game                         
    print("Opponent guessed:", messageDecoded)
    lifeLost = True
    solved = True

    # Used below reference to alter string
    # reference: https://stackoverflow.com/questions/10631473/str-object-does-not-support-item-assignment/18006499
    listTemp = list(opponentProgress)
    # iterate through hangmanWord, update opponentProgress with letter if it is correct and set lifeLost to false
    # if opponentProgress still has missing letters "_", set solved to false
    for x in range(0, len(hangmanWord)):
      if hangmanWord[x] == messageDecoded:
        if listTemp[x] != messageDecoded:
          listTemp[x] = messageDecoded
          opponentProgress = ''.join(listTemp)
          lifeLost = False
      if opponentProgress[x] == "_":
        solved = False

    # decrement lives if necessary
    if lifeLost:
      livesRemaining -= 1

    # print game progress
    print("Opponents progress:", opponentProgress)
    print("Lives remaining:", livesRemaining)

    # if no lives remaining, client wins, send "0", which server receives and handles as a loss, end program
    if livesRemaining == 0:
      toSend = "0" + opponentProgress + "   Guesses remaining: " + str(livesRemaining)
      clientSocket.send(toSend.encode())
      print("YOU WON!")
      break

    # if word is solved, opponent wins, send "1" and complete word, which server receives and handles as a win
    # end program
    if solved:
      toSend = "1" + opponentProgress
      clientSocket.send(toSend.encode())
      print("Opponent wins!")
      break

clientSocket.close()