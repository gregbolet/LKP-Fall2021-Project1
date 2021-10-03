## [M1: point 1]
#  MAKEFILE_LIST contains the name of the current Makefile being used
#  which is located as the last word in the list. We get this last
#  word using lastword and then grab the full path to the file 
#  using realpath. We lastly grab the path of which this Makefile
#  is executing from using the shell command. This makes the CUR_DIR
#  variable be the current directory from which we are executing 
#  the Makefile.
#  ...
CUR_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

## [M2: point 1]
#  In these lines we are setting the CC variable to gcc. This sets our 
#  compiler to the GNU C Compiler. 
#  We then add to the the compiler flags (CFLAGS) using the += and it
#  appends -g (include debug information), the -O2 (use optimizaiton
#  level number 2), set all warnings as errors (-Werror), and set all 
#  warnings to be printed (-Wall). 
#  We lastly append nothing to the loader flags (LDFLAGS).
#  ...
CC = gcc
CFLAGS += -g -O2 -Werror -Wall
LDFLAGS +=

## [M3: point 1]
#  Here we set the dependencies directory (DEPS_DIR) to a hidden .deps
#  directory. We also set the compiler flags for these dependencies
#  as -MD, -MF, -MP and pass all *.d files in the DEPS_DIR as flags.
#  ...
DEPS_DIR  := $(CUR_DIR)/.deps$(LIB_SUFFIX)
DEPCFLAGS = -MD -MF $(DEPS_DIR)/$*.d -MP

## [M5: point 1]
#  Here we use the wildcard keyword to gather all the files in the
#  current directory which are .c files. We store these files into the
#  SRC_FILES variable.
#  ...
SRC_FILES = $(wildcard *.c)

## [M6: point 1]
#  Here we take each element in SRC_FILES and replace the .c with 
#  the empty string. This makes the EXE_FILES variable contain all
#  the elements in SRC_FILES, but without the .c extension. These will
#  be the file names of the executables.
#  ...
EXE_FILES = $(SRC_FILES:.c=)

## [M7: point 1]
#  This is a makefile rule called 'all' and it's execution is dependent
#  on the EXE_FILES and whether or not they exist or have recently been
#  modified. If the EXE_FILES exist, nothing is built. If the EXE_FILES
#  do not exist, we then invoke the rules to build them.
#  ...
all: $(EXE_FILES)
	echo $(EXE_FILES)

## [M8: point 1]
#  This is a rule that gets applied to all files with the '.c' extension
#  in the current directory and any underlying directories (the %/%.c).
#  The $@ is the target name (a .c file), the $< is name of the first 
#  prerequisite for that file. The rule then goes and uses the C 
#  compiler (CC) and applies the compiler flags (CFLAGS) along with 
#  the dependency compiler flags (DEPCFLAGS) to compile the input .c files.
#  It should also be noted that this is a junk rule that doesn't get
#  used. You can comment it out and run the makefile and it'll still
#  generate the server and client executables. 
#  ...
%/%.c:%.c $(DEPS_DIR)
	$(CC) $(CFLAGS) $(DEPCFLAGS) -c $@ $<

## [M9: point 1]
#  This is the generic makefile clean rule which simply deletes
#  all the executable files (EXE_FILES).  
#  ...
clean:
	rm -f $(EXE_FILES)

## [M10: point 1]
#  This is a makefile convention to make phony targets which are targets
#  that do not have a filename associated with them. This is to let make
#  know that if a file named 'clean' or 'all' were to ever be created
#  within the working directory, that a call to 'make clean' will still
#  execute since it won't always be considered up-to-date. 
#  ...
.PHONY: all clean
