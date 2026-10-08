# Getting started

## 1. Build and install

The extension is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. First install `quantum-script`
(with everything below it: `xyo-system`, `xyo-encoding`, ...),
`quantum-script--console` and `quantum-script--buffer` to the SDK. Then,
from the repository root:

```bash
fabricare make       # build into output/ (make.prepare first turns Library.js into Library.Source.cpp)
fabricare test       # build and run test/test.01 (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

`test/test.01.js` runs `pwsh -command "Start-Sleep -s 5"`, so the test needs
PowerShell 7 (`pwsh`) on the `PATH`.

The build produces two libraries:

| Project                                     | Kind                                | Use it when                                         |
|---------------------------------------------|-------------------------------------|-----------------------------------------------------|
| `quantum-script--processinteractive`        | DLL / shared library (`dll-or-lib`) | scripts run by `quantum-script`, or a host using the engine DLL |
| `quantum-script--processinteractive.static` | static library, static CRT          | self-contained hosts built with `quantum-script.static` |

After `fabricare install`, the SDK `bin` folder holds
`quantum-script--processinteractive.dll` (Windows) /
`libquantum-script--processinteractive.so` (Linux) next to
`quantum-script.exe`. `Script.requireExtension("ProcessInteractive")` finds
it there.

## 2. A first script

```javascript
Script.requireExtension("Console");
Script.requireExtension("ProcessInteractive");

// All the output at once
var commit = ProcessInteractive.run("git rev-parse HEAD");
if (Script.isUndefined(commit)) {
	throw "git not found";
};
Console.writeLn("commit: " + commit.trim());

// Line by line, as the program writes them
ProcessInteractive.runLn("git log --oneline -5", function(line) {
	Console.write("  " + line);       // line keeps its "\n" / "\r\n"
	return true;                      // false (or no return) stops reading
});

// A process object: exit code
var p = new ProcessInteractive();
if (p.execute("git diff --quiet")) {
	p.close();                        // waits for the end
	Console.writeLn(p.getReturnValue() == 0 ? "clean" : "modified");
};
```

Run it with:

```bash
quantum-script first-process.js
```

`Script.requireExtension("ProcessInteractive")` first looks for an external
`quantum-script--processinteractive` library: the file as named, then each
include path folder (next to the interpreter, next to the script). If none
is found, it uses an internal extension registered by the host. Loading the
extension twice does nothing. A missing extension throws
`Unable to open "ProcessInteractive"`. The extension loads `Buffer` itself,
so the `Buffer` global exists afterwards.

> Commands are **not** run through a shell. `echo`, `dir`, pipes, redirection
> and `.cmd` scripts need `cmd /c ...` on Windows or `/bin/sh -c "..."` on
> Linux. See [Process model](process-model.md#starting-the-program).

## 3. fabricare build scripts

`fabricare` registers `ProcessInteractive` as an internal extension and loads
it before running the build scripts. They can call it directly:

```javascript
// fabricare/version.js (any fabricare script)
var gitHead = ProcessInteractive.run("git rev-parse --short HEAD");
if (Script.isString(gitHead)) {
	messageAction("build of " + gitHead.trim());
};
```

## 4. Register it in a C++ host

A host that embeds Quantum Script registers the extension (and `Buffer`,
which it requires) as internal extensions in its init callback. This is what
`test/test.01.cpp` does:

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/Buffer.hpp>
#include <XYO/QuantumScript.Extension/ProcessInteractive.hpp>

using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	Extension::Buffer::registerInternalExtension(executive);
	Extension::ProcessInteractive::registerInternalExtension(executive);
};

int main(int cmdN, char *cmdS[]) {
	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		if (!ExecutiveX::executeString(
		        "Script.requireExtension(\"Console\");"
		        "Script.requireExtension(\"ProcessInteractive\");"
		        "Console.write(ProcessInteractive.run(\"git --version\"));")) {
			printf("%s\n", (ExecutiveX::getError()).value());
			printf("%s", (ExecutiveX::getStackTrace()).value());
		};
		ExecutiveX::endProcessing();
	};
	return 0;
};
```

Registering only makes the extension *available*. Scripts still call
`Script.requireExtension("ProcessInteractive")`. With the DLL build of the
engine, `requireExtension` prefers an external
`quantum-script--processinteractive.dll` on the include path over the
internal one. Use `Script.requireInternalExtension("ProcessInteractive")` to
force the internal one.

In the host's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--processinteractive"
	]
}
```

## 5. Static builds

For a self-contained executable, depend on the static variants and the
static CRT:

```json
{
	"name": "my-host.static",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script.static",
		"quantum-script--console.static",
		"quantum-script--buffer.static",
		"quantum-script--processinteractive.static"
	],
	"crt": "static"
}
```

`quantum-script--processinteractive.static` exports the define
`XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_LIBRARY` to its consumers.
This define makes `XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT`
empty and leaves out the `quantumScriptExtension` DLL entry point. A static
host must register both `Buffer` and `ProcessInteractive` with
`registerInternalExtension` (section 4), because it cannot load external
DLLs.

## 6. Threads

Each thread that runs scripts has its own engine and its own
`ProcessInteractive` prototype (`ProcessInteractiveContext` is a per thread
singleton). Every thread therefore loads the extension itself. Use a
`ProcessInteractive` object only in the thread that created it. To run
several processes in parallel, either:

- use one object per thread (the `Thread` / `Job` extensions), or
- from one thread, poll several objects with `waitToRead(0)` or short
  timeouts (see [Recipes](recipes.md#several-processes-from-one-thread)).
