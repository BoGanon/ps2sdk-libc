# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

ifeq (x$(PS2SDKSRC), x)
  export PS2SDKSRC=$(shell pwd)
endif

SUBDIRS = tools
SUBDIRS += iop
SUBDIRS += ee
SUBDIRS += common
SUBDIRS += samples

all: build
	@$(ECHO) .;
	@$(ECHO) .PS2SDK Built.;
	@$(ECHO) .;

# Common rules shared by all build targets.

.PHONY: dummy


# Use SUBDIRS to descend into subdirectories.
subdir_list  = $(patsubst %,all-%,$(SUBDIRS))
subdir_clean = $(patsubst %,clean-%,$(SUBDIRS))
subdir_release = $(patsubst %,release-%,$(SUBDIRS))
subdirs: dummy $(subdir_list)

$(subdir_list): dummy
	$(MAKE) -C $(patsubst all-%,%,$@)
$(subdir_clean): dummy
	$(MAKE) -C $(patsubst clean-%,%,$@) clean
	$(MAKE) -C ee/erl-loader clean
$(subdir_release): dummy
	$(MAKE) -C $(patsubst release-%,%,$@) release
	$(MAKE) -C ee/erl-loader release


build: env_build_check $(subdir_list)
	$(MAKE) -C ee/erl-loader all

clean: env_build_check $(subdir_clean)

clean_base: env_release_check
	  rm -f $(PS2SDK)/README.md
	  rm -f $(PS2SDK)/CHANGELOG
	  rm -f $(PS2SDK)/AUTHORS
	  rm -f $(PS2SDK)/LICENSE.md
	  rm -f $(PS2SDK)/ID

release-clean: env_release_check clean_base
	$(MAKE) -C common release-clean
	$(MAKE) -C iop release-iop-clean
	$(MAKE) -C ee release-ee-clean
	$(MAKE) -C samples release-clean
	$(MAKE) -C tools release-clean

rebuild: env_build_check $(subdir_clean) $(subdir_list)
	$(MAKE) -C ee/erl-loader all

$(PS2SDK)/ports:
	$(MKDIR) -p $(PS2SDK)/ports

install: release

release: build release-clean release_base $(PS2SDK)/ports $(subdir_release)
	@$(ECHO) .;
	@$(ECHO) .PS2SDK Installed.;
	@$(ECHO) .;

release_base: env_release_check
	@if test ! -d $(PS2SDK) ; then \
	  $(MKDIR) -p $(PS2SDK) ; \
	fi
	cp -f README.md $(PS2SDK)
	cp -f CHANGELOG $(PS2SDK)
	cp -f AUTHORS $(PS2SDK)
	cp -f LICENSE.md $(PS2SDK)
	cp -f ID $(PS2SDK)

env_build_check:
	@if test -z $(PS2SDKSRC) ; \
	then \
	  $(ECHO) PS2SDKSRC environment variable should be defined. ; \
	fi

env_release_check:
	@if test -z $(PS2SDK) ; \
	then \
	  $(ECHO) PS2SDK environment variable must be defined. ; \
	  exit 1; \
	fi

install-libc: env_release_check
	$(MKDIR) -p $(PS2SDK)/ee
	$(MKDIR) -p $(PS2SDK)/ee/include
	$(MAKE) -C  ee/libc release-ee-include
	$(MAKE) -C ee/fdlibm release-ee-include

install-headers: release-clean release_base install-libc
	$(MAKE) -C common release-dirs
	$(MAKE) -C common install-include
	$(MAKE) -C common link-ee-include
	@$(ECHO) .;
	@$(ECHO) PS2SDK LIBC headers installed.;
	@$(ECHO) .;

docs:
	doxygen doxy.conf

include Defs.make
