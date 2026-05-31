# VARIABLES

# User-modifiable variables.
SRCDIR := ../src
AUTHOR := Nikola Hadžić
PACKAGE := Airpad
VERSION := 1.0
BUGS_ADDRESS := nikola.hadzic.000@protonmail.com
MERGE_BACKUP := simple

# RULES

.PHONY: help clean

help:
	$(info VARIABLES)
	$(info =================================================================================)
	$(info SRCDIR:          Source directory.)
	$(info AUTHOR:          Package author's name.)
	$(info PACKAGE:         Package's name.)
	$(info VERSION:         Package's version.)
	$(info BUGS_ADDRESS:    E-mail address to report the program bugs to (usually author's).)
	$(info MERGE_BACKUP:    Merge backup option; see msgmerge(1).)
	$(info )
	$(info RULES)
	$(info =================================================================================)
	$(info help:            Display this help menu.)
	$(info messages.pot:    Build POT file.)
	$(info %.po:            Generate PO file.)
	$(info clean:           Remove unnecessary generated files.)

messages.pot: $(SRCDIR)/*.h $(SRCDIR)/*.c
	xgettext -o '$@' -L C --from-code=UTF-8 -k_ --no-wrap --copyright-holder='$(AUTHOR)' --package-name='$(PACKAGE)' --package-version='$(VERSION)' --msgid-bugs-address='$(BUGS_ADDRESS)' $(patsubst %,'%',$^)

%.po: messages.pot
	$(if $(wildcard $@),msgmerge --backup '$(MERGE_BACKUP)' -U --no-wrap '$@' '$<',msginit --no-wrap -l $(patsubst %.po,'%',$@))

clean:
	$(info Cleaning directory...)
	@rm -f messages.pot
	@rm -f *.po~
