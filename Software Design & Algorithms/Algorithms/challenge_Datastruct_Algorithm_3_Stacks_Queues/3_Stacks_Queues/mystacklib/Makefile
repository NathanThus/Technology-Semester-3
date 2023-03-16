ASSIGNMENT=libstack.so
ASSIGNMENT_TEST=$(ASSIGNMENT)_test

UNITY_FOLDER=./Unity
INC_DIRS=-Isrc
TEST_INC_DIRS=$(INC_DIRS) -I$(UNITY_FOLDER)

SHARED_FILES=src/linked_list.c

ASSIGNMENT_FILES=$(SHARED_FILES) \
				src/mystack.c

ASSIGNMENT_TEST_FILES=$(SHARED_FILES) \
				src/mystack.c \
	           $(UNITY_FOLDER)/unity.c \
				test/mystack_test.c

HEADER_FILES=src/*.h

CC=gcc

SYMBOLS=-g  -O0 -std=c99 -Wall -Wuninitialized -Wmaybe-uninitialized -Werror -DDEBUG=0 -fPIC
TEST_SYMBOLS=$(SYMBOLS) -DTEST
LDFLAGS = -shared

.PHONY: clean test klocwork klocwork_after_makefile_change

all: $(ASSIGNMENT) 

$(ASSIGNMENT): $(ASSIGNMENT_FILES) $(HEADER_FILES)
	$(CC) $(INC_DIRS) $(LDFLAGS) $(SYMBOLS) $(ASSIGNMENT_FILES) -o $(ASSIGNMENT)

$(ASSIGNMENT_TEST): Makefile $(ASSIGNMENT_TEST_FILES) $(HEADER_FILES)
	$(CC) $(TEST_INC_DIRS) $(TEST_SYMBOLS) $(ASSIGNMENT_TEST_FILES) -o $(ASSIGNMENT_TEST)

clean:
	@rm -f $(ASSIGNMENT) $(ASSIGNMENT_TEST)
	@rm -rf kwinject.out .kwlp .kwps

test: $(ASSIGNMENT_TEST)  $(HEADER_FILES)
	  @valgrind --track-origins=yes --leak-check=full ./$(ASSIGNMENT_TEST)

klocwork:
	@kwcheck run

klocwork_after_makefile_change: clean
	@/opt/klocwork/kwenv.sh
	@kwcheck run
