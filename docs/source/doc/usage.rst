How to use the library
======================

It is not adviced to use this library directly in your project as it does not provide a simple API for development.
Indeed, it is a middle-level code on which rely other user-oriented libraries (pyclesperanto, clesperantoJ, `etc.`).

However, as a developer, you may have to use this library to implement new operations or to integrate it into your own project.
In the following, we provide a simple example to show how to use this library.

Initialization
~~~~~~~~~~~~~~~

First, we need to initialize the ``Backend``. This is done through the singleton pattern ``BackendManager``.

.. code-block:: c++

    #include "cle.hpp"
    /* ... */
    cle::BackendManager::getInstance().setBackend("opencl");

The ``cle::BackendManager::getInstance()`` function returns the singleton instance of the ``BackendManager``.
We can then call the ``setBackend()`` function to set the backend we want to use.
Here we use the OpenCL backend. This is the only backend available at the moment.

Once set, the backend will initialize all ressources available on the machine, and make them available to the library.
Here, by ressources, we mean devices (CPU, GPU, etc.) that are available on the system. 
If no devices are available, the backend will not be able to initialize and will throw an error.

Now that the backend is initialized, we can access it through the ``getBackend()`` function of the singleton as such:

.. code-block:: c++

    auto backend = cle::BackendManager::getInstance().getBackend();

With the exception of the device initialisation, most operation involving the backend will be done in the background and are not meant to be accessed by the user.

Get a device
~~~~~~~~~~~~

Operations are done to be executed by a compatible device (CPU, GPU, etc.).
Hence, before we can continue, we need to select a device on which we will run our operation. This is done through the ``getDevice()`` or ``getDeviceByIndex()`` functions of the backend.
Here, we can specify the device we want using its name or index, as well as filter the devices by type.

.. code-block:: c++

    auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

By default, we get the first available device of any type possible (CPU or GPU). 
If multiple devices have the same name, we can identify them by their index.

For example, if we have two GPUs named ``"RTX 2090``, we can select the second one with the following code:

.. code-block:: c++

    auto device = cle::BackendManager::getInstance().getBackend().getDeviceByIndex(1, "gpu");


Create, Write, and Read an Array
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``cle::Array`` is a class that represents an array on the device.
Similarly to most data structures in C++, it as several constructors and getters.
Setters, however, are limited to avoid discrepancies between the data on the device and the information stored in the object.

To create an array, we need to provide the device, the size of the array and the data type of the array.

.. code-block:: c++

    auto gpu_array = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);

Here we create a 3D array of size 10x5x3 of type ``float`` on the device.
``cle::mType::BUFFER`` is the memory type of the array, here a BUFFER (see the `Array class <https://clesperanto.github.io/CLIc/array.html>`__ for more information on the memory types and methods).

Here, we only created a memory space on the device, similarly as a ``malloc()`` in `C`.
We need now to write data into the device array.

.. code-block:: c++

    gpu_array->writeFrom(array.data());

Where ``array`` is a ``std::vector`` or ``std::array`` of the same size and type as the ``gpu_array`` we are trying to write into.
We can do the oposite operation and read the ``gpu_array`` into a ``std::vector`` or ``std::array``.

.. code-block:: c++

    gpu_array->readTo(array.data());

.. note::

    The ``read()`` and ``write()`` functions are blocking functions.
    They will wait for the operation to be done before returning.

.. warning::

    As we are operating at a low-level API, no proper verification is done on the size or type of the array.
    We assume here that the developer knows what he is manipulating.
    Wrong size or type will result in undefined behavior.

Execute an Operation
~~~~~~~~~~~~~~~~~~~~

Now that we have an array on the device, we can execute an operation on it.
For this example, we will use the ``AddImageAndScalar`` operation.

.. code-block:: c++

    auto gpu_result = cle::tier1::add_image_and_scalar(device, gpu_array, nullptr, 5);

``gpu_result`` is a new array on the device that is the result of the operation.

.. note::

    The majority of the operations in the library are requiring to have twice the memory space on the device.
    More advance operations may require more memory space depending on the number of intermediate values needed.
