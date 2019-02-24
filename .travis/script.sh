#!/bin/bash
#
# This is a distribution-agnostic build script. Do not use "apt-get", "dnf", or
# similar in here. Add any package installation gunk into the appropriate
# install script instead.
#
set -ex

HAVE_MESON=0
if [[ "$NOMESON" -ne 1 ]]; then
	if type -P meson &>/dev/null; then
		HAVE_MESON=1
	fi
fi

make distclean
make CC=${CC} CXX=${CXX} V=1
./nbody --bodies 8 --cycle-after 3 --iterations 1 --verbose

make distclean
make CC=${CC} CXX=${CXX} NO_OPENMP=1 V=1
./nbody --bodies 8 --cycle-after 3 --iterations 1 --verbose

if type -P nvcc &>/dev/null; then
	make distclean
	make CC=${CC} CXX=${CXX} CUDA=1 V=1
	./nbody --bodies 8 --cycle-after 3 --iterations 1 --verbose
fi

if [[ $HAVE_MESON -eq 1 ]]; then
	rm -rf build
	mkdir build
	meson . build
	ninja -C build
	build/nbody --bodies 8 --cycle-after 3 --iterations 1 --verbose

	if type -P nvcc &>/dev/null; then
		rm -rf build
		mkdir build
		meson . build -Dcuda=true
		ninja -C build
		build/nbody --bodies 8 --cycle-after 3 --iterations 1 --verbose
	fi
fi
