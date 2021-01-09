#!/bin/bash
#
# This is an install script for Fedora-specific packages.
#
set -ex

# Base build packages
PACKAGES=(
	gcc-c++
	clang
	meson
	libomp-devel
	pkgconf-pkg-config
	glew-devel
	SDL2-devel
)

dnf install -y "${PACKAGES[@]}"
