# The main (project top) file without .c
#TARGET_STATIC = buff-lib.so
TARGET_DYNAMIC = bufflib.so
# buff_lib sourses
SRC += fifo.c errors.c

CC = gcc
CFLAGS = -std=gnu18 -O2 -Wall -Wextra -Wpedantic -pthread -fPIC

BUILDDIR = ./build
SRCDIR = ./src
INCDIR = ./inc

.PHONY: all clean tidy

all: $(BUILDDIR) $(TARGET_DYNAMIC)
	rm -f socket
	rm -f *.txt

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(TARGET_DYNAMIC): $(addprefix $(BUILDDIR)/,$(SRC:.c=.o))
	$(CC) $(CFLAGS) -shared $^ -o $@

$(BUILDDIR):
	mkdir -p $@

clean:
	rm -rf $(BUILDDIR)

tidy: clean
	rm -f $(CLI)
	rm -f $(SNIFFER)


lint: lint-all

lint-all: | lint-clang-tidy lint-splint lint-oclint lint-cppcheck
_oclint := oclint
_splint := splint +checks $(addprefix -I ,$(INCDIR))
_cppcheck := cppcheck -q -j$$(($$(nproc)+1)) $(addprefix -I,$(INCDIR)) \
	--platform=unix64 \
	--enable=warning,style,performance,portability,information \
	--std=c11 --language=c --verbose --inconclusive
_clang-tidy := clang-tidy --quiet --checks='*'

lint-oclint: $(addprefix $(SRCDIR)/,$(SRC))
	@echo -e "\e[1m\e[92m>>> OCLint report\e[38;5;130m"
	$(_oclint) 2>/dev/null $^ | head -n -2 | tail -n +1
	@echo -en "\e[0m"

lint-clang-tidy: $(addprefix $(SRCDIR)/,$(SRC))
	@echo -e "\e[1m\e[92m>>> Clang-Tidy report\e[38;5;130m"
	$(_clang-tidy) $^ -- $(addprefix -I,$(INCDIR)) $(CFLAGS) 2>/dev/null | cat
	@echo -en "\e[0m"

lint-splint: $(addprefix $(SRCDIR)/,$(SRC))
	@echo -e "\e[1m\e[92m>>> SPLint report\e[38;5;130m"
	$(_splint) 2>&1 $^ | tail -n +2
	@echo -en "\e[0m"

lint-cppcheck: $(addprefix $(SRCDIR)/,$(SRC))
	@echo -e "\e[1m\e[92m>>> CPPCheck report\e[38;5;130m"
	$(_cppcheck) $^ && echo
	@echo -en "\e[0m"
