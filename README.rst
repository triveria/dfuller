.. _README:

dfuller
===============================================

CURRENTLY NOT MAINTAINED


Overview
--------

A device agnostic DFU library


Features
--------

The main features of dfuller are:
 * A
 * B
 * C


Getting Started
---------------

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.
See deployment for notes on how to deploy the project on a live system.

Prerequisites
~~~~~~~~~~~~~

What things you need to install the software and how to install them.

::

    Give examples


Building the C++ project
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

    $ cmake -S . -B build
    $ cmake --build build


Running demos
~~~~~~~~~~~~~

You can find demonstrations on how to use the library in the demos folder.


Installing
~~~~~~~~~~

A step by step series of examples that tell you how to get a development env running.

Say what the step will be

::

    Give the example

And repeat

::

    until finished

End with an example of getting some data out of the system or using it for a little demo

Running the tests
-----------------

.. code-block:: bash

    $ cd build
    $ ctest

Break down into end to end tests
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Explain what these tests test and why

::

    Give an example

And coding style tests
~~~~~~~~~~~~~~~~~~~~~~

Explain what these tests test and why

::

    Give an example
    

    
Deployment
----------

Add additional notes about how to deploy this on a live system


Contributing
------------

Please read :ref:`CONTRIBUTING <CONTRIBUTING>` for details on our code of conduct, and the process for submitting pull requests to us.


Code Style
----------

This project uses clang-format to automatically format the code and keep a consistent code style.
To use it, use the following command:

.. code-block:: bash

    $ clang-format -i -style=file src/*.cpp include/*/*.hpp


Versioning
----------

We use `SemVer <http://semver.org/>`__ for versioning.
To increment the version number use bump2version

.. code-block:: bash

    $ bump2version major

This increments the version from, e.g. 0.1.0 to 1.0.0
See .bumpversion.cfg for details on what happens.


Authors
-------

See the list of :ref:`AUTHORS <AUTHORS>` to see who helped creating dfuller.


License
-------

This project is licensed under the MIT license - see the :ref:`LICENSE <LICENSE>` file for details


Credits
-------

This package was created with `Cookiecutter <https://github.com/audreyr/cookiecutter>`__ and the `nmusolino/cookiecutter-cpplib-tools <https://github.com/nmusolino/cookiecutter-cpplib-tools>`__ project template.
