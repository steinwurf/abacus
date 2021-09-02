#! /usr/bin/env python
# encoding: utf-8

import os
from waflib.Build import BuildContext


APPNAME = "abacus"
VERSION = "0.0.0"


def build(bld):

    # Build static library if this is top-level, otherwise just .o files
    features = ["cxx"]
    if bld.is_toplevel():
        features += ["cxxstlib"]

    # Fix MSVC error C2131 about expression not being constexpr
    cxxflags = []
    compiler_binary = bld.env.get_flat("CXX").lower()
    if "cl.exe" in compiler_binary:
        cxxflags += ["/constexpr:steps10000000"]

    bld(
        features=features,
        source=bld.path.ant_glob("src/**/*.cpp"),
        target="abacus",
        use=["endian_includes", "bourne"],
        install_path="${PREFIX}/lib",
        cxxflags=cxxflags,
        export_includes=["src"],
    )

    if bld.is_toplevel():

        # Only build tests when executed from the top-level wscript,
        # i.e. not when included as a dependency
        bld.program(
            features="cxx test",
            source=bld.path.ant_glob("test/**/*.cpp"),
            target="abacus_tests",
            use=["abacus", "gtest"],
        )

        bld.program(
            features="cxx",
            source="examples/metrics_simple.cpp",
            target="metrics_simple",
            install_path=None,
            use=["abacus"],
        )
        bld.program(
            features="cxx",
            source="examples/multiple_metrics.cpp",
            target="multiple_metrics",
            install_path=None,
            use=["abacus"],
        )

        sourcepath = bld.path.find_node("src")

        bld.install_files(
            dest="${PREFIX}/include",
            files=sourcepath.ant_glob("**/*.hpp"),
            cwd=sourcepath,
            relative_trick=True,
        )

        bld.install_files(dest="${PREFIX}/", files=bld.path.ant_glob("NEWS.rst"))


class ReleaseContext(BuildContext):
    cmd = "prepare_release"
    fun = "prepare_release"


def prepare_release(ctx):
    """Prepare a release."""

    # Rewrite versions
    with ctx.rewrite_file(filename="src/abacus/version.hpp") as f:

        pattern = r"#define STEINWURF_ABACUS_VERSION v\d+_\d+_\d+"
        replacement = "#define STEINWURF_ABACUS_VERSION v{}".format(
            VERSION.replace(".", "_")
        )

        f.regex_replace(pattern=pattern, replacement=replacement)

    with ctx.rewrite_file(filename="src/abacus/version.cpp") as f:
        pattern = r'return "\d+\.\d+\.\d+"'
        replacement = 'return "{}"'.format(VERSION)

        f.regex_replace(pattern=pattern, replacement=replacement)


def docs(ctx):
    """Build the documentation in a virtualenv"""

    with ctx.create_virtualenv() as venv:

        # To update the requirements.txt just delete it - a fresh one
        # will be generated from test/requirements.in
        if not os.path.isfile("docs/requirements.txt"):
            venv.run("python -m pip install pip-tools")
            venv.run("pip-compile docs/requirements.in")

        venv.run("python -m pip install -r docs/requirements.txt")

        build_path = os.path.join(ctx.path.abspath(), "build", "site", "docs")

        venv.run(
            "giit clean . --build_path {}".format(build_path), cwd=ctx.path.abspath()
        )
        venv.run(
            "giit sphinx . --build_path {}".format(build_path), cwd=ctx.path.abspath()
        )
