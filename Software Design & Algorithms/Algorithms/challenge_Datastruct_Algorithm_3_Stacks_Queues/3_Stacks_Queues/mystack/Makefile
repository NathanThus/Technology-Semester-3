ASSIGNMENT=mystack

ASSIGNMENT_FILES=product/main.c
HEADER_FILES=product/*.h

CC=gcc

INC= -I../mystacklib/src/
LIBS= -L../mystacklib/

SYMBOLS=-g  -O0 -std=c99 -Wall -Werror $(INC)
LDLIBS= -lstack

.PHONY: clean

all: $(ASSIGNMENT)

$(ASSIGNMENT): $(ASSIGNMENT_FILES) $(HEADER_FILES)
	$(CC) $(ASSIGNMENT_FILES) $(SYMBOLS) $(LIBS) $(LDLIBS) -o $(ASSIGNMENT)

clean:
	@rm -f $(ASSIGNMENT)
