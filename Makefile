MAKEFLAGS += --no-builtin-rules

# Build options can either be changed by modifying the makefile, or by building with 'make SETTING=value'

# if WERROR is 1, compile source files with -Werror
WERROR ?= 0
# 
SAN_DEBUG ?= 0

CFLAGS ?=
CXXFLAGS ?=
CPPFLAGS ?=
COMMON_DEFINES ?=

# Set prefix to mips binutils binaries (mips-linux-gnu-ld => 'mips-linux-gnu-') - Change at your own risk!
# In nearly all cases, not having 'mips-linux-gnu-*' binaries on the PATH is indicative of missing dependencies
MIPS_BINUTILS_PREFIX ?= mips-linux-gnu-

MAKE = make
CPPFLAGS += -P

ifeq ($(OS),Windows_NT)
    DETECTED_OS=windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        DETECTED_OS=linux
    endif
    ifeq ($(UNAME_S),Darwin)
        DETECTED_OS=macos
        MAKE=gmake
        CPPFLAGS += -xc++
    endif
endif

N_THREADS ?= $(shell nproc)

#### Tools ####
ifneq ($(shell type $(MIPS_BINUTILS_PREFIX)ld >/dev/null 2>/dev/null; echo $$?), 0)
    $(error Please install or build $(MIPS_BINUTILS_PREFIX))
endif

CC         := $(MIPS_BINUTILS_PREFIX)gcc
CXX        := $(MIPS_BINUTILS_PREFIX)g++
AS         := $(MIPS_BINUTILS_PREFIX)as
LD         := $(MIPS_BINUTILS_PREFIX)ld
OBJCOPY    := $(MIPS_BINUTILS_PREFIX)objcopy
OBJDUMP    := $(MIPS_BINUTILS_PREFIX)objdump
CPP        := $(MIPS_BINUTILS_PREFIX)cpp
AR         := $(MIPS_BINUTILS_PREFIX)ar

## C / C++ flags

INC        := -Iinclude

# COMMON_WARNINGS := -Wall -Wextra -Wpedantic -Wshadow -Wvla -Werror=implicit-function-declaration -Wno-format-security -Wno-unknown-pragmas -Wno-unused-parameter -Wno-unused-variable
COMMON_WARNINGS := -Wall -Wextra -Wpedantic -Wshadow -Wvla -Werror=implicit-function-declaration
ifneq ($(WERROR), 0)
    COMMON_WARNINGS += -Werror
endif
C_WARNINGS      := $(COMMON_WARNINGS) -Wno-int-conversion
CXX_WARNINGS    := $(COMMON_WARNINGS)

OPTFLAGS       := -Os -g -fno-zero-initialized-in-bss -fno-toplevel-reorder -fno-merge-constants -fno-lto
MIPS_VERSION   := -mips3
ARCH_FLAGS     := -march=vr4300 -mfix4300 -mabi=32

COMMON_DEFINES +=
ifneq ($(SAN_DEBUG), 0)
    COMMON_DEFINES += -DSANITIZER_DEBUG=1
endif

COMMON_FLAGS   := -G 0 $(INC) $(COMMON_DEFINES) -funsigned-char -ffreestanding -fno-common -fexec-charset=euc-jp -mno-abicalls -mdivide-breaks -mno-explicit-relocs -mno-split-addresses
CFLAGS         += $(COMMON_FLAGS) -nostdinc -std=c11 $(C_WARNINGS)
CXXFLAGS       += $(COMMON_FLAGS) -nostdinc++ -std=c++17 -fno-exceptions -fno-rtti $(CXX_WARNINGS)


## Assembly flags

ASFLAGS        := -march=vr4300 -32 -Iinclude


#### Files ####

SRC_DIRS_N64_SAN_COMMON      := $(shell find src/n64_sanitizer_common -type d)

SRC_DIRS_N64_ASAN            := $(shell find src/n64_asan -type d) $(SRC_DIRS_N64_SAN_COMMON)
SRC_DIRS_N64_UBSAN           := $(shell find src/n64_ubsan -type d) $(SRC_DIRS_N64_SAN_COMMON)
SRC_DIRS_N64_WRAPPER         := $(shell find src/n64_wrapper -type d)

SRC_DIRS_ALL                 := $(SRC_DIRS_N64_ASAN) $(SRC_DIRS_N64_UBSAN) $(SRC_DIRS_N64_WRAPPER)


ASM_DIRS_N64_SAN_COMMON      := $(shell find asm/n64_sanitizer_common -type d)

