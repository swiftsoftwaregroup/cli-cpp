import os
import subprocess

# -- Project information -----------------------------------------------------
project = 'CLI-CPP'
copyright = '2024, Swift Software Group'
author = 'Swift Software Group'

# -- General configuration ---------------------------------------------------
extensions = [
    'breathe',
    'sphinx.ext.autodoc',
    'sphinx.ext.viewcode',
]

# Breathe configuration
breathe_default_project = "CLI-CPP"
breathe_projects = {
    "CLI-CPP": "_build/xml"
}

# Theme configuration
html_theme = 'sphinx_rtd_theme'