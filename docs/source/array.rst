Array
=====

The ``Array`` class is the primary data structure class of CLIc. It stores the pointer to the memory space in the ``Device``, along with the necessary information to access and manipulate it.
The class is intended to be used through smart pointers defined as ``Array::Pointer`` and should not be accessed directly.

.. doxygenclass:: cle::Array
    :members:
