\dir .
\brief Package \ref enterprise_business_rules

\page enterprise_business_rules Enterprise Business Rules
\brief Defines rules valid for the overall organization

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

