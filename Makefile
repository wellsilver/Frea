cc := gcc
libs += -lpthread
out := Frea.exe

$(out):
  $(cc) src/main.c $(libs) -o $(out)
  

clean:
  del $(out)
