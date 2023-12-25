SRC := ./src
BIN := ./bin
OBJS := ./objs
INC := -I ./include
FLAGS := -c $(INC)
CC := gcc

TESTS := ./tests
TEST_TARGETS := AliasTable_test LineChecker_test

OBJ := ./objs/Slot.o ./objs/Renderer.o ./objs/LineChecker.o ./objs/Player.o ./objs/AliasTable.o

slot: $(OBJ)
	$(CC) $^ -o $(BIN)/$@ -lm

$(OBJS)/%.o: $(SRC)/%.c
	$(CC) $(FLAGS) $< -o $@

debug:
	$(CC) $(INC) $(SRC)/*.c -pthread -g -o $(BIN)/db
	gdb -tui $(BIN)/db


#Testing
test: all_tests
	@for target in $(TEST_TARGETS); do \
		$(TESTS)/bin/$$target; \
	done

all_tests: $(addprefix $(TESTS)/bin/, $(TEST_TARGETS))

$(TESTS)/bin/%_test: $(TESTS)/testLib.c $(TESTS)/%_test.c $(SRC)/%.c
	$(CC) $(INC) $^ -lm -g -o $@


#TEST_TARGETS = AliasTable_test LineChecker_test
#test: $(TEST_TARGETS)
#%: $(TESTS)/bin/%
	#./$<
#$(TESTS)/bin/%_test: $(TESTS)/testLib.c $(TESTS)/%_test.c $(SRC)/%.c
	#$(CC) $(INC) $^ -lm -g -o $@
#.PHONY: test
#test: $(addprefix $(TESTS)/bin/, $(TEST_TARGETS))


clean:
	rm -rf $(OBJS)/*.o $(BIN)/*
	rm -rf $(TESTS)/bin/*

run:
	$(BIN)/slot