# ASM_DIRS_N64_ASAN            := $(shell find asm/n64_asan -type d) $(ASM_DIRS_N64_SAN_COMMON)
ASM_DIRS_N64_ASAN            := $(ASM_DIRS_N64_SAN_COMMON)
# ASM_DIRS_N64_UBSAN           := $(shell find asm/n64_ubsan -type d) $(ASM_DIRS_N64_SAN_COMMON)
ASM_DIRS_N64_UBSAN           := $(ASM_DIRS_N64_SAN_COMMON)
# ASM_DIRS_N64_WRAPPER         := $(shell find asm/n64_wrapper -type d)
ASM_DIRS_N64_WRAPPER         := 

ASM_DIRS_ALL                 := $(ASM_DIRS_N64_ASAN) $(ASM_DIRS_N64_UBSAN) $(ASM_DIRS_N64_WRAPPER)


# source files
C_FILES_N64_ASAN             := $(foreach dir,$(SRC_DIRS_N64_ASAN),$(wildcard $(dir)/*.c))
CXX_FILES_N64_ASAN           := $(foreach dir,$(SRC_DIRS_N64_ASAN),$(wildcard $(dir)/*.cpp))
S_FILES_N64_ASAN             := $(foreach dir,$(ASM_DIRS_N64_ASAN),$(wildcard $(dir)/*.s))
O_FILES_N64_ASAN             := $(foreach f,$(S_FILES_N64_ASAN:.s=.o),build/$f) \
                                $(foreach f,$(C_FILES_N64_ASAN:.c=.o),build/$f) \
                                $(foreach f,$(CXX_FILES_N64_ASAN:.cpp=.o),build/$f)

C_FILES_N64_UBSAN            := $(foreach dir,$(SRC_DIRS_N64_UBSAN),$(wildcard $(dir)/*.c))
CXX_FILES_N64_UBSAN          := $(foreach dir,$(SRC_DIRS_N64_UBSAN),$(wildcard $(dir)/*.cpp))
S_FILES_N64_UBSAN            := $(foreach dir,$(ASM_DIRS_N64_UBSAN),$(wildcard $(dir)/*.s))
O_FILES_N64_UBSAN            := $(foreach f,$(S_FILES_N64_UBSAN:.s=.o),build/$f) \
                                $(foreach f,$(C_FILES_N64_UBSAN:.c=.o),build/$f) \
                                $(foreach f,$(CXX_FILES_N64_UBSAN:.cpp=.o),build/$f)

C_FILES_N64_WRAPPER          := $(foreach dir,$(SRC_DIRS_N64_WRAPPER),$(wildcard $(dir)/*.c))
CXX_FILES_N64_WRAPPER        := $(foreach dir,$(SRC_DIRS_N64_WRAPPER),$(wildcard $(dir)/*.cpp))
S_FILES_N64_WRAPPER          := $(foreach dir,$(ASM_DIRS_N64_WRAPPER),$(wildcard $(dir)/*.s))
O_FILES_N64_WRAPPER          := $(foreach f,$(S_FILES_N64_WRAPPER:.s=.o),build/$f) \
                                $(foreach f,$(C_FILES_N64_WRAPPER:.c=.o),build/$f) \
                                $(foreach f,$(CXX_FILES_N64_WRAPPER:.cpp=.o),build/$f)

N64_ASAN                     := libn64_asan.a
N64_UBSAN                    := libn64_ubsan.a
N64_WRAPPER                  := libn64_wrapper.a

TARGET_LIBS                  := $(N64_ASAN) $(N64_UBSAN) $(N64_WRAPPER)

$(N64_ASAN):    $(O_FILES_N64_ASAN)
$(N64_UBSAN):   $(O_FILES_N64_UBSAN)
$(N64_WRAPPER): $(O_FILES_N64_WRAPPER)

# create build directories
$(shell mkdir -p $(foreach dir,$(SRC_DIRS_ALL) $(ASM_DIRS_ALL),build/$(dir)))


#### Main Targets ###

all: $(TARGET_LIBS)
	@echo "Done"

clean:
	$(RM) -r build

distclean: clean
	$(RM) -r $(TARGET_LIBS)


.PHONY: all clean distclean
.DEFAULT_GOAL := all

#### Various Recipes ####

lib%.a:
	$(AR) cr $@ $^

build/asm/%.o: asm/%.s
	$(AS) $(ASFLAGS) $< -o $@

build/src/%.o: src/%.c
	$(CC) -c $(MIPS_VERSION) $(ARCH_FLAGS) $(OPTFLAGS) $(CFLAGS) -o $@ $<
	@$(OBJDUMP) -dlr $@ > $(@:.o=.s)

build/src/%.o: src/%.cpp
	$(CXX) -c $(MIPS_VERSION) $(ARCH_FLAGS) $(OPTFLAGS) $(CXXFLAGS) -o $@ $<
	@$(OBJDUMP) -dlr $@ > $(@:.o=.s)
