FastArduino Coding Guidelines
=============================

Preamble
--------
The following coding guidelines apply to FastArduino core and devices support source code.
Provided examples do not have to follow these guidelines.

Note that these are guidelines, hence they should be followed whenever possible, but if some guidelines are judged
counter-productive in some situations, then they can be relaxed with a comment specifying some justification.

Files
-----
All FastArduino source code files names are in `lower_case`, with `.h` or `.cpp` extensions.

Directories are also named in `lowercase` (no underscore).

Using namespaces is mandatory for all API.

Naming
------
Throughout FastArduino library code, the following naming conventions apply:

1. Namespaces: always written in `lower_case` (possibly including underscores to separate different words); one exception to this rule is when a namespace is defined to be used like an "extensible" `enum class` and thus contains only `static const` fields, all of the same type, e.g. `board::PWMPin`.
2. Template arguments: always written in `UPPER_CASE`, often ending with an underscore (`UPPER_CASE_`).
3. Macros: always written in `UPPER_CASE` for "public" macros (part of FastArduino official API) or `UPPER_CASE_` for internal macros (not part of the API)
4. struct, enum, union, class types: always written as `CamelCase`, possibly with a trailing underscore for internal (not public) types.
5. enum values: always `UPPER_CASE`
6. Constant values (e.g. `static const...`): written in `UPPER_CASE` or `UPPER_CASE_`.
7. functions, methods: always written in `lower_case`; uppercase letters may be used for official measurement units though (e.g. `voltage_mV()`); when one function exists in 2 flavours, one `synchronized` and one not `synchronized`, then the not synchronized flavour bears the same name as the synchronized function, with a trailing undercore added, e.g. `Queue::push()` and `Queue::push_()`.
8. `private` fields of a class: always written as `lower_case_` with a trailing underscore; uppercase letters may be used for official measurement units though.
9. Local variables are always written as `lower_case`; uppercase letters may be used for official measurement units though.
10. `static constexpr` functions: not yet defined; hesitating between `UPPER_CASE` and `lower_case`, currently FastArduino has methods with both conventions.
11. Types alias with `using` should always be `UPPER_CASE`.

Currently, FastArduino has no automatic check of these guidelines, hence control must be performed manually before merging Pull Requests.

Coding Style
------------
All other conventions for coding styles are directly defined within project's `.clang-format` file rules.

Currently FastArduino uses clang-format 6.0 because this is the latest supported version on my Linux distribution (Fedora); however, its current settings are not fully satisfactory and are not up to my expectations for FastArduino, in particular, I am not happy with the results obtained from the current settings on the following topics:
- spaces used instead of tabs when aligning `\` at end of lines (used for macros)
- spaces used in addition to tabs when aligning 2 lines (e.g. template or function arguments)
- line breaking of ternary operator after both `?` and `:` where I would like breaking only after `:`
- template arguments not broken at one per line if I want so
- buggy value alignment in assignments
