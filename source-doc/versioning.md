# Version Numbering

@section schm       Numbering Scheme

UberPOV builds have version numbers of the form `A.XY.Z.B`[`-PRE`[`.ID`]], where:

  - `A` is the UberPOV major version number, and is intended to indicate radical changes.
  - `XY.Z` are the major, minor and sub-minor version numbers of the official POV-Ray release on which this UberPOV
    version is based; e.g. `37.1` indicates POV-Ray version `3.7.1` as the basis.
  - `B` is an additional UberPOV-specific sub-minor version number, to allow for incrementing the version number while
    the underlying POV-Ray version remains the same.

  - `PRE` is a pre-release identifier, such as `alpha`, `beta` or `rc`, indicating that it is a forerunner to, but may
    still differ in functionality from, what will ultimately become the official version `X.Y.Z`.
  - `ID` is a numeric value uniquely distinguishing the pre-release in question from other pre-releases.

Unofficial builds should have version numbers of the form `A.XY.Z.B-`[`PRE`[`.ID`]`.`]`unofficial`.


@section chng       What To Change

When updating a version number, the following files need to be changed accordingly:

  - `unix/VERSION`
  - `source/base/branch.h`
