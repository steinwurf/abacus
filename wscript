#! /usr/bin/env python
# encoding: utf-8

import os
import shutil
import waflib

APPNAME = "abacus"
VERSION = "7.0.0"


def options(ctx):
    ctx.load("cmake")


def configure(ctx):

    ctx.load("cmake")


def build(ctx):

    ctx.load("cmake")


def clean(ctx):

    ctx.load("cmake")

    # Set the default clean paths
    ctx.clean_paths = ["build", "build_current"]


def protogen(ctx):
    # check if protec is available
    protoc_location = "build_current/resolve_symlinks/protobuf/protoc"
    if not os.path.isfile(protoc_location):
        ctx.fatal(
            "protoc not found. Make sure to configure waf with `--with_protoc` to include protoc in build."
        )
        return
    try:
        shutil.rmtree("src/abacus/protobuf")
    except:
        pass
    os.mkdir("src/abacus/protobuf")

    ctx.exec_command(
        f"(./{protoc_location} --cpp_out ./src --proto_path .. ../abacus/protobuf/*.proto)"
    )

    ctx.exec_command(
        "echo 'DisableFormat: true\nSortIncludes: false' > src/abacus/protobuf/.clang-format"
    )


class ReleaseContext(waflib.Build.BuildContext):
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
