PROG=test-lookup
CFLAGS=-O0 -Wall -Wextra

.PHONY: clean

$(PROG):

clean:
	@$(RM) $(PROG).exe $(PROG) *~
