flags = -O2 -Wall -fsanitize=address
ldflags = 
builddir = build
bindir = bin
srcdir = src
includedir = include
testdir = test

.PHONY: all clean test

all: $(builddir) $(bindir) $(bindir)/birchvm

$(builddir):
	mkdir -p $(builddir)

$(bindir):
	mkdir -p $(bindir)

# Build the main executable
$(bindir)/birchvm: $(builddir)/birchvm.o $(builddir)/utils.o
	cc $(flags) $^ -o $@ $(ldflags)

$(builddir)/birchvm.o: $(srcdir)/birchvm.c $(includedir)/birchvm.h | $(builddir)
	cc $(flags) -I $(includedir) -c $< -o $@

$(builddir)/utils.o: $(srcdir)/utils.c $(includedir)/utils.h | $(builddir)
	cc $(flags) -I $(includedir) -c $< -o $@

# Build and run the tests
test: $(bindir)/test_runner
	$(bindir)/test_runner

$(bindir)/test_runner: $(builddir)/birchvm_test.o $(builddir)/birchvm.o $(builddir)/utils.o | $(bindir)
	cc $(flags) $^ -o $@ $(ldflags)

$(builddir)/birchvm_test.o: $(testdir)/birchvm_test.c $(testdir)/birchvm_test.h $(includedir)/birchvm.h | $(builddir)
	cc $(flags) -I $(includedir) -c $< -o $@

clean:
	rm -rf $(bindir) $(builddir)
