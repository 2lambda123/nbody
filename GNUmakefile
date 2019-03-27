BIN := nbody

CP := cp

QUIET := @

ifeq ($(strip $(MAKE_JOBS)),)
    ifeq ($(uname_S),Darwin)
        CPUS := $(shell /usr/sbin/sysctl -n hw.ncpu)
    endif
    ifeq ($(uname_S),Linux)
        CPUS := $(shell grep ^processor /proc/cpuinfo | wc -l)
    endif
    ifneq (,$(findstring MINGW,$(uname_S))$(findstring CYGWIN,$(uname_S)))
        CPUS := $(shell getconf _NPROCESSORS_ONLN)
    endif
    MAKE_JOBS := $(CPUS)
endif

ifeq ($(strip $(MAKE_JOBS)),)
    MAKE_JOBS := 8
endif

top-level-make:
	@$(MAKE) -f GNUmakefile -j$(MAKE_JOBS) all

all: $(BIN)

info:
	$(QUIET)$(MAKE) -C src $@

info%:
	$(QUIET)$(MAKE) -C src $@

$(BIN): src/$(BIN)
	$(QUIET)$(CP) $< $@

distclean: clean

clean:
	$(RM) $(BIN)
	$(MAKE) -C src clean

src/$(BIN): $(DEPS)
	$(QUIET)$(MAKE) -C src $(BIN)
.PHONY: src/$(BIN)
