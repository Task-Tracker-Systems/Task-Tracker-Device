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

\note The description will use the term '*level*' or '*policy* level'.
      It is used according to the description given in \cite CleanArchitecture p. 184 ff..

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

\note The description will use the term '*package*'.
      In this context a package groups related implementation of functionality of a common policy level.
      It does not refer to a specific artifact type in a programming language.

### Interpretation of the Dependency Rule

For this software architecture the Dependency Rule (see \cite CleanArchitecture p. 203) is applied on source code dependencies between *packages*.
That means that source code may only depend on source code within its own package or on source code in packages of higher-level policies.

### Dependency Injection

For a higher level package to use an implementation from a lower level, interfaces shall be used.
At runtime the higher level package needs to use the actual object realizing the interface.
But this shall be done without revealing the implementation details to the higher level package.
For this, non-member "getter"- or "factory"- functions (Dependency Injector Function; DIF) shall be declared in the higher level.
Using functions as dependency injector allows to call the constructor of the implementation with run-time arguments.
The return type is a reference to an object of the base class (interface).
The actual object is created and kept in the lower layer package.
In general a DIF may return different objects which have to be allocated in dynamic memory.

The declaration of a DIF must reside in the (highest) level which calls it.
Its definition must reside at the level which implements the dependency or at a lower level.
But it should not reside in the same unit as the dependency.
As the definition of the DIF depends on its usage it should not pollute the implementation's unit.
This simplifies reusability and testability of the implementation.

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

Also this definition of *components* from the [C4 software architecture model](https://c4model.com/) by Simon Brown applies (\cite CleanArchitecture p. 314) applies:

> A grouping of related functionality behind a nice clean interface, which resides inside an execution environment like an application.

Component Architecture
----------------------

The following diagram sketches the plan for the component architecture.
The elements in the diagram are placeholders for components which will be defined in a generic way further below.

The component architecture shall support the objectives of the software architecture.
Mechanisms for dependency injection are omitted in this diagram for readability.

\dotfile component_architecture.dot "component architecture for this software"

The rectangles with bold border are packages.
The label at the bottom of each package is the name of the package.
Each package consists of one or more components.
All components in this diagram are exemplarily.

### Enterprise Business Rules

\dotfile component_entities.dot

This is the package with the policies of highest level.
This package contains policies which are valid not only for the software of this application.
Instead they define rules which apply to more than one application within their organization.

The enterprise business rules are a distinct package as they have a specific reason for changing:
Something changes at the organization level.
In general they should be very stable.

#### Entities

The following can be entities:

 - The format *task* data is interchanged between applications.
 - Requirements for calculation of task duration (i.e. how to deal with changing time zones).
 - Requirements for labeling tasks.

### Application Business Rules

\dotfile component_uci.dot

The application business rules package contains everything which is specific for the application.
That is all policies or rules which will only change if the core logic of the application changes.

The enterprise business rules have a different reason to change, as they are defined on the level of the organization.
Thus they are not part of the application package.

Everything which could possibly change without changing the behavior of the use cases is not part of the application package.
This is in particular:

 - The (graphical) user interface.
 - Realization of storage.
 - Any input or output interfaces to or from the software system.
 - Any hardware adapters, hardware abstraction layers, processor abstraction layers.
   This includes peripheral drivers.
 - Any software development kit or library which is not exclusive for this application.

#### Use Case Interactors, Input- and Output Boundaries

Use case interactors implement use cases as defined in \cite CleanArchitecture p. 192:

> A use case is a description of the way that an automated system is used.
> It specifies the input to be provided by the user, 
> the output to be returned to the user, 
> and the processing steps involved in producing that output.
> A use case describes *application-specific* business rules [...].

Note, that all input data (a request) to a use case interactor is obtained outside the use case interactor 
(typically by a "Controller").
The output data of a use case interactor (a response) is passed to an output boundary
(typically by a "Presenter").

#### Data Gateway

Data gateway represents an interface to a persistent data storage.
Implementation will be specific to the capabilities of the board.

### Board Adapters (interface adapters)

\dotfile component_board.dot

\note The "board" means the sum of hardware of the device which can be interfaced - directly or indirectly - by the software.
      Examples are the processor, Bluetooth modules, memory devices, displays and buttons.
      
The board adapter is a special kind of interface adapters.
It is *the* adapter to the user.
This package contains components which act as a bridge between the application and "the outside world" (anything outside the system).
Any interaction with a user (whether via a human-machine interface or indirectly using a machine-to-machine interface)
passes through the board.

The (implementation of the) components of the board adapter usually depend on the concrete hardware.
The application business rules may be realized with different variants of the board.
A change of board should not result in changes other than in the board adapter.

Only the board adapter knows the hardware and its capabilities.
Thus only the board adapter can decide best on how to specifically interact with the user.

The board adapter may realize concepts as

 - Controllers (see \cite CleanArchitecture p. 207) using input boundaries
 - Presenters (see \cite CleanArchitecture p. 208) implementing output boundaries
 - View Models and Views (see \cite CleanArchitecture p. 212 f.) used to present information to the user

Whether those concepts are realized as dedicated components may depend on the concrete board the board adapter interfaces.

### 3rd Party Adapters (interface adapters)

\dotfile component_libadapter.dot

This package is a special kind of interface adapters.
It contains components which act as a bridge between software's own code and external code.

"Own" code is authored and fully controlled by the organization. 
"External" code is code which is provided by other parties.
That code is used by the "own" code.
External code may be incorporated as source code or (pre-)compiled code.

3rd party adapters shield the own code from the external code.
That is, any changes to external code shall only impact the 3rd party adapters.
That includes for example if one library is exchanged by another library which servers the same purpose.

The software development kits (or frameworks) should not be directly used by the own code (for example the Arduino environment).
Third party (external) code may depend on the SDKs.
In case the own code needs to use a SDK, using a 3rd party adapter should be aimed at.
The reason for this is that this simplifies moving the own code to a different SDK.

Further References
------------------

* \cite TheCleanArchitecture
