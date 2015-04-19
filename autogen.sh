#!/bin/sh

if [ ! -f "configure.in" ]; then 
    echo "autoscan"
    autoscan
    mv configure.scan configure.in
fi

echo "libtoolize"
libtoolize --automake --copy --debug --force >/dev/null 2>&1
echo "aclocal"
aclocal
echo "autoheader"
autoheader
echo "automake"
automake --add-missing
echo "autoconf"
autoconf
