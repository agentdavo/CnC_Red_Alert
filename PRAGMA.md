# Legacy Pragmas

This project was originally built with the Watcom C/C++ and Borland compilers. Numerous `#pragma` directives are scattered throughout the source to tweak warnings, calling conventions and other compiler behavior. When porting to C11 we aim to either remove or replace these directives.

## Watcom specific pragmas

The file `CODE/watcom.h` defined many `#pragma warning` lines to disable compiler warnings in Watcom. These covered issues such as virtual destructors, truncated assignments and unreferenced parameters. Modern compilers either do not support these options or provide equivalent warning flags. The include of `watcom.h` has been removed from all source files. The header remains for reference while we audit the code.

Other Watcom pragmas include the `#pragma aux` directives in assembly helper headers (`jshell.h`, `COORD.CPP`, etc.) to describe register usage. These will eventually be rewritten in portable C or inline assembly.

## Borland and lint pragmas

A few modules use `#pragma warn` to silence Borland warnings (e.g., `#pragma warn -sig` in `SHA.CPP`). There are also pragmas controlling structure packing and intrinsic functions inside `memcheck.h`. These pragmas are mostly ignored by modern compilers but are documented here for historical completeness.

## Pruning strategy

- Remove includes of `watcom.h` and the obsolete `#pragma warn` directives.
- Retain specialized pragmas such as `#pragma pack` or `#pragma aux` until their code is replaced with portable equivalents.
- Document unusual pragmas and their purpose in this file as they are encountered.

Over time all platform-specific pragmas should disappear as we migrate the codebase to standard C11 and portable libraries.
