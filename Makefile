CFLAGS+=-Wall -g -O3

RPR_OBJS=raspberry_pi_revision.o

TEST_BIN=test
TEST_LDFLAGS=$(LDFLAGS) -ldl
TEST_OBJS=test.o

TABLE_BIN=table
TABLE_OBJS=table.o

OBJS=$(RPR_OBJS) $(TEST_OBJS) $(TABLE_OBJS)
BIN=$(TEST_BIN) $(TABLE_BIN)

all: $(BIN)

%.o: %.c
	@rm -f $@ 
	$(CC) $(CFLAGS) $(INCLUDES) -g -c $< -o $@ -Wno-deprecated-declarations

$(TEST_BIN): $(TEST_OBJS) $(RPR_OBJS)
	$(CC) -o $@ $(TEST_OBJS) $(RPR_OBJS) $(TEST_LDFLAGS)

$(TABLE_BIN): $(TABLE_OBJS) $(RPR_OBJS)
	$(CC) -o $@ $(TABLE_OBJS) $(RPR_OBJS) $(LDFLAGS)

clean:
	@rm -f $(OBJS) $(TEST_BIN) $(TABLE_BIN)
