# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import sys
import re

from sphinx.locale import _

sys.path.insert(0, os.path.abspath("../../"))

# Open the CMakeCache.txt file and search for the project version
with open('../../CMakeLists.txt', 'r') as f:
    for line in f:
        if 'project' in line:
            start = line.find('VERSION') + 8
            end = line.find(')', start)
            release = line[start:end].strip()
            break

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = u'CLIc'
slug = re.sub(r'\W+', '-', project.lower())
author = u'Stephane Rigaud'
copyright = f'2024, {author}'
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
html_context = {}

if not 'READTHEDOCS' in os.environ:
    html_static_path = ['_static/']
    html_js_files = ['debug.js']

    # Add fake versions for local QA of the menu
    html_context['test_versions'] = list(map(
        lambda x: str(x / 10),
        range(1, 100)
    ))

# html_logo = "demo/static/logo-wordmark-light.svg"
html_show_sourcelink = True
# html_favicon = "demo/static/favicon.ico"
htmlhelp_basename = slug

latex_documents = [
  ('index', '{0}.tex'.format(slug), project, author, 'manual'),
]

man_pages = [
    ('index', slug, project, [author], 1)
]

texinfo_documents = [
  ('index', slug, project, author, slug, project, 'Miscellaneous'),
]

html_static_path = ['_static']

# Extensions to theme docs
def setup(app):
    from sphinx.domains.python import PyField
    from sphinx.util.docfields import Field

    app.add_object_type(
        'confval',
        'confval',
        objname='configuration value',
        indextemplate='pair: %s; configuration value',
        doc_field_types=[
            PyField(
                'type',
                label=_('Type'),
                has_arg=False,
                names=('type',),
                bodyrolename='class'
            ),
            Field(
                'default',
                label=_('Default'),
                has_arg=False,
                names=('default',),
            ),
        ]
    )
