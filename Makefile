#CC = gcc
#RM = rm
#AR = ar
LN = ln
INSTALL = install
MKDIR = mkdir
CP = cp
FMT=clang-format

DIR_BUILD = .build
DIR_BUILD_R = $(DIR_BUILD)/release
DIR_BUILD_D = $(DIR_BUILD)/debug
prefix = /usr/local
DIR_INCLUDES = include

PPFLAGS = -MT $@ -MMD -MP -MF $(patsubst %.o, %.d, $@)

CFLAGS_LOCAL = -Wall -Werror -std=c99 -I$(DIR_INCLUDES)
CFLAGS_LOCAL += $(CFLAGS)

CFLAGS_LOCAL_D = $(CFLAGS_LOCAL) -g -coverage
CFLAGS_LOCAL_D += $(CFLAGS)

ifeq ($(WITH_VALGRIND), yes)
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all
endif

TEST_SOURCES = $(wildcard  tests/*/*.c)
TEST_OBJECTS = $(patsubst %.c, %.o, $(TEST_SOURCES))
TESTS = $(patsubst %.o, %, $(TEST_OBJECTS))

APP_SOURCES = $(TEST_SOURCES)
APP_OBJECTS = $(patsubst %.c, %.o, $(APP_SOURCES))
APPS = $(patsubst %.c, %, $(APP_SOURCES))

LIB_SOURCES = $(wildcard src/*/*.c)
LIB_OBJECTS = $(patsubst %.c, %.o, $(LIB_SOURCES))
LIB_VERSION = 0.0.1
LIB_NAME = ccc
LIB_SO = lib$(LIB_NAME).so.$(LIB_VERSION)
LIB_A = lib$(LIB_NAME).a.$(LIB_VERSION)
LIB_SO_D = lib$(LIB_NAME)d.so.$(LIB_VERSION)
LIB_A_D = lib$(LIB_NAME)d.a.$(LIB_VERSION)

DEPFILES = $(patsubst %.o, %.d, $(addprefix $(DIR_BUILD_D)/, $(APP_OBJECTS)) $(addprefix $(DIR_BUILD_D)/, $(LIB_OBJECTS_D)))

.PHONY : all clean install uninstall test

all : $(addprefix $(DIR_BUILD_D)/, $(APPS)) $(DIR_BUILD_R)/$(LIB_SO) $(DIR_BUILD_R)/$(LIB_A)

# release library
$(DIR_BUILD_R)/$(LIB_SO) : $(addprefix $(DIR_BUILD_R)/, $(LIB_OBJECTS)) Makefile | $(DIR_BUILD_R)
	$(CC) $(CFLAGS_LOCAL) -shared -o $@ $(filter %.o, $^)
	$(LN) -sf $(shell pwd)/$@ $(DIR_BUILD_R)/lib$(LIB_NAME).so

$(DIR_BUILD_R)/$(LIB_A) : $(addprefix $(DIR_BUILD_R)/, $(LIB_OBJECTS)) Makefile | $(DIR_BUILD_R)
	$(AR) $(ARFLAGS) $@ $(filter %.o, $^)
	$(LN) -sf $(shell pwd)/$@ $(DIR_BUILD_R)/lib$(LIB_NAME).a

$(addprefix $(DIR_BUILD_R)/, $(LIB_OBJECTS)) : $(DIR_BUILD_R)/%.o : %.c Makefile | $(DIR_BUILD_R)
	$(MKDIR) -p $(@D)
	$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -fPIC -c $< -o $@

$(DIR_BUILD_R)/%.d : ;
.PRECIOUS : $(DIR_BUILD_R)/%.d

# for debug
$(addprefix $(DIR_BUILD_D)/, $(APPS)) : % : %.o $(DIR_BUILD_D)/$(LIB_SO_D) $(DIR_BUILD_D)/$(LIB_A_D) Makefile | $(DIR_BUILD_D)
	$(CC) $(CFLAGS_LOCAL_D) -o $@ $< -L$(shell pwd)/$(DIR_BUILD_D) -l$(LIB_NAME)d

$(DIR_BUILD_D)/$(LIB_SO_D) : $(addprefix $(DIR_BUILD_D)/, $(LIB_OBJECTS)) Makefile | $(DIR_BUILD_D)
	$(CC) $(CFLAGS_LOCAL_D) -shared -o $@ $(filter %.o, $^)
	$(LN) -sf $(shell pwd)/$@ $(DIR_BUILD_D)/lib$(LIB_NAME)d.so

