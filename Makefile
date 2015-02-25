CC=gcc
CFLAGS=-Wall -Wpedantic -Wextra -fPIC -std=c90
LDFLAGS=-shared
TARGET=liblinkedlist.so
SRCDIR=src
OBJDIR=build
FILES=linkedlist.c
OBJS=$(addprefix $(OBJDIR)/,$(FILES:%.c=%.o))

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h $(OBJDIR)
	$(CC) $(CFLAGS) $< -c -o $@ 

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONEY: clean

clean:
	rm -rf $(TARGET) $(OBJDIR)
