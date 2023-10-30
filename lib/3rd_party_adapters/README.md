\dir .
\brief Package \ref third_party_adapters

\page third_party_adapters 3rd Party Adapters
\brief Servers as a bridge to external code.

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

