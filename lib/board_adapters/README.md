\dir .
\brief Package \ref board_adapters

\page board_adapters Board Adapters
\brief Implements board specific software.

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

