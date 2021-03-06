srcdir = src
hdir = $(srcdir)/heads
objdir = obj
bindir = bin
docdir = doc
utils = .gitignore makefile

cc = g++
dbg = -g
opt =
cflags = -c $(dbg) $(opt) -Wall --std=c++11
lflags = -lgsl -lgslcblas -lm -lcurses

objects = $(subst $(srcdir),$(objdir),\
$(patsubst %.cc,%.o,$(wildcard $(srcdir)/*.cc)))
target = $(bindir)/percolate

default : dirs $(target)

# Compilation to object code
$(objdir)/%.o : $(srcdir)/%.cc $(hdir)/%.h
	$(cc) $(cflags) -o $@ $<

# Compile binary
$(bindir)/percolate : $(objects)
	$(cc) $(lflags) -o $@ $^ $(lflags)

# Utilities
dirs :
	@ if [ ! -d $(objdir) ]; then mkdir $(objdir); fi
	@ if [ ! -d $(bindir) ]; then mkdir $(bindir); fi

clean :
	@ rm -f obj/* bin/*

tar :
	@ tar czf percolation.tar.gz $(srcdir) $(docdir) $(utils)
