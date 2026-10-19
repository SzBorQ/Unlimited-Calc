CC      ?= gcc
CFLAGS  ?= -Wall -Wextra -std=c11 -Iinclude
LDFLAGS ?=

SRCDIR  := src
INCDIR  := include
OBJDIR  := build
BINDIR  := bin

TARGET  := $(BINDIR)/unlimited_calc

SRCS    := $(wildcard $(SRCDIR)/*.c)
OBJS    := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR) $(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(BINDIR) out*.txt
