# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import sys
import re
import subprocess

from sphinx.locale import _

sys.path.insert(0, os.path.abspath("../../"))

# Get the TAG_NAME environment variable
tag_name = os.getenv('RELEASE_TAG_NAME')
if tag_name:
    release = tag_name
else:
    # Get the current Git branch name
    try:
        branch_name = subprocess.check_output(['git', 'rev-parse', '--abbrev-ref', 'HEAD']).strip().decode('utf-8')
        release = branch_name
    except subprocess.CalledProcessError:
        release = 'unknown'


# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = u'CLIc'
slug = re.sub(r'\W+', '-', project.lower())
author = u'Stephane Rigaud'
copyright = f'2020-%Y, {author}'
language = 'en'
version = release

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'breathe',
    'sphinx.ext.intersphinx',
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    'sphinx.ext.mathjax',
    'sphinx.ext.viewcode',
    'sphinx_rtd_theme',
    'sphinxemoji.sphinxemoji',
    ]
breathe_projects = {'CLIc': os.environ.get('CLIC_DOCS', './xml')}
breathe_default_project = 'CLIc'

templates_path = ['_templates']
source_suffix = '.rst'
exclude_patterns = ["build", "_build", "Thumbs.db", ".DS_Store"]
cpp_index_common_prefix = [
    'cle::',
    'cle::tier1::',
    'cle::tier2::',
    'cle::tier3::',
    'cle::tier4::',
    'cle::tier5::',
    'cle::tier6::',
    'cle::tier7::',
    'cle::tier8::',
    ]
gettext_compact = False
master_doc = 'index'
suppress_warnings = ['image.nonlocal_uri']
pygments_style = 'default'

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'logo_only': False,
    'navigation_depth': 5,
    'display_version': True,
    'collapse_navigation': False,
}

html_logo = "./images/logo_d_small.png"
html_show_sourcelink = True
htmlhelp_basename = slug

html_static_path = ['_static']

