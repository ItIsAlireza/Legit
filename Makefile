CC      = gcc
CFLAGS  = -Wall -Wextra -Iinclude -DLEGIT_DIR_NAME=\".legit\"

OBJS = src/main.o src/commands.o src/util.o src/repo.o cmds/cmd_init.o

# ensure build dir exists
build:
	mkdir -p build

# final binary (pick one name; here I use build/legit)
build/legit: $(OBJS) | build
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# generic rule: compile .c into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) build/legit build/pg



