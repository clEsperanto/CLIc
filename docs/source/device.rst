Device
======

A ``Device`` is defined as an object class. This class holds a set of information allowing identification of a specific hardware, typically a GPU, in the system. Hardware detection and communication rely on the ``Backend`` class, as presented below. 
The class has no major processing functions; it is mainly used to store necessary information required to communicate with the hardware.

Users should not need to interact with the ``Device`` class extensively, except during the initialization of the library or when using multiple devices for computation.

.. doxygenclass:: cle::Device
    :members:
