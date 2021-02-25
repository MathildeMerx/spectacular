# ----------------------------------------------------------------- #
# 
# FILENAME  : makefile
# CREATION  : 29/01/2021
# AUTHOR    : Luca Jakomulski
# PURPOSE   : makefile for the spectacular program
#
# ----------------------------------------------------------------- #

# binary name ----------------------------------------------------- #
BINNAME     = spectacular

# project directories --------------------------------------------- #
PREFIX      = ../
BINDIR      = bin/
INCDIR      = include/
LIBDIR      = lib/
SRCDIR      = src/




# specification of the compilers used ----------------------------- #
CC_NATIVE   = gcc
CC_CROSS    = arm-linux-gnueabihf-gcc

# C preprocessor options ------------------------------------------ #
#    -D_POSIX_C_SOURCE=200809L  : POSIX.1-2008 compliant
# ----------------------------------------------------------------- #
CPPFLAGS    = -D_POSIX_C_SOURCE=200809L

# compilation options --------------------------------------------- #
#    -Wall      : displays the main warning messages
#    -Wextra    : displays additional warning messages
#    -Werror    : treats warnings as errors
#    -std=c99   : compliance with ISO C99 standard
#    -pedantic  : only ISO C99 standard
#    -g         : generates debug information to be used by GDB
# ----------------------------------------------------------------- #
CFLAGS      = -Wall
CFLAGS     += -Wextra
CFLAGS     += -Werror
CFLAGS     += -std=c99
CFLAGS     += -pedantic
CFLAGS     += -g

CFLAGS_PC   = -I $(PREFIX)$(INCDIR)ncurses_pc
CFLAGS_PC  += -I $(PREFIX)$(INCDIR)ncurses_pc/ncurses

CFLAGS_PI   = -I $(PREFIX)$(INCDIR)ncurses_pi
CFLAGS_PI  += -I $(PREFIX)$(INCDIR)ncurses_pi/ncurses

# linker options -------------------------------------------------- #
#
# ----------------------------------------------------------------- #
LDFLAGS_PC  = -L $(PREFIX)$(LIBDIR)ncurses_pc
LDFLAGS_PC += -lncurses

LDFLAGS_PI  = -L $(PREFIX)$(LIBDIR)ncurses_pi
LDFLAGS_PI += -lncurses



# source files ---------------------------------------------------- #
SRC = $(wildcard *.c)

# header files ---------------------------------------------------- #
HEADERS =	$(shell ls *.h)

# object files ---------------------------------------------------- #
OBJ = $(SRC:.c=.o)

# binary files ---------------------------------------------------- #
BIN = $(BINNAME)

# variables ------------------------------------------------------- #
VERBOSE  ?= no
ifeq ($(VERBOSE),no)
	V = @
endif

CROSS   ?= no
PI      ?= no


# all targets ----------------------------------------------------- #
all: clean $(BIN) install

# object files creation ------------------------------------------- #
%.o: %.c
	$V echo "Create object files"
ifneq ($(PI),no)
	$V $(CC_NATIVE) $(CPPFLAGS) $(CFLAGS) $(CFLAGS_PI) -D'TARGET="PI"' -c $< -o $@
else
	$V $(CC_NATIVE) $(CPPFLAGS) $(CFLAGS) $(CFLAGS_PC) -c $< -o $@
endif
ifneq ($(CROSS),no)
	$V $(CC_CROSS) $(CPPFLAGS) $(CFLAGS) $(CFLAGS_PI) -D'TARGET="PI"' -c $< -o $@_cross
endif

# binary file creation -------------------------------------------- #
$(BIN): $(OBJ)
	$V echo "Create binary file"
ifneq ($(PI),no)
	$V $(CC_NATIVE) $(CPPFLAGS) $(CFLAGS) $(CFLAGS_PI) -D'TARGET="PI"' $(LDFLAGS_PI) $^ -o $@
else
	$V $(CC_NATIVE) $(CPPFLAGS) $(CFLAGS) $(CFLAGS_PC) $(LDFLAGS_PC) $^ -o $@
endif
ifneq ($(CROSS),no)
	$V $(CC_CROSS) $(CPPFLAGS) $(CFLAGS) $(CFLAGS_PI) -D'TARGET="PI"' $(LDFLAGS_PI) $(addsuffix _cross, $^) -o $@_cross
endif

# install binaries ------------------------------------------------ #
.PHONY: install
install:
	$V echo "Move binary to its directory"
	$V install -m 0755 -d $(PREFIX)$(BINDIR)
	$V install -m 0755 $(BIN) $(PREFIX)$(BINDIR)
ifneq ($(CROSS),no)
	$V install -m 0755 $(BIN)_cross $(PREFIX)$(BINDIR)
endif
	$V rm -f $(BIN) $(BIN)_cross

# intermediate file cleaning -------------------------------------- #
.PHONY: clean
clean:
	$V echo "Clean generated files"
	$V rm -f $(PREFIX)$(BINDIR)$(BIN) $(PREFIX)$(BINDIR)$(BIN)_cross
	$V rm -f $(OBJ) $(addsuffix _cross, $(OBJ))