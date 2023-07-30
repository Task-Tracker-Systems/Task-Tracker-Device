Software Architecture
=====================

Objectives of the Software Architecture
---------------------------------------

The objectives of the software architecture are:

 - Designed for testability: The software parts have interfaces which allow to test that part individually.
 - A use case driven approach: The architecture shall be centered on use cases and not depend on implementation details.
 - Flexible structure: The software shall not be hard to adjust to changing hardware, development frameworks or external/third party libraries.

Achieving a Flexible Structure
------------------------------

For maintaining flexibility the software architecture shall adhere to:

 - The 'Acyclic Dependencies Principle' in [TCA] p. 112 ff..:  
   > Allow no cycles in the component dependency graph
 - The 'Dependency Rule' in [TCA] p. 203 ff.:  
   > Source code dependencies must point only [...] toward higher-level policies.

For this software the implementation details (lower level) are:

 - The board (hardware) with the processor.
 - The software development framework (SDK).
 - Other external/3rd party libraries.

The application (higher level policies) shall not depend on those implementation details.

In contrast the software will commit to the following dependencies:

 - The code may depend on a compiler which is capable to compile C++17 as defined in ISO/IEC 14882:2017.
 - The code may depend on an implementation of the C++ Standard Library as defined in ISO/IEC 14882:2017.
  
Implementing a Plug-in Architecture
-----------------------------------

The software is structured in software layers.

Implementation details are in the lower layers.

For a higher level to use an implementation from a lower level, interfaces shall be used.
Now, at some point, the higher level needs to use the actual object.
But this shall be done without revealing the implementation details to the higher level.
For this, in the higher level, non-member getter- or factory functions shall be declared.
Those will return the object of interest.
The return type is a reference to an object of the base class.
The actual object is created and kept (in the sense of definition) in the lower layer.

Using functions allow to call the constructor of the implementation with run-time arguments.
In general, those functions may return different objects which have to be allocated in dynamic memory.
In our case, often only one instance of each class is required.
As this depends on the actual application, the factory function is not part of the module definition or declaration or of its interface.

In case the implementation detail can not be changed:
For example because it is a stable module/component/sw part, or 3rd party or otherwise immutable.
Then interface adapters must be implemented.
Those interface adapters realize the higher layer interface while using the stable component.
The usage may for example be realized using the delegation pattern.

The factory functions may be bundled for each component in a dedicated module.
That may consist of a single file, or multiple files in a dedicated module directory.

Components are parts of the software which may be exchanged or changed as a whole.
For example a library (including any interface adaptors) can be considered as one component.
Another example of a component is the software for the board (the device's hardware used by the software).
As a different board may be used for the same application.

The factory functions for each component shall be in a dedicated namespace.
In order to avoid false include dependencies, the factory function declarations and definitions must be in separate files.
The declarations are the interface required by the higher level.
Those files must be in the directory of the higher level.
The definitions of the factory functions must be in the lower level directory.

References
----------

* 'The Clean Architecture' by R.C. Martin aka Uncle Bob. He has also written [an article](https://blog.cleancoder.com/uncle-bob/2012/08/13/the-clean-architecture.html).
