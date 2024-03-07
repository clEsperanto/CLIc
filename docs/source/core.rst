Core
====

Device class
------------

We define a `Device` as an object class. The class hold a set of information allowing to identify a specific hardware, usually a GPU, in the system. 
This hardware detection and communication rely on the `Backend` class presented below. The class as no major processing function, it is mainly used to store
necessary information required to communicate with the hardware.

Users should not be required to interact with the `Device` class much, except at the initialization of the library or when using multiple device for computation.

.. doxygenclass:: cle::Device
    :members:


Array class
-----------

The `Array` class is the main data structure class of CLIc. It stores the pointer to the memory space in the `Device` as well as the necessary information to access it and manipulate it.
The class is designed to be used through smart pointers defined as `Array::Pointer` and should not be used directly.  

.. doxygenclass:: cle::Array
    :members:


Backend class
-------------

The `Backend` class is an abstract class defining the interface for the different hardware backends supported by CLIc. The inherited classes are implementing the necessary low-level functions
to operate the hardware. The abstract enforce a design pattern allowing to switch between different hardware backends without changing the high-level code. Currently, the library support
OpenCL and CUDA backends, more backends can be added in the future.

.. note:: 

    This is the closest level to the hardware and the user nor casual developers should not be required to interact with this class.

.. warning:: 

    The `CUDABackend` class is operational but not yet fully available released yet.

.. doxygenclass:: cle::Backend
    :members:  

OpenCL Backend
~~~~~~~~~~~~~~

.. doxygenclass:: cle::OpenCLBackend
    :members:  

CUDA Backend
~~~~~~~~~~~~

.. doxygenclass:: cle::CUDABackend
    :members:  

Backend Manager
---------------

The Backend Manager is a singleton class that manage the backend selection and initialization. The class is designed only allowing one backend to be active at a time. The user can select 
backends at runtime, during the initialisation phase. Switching between backends during runtime is not supported and may lead to undefined behavior. By default, the library will try to
initialize the OpenCL backend, if it fails, it will try to initialize the CUDA backend. If both fail, the library will throw an exception.

.. doxygenclass:: cle::BackendManager
    :members:


