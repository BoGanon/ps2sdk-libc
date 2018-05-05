# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

ifeq (x$(PS2SDKSRC), x)
  export PS2SDKSRC := $(shell pwd)
endif

export PS2SDK := $(abspath $(PS2SDK))


all: build
	@$(ECHO) .;
	@$(ECHO) .PS2SDK Standard C Library Built.;
	@$(ECHO) .;

# Environment rules

env_build_check:
	@if test -z $(PS2SDKSRC) ; \
	then \
	  $(ECHO) PS2SDKSRC environment variable should be defined. ; \
	  exit 1; \
	fi
	@if test -z $(PS2SDK) ; \
	then \
	  $(ECHO) PS2SDK environment variable must be defined. ; \
	  exit 1; \
	fi


# Common rules shared by all build targets.
SUBDIRS += ee
SUBDIRS += samples

.PHONY: dummy ee samples

# Use SUBDIRS to descend into subdirectories.
subdir_clean = $(patsubst %,clean-%,$(SUBDIRS))
subdir_release = $(patsubst %,release-%,$(SUBDIRS))
subdirs: dummy $(subdir_list)

$(subdir_clean): dummy
	$(MAKE) -C $(patsubst clean-%,%,$@) clean
$(subdir_release): dummy
	$(MAKE) -C $(patsubst release-%,%,$@) release

# Build rules

ee_lib: env_build_check
	$(MAKE) -C ee all

ee: ee_lib

build: ee

rebuild: env_build_check $(subdir_clean) build

# Clean rules
clean: env_build_check $(subdir_clean)

# Release rules
install: release

release_base_dir: env_build_check
	$(MKDIR) -p $(PS2SDK)

release_ports_dir: env_build_check
	$(MKDIR) -p $(PS2SDK)/ports

release_base: release_base_dir
	cp -f README.md $(PS2SDK)
	cp -f CHANGELOG $(PS2SDK)
	cp -f AUTHORS $(PS2SDK)
	cp -f LICENSE.md $(PS2SDK)
	cp -f ID $(PS2SDK)

release-clean: env_build_check
	$(MAKE) -C ee release-ee-clean
	$(MAKE) -C samples release-clean
	$(MAKE) -C tools release-clean
	rm -f $(PS2SDK)/README.md
	rm -f $(PS2SDK)/CHANGELOG
	rm -f $(PS2SDK)/AUTHORS
	rm -f $(PS2SDK)/LICENSE.md
	rm -f $(PS2SDK)/ID

release: env_build_check build release-clean release_base release_ports_dir $(subdir_release)
	@$(ECHO) .;
	@$(ECHO) .PS2SDK Standard C Library Installed.;
	@$(ECHO) .;

# install-headers rules
libc_dirs: release_base
	$(MAKE) -C ee/libc release-libc-dirs

install-headers: libc_dirs
	$(MAKE) -C ee/libc release-libc-include
	$(MAKE) -C ee/fdlibm release-libm-include
	$(MAKE) -C ee/startup all
	$(MAKE) -C ee/startup release
	@$(ECHO) .;
	@$(ECHO) PS2SDK LIBC headers installed.;
	@$(ECHO) .;


# Miscellaneous rules
help:
	@$(ECHO) "usage: make <options> <target>";
	@$(ECHO) "";
	@$(ECHO) "  Variables:        Value:";
	@$(ECHO) "    PS2SDK            Install directory";
	@$(ECHO) "    PS2SDKSRC         Source directory";
	@$(ECHO) "";
	@$(ECHO) "  Targets:";
	@$(ECHO) "    all               Build ps2sdk";
	@$(ECHO) "    install           Install ps2sdk";
	@$(ECHO) "    clean             Clean source directory";
	@$(ECHO) "    rebuild           Clean then build ps2sdk";
	@$(ECHO) "    release-clean     Clean installed files";
	@$(ECHO) "    install-headers   Install LIBC headers";
	@$(ECHO) "    help              Print this message";

include Defs.make
