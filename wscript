#! /usr/bin/env python
# encoding: utf-8

import os
import shutil
import waflib

APPNAME = "abacus"
VERSION = "7.0.0"


def options(opt):

    if not opt.is_toplevel():
        return

    opt.add_option(
        "--cmake-build-type",
        default="Release",
        help="CMake build type (Release, Debug, RelWithDebInfo, etc.)",
    )
    opt.add_option("--cmake-toolchain", default="", help="Path to CMake toolchain file")

    opt.add_option(
        "--cmake-verbose",
        action="store_true",
        default=False,
        help="Enable verbose output for CMake configure and build",
    )


def configure(cfg):

    if not cfg.is_toplevel():
        return

    cfg.env.BUILD_DIR = cfg.path.get_bld().abspath()

    cmake_cmd = [
        "cmake",
        "-S",
        cfg.path.abspath(),
        "-B",
        cfg.env.BUILD_DIR,
        f"-DCMAKE_BUILD_TYPE={cfg.options.cmake_build_type}",
        "-DCMAKE_POLICY_VERSION_MINIMUM=3.5",
    ]

    if cfg.options.cmake_toolchain:
        cmake_cmd.append(f"-DCMAKE_TOOLCHAIN_FILE={cfg.options.cmake_toolchain}")

    if cfg.options.cmake_verbose:
        cmake_cmd.append("-DCMAKE_VERBOSE_MAKEFILE=ON")

    ret = cfg.exec_command(cmake_cmd, stdout=None, stderr=None)
    if ret != 0:
        cfg.fatal(f"CMake configuration {cmake_cmd} failed with exit code {ret}")


def build(bld):

    if not bld.is_toplevel():
        return

    jobs = str(bld.options.jobs) if hasattr(bld.options, "jobs") else "1"
    cmake_build_cmd = [
        "cmake",
        "--build",
        bld.env.BUILD_DIR,
        "--parallel",
        jobs,
    ]

    if bld.options.cmake_verbose:
        cmake_build_cmd.append("--verbose")

    ret = bld.exec_command(cmake_build_cmd, stdout=None, stderr=None)
    if ret != 0:
        bld.fatal(f"CMake build failed with exit code {ret}")


class Clean(waflib.Context.Context):
    cmd = "clean"
    fun = "clean"


def clean(ctx):
    ctx.logger = waflib.Logs.make_logger("/tmp/waf_clean.log", "cfg")

    build_dir = os.path.join(ctx.path.abspath(), "build")
    build_symlink = os.path.join(ctx.path.abspath(), "build_current")

    # Remove the "build" folder if it exists, with start and end messages
    ctx.start_msg("\nChecking and removing build directory")
    if os.path.isdir(build_dir):
        shutil.rmtree(build_dir)
        ctx.end_msg("Removed")
    else:
        ctx.end_msg("Not found", color="YELLOW")

    # Remove the "build_current" symlink if it exists, with start and end messages
    ctx.start_msg("Checking and removing build_current symlink")
    if os.path.islink(build_symlink):
        os.unlink(build_symlink)
        ctx.end_msg("Removed")
    else:
        ctx.end_msg("Not found", color="YELLOW")


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
