# On command line:
#
# make = Make software.
#
# make clean = Clean out built project files.
#

# To make "Debug" the default configuration if invoked with just "make":
#ifeq ($(CFG),)
#CFG=Debug
#endif

# Toolchain prefix (i.e arm-elf -> arm-elf-gcc.exe)
ifneq ($(CFG),Debug_x86)
ifneq ($(CFG),Debug_x86_Windows)
TCHAIN = bfin-linux-uclibc-
endif
endif

# Target file name (without extension).
TARGET = BootloaderLauncher
ifeq ($(CFG),Debug_x86_Windows)
TARGET = BootloaderLauncher.exe
endif

# List C source files here. (C dependencies are automatically generated.)
# use file-extension c for "c-only"-files
SRC  = 

# List C++ source files here.
# use file-extension cpp for C++-files (use extension .cpp)
CPPSRC = $(shell find src -name *.cpp -print)

# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC = 

# Optimization level, can be [0, 1, 2, 3, s]. 
# 0 = turn off optimization. s = optimize for size.
OPT = 3

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS = 

# List any extra directories to look for library files here.
#     Each directory must be seperated by a space.
EXTRA_LIBDIRS = 

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD =-std=gnu99

# Place -D or -U options for C here
CDEFS =
ifneq ($(CFG),Debug_x86)
ifneq ($(CFG),Debug_x86_Windows)
CDEFS += -mcpu=bf561-0.5 -mfast-fp
endif
endif
ifeq ($(CFG),Debug_x86)
CDEFS += -DDEBUG_X86
endif
ifeq ($(CFG),Debug_x86_Windows)
CDEFS += -DDEBUG_X86
CDEFS += -DWIN32
endif

# Place -I options here
CINCS =

# Place -D or -U options for ASM here
ADEFS = 

# Compiler flags.

#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
#
# Flags for C and C++ (arm-elf-gcc/arm-elf-g++)
CFLAGS = -g
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += -O$(OPT)
CFLAGS += -Wall -pedantic # -Werror
CFLAGS += -fmessage-length=0
# Be really, really pedantic
#CFLAGS += -Weffc++ -Wold-style-cast 
#CFLAGS += -Wstrict-null-sentinel -Wsign-promo -Woverloaded-virtual
#
# mudflap
#CFLAGS += -fmudflap -fmudflapth -funwind-tables 

CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))

# flags only for C
CONLYFLAGS += $(CSTANDARD)

# flags only for C++ (g++)
CPPFLAGS = 

# Assembler flags.
#  -Wa,...:    tell GCC to pass this to the assembler.
#  -ahlns:     create listing
#  -g$(DEBUG): have the assembler create line number information
ASFLAGS = $(ADEFS) -Wa,-g
ASFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))

#Additional libraries.

# Extra libraries
#    Each library-name must be seperated by a space.
#    To add libxyz.a, libabc.a and libefsl.a: 
#    EXTRA_LIBS = xyz abc efsl
#EXTRA_LIBS = efsl
#EXTRA_LIBS = STR71Xlibstd
EXTRA_LIBS = pthread
ifeq ($(CFG),Debug_x86_Windows)
EXTRA_LIBS += ws2_32
endif

# Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = 
#mudflap
#LDFLAGS += -lmudflapth -rdynamic
 
LDFLAGS += $(patsubst %,-L%,$(EXTRA_LIBDIRS))
LDFLAGS += $(patsubst %,-l%,$(EXTRA_LIBS))

# Define programs and commands.
SHELL = sh
CC = $(TCHAIN)gcc
CPP = $(TCHAIN)g++
AR = $(TCHAIN)ar
OBJCOPY = $(TCHAIN)objcopy
OBJDUMP = $(TCHAIN)objdump
SIZE = $(TCHAIN)size
NM = $(TCHAIN)nm
REMOVE = rm -f
REMOVEDIR = rm -f -r
COPY = cp
LINT = LIN

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = "-------- begin --------"
MSG_END = --------  end  --------
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILINGCPP = Compiling C++:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:

# Define all object files.
COBJ      = $(subst src,$(CFG)/src,$(SRC:.c=.o) )
AOBJ      = $(subst src,$(CFG)/src,$(ASRC:.S=.o))
CPPOBJ    = $(subst src,$(CFG)/src,$(CPPSRC:.cpp=.o)) 

# Compiler flags to generate dependency files.
### GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,$@.d
GENDEPFLAGS = -MMD -MP -MF $@.d

