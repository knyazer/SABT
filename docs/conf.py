# The `extensions` list should already be in here from `sphinx-quickstart`
extensions = [
    # there may be others here already, e.g. 'sphinx.ext.mathjax'
    'breathe',
    'exhale'
]

# Setup the breathe extension
breathe_projects = {
    "My Project": "./doxygen/xml"
}

breathe_default_project = "My Project"

html_theme = "sphinx_rtd_theme"

# Setup the exhale extension
exhale_args = {
    # These arguments are required
    "containmentFolder":     "./api",
    "rootFileName":          "library_root.rst",
    "doxygenStripFromPath":  "..",
    # Heavily encouraged optional argument (see docs)
    "rootFileTitle":         "Library API",
    # Suggested optional arguments
    "createTreeView":        True,
    # TIP: if using the sphinx-bootstrap-theme, you need
    # "treeViewIsBootstrap": True,
    "exhaleExecutesDoxygen": True,
    "exhaleDoxygenStdin":    "INPUT = ../core/include"
}

# Tell sphinx what the primary language being documented is.
primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'cpp'