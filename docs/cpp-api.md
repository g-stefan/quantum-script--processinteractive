# C++ API

This page is for hosts that embed Quantum Script and for extensions that
work with `ProcessInteractive` values. Read the `quantum-script`
repository's `docs/embedding.md` and `docs/writing-extensions.md` first.
Native functions, `Variable` and `TPointer` work the same way here. To run
a process from C++ only, use `XYO::System::ProcessInteractive` directly
(`xyo-system`, `docs/processes.md`). This extension is the script binding
for that class.

## Headers and namespace

```cpp
#include <XYO/QuantumScript.Extension/ProcessInteractive.hpp>    // Library.hpp: register / init
#include <XYO/QuantumScript.Extension/ProcessInteractive/VariableProcessInteractive.hpp>   // the value type
#include <XYO/QuantumScript.Extension/ProcessInteractive/Context.hpp>   // ProcessInteractiveContext (rarely needed)

using namespace XYO::QuantumScript;
using Extension::ProcessInteractive::VariableProcessInteractive;
```

The namespace is `XYO::QuantumScript::Extension::ProcessInteractive`. The
export macro is `XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT`. It
is empty when `XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_LIBRARY` is
defined, as in static builds. The umbrella header includes only
`Library.hpp`. Include `VariableProcessInteractive.hpp` yourself when you
need the type.

## Registering the extension

```cpp
void Extension::ProcessInteractive::registerInternalExtension(Executive *executive);
void Extension::ProcessInteractive::initExecutive(Executive *executive, void *extensionId);
```

- `registerInternalExtension` registers `"ProcessInteractive"` as an internal
  extension. Call it from the host's init callback, together with
  `Extension::Buffer::registerInternalExtension` (see
  [Getting started](getting-started.md#4-register-it-in-a-c-host)).
- `initExecutive` is the extension's init function. The engine runs it when
  a script first requires `ProcessInteractive` in a thread. It does the
  following:
  1. Sets the extension name, info (license text) and version, and marks the
     extension public.
  2. Creates the `ProcessInteractive` global function and its prototype
     (`ProcessInteractiveContext`).
  3. Runs `Script.requireExtension("Buffer")`.
  4. Registers the native functions with `setFunction2`.
  5. Compiles `Library.js` (`librarySource`), which defines
     `ProcessInteractive.runLn`.

  Do not call it directly.
- The DLL build also exports
  `extern "C" void quantumScriptExtension(Executive *, void *)`, which
  forwards to `initExecutive`. This is the entry point
  `Script.requireExtension` looks up in `quantum-script--processinteractive.dll`.

## VariableProcessInteractive

```cpp
class VariableProcessInteractive : public Variable {
	public:
		XYO::System::ProcessInteractive value;    // the process, used directly by the native functions

		static Variable *newVariable();           // new object, no process
		String getVariableType();                 // "ProcessInteractive"
		Variable *instancePrototype();            // ProcessInteractive.prototype of this thread
		bool toBoolean();                         // always true
		String toString();                        // "ProcessInteractive"
		void activeDestructor();                  // value.close(): waits for the process
};
```

- The type uses `TMemoryPoolActive` (`xyo-managed-memory`). When the last
  reference goes, `activeDestructor` runs `value.close()`, which closes the
  input and **waits for the process to end**.
- `newVariable()` needs the extension to be loaded in the current thread,
  because the prototype comes from the per thread context.
- The type does not override `clone`. A value passed to another thread by
  the `Thread` extension does not arrive as a process object.

## Using it from a native function

Take a process object as an argument:

```cpp
#include <XYO/QuantumScript.Extension/ProcessInteractive/VariableProcessInteractive.hpp>

static TPointer<Variable> myExitCode(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	if (!TIsType<VariableProcessInteractive>(arguments->index(0))) {
		throw(Error("invalid parameter"));
	};
	XYO::System::ProcessInteractive &process = ((VariableProcessInteractive *)(arguments->index(0)).value())->value;
	process.close();
	return VariableNumber::newVariable(process.getReturnValue());
};
```

Return a running process to the script:

```cpp
static TPointer<Variable> myStart(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	TPointer<Variable> retV(VariableProcessInteractive::newVariable());
	XYO::System::ProcessInteractive &process = ((VariableProcessInteractive *)retV.value())->value;
	process.useConPTY(true);
	if (!process.execute((arguments->index(0))->toString())) {
		return Context::getValueUndefined();
	};
	return retV;
};
```

In your extension:

- Depend on `"quantum-script--processinteractive"` /
  `"quantum-script--processinteractive.static"` in `fabricare.json`.
- In your `initExecutive`, run
  `executive->compileStringX("Script.requireExtension(\"ProcessInteractive\");");`
  before anything creates process objects.

## Adding a method to this extension

1. Write it in `Library.cpp` as
   `static TPointer<Variable> processInteractiveName(VariableFunction *, Variable *this_, VariableArray *arguments)`.
   Check `TIsType<VariableProcessInteractive>(this_)` first and throw
   `Error("invalid parameter")` otherwise.
2. Register it in `initExecutive` with
   `executive->setFunction2("ProcessInteractive.prototype.name(args)", processInteractiveName);`.
   For a static function, use `"ProcessInteractive.name(args)"`.
3. A function that is easier to write in script goes into `Library.js`.
   `fabricare make` regenerates `Library.Source.cpp` (`fabricare/make.prepare.js`,
   `file-to-cs`).
4. Update `README.md`, `docs/script-api.md`, `docs/reference.md` and the
   skill in `.claude/skills/quantum-script--processinteractive/SKILL.md`.
