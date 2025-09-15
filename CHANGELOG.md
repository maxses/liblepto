# Changes for v0.6.1

* [#685] Support for expandable lists
* [#684] Add unit tests for text strings
* Avoid signed division to shrink binary size
* minor fixes

# Changes for v0.6.0

* Cleanup and reformating
* Added more unit tests
* Reworked signals
* Unified CList and CRing.
* Removed printf functions; they are part of libpharao now.
* log: avoid blended mechanism
* list/ring: multiple thread support for producer
* Updated documentation
* Added distance.hpp
* Several minor fixes

# Changes for v0.5.1

* minor changes and fixes

# Changes for v0.5.0

* GitVersion: create files per instance
* minor fixes
* removed tdt; its an seperate library now
* added macro IS_SET()

# Changes for v0.4.2

* fixed compiling on older distributions
* tdt: updated commands/objects/units

# Changes for v0.4.1

* fixed using canpong test

# Changes for v0.4.0

* can: fixed building on older compilers
* git version: added field for bootloader
* TDT: Implemented MMP, multi message packages
* TDT: use shifted EEvent in ELogCode

# Changes for v0.3.0

* moved imago from campo to lepto
* fixed GitVersion
* Dont use __FILE_NAME__ for old GCC compilers
* [#567] clean up log-heartbeat code
* added config defines
* shrink binary size
* minor fixes

# Changes for v0.2.15

* fixed building older versions of Qt and Catch
* dont try to parse tdt db file when not existing
* updated tdt objects, units and events
* tdt-printer: fixed utf8 conversion, updated units
* fixed LEPTO_LOG_SILENT
* can_message_tdt: use reference instead of copy

# Changes for v0.2.14

* updated tdt objects
* added macros for logging with events
* cmake: git-version: use project specific variable names

# Changes for v0.2.13

* print: simplified functions; made printf more complete
* fixed compiling with Qt5
* updated tdt commands
* minor fixes

# Changes for v0.2.12

* TDT: added objects/units
* TDT-DB: added getObjectName()

# Changes for v0.2.11

* added TDT objects

# Changes for v0.2.10

* CRing: fixed data types (fixed USB in libbiwak)
* added TDT objects

# Changes for v0.2.9

* added support for CRC32-MPEG
* CRing: made 'volatile' feature optional
* [#447] fixed calcCrc32()
* TDT: added nmt object 'beckon'
* minor fixes

# Changes for v0.2.8

* updated tdt opjects/units
* minor fixes

# Changes for v0.2.7

* print: support format %p

# Changes for v0.2.6

* minor fixes

# Changes for v0.2.5

* ring: make dropping empty buffer non-fatal

# Changes for v0.2.4

* Tdt::CMessage: added setValue()
* tdt-printer: show texts for air quality
* tdt: added objects

# Changes for v0.2.3

* fixed building on ARM

# Changes for v0.2.2

* [#413] use pointer to CTdtDb in CTdtPrinter

# Changes for v0.2.1

* minor TDT adjustments
* fixed l_vfprintf output

# Changes for v0.2.0

* [#371] reconstructed Tdt namespaces
* rework Tdt protocol
* many minor fixes

# Changes for v0.1.4

* reworked logging
* reworkd CRing classes
* cleanup and fixed compiler warnings
* updated enums for CAN TDT protocol

