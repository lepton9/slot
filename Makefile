SRC := ./src
BIN := ./bin
OBJS := ./objs
INC := -I ./include
FLAGS := -c $(INC)
CC := gcc

OBJ := ./objs/Slot.o ./objs/Renderer.o ./objs/LineChecker.o ./objs/Player.o ./objs/AliasTable.o

slot: $(OBJ)
	$(CC) $^ -o $(BIN)/$@ -lm

$(OBJS)/%.o: $(SRC)/%.c
	$(CC) $(FLAGS) $< -o $@

debug:
	$(CC) $(INC) $(SRC)/*.c -pthread -g -o $(BIN)/db
	gdb -tui $(BIN)/db

clean:
	rm -rf $(OBJS)/*.o $(BIN)/*

run:
	$(BIN)/slot
