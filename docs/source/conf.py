# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import sys
import re
import subprocess
import yaml

from typing import Dict, List, Any
from sphinx.locale import _

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information


project = u'CLIc'
slug = re.sub(r'\W+', '-', project.lower())
author = u'Stephane Rigaud'
copyright = ', '.join(['2020-%Y', author])
language = 'en'
try:
    version = os.environ.get("current_version")
except:
    version = 'unknown'

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
breathe_projects = {'CLIc': '../build/doxygen/xml'}
breathe_default_project = 'CLIc'

static_path = ['_static']
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
    'logo_only': True,
    'navigation_depth': 5,
    'collapse_navigation': False,
    'sticky_navigation': True,
    'version_selector': True,
}

html_logo = "./images/logo_d_small.png"
html_show_sourcelink = True
htmlhelp_basename = slug

html_static_path = ['_static']
html_template_path = ['_templates']

# -- Options for versionning ------------------------------------------------

# get the environment variable build_all_docs and pages_root
build_all_docs = os.environ.get("build_all_docs")
pages_root = os.environ.get("pages_root", "")

# if not there, we dont call this
if build_all_docs is not None:
  # we get the current language and version
  current_language = os.environ.get("current_language")
  current_version = os.environ.get("current_version")

  # we set the html_context wit current language and version
  # and empty languages and versions for now
  html_context: Dict[str, Any] = {
    'current_language' : current_language,
    'languages' : [],
    'current_version' : current_version,
    'versions' : [],
  }

  # and we append all versions and langauges accordingly
  # we treat t he main branch as latest
  if (current_version == 'latest'):
    html_context['languages'].append(['en', pages_root])

  if (current_language == 'en'):
    html_context['versions'].append(['latest', pages_root])

  # and loop over all other versions from our yaml file
  # to set versions and languages
  with open("versions.yaml", "r") as yaml_file:
    docs = yaml.safe_load(yaml_file)

  if docs :
    if (current_version != 'latest'):
        for language in docs[current_version].get('languages', []):
            html_context['languages'].append([language, pages_root+'/'+current_version+'/'+language])
    for version, details in docs.items():
        html_context['versions'].append([version, pages_root+'/'+version+'/'+current_language])