# Combine all necessary flags and optional flags.
ALL_CFLAGS  = $(CFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -x assembler-with-cpp $(ASFLAGS)

# Default target.
all: inform begin "$(CFG)/$(TARGET)" finished end

inform:
ifneq ($(CFG),Debug)
ifneq ($(CFG),Debug_x86)
ifneq ($(CFG),Debug_x86_Windows)
	@echo "Invalid configuration '"$(CFG)"' specified."
	@echo "You must specify a configuration when running make, e.g."
	@echo  "make CFG=Debug"
	@echo  
	@echo  "Possible choices for configuration are 'Debug', 'Debug_x86' and 'Debug_x86_Windows'"
	@exit 1
endif
endif
endif
	@echo "Selected configuration: "$(CFG)
	@echo "------------------------"

# Eye candy.
begin: | inform
	@echo
	@echo $(MSG_BEGIN)
	@mkdir -p "$(CFG)"
	@mkdir -p "$(CFG)/src"

finished: | inform
	@echo $(MSG_ERRORS_NONE)

end: | inform
	@echo $(MSG_END)
	@echo

# Display compiler version information.
gccversion : | inform
	@$(CC) --version

# Link: create ELF output file from object files.
"$(CFG)/$(TARGET)": $(AOBJ) $(COBJ) $(CPPOBJ) | inform begin
	@echo
	@echo $(MSG_LINKING) $@
#	$(CC) $(ALL_CFLAGS) $(AOBJ) $(COBJ) $(CPPOBJ) -o $@ $(LDFLAGS)
	$(CPP) $(AOBJ) $(COBJ) $(CPPOBJ) -o $@ $(LDFLAGS)

# Compile: create object files from C source files.
$(COBJ) : $(CFG)/src/%.o: src/%.c
	@echo
	@echo $(MSG_COMPILING) $<
	@test -d "$(CFG)" || mkdir -pm 775 "$(CFG)"
	@test -d $(@D) || mkdir -pm 775 $(@D)
	$(CC) -c $(ALL_CFLAGS) $(CONLYFLAGS) $< -o $@ 

# Compile: create object files from C++ source files.
$(CPPOBJ) : $(CFG)/src/%.o: src/%.cpp
	@echo
	@echo $(MSG_COMPILINGCPP) $<
	@test -d "$(CFG)" || mkdir -pm 775 "$(CFG)"
	@test -d $(@D) || mkdir -pm 775 $(@D)
	$(CPP) -c $(ALL_CFLAGS) $(CPPFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
## does not work - TODO - hints welcome
##$(COBJ) : %.s : %.c
##	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
$(AOBJ) :  $(CFG)/src/%.o: src/%.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	@test -d "$(CFG)" || mkdir -pm 775 "$(CFG)"
	@test -d $(@D) || mkdir -pm 775 $(@D)
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

#################### Clean ####################

# Target: clean project.
clean: inform begin clean_list finished end


clean_list : | inform begin
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) "$(CFG)/$(TARGET)"
	$(REMOVE) $(COBJ)
	$(REMOVE) $(CPPOBJ)
	$(REMOVE) $(AOBJ)
	$(REMOVE) $(COBJ:.o=.o.d)
	$(REMOVE) $(CPPOBJ:.o=.o.d)
	$(REMOVEDIR) "$(CFG)/src" | exit 0

#################### /Clean ####################

#################### PC-Lint ####################

ifeq ($(CFG),Debug_x86)
LINT_CDEFS = -dDEBUG_X86
endif
ifeq ($(CFG),Debug_x86_Windows)
LINT_CDEFS = -dDEBUG_X86
LINT_CDEFS += -dWIN32
endif

# Target: lint project
lint : | inform begin
	@echo $(SRC) $(patsubst %.pb.cpp,,$(CPPSRC)) > tests/lint-files.lnt
	-$(LINT) tests/lint.lnt $(LINT_CDEFS) tests/lint-files.lnt 
	$(REMOVE) tests/lint-files.lnt
	
#################### /PC-Lint ####################

# Include the dependency files.
ifneq ($(MAKECMDGOALS),clean)
-include $(shell mkdir "$(CFG)" 2>/dev/null) $(shell mkdir "$(CFG)/src" 2>/dev/null) $(shell find "$(CFG)/src" -name *.o.d -print)
endif

# Listing of phony targets.
.PHONY : all begin finish end gccversion build clean clean_list lint
