Device
======

A ``Device`` is a processing unit that can execute OpenCL kernels. 
It is mainly a GPU, but it can also be a CPU or a simulation of a device (e.g. `pocl`, `oclgrind`).
It is a good practice to consider a device as a mini computer with its own memory with which you can communicate through the OpenCL API.
This class holds a set of information allowing the identification of a specific hardware for future communication with it during execution. 


The class has no processing functions; it is mainly used to store necessary information required to identify the hardware.
The detection and the communication is managed by ``Backend`` class. 

Users should not need to interact with the ``Device`` class extensively, except during the initialization of the library or when using multiple devices for computation.

.. doxygenclass:: cle::Device
    :members:
