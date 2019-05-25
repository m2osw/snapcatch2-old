
<p align="center">
<img alt="advgetopt" title="CMake Module Extensions to Build Snap! C++."
src="https://snapwebsites.org/sites/snapwebsites.org/files/images/catch2-logo-small.png" width="502" height="242"/>
</p>

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

At first, I had [two problems compiling version
2.7.2](https://github.com/catchorg/Catch2/issues/1636).

1. Building in an outside BUILD folder would fail with some "could not find
this or that".
2. Building in the root source folder would also fail (although that in itself
was not a bad thing).

The only way to resolve the problem was to create a build directory inside
the source tree and run `cmake` there. Newer version (post 2.7.2) will work
as expected (I've already tested the newest from the git and it worked.)

So, I resolved the two problems by following the steps from an old Catch2
documentation file which was to create a `projects/Generated` sub-directory
and building in there.

### How to Create a Patch

Just in case I wanted to keep the command line to create a patch since
our build knows how to automatically apply it from a catch2 tarball:

    LC_ALL=C TZ=UTC0 diff -Naur Catch2-2.7.2 Catch2-2.7.2a >Catch2-2.7.2.patch

Change the version to the current Catch2 version tarball, of course.


## Build & Install of Catch itself

To build, I run:

    rm -rf Catch2-2.7.2
    tar xf catch2-2.7.2.tar.gz
    patch Catch2-2.7.2/projects/CMakeLists.txt <Catch2-2.7.2.patch
    cd Catch2-2.7.2
    cmake -DCATCH_BUILD_TESTING=off-DCATCH_INSTALL_HELPERS=on -DCATCH_INSTALL_DOCS=on -DCMAKE_INSTALL_PREFIX=... .
    make install

The `patch` command is run only if a patch is defined. The version can change
over time.


# Bugs

Submit bug reports and patches to the packaging on
[snapcatch2 issues on github](https://github.com/m2osw/snapcatch2/issues).

Bugs and patches to the Catch2 environment itself, please make sure
to use the catch2 issue queue:
[catch2 issues on github](https://github.com/catchorg/Catch2/issues)


_This file is part of the [snapcpp project](https://snapwebsites.org/)._
