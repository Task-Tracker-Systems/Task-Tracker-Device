\dir .
\brief Package \ref application_business_rules

\page application_business_rules Application Business Rules
\brief Implement the core logic of the application.

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

Use Case Interactors, Input- and Output Boundaries
==================================================

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

Data Gateway
============

Data gateway represents an interface to a persistent data storage.
Implementation will be specific to the capabilities of the board.

