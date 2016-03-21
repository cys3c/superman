#!/bin/bash

. ./common.sh

prereqInstall()
{
	local TARGET_PATH="$1"
	local INCLUDE_DBG_SYMBOLS=N
	local PACKAGES="libc6 libnl-3-200 libnl-genl-3-200 libssl1.0.0 linux-image-`uname -r` openssl"
	
	if [ "${INCLUDE_DBG_SYMBOLS}" = "Y" ]; then
		PACKAGES="${PACKAGES} linux-image-$(uname -r)-dbgsym"
		aptAddDbgSymbolsRepo
	fi

	mkdir -p {${TARGET_PATH},pkgs}

	debDownload "${PACKAGES}" ./pkgs
	debExtract "${PACKAGES}" ./pkgs ${TARGET_PATH}

	echob Tidying up...
	rm -R ${TARGET_PATH}/boot ${TARGET_PATH}/usr/share/doc pkgs
}

[ "$0" = "$BASH_SOURCE" ] && prereqInstall ./initrd-prereqs
