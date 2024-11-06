import os
import subprocess

project = 'CLI-CPP'
copyright = '2024, Swift Software Group'
author = 'Swift Software Group'

# Extensions
extensions = [
    'breathe',
    'sphinx.ext.autodoc',
    'sphinx.ext.viewcode',
]

# Breathe configuration
breathe_default_project = "CLI-CPP"
breathe_projects = {
    "CLI-CPP": os.path.abspath("xml")
}
breathe_default_members = ('members', 'undoc-members')

# Theme
html_theme = 'sphinx_rtd_theme'