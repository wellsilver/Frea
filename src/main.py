from classes import tomll
from classes import player
# read toml

jhost,jport,bhost,bport,maxjava,maxrock,motd=tomll.readtoml()
  

def java():
  global jhost,jport
  # ^ vars
  global pjmanage
  # ^ func
  s=socket.socket()
  s.bind((jhost,jport))
  s.listen(5)
  while True:
    a,b = s.accept()
    # a=socket b=address (ip,port)
    pjmanage(a)

def bedrock():
  global bhost,bport
  s=socket.socket()
  s.bind(("0.0.0.0",bport))
  s.listen(5)
  
  while True:
    a,b = s.accept()
    a.close()

import threading
import socket
pjmanage=player.java.manage
threading.Thread(target=java).start()
threading.Thread(target=bedrock).start()
