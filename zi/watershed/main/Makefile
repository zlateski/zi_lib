HERE   	=       $(shell pwd)
BINDIR	=	bin
AT	=
CD	=	$(AT)cd
CP	=	$(AT)cp
ECHO	=	@echo
CAT	=	$(AT)cat
IF	=	$(AT)if
LN	=	$(AT)ln
MKDIR	=	$(AT)mkdir
MV	=	$(AT)mv
SED	=	$(AT)sed
RM	=	$(AT)rm
TOUCH	=	$(AT)touch
CHMOD	=	$(AT)chmod
DATE    =       $(AT)date
PERL	=	$(AT)perl
AR	=	$(AT)ar

ARFLAGS	=	rcs
CC	=	$(AT)gcc
CXX	=	$(AT)g++

ZI_LIB	=	$(shell cd ../../../; pwd)

INCLUDES	=	-I$(ZI_LIB) -I$(ZI_LIB)/external/include
WARN_FLAGS	=	-Wall -Wextra
OPT_FLAGS	=	-O3 -DNDEBUG -fno-omit-frame-pointer
DBG_FLAGS	=	-g
LINK_FLAGS	=	-lpthread -lrt

COMMON_FLAGS	=	$(INCLUDES) $(WARN_FLAGS) $(LINK_FLAGS)

ifneq ($(strip $(VERBOSE)),)
  TMP_CXX_COMMON_FLAGS = $(COMMON_FLAGS) -D_ZI_WATERSHED_VERBOSE
else
  TMP_CXX_COMMON_FLAGS = $(COMMON_FLAGS)
endif

ifneq ($(strip $(USEMP)),)
  CXX_COMMON_FLAGS = $(COMMON_FLAGS) -DZI_USE_OPENMP -fopenmp
else
  CXX_COMMON_FLAGS = $(COMMON_FLAGS)
endif



$(BINDIR)/xxlws_dbg: $(HERE)/main.cpp
	$(MKDIR) -p $(BINDIR)
	$(ECHO) "Compiling in debug mode: $(BINDIR)/xxlws_dbg"
	$(CXX) $(CXX_COMMON_FLAGS) $(DBG_FLAGS) -o $(BINDIR)/xxlws_dbg main.cpp

$(BINDIR)/xxlws_fastdbg: $(HERE)/main.cpp
	$(MKDIR) -p $(BINDIR)
	$(ECHO) "Compiling in fast debug mode: $(BINDIR)/xxlws_fastdbg"
	$(CXX) $(CXX_COMMON_FLAGS) $(DBG_FLAGS) -O2 -o $(BINDIR)/xxlws_fastdbg main.cpp

$(BINDIR)/xxlws: $(HERE)/main.cpp
	$(MKDIR) -p $(BINDIR)
	$(ECHO) "Compiling in release mode: $(BINDIR)/xxlws"
	$(CXX) $(CXX_COMMON_FLAGS) $(OPT_FLAGS) -o $(BINDIR)/xxlws main.cpp

$(BINDIR)/xxlws_bin: $(HERE)/main.cpp
	$(MKDIR) -p $(BINDIR)
	$(ECHO) "Compiling $(BINDIR)/xxlws_bin"
	$(CXX) $(CXX_COMMON_FLAGS) -o $(BINDIR)/xxlws_bin main.cpp

.PHONY: all
all:

.PHONY: clean
clean:
	$(ECHO) Cleaning...
	$(RM) -rf $(BINDIR)

.PHONY: debug
debug: $(BINDIR)/xxlws_dbg

.PHONY: release
release: $(BINDIR)/xxlws

.PHONY: fastdebug
fastdebug: $(BINDIR)/xxlws_fastdbg

all: $(BINDIR)/xxlws_bin
