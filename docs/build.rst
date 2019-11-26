..  _build:

================================================
Build Library
================================================

It is simple to build the library.

(1) Copy ExternallibConfig.cmake.in.example to ExternallibConfig.cmake.in and modify the path of dependent libraries.

(2) Run the following build script.

.. code-block:: Shell

    build_type=Debug # Debug/Release/Profile
    bash shell/build ${build_type}

We use the following versions.

*    gcc-7
*    g++-7
*    googletest release-1.8.1 (Set INSTALL_GTEST in googletest to OFF)
*    spdlog v1.2.1
*    boost v1.65.1
*    eigen v3.3.7
*    abseil master 7b46e1d (HEAD has bug as of 2019.01.07)
*    swig v3.0.12
*    java 1.8

