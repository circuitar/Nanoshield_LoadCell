import sys
import os
import shlex
import subprocess

read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

if read_the_docs_build:
    subprocess.call('doxygen', shell=True)

extensions = ['breathe']
breathe_projects = { 'Nanoshield_LoadCell': 'xml' }
breathe_default_project = "Nanoshield_LoadCell"
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'
project = u'Nanoshield_LoadCell'
copyright = u'2015, Nanoshield_LoadCell'
author = u'Nanoshield_LoadCell'
version = '1.0'
release = '1.0'
language = None
exclude_patterns = ['_build']
pygments_style = 'sphinx'
todo_include_todos = False
html_static_path = ['_static']
htmlhelp_basename = 'Nanoshield_LoadCelldoc'
latex_elements = {
}
latex_documents = [
  (master_doc, 'Nanoshield_LoadCell.tex', u'Nanoshield\_LoadCell Documentation',
   u'Nanoshield\_LoadCell', 'manual'),
]
