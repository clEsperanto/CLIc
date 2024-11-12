.. image:: ./images/logo_w.png
   :alt: CLIc logo
   :align: center

******************
CLIc Documentation
******************

CLIc is a C++ library for image processing on the GPU. It serves as a backend for the clEsperanto project. The library primarily relies on OpenCL (and other GPU languages) for computation but is designed to have other backend in the future.
This documentation will focus on the library design and code.

For usage documentation, please refer to one of the following links:

- |:snake:| - `pyclesperanto documentation <https://clesperanto.github.io/pyclesperanto/>`__
- |:coffee:| - `clEsperantoJ documentation <https://clesperanto.github.io/clEsperantoj/>`__  (WIP)
- |:rocket:| - `clEsperanto documentation <https://clesperanto.github.io/clEsperanto/>`__  (WIP)

.. note::

   This project is under active development, along with its documentation. Any feedback and contributions are welcome.

.. toctree::
   :caption: 🛠️ Developer Documentation
   :maxdepth: 1

   doc/compilation
   doc/tests
   doc/usage
   doc/functions
   doc/contributing

.. toctree::
   :caption: 📘 API Doxygen
   :maxdepth: 1

   api/backend
   api/device
   api/array
   api/tiers
   genindex

📖 Citation
-----------

There is no publication for CLIc yet. If you use it in your research or project, please use the zenodo DOI to cite it.

.. image:: https://zenodo.org/badge/DOI/10.5281/zenodo.6075345.svg
   :target: https://doi.org/10.5281/zenodo.6075345
   :alt: DOI: 10.5281/zenodo.6075345
   :align: center 