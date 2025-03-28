# -*- coding: utf-8 -*-

# -- General configuration ------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.doctest",
    "sphinx.ext.intersphinx",
    "sphinx.ext.todo",
    "sphinx.ext.coverage",
    "sphinx.ext.mathjax",
    "sphinx.ext.ifconfig",
    "sphinx.ext.autosectionlabel",
    "guzzle_sphinx_theme",
    "wurfapi",
]

# wurfapi options - relative to your docs dir
wurfapi = {
    "source_paths": [
        # API
        "../src/abacus/boolean.hpp",
        "../src/abacus/constant.hpp",
        "../src/abacus/description.hpp",
        "../src/abacus/enum8.hpp",
        "../src/abacus/float32.hpp",
        "../src/abacus/float64.hpp",
        "../src/abacus/info.hpp",
        "../src/abacus/int32.hpp",
        "../src/abacus/int64.hpp",
        "../src/abacus/kind.hpp",
        "../src/abacus/max.hpp",
        "../src/abacus/metric.hpp",
        "../src/abacus/metrics.hpp",
        "../src/abacus/min.hpp",
        "../src/abacus/name.hpp",
        "../src/abacus/parse_metadata.hpp",
        "../src/abacus/protocol_version.hpp",
        "../src/abacus/to_json.hpp",
        "../src/abacus/uint32.hpp",
        "../src/abacus/uint64.hpp",
        "../src/abacus/unit.hpp",
        "../src/abacus/version.hpp",
        "../src/abacus/view.hpp",
    ],
    "recursive": False,
    "include_paths": ["../src"],
    "parser": {
        "type": "doxygen",
        "download": True,
        "warnings_as_error": True,
        "patch_api": [
            {
                "selector": "abacus::STEINWURF_ABACUS_VERSION",
                "key": "inline",
                "value": True,
            }
        ],
        "collapse_inline_namespaces": ["abacus::STEINWURF_ABACUS_VERSION"],
    },
}

# Add any paths that contain templates here, relative to this directory.
templates_path = []

# The suffix(es) of source filenames
source_suffix = ".rst"

# The master toctree document.
master_doc = "index"

# General information about the project.
project = "Abacus"
copyright = "2021, Steinwurf"
author = "Steinwurf"

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.
version = ""
# The full version, including alpha/beta/rc tags.
release = ""

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This patterns also effect to html_static_path and html_extra_path
# exclude_patterns = ["rst_templates"]

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = "sphinx"

# -- Options for HTML output ----------------------------------------------

try:
    import guzzle_sphinx_theme

    html_theme = "guzzle_sphinx_theme"
    html_theme_path = guzzle_sphinx_theme.html_theme_path()
except ImportError:
    print(
        "Unable to import the used theme.\n"
        "Please install requirements.txt before building"
    )
    pass

html_sidebars = {
    "**": ["logo.html", "logo-text.html", "globaltoc.html", "searchbox.html"]
}

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.

html_theme_options = {
    "h1_background_color": "d7eef4ff",
    "sidebar_hover_color": "2c89a0ff",
    "logo_text_background_color": "37abc8ff",
    "link_color": "37abc8",
    "link_hover_color": "87cddeff",
    "code_color": "37abc8ff",
    "target_highlight_color": "37abc8ff",
    "highlighted_color": "37abc8ff",
    "search_form_focus_color": "37abc8ff",
}

# The name of an image file (relative to this directory) to use as a favicon of
# the docs.  This file should be a Windows icon file (.ico) being 16x16 or 32x32
# pixels large.
#
html_logo = "images/abacus.svg"

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = []

# Add any extra paths that contain custom files (such as robots.txt or
# .htaccess) here, relative to this directory. These files are copied
# directly to the root of the documentation.
#
# html_extra_path = []

# If true, the reST sources are included in the HTML build as _sources/name
html_copy_source = False

# If true, links to the reST sources are added to the pages.
html_show_sourcelink = False
