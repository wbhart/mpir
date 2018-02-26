#! /bin/sh

# Save FSF scripts wrappers.
mv -f config.guess configmpir.guess
mv -f config.sub configmpir.sub
# FSF scripts should be updated manually if needed.
#mv configfsf.guess config.guess
#mv configfsf.sub config.sub

# Delete cached stuff
rm -rf autom4te.cache

# Regenerate autotools stuff
autoreconf -isv

# Restore the FSF scripts wrappers.
#mv config.guess configfsf.guess
#mv config.sub configfsf.sub
mv -f configmpir.guess config.guess
mv -f configmpir.sub config.sub

