import socket

c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
c.connect(("172.104.177.86", 8080))

#Compose a string from user input
request=input("Enter a message to send to the server: ")
#send user input string to the server
c.send(request.encode())
#print incoming input string to the server
response = c.recv(1024).decode()
print(f"Server reply: {response}")

c.close()
