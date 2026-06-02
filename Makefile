# INSTALL COMMANDS
INSTALL := install
INSTALL_PROGRAM := $(INSTALL)
INSTALL_DATA := $(INSTALL) -m 644

# LIBRARIES
LIB_GTK := gtk+-3.0

# VARIABLES

# User-modifiable variables.
PROG := stilus
IDEN := com.gitlab.NH000.Airpad
DEBUG :=
PROFILE :=
OPTIMIZE :=
SRCDIR := src
OBJDIR := obj
PODIR := po
DEPSDIR := .deps
ICON := icon
INSTALLDIR := /usr/bin
DESKTOPDIR := /usr/share/applications
ICONDIR := /usr/share/icons
LOCALEDIR := /usr/share/locale
SWITCHES := AIRPAD_EXEC_NAME="\"$(PROG)\"" AIRPAD_LOCALE_DIR="\"$(LOCALEDIR)\"" AIRPAD_INFO_IDEN=\"$(IDEN)\"
CFLAGS := -std=c99 -Wall -c
LDFLAGS :=

# User-non-modifiable variables.
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
SWITCHES_FLAGS = $(patsubst %,-D%,$(SWITCHES))
COMM_COMPILE = $(CC) $(CFLAGS) $(SWITCHES_FLAGS) -o
COMM_LINK = $(CC) $(LDFLAGS) -o
INCLUDES := $(shell pkg-config --cflags $(LIB_GTK))
LIBS := $(shell pkg-config --libs $(LIB_GTK))

# DIRECTIVES

ifdef DEBUG
    CFLAGS := $(CFLAGS) -g
endif

ifdef PROFILE
    CFLAGS := -pg $(CFLAGS)
    LDFLAGS := -pg $(LDFLAGS)
endif

ifdef OPTIMIZE
    CFLAGS := -O3 $(CFLAGS)
endif

# FUNCTIONS

# Generate MO file from PO file into $(LOCALEDIR) given language code as $(1).
generate_mo = mkdir -p '$(LOCALEDIR)/$(1)/LC_MESSAGES' && msgfmt -o '$(LOCALEDIR)/$(1)/LC_MESSAGES/$(PROG).mo' '$(PODIR)/$(1).po'

# Delete MO file from $(LOCALEDIR) given language code as $(1).
# Then delete all of its parent directories in bottom-to-top order if they are empty.
uninstall_mo = if test -d '$(LOCALEDIR)/$(1)/LC_MESSAGES'; then rm -f '$(LOCALEDIR)/$(1)/LC_MESSAGES/$(PROG).mo' && rmdir -p --ignore-fail-on-non-empty '$(LOCALEDIR)/$(1)/LC_MESSAGES'; fi

# Generate desktop file into its appropriate directory, optionally creating the directory if it does not exist.
generate_desktop = mkdir -p '$(DESKTOPDIR)' && echo -e\
                   '[Desktop Entry]\n$\
                   Type=Application\n$\
                   Version=1.5\n$\
                   Name=Airpad\n$\
                   GenericName=Text editor\n$\
                   GenericName[sr]=Уређивач текста\n$\
                   GenericName[sr_RS@latin]=Uređivač teksta\n$\
                   Comment=Edit text\n$\
                   Comment[sr]=Уредите текст\n$\
                   Comment[sr_RS@latin]=Uredite tekst\n$\
                   Icon=$(PROG)\n$\
                   DBusActivatable=true\n$\
                   TryExec=$(INSTALLDIR)/$(PROG)\n$\
                   Exec=$(INSTALLDIR)/$(PROG) %F\n$\
                   Terminal=false\n$\
                   MimeType=text/english;text/plain;text/x-makefile;text/x-c++hdr;text/x-c++src;text/x-chdr;text/x-csrc;text/x-java;text/x-moc;text/x-pascal;text/x-tcl;text/x-tex;application/x-shellscript;text/x-c;text/x-c++;\n$\
                   Categories=Utility;TextEditor;\n$\
                   StartupNotify=true' > '$(DESKTOPDIR)/$(IDEN).desktop'

# INCLUDES
include $(wildcard $(DEPSDIR)/*.d)

# RULES

.PHONY: help all deps clean install uninstall

help:
	$(info VARIABLES)
	$(info ================================================================================)
	$(info PROG:        Name of the executable.)
	$(info IDEN:        Application identifier.)
	$(info DEBUG:       Embed debugging information.)
	$(info PROFILE:     Embed profiling information.)
	$(info OPTIMIZE:    Enable compiler optimizations.)
	$(info SWITCHES:    Macros passed on the command-line.)
	$(info CC:          Compiler to use.)
	$(info CFLAGS:      Compiler flags.)
	$(info LDFLAGS:     Linker flags.)
	$(info SRCDIR:      Source directory.)
	$(info OBJDIR:      Object directory.)
	$(info PODIR:       PO directory.)
	$(info DEPSDIR:     Directory that contains dependency information.)
	$(info ICON:        Name of the icon file (without extension).)
	$(info INSTALLDIR:  Executable installation directory.)
	$(info DESKTOPDIR:  Desktop file installation directory.)
	$(info ICONDIR:     Icon installation directory.)
	$(info LOCALEDIR:   Locale installation directory.)
	$(info )
	$(info RULES)
	$(info ================================================================================)
	$(info help:         Display this help menu.)
	$(info deps:         Create dependency information for compiling objects.)
	$(info all:          Build object files and link them into executable.)
	$(info clean:        Remove files produced by the building process.)
	$(info install:      Install the program and its data.)
	$(info uninstall:    Uninstall the program and its data.)

# Directory creator.
$(OBJDIR) $(DEPSDIR):
	@mkdir -p '$@'

deps: $(SRCS) | $(DEPSDIR)
	$(info Building dependencies...)
	@$(foreach src,$(SRCS),$(CC) $(SWITCHES_FLAGS) -MT $(patsubst $(SRCDIR)/%.c,'$(OBJDIR)/%.o',$(src)) -MM -MF $(patsubst $(SRCDIR)/%.c,'$(DEPSDIR)/%.d',$(src)) '$(src)' $(INCLUDES);)

all: $(PROG)

$(PROG): $(OBJS)
	$(COMM_LINK) '$(PROG)' $(patsubst %,'%',$^) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(COMM_COMPILE) '$@' '$<' $(INCLUDES)

clean:
	$(info Cleaning directory...)
	@rm -rf '$(PROG)' '$(OBJDIR)' '$(DEPSDIR)'

install: $(PROG) $(ICON).svg
	$(INSTALL_PROGRAM) -D '$(PROG)' '$(INSTALLDIR)'
	# Translations are intentionally not installed until Stilus has complete locale catalogs.
	$(INSTALL_DATA) -D '$(ICON).svg' '$(ICONDIR)/hicolor/scalable/apps/$(PROG).svg'
	$(call generate_desktop)

uninstall:
	rm -f '$(INSTALLDIR)/$(PROG)'
	$(foreach po,$(wildcard $(PODIR)/*.po),$(call uninstall_mo,$(patsubst $(PODIR)/%.po,%,$(po)));)
	rm -f '$(ICONDIR)/hicolor/scalable/apps/$(PROG).svg'
	rm -f '$(DESKTOPDIR)/$(IDEN).desktop'