$(DIR_BUILD_D)/$(LIB_A_D) : $(addprefix $(DIR_BUILD_D)/, $(LIB_OBJECTS)) Makefile | $(DIR_BUILD_D)
	$(AR) $(ARFLAGS) $@ $(filter %.o, $^)
	$(LN) -sf $(shell pwd)/$@ $(DIR_BUILD_D)/lib$(LIB_NAME)d.a

$(addprefix $(DIR_BUILD_D)/, $(APP_OBJECTS)) : $(DIR_BUILD_D)/%.o : %.c Makefile | $(DIR_BUILD_D)
	$(MKDIR) -p $(@D)
	$(CC) $(PPFLAGS) $(CFLAGS_LOCAL_D) -c $< -o $@

$(addprefix $(DIR_BUILD_D)/, $(LIB_OBJECTS)) : $(DIR_BUILD_D)/%.o : %.c Makefile | $(DIR_BUILD_D)
	$(MKDIR) -p $(@D)
	$(CC) $(PPFLAGS) $(CFLAGS_LOCAL_D) -fPIC -c $< -o $@

$(DIR_BUILD_D)/%.d : ;
.PRECIOUS : $(DIR_BUILD_D)/%.d

$(DIR_BUILD) :
	$(MKDIR) -p $@

$(DIR_BUILD_D) : $(DIR_BUILD)
	$(MKDIR) -p $@

$(DIR_BUILD_R) : $(DIR_BUILD)
	$(MKDIR) -p $@

install : all
	$(INSTALL) -d "$(prefix)/lib"
	$(INSTALL) "$(DIR_BUILD_R)/$(LIB_SO)" "$(prefix)/lib"
	$(LN) -sf "$(prefix)/lib/$(LIB_SO)" "$(prefix)/lib/lib$(LIB_NAME).so"
	$(INSTALL) "$(DIR_BUILD_R)/$(LIB_A)" "$(prefix)/lib"
	$(LN) -sf "$(prefix)/lib/$(LIB_A)" "$(prefix)/lib/lib$(LIB_NAME).a"
	$(INSTALL) "$(DIR_BUILD_D)/$(LIB_SO_D)" "$(prefix)/lib"
	$(LN) -sf "$(prefix)/lib/$(LIB_SO_D)" "$(prefix)/lib/lib$(LIB_NAME)d.so"
	$(INSTALL) "$(DIR_BUILD_D)/$(LIB_A_D)" "$(prefix)/lib"
	$(LN) -sf "$(prefix)/lib/$(LIB_A_D)" "$(prefix)/lib/lib$(LIB_NAME)d.a"
	$(INSTALL) -d "$(prefix)/include"
	$(CP) -r $(DIR_INCLUDES)/* "$(prefix)/include/"
#	$(INSTALL) -d "$(prefix)/bin"
#	$(INSTALL) "$(DIR_BUILD)/$(APP)" "$(prefix)/bin"

uninstall :
	$(RM) -f  "$(prefix)/lib/$(LIB_SO)"
	$(RM) -f  "$(prefix)/lib/$(LIB_SO_D)"
	$(RM) -f  "$(prefix)/lib/lib$(LIB_NAME).so"
	$(RM) -f  "$(prefix)/lib/lib$(LIB_NAME)d.so"
	$(RM) -f  "$(prefix)/lib/$(LIB_A)"
	$(RM) -f  "$(prefix)/lib/$(LIB_A_D)"
	$(RM) -f  "$(prefix)/lib/lib$(LIB_NAME).a"
	$(RM) -f  "$(prefix)/lib/lib$(LIB_NAME)d.a"
	$(RM) -rf "$(prefix)/include/$(LIB_NAME)"
#	$(RM) -f  "$(prefix)/bin/$(APP)"

test : $(addprefix $(DIR_BUILD_D)/, $(TESTS))
	set -e; for test in $^; do $(VALGRIND) ./$${test}; echo -en "\n\n"; done

clean :
	$(RM) -rf $(DIR_BUILD)

fmt :
	$(FMT) -i --style=file $$(find . -type f \( -name '*.c' -o -name '*.h' \))

include $(DEPFILES)
