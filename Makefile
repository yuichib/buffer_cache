CC = gcc

TARGET = bufcache
SRCS   = mymain.c buf_administer.c commnad.c
OBJS   = mymain.o buf_administer.o command.o


RM = rm -f


$(TARGET): $(OBJS)
	$(CC) -o $@ $^

.c.o:
	$(CC) -c $<

mymain.o: buf_administer.h command.h
mymain.h: buf_administer.h
buf_administer.o: mymain.h buf_administer.h
command.o: mymain.h buf_administer.h command.h
command.h: buf_administer.h

clean:
	$(RM) $(OBJS)

clean_target:
	$(RM) $(TARGET)

clean_all:
	$(RM) $(TARGET) $(OBJS)