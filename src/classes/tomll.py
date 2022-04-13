# didnt think I would be making this in a seperate file
import toml
def readtoml():
  #jhost,jport,maxjava,maxrock,motd,bhost,bport = toml.readtoml()
  k=toml.load('config.toml')
  sk=k['java']
  jhost = sk['host']
  jport = sk['port']
  sk=k['bedrock']
  bhost = sk['host']
  bport = sk['port']
  sk=k['config']
  maxrock=sk['maxrock']
  maxjava=sk['maxjava']
  motd=sk['motd']
  return jhost,jport,maxjava,maxrock,motd,bhost,bport
def maketoml():
  f=open('config.toml','x')
  f.write("""
[java]
host="0.0.0.0"
port=25565
[bedrock]
host="0.0.0.0"
port=19133
[config]
maxrock=12
maxjava=12
motd="Hello!"
""")
readtoml()
