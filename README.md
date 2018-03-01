# putils
Phiste's C++ Utilities

### Stand-alone utility classes

* [AbstractFactory](AbstractFactory.md) Base class for a factory able to create any type in a given typelist calling ``make<T>()``.
* [AStar](AStar.md) An implementation of the AStar pathfinding algorithm.
* [CellularAutomaton](CellularAutomaton.md) Simple cellular automaton implementation
* [CLI](CLI.md) Simple and extensible Command-Line Interface.
* [Csv](Csv.md) Simple and extensible Command-Line Interface.
* [Directory](Directory.md) An abstraction to Unix and Windows directories, which lets you iterate through a directory and get information about each file.
* [JSON](json.md) JSON lexer and pretty-printer
* [NamedPipe](NamedPipe.md) Abstraction to named pipes
* [Observable](Observable.md) A minimalistic (but greatly sufficient) implementation of the Observer pattern.
* [OperatorForward](OperatorForwarder.md) Mix-in class which lets you forward all operators to a given member.
* [Point](Point.md) Simple abstraction to a 2D or 3D point/rectangle.
* PooledObject: A mix-in class that turns any type into a pooled type
* [Process](Process.md) Abstraction to a process (useful to run system commands and send them input/get their output)
* [RAII](RAII.md) A class template able to perform RAII on any type.
* [Timer](Timer.md) Simple timer based on std::chrono.

### Packages

* [Connection](connection/README.md)
* [Devices](devices/README.md)
* [Http](http/README.md) (enabled with the PUTILS_BUILD_HTTP CMake option)
* [Lua](lua/README.md) (enabled with the PUTILS_BUILD_LUA CMake option)
* [Python](python/README.md) (enabled with the PUTILS_BUILD_PYTHON CMake option)
* [Mediator](mediator/README.md) (enabled with the PUTILS_BUILD_MEDIATOR CMake option)
* [Meta](meta/README.md)
* [PluginManager](pluginManager/README.md)
* [POgre](pogre/README.md) (enabled with the PUTILS_BUILD_POGRE CMake option)
* [PSE](pse/README.md) (enabled with the PUTILS_BUILD_PSE CMake option)
* [Reflection](reflection/README.md)

### Function-only headers

* casts.hpp: static and dynamic casts for std::unique_ptr
* chop.hpp: function removing any newline and carriage return characters from the end of a string.
* concat.hpp: function taking any number of parameters and concatenating them into a string.
* erase.hpp: function taking a container and a value and erasing the value from the container.
* fwd.hpp: macro which calls std::forward for the correct type
* get_current_dir.hpp: function which returns the current working directory as a string
* get_last_error_as_string.hpp: function for Windows
* go_to_bin_dir.hpp: function which takes `argv[0]` as parameter and changes the current working directory to that containing the executed file.
* hash.hpp: Pre-defined hashers for enums and pairs
* lock_and_run.hpp: function which takes a mutex, a function and its parameters. Creates a scoped lock for the mutex and runs the function.
* rand.hpp: function using C++ 11's random number generation facilities
* read_stream.hpp: function which extracts an entire std::istream to a std::string
* regex.hpp: utilities to easily construct and match regexes through literal operators
* remove_if.hpp: function taking a container and a predicate and calling std::remove_if
* same_sign.hpp: function indicating whether two scalars have the same sign
* sign.hpp: function returning -1, 0 or 1 depending on the sign of the parameter
* to_string.hpp: uses a std::stringstream to serialize an object
* traits.hpp: type traits
* url.hpp: URL encoder and decoder
