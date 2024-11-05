Backends
========

Backend Manager
---------------

The Backend Manager is a singleton class that manages backend selection and initialization.
It is designed to allow only one backend to be active at a time. Users can select
backends at runtime, during the initialization phase.
By default, the library will try to initialize the OpenCL backend.
If initialization fails, it will attempt to initialize the CUDA backend.
If both fail, the library will throw an exception.

.. warning::

    Switching between backends during runtime is not supported and may lead to undefined behavior.

.. doxygenclass:: cle::BackendManager
    :members:


Backend Class
-------------

The `Backend` class is an abstract class defining the interface for the different hardware backends supported by CLIc.
Inherited classes implement the necessary low-level functions to operate the hardware.
This abstraction enforces a design pattern allowing switching between different hardware backends without changing the high-level code.
Currently, the library supports OpenCL and CUDA backends, with the possibility of adding more in the future.

.. note::

    This class operates closest to the hardware, and casual developers should not need to interact with it.

.. warning::

    The `CUDABackend` class is operational but not yet fully released.

.. doxygenclass:: cle::Backend
    :members:

OpenCL Backend
~~~~~~~~~~~~~~~

.. doxygenclass:: cle::OpenCLBackend
    :members:

CUDA Backend
~~~~~~~~~~~~

.. doxygenclass:: cle::CUDABackend
    :members:
