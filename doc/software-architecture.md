Software Architecture
=====================

Objectives of the Software Architecture
---------------------------------------

The objectives of the software architecture are:

 - Designed for testability: The software parts have interfaces which allow to test those parts individually.
 - A use case driven approach: The architecture shall be centered on use cases and not depend on implementation details.
 - Flexible structure: The software shall not be hard to adjust to changing hardware, development frameworks or external/third party libraries.

Achieving a Flexible Structure
------------------------------

For maintaining flexibility the software architecture shall adhere to:

- The 'Acyclic Dependencies Principle' in \cite CleanArchitecture p. 112 ff..:  
  > Allow no cycles in the component dependency graph.

- The 'Dependency Rule' in \cite CleanArchitecture p. 203 ff.:  
  > Source code dependencies must point only [...] toward higher-level policies.


For this software the implementation details (lower level) are:

 - The board (hardware) including the processor.
 - The software development kit (SDK) or [framework][PIO_FRAMEWORK].
 - Other external/3rd party libraries.

The application (higher level policies) shall not depend on those implementation details.

In contrast the software will commit to the following dependencies:

 - The code may depend on a compiler which is capable to compile C++17 as defined in ISO/IEC 14882:2017.
 - The code may depend on an implementation of the C++ Standard Library as defined in ISO/IEC 14882:2017.

[PIO_FRAMEWORK]: https://github.com/platformio/platformio-docs/blob/5ae4fa7e895f5d3a04514314b1af31b37469d274/frameworks/index.rst "List of frameworks written by PlatformIO."

Implementing a Plug-in Architecture
-----------------------------------

\note The description will use the term '*level*' or '*policy* level'.
      It is used according to the description given in \cite CleanArchitecture p. 184 ff..

\note The description will use the term '*package*'.
      In this context a package groups related implementation of functionality of a common policy level.
      This does not refer to a specific artifact type in a programming language.

### Dependency Injection

For a higher level package to use an implementation from a lower level, interfaces shall be used.
Now, at some point, the higher level package needs to use the actual object realizing the interface.
But this shall be done without revealing the implementation details to the higher level package.
For this, in the higher level package, non-member "getter"- or "factory" functions shall be declared.
These declarations can be interpreted as an interface.
Those functions will return the object of interest.
The return type is a reference to an object of the base class (interface).
The actual object is created and kept (in the sense of definition) in the lower layer package.

Using functions allows to call the constructor of the implementation with run-time arguments.
In general, those functions may return different objects which have to be allocated in dynamic memory.
In our case, often only one instance of each class is required.
As the specific realization of dependency injection is specific for its usage, the factory function shall be declared and defined in
separate files of the interface and its realization.
It must reside in a package of a level equal or lower than the package containing the realization of the interface.

### Interface Adapters

In case an implementation detail can not be changed:
For example because it is a stable software part, or 3rd-party or otherwise immutable.
Then interface adapters must be implemented.
Those interface adapters realize the higher layer interface while using the stable component.
The usage may for example be realized using the delegation pattern.

\dotfile dependency_injection_interface_adapter.dot "example of dependency injection and interface adapter"

### Components

Components are parts of the software which may be exchanged or changed as a whole.
For example a library (including any interface adapters) can be considered as one component.
Another example of a component is the software for adapting to the board (the device's hardware used by the software).
As a different board may be used for the same application.

Also this definition of *components* from the [C4 software architecture model](https://c4model.com/) by Simon Brown applies (\cite CleanArchitecture p. 314):

> A grouping of related functionality behind a nice clean interface, which resides inside an execution environment like an application.

Further References
------------------

* \cite TheCleanArchitecture
