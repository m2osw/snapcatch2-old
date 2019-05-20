
# Snap! Catch2

This package is used so we can have the latest version of Catch to run
along Snap! Websites. We  tried to move to Ubuntu 18.04 and had problems
to support 16.04 and 18.04 because it changes so much so quickly.

This package will allow us to upgrade all our projects to the latest
available quickly and have a consistant version of Catch through various
versions of Ubuntu.

# Building

## Within the Snap! Websites Environment

In our environment, we have the source in a tarball and a patch.
We extract the tarball, run the make of Catch, then the install target.
Once that is done, our own CMakeLists.txt can install the results so
it can get installed in our Debian environment.

## cmake files

Note that the Catch people somehow install the the cmake files under
`/usr/lib/cmake/...`. I moved those to `/usr/shared/cmake/...` because
that's where I see those files under Ubuntu.

## Compiler Version

By default it compiles with C++11.

Use `-DUSE_CPP17` to make it compile with C++17.

Use `-DUSE_CPP14` to make it compile with C++14.

## Patch

In version 2.7.2 there is a bug in the CMakeLists.txt so I created a patch:

    LC_ALL=C TZ=UTC0 diff -Naur Catch2-2.7.2 Catch2-2.7.2a >Catch2-2.7.2.patch

## Build & Install of Catch itself

To build, I run:

    rm -rf Catch2-2.7.2
    tar xf catch2-2.7.2.tar.gz
    patch Catch2-2.7.2/projects/CMakeLists.txt <Catch2-2.7.2.patch
    cd Catch2-2.7.2
    cmake -DUSE_CPP14 -DCMAKE_INSTALL_PREFIX=... .
    make install

