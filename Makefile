# TARGET = name of executable you want to create 
TARGET=pastry
# If you are using gcc to compile then make CC=gcc
CC=g++
# list of headerfiles space seperated
HEADERS=header.h
#flags 
FLAGS=-Wall -pthread
# you can add flags like this: FLAGS += -std=c++0x

#list of object files that you want to create[space seperated].
OBJS=pastry.o utility.o commands.o md5.o

$(TARGET) : $(OBJS) $(HEADERS)
	$(CC) $(FLAGS) -o $@ $^
%.o : %.c 
	$(CC) $(FLAGS) -c -o $@ $^

# clean the build, removes all .o files and executable named in TARGET. To execute run make clean from command line
clean : 
	rm *.o $(TARGET)
