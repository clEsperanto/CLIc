Array Class
===========

The ``Array`` class is the primary data structure class of the library.
It stores the pointer to the memory space in the ``Device``, along with the necessary information to access and manipulate it.
The class is intended to be used through smart pointers defined as ``Array::Pointer`` and should not be accessed directly.
Hence, the creation of an ``Array`` object is done through ``Array::create()`` function.

The class provides a set of methods that describe the array, such as its ``size``, ``width``, ``dtype``, among others.
It also provides methods to write/read data to and from the host, and to perform operations on the array, such as ``fill`` and ``copy``.

This object is the main input and output of the library functions.

.. doxygenclass:: cle::Array
    :members:
