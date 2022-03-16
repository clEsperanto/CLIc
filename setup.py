from skbuild import setup

setup(
    name="pyclic",
    version="0.5.2",
    packages=['pyclic'],
    package_dir={'': 'python'},
    cmake_install_dir='python/pyclic')

# When building extension modules `cmake_install_dir` should always be set to the
# location of the package you are building extension modules for.
# Specifying the installation directory in the CMakeLists subtley breaks the relative
# paths in the helloTargets.cmake file to all of the library components.