SRC := ./src
BIN := ./bin
OBJS := ./objs
INC := -I ./include -I ../testLibC
FLAGS := -c $(INC)
LINK := -lm -lncurses
CC := gcc

TESTS := ./tests
TEST_TARGETS := AliasTable_test LineChecker_test Slot_test Player_test

OBJ := ./objs/Slot.o ./objs/Renderer.o ./objs/LineChecker.o ./objs/Player.o ./objs/AliasTable.o

main: $(OBJ)
	$(CC) $^ $(SRC)/main.c -o $(BIN)/$@ $(LINK)

$(OBJS)/%.o: $(SRC)/%.c
	$(CC) $(FLAGS) $< -o $@

debug:
	$(CC) $(INC) $(SRC)/*.c -pthread -g -o $(BIN)/db $(LINK)
	gdb -tui $(BIN)/db


#Testing
test: all_tests
	@for target in $(TEST_TARGETS); do \
		$(TESTS)/bin/$$target; \
	done

all_tests: $(addprefix $(TESTS)/bin/, $(TEST_TARGETS))

#$(TESTS)/bin/%_test: $(TESTS)/testLib.c $(TESTS)/%_test.c $(SRC)/%.c
$(TESTS)/bin/%_test: ../testLibC/utestC.c $(TESTS)/%_test.c $(OBJ)
	$(CC) $(INC) $^ $(LINK) -g -o $@


clean:
	rm -rf $(OBJS)/*.o $(BIN)/*
	rm -rf $(TESTS)/bin/*

run:
	$(BIN)/main

