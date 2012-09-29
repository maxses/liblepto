/**
  \page configuration Configuration
  \section logging Logging

  \subsection subsection1 CONFIG_LEPTO_LOG_PRETTYPRINT
  Contain file names and line numbers in log messages

  \subsection CONFIG_LEPTO_LOG_SILENTT
  Completely disable logging. No text is stored in the binary.

  \subsection CONFIG_LEPTO_LOG_DOWNSIZE
  Use small log string instead of long log string. The macro 'LDS' has to be
  used to give two different string for the same output.

  Example:
  ```
  qWarn( LDS( "CNRH", "Could not read hardware id." ) );
  ```
  \subsection CONFIG_LEPTO_LOG_USE_ANSI
  Use colored strings via ansi codes.

  \subsection LEPTO_LOG_DEBUG
  Show all ```lDebug()``` outputs. By default the debug output is completely
  removed from the binary and does not waste binary size.
  This define can set on top of any compile unit to enable debug output for
  this file. (Don't try to do this with any other configuration define)

  \subsection CONFIG_LEPTO_LOG_SILENT
  Remove any log output from the binary for very small binary sizes.

  \page logging Logging
  Similar functions like in Qt are available
  \code
  lInfo( <format>, <args> );
  lWarn( <format>, <args> );
  lCritical( <format>, <args> );
  lFatal( <format>, <args> );
  \endcode

  They are printing the strings directly to stdout (optionally along with
  filename and line number). When you use libbiwak, the terminal functions from
  liblepto will be overloaded.


*/
