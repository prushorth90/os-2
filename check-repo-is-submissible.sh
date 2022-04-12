#!/bin/sh

if [ ! -d .git ]; then
    echo "This is not a git repository!" 1>&2
    echo "Expected a directory called '.git'" 1>&2
    exit 1
fi

branch="$( git branch -v | grep '^\* master' )"
if ! [ $? -eq 0 ]; then
    echo "workdir is not on branch 'master'" 1>&2
    exit 1
fi

commitlog="$( git log 3e5e0f747340f07da9a359b1a408c3e18191de23 )"
if [ -z "$commitlog" ]; then
    echo "Did not find any commits after Stephen's" 1>&2
    exit 1
fi

bytes="$( zip -r - . 2>/dev/null | wc -c )"
if [ $bytes -gt 52428800 ]; then
    echo "Your directory is too big (size of zip file: ${bytes})" 1>&2
    exit 1
fi

diff_wc="$( git diff | wc -l )"
if ! [ $diff_wc -eq 0 ]; then
    echo "WARNING: you have uncommitted changes, according to 'git diff'." 1>&2
    echo "You must commit these if you want them to be marked." 1>&2
    echo "Continuing anyway." 1>&2
    # don't exit
    # exit 1
fi

echo "Looks good!" 1>&2
echo "To create a zip file you could (at least on Linux) do something like this" 1>&2
echo "(which will print the name of the zip file to the terminal):" 1>&2
echo "zip -q -r \`mktemp -u --suffix=.zip | tee /dev/stderr\` ." 1>&2
exit 0
