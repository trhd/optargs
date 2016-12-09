#!/bin/bash -eu

MESON=meson
MESONCONF=mesonconf

get_meson()
{
	local url=https://github.com/

	printf "$0: Setting up a local installation of meson.\n"

	url+=$(curl ${VERBOSE:--s} -L https://github.com/mesonbuild/meson/releases/latest/ \
		| grep -oE 'mesonbuild/meson/releases/download/[^/]+/meson-[0-9]+(\.[0-9]+)+.tar.gz"' \
		| sed 's/"$//')

	pushd "$BDIR" &>/dev/null

	wget ${VERBOSE:--q} $url

	tar -xf $(basename $url)

	PATH=$PWD/$(basename ${url%.tar.gz})/:$PATH

	MESON+=.py
	MESONCONF+=.py

	popd &>/dev/null
}

get_ninja()
{
	local url=https://github.com/

	printf "$0: Setting up a local installation of ninja.\n"

	url+=$(curl ${VERBOSE:--s} -L https://github.com/ninja-build/ninja/releases/latest/ \
		| grep -oE 'ninja-build/ninja/releases/download/v[^/]+/ninja-linux.zip' \
		| sed 's/"$//')

	pushd "$BDIR" &>/dev/null

	wget ${VERBOSE:--q} $url

	mkdir ninjabin
	unzip -q $(basename $url) -d ninjabin

	PATH=$PWD/ninjabin:$PATH

	popd &>/dev/null
}

get_cmocka()
{
	local url=https://cmocka.org/files/1.1/cmocka-1.1.0.tar.xz

	pushd $BDIR &>/dev/null

	wget ${VERBOSE:--q} $url
	tar -xf $(basename $url)
	cd $(basename ${url%.tar.xz})
	mkdir build
	( cd build ; cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug .. )
	make -C build
	sudo make -C build install

	popd &>/dev/null
}

cleanup()
{
	[ ! -d "${BDIR:-}" ] || rm -rf $BDIR
}

prepare_env()
{
	trap cleanup EXIT

	BDIR=`mktemp -d`

	which $MESON &>/dev/null || get_meson
	which ninja &>/dev/null || get_ninja
	[ ! /usr/lib/libcmocka.so ] || get_cmocka
}

main()
{
	if [ "${1:-}" = --verbose ] ; then
		shift
		VERBOSE=""
		set -x
	fi

	prepare_env

	$MESON -Derrorlogs=true "$BDIR"

	# Print the whole config to the build log.
	$MESONCONF "$BDIR"

	ninja -v -C "$BDIR" ${@:-test $(! which valgrind &>/dev/null || echo test-valgrind)}
}

main "$@"
