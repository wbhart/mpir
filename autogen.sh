#! /bin/sh

# Save FSF scripts wrappers.
mv config.guess configmpir.guess
mv config.sub configmpir.sub
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
mv configmpir.guess config.guess
mv configmpir.sub config.sub

