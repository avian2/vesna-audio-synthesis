UNITY ?= ../../Unity

INCLUDES += -I$(UNITY)/src -I..

CFLAGS += -g -Wall $(INCLUDES)

UNITY_OBJ = $(UNITY)/src/unity.o

TESTS =	test_dds_run

DEPS =	$(TESTS:test_%_run=%.d) \
	$(TESTS:test_%_run=test_%.d) \
	$(TESTS:test_%_run=test_%_runner.d)

all: test

test: $(TESTS)

test_dds_runner: test_dds_runner.o test_dds.o dds.o sin.o $(UNITY_OBJ)

test_%_runner: test_%_runner.o test_%.o %.o $(UNITY_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

vesna-fm-synthesis-host: main-unix.o smile-notes.o dds.o sin.o
	$(CC) $(CFLAGS) -o $@ $^

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%_run: %_runner
	./$^

%_runner.c: %.c
	ruby $(UNITY)/auto/generate_test_runner.rb $^ $@

clean:
	rm -f *.o *.d
	rm -f *_runner *_runner.c

include $(DEPS)

vpath %.c ..

.PHONY: clean test
