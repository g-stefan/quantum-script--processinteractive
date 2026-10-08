# Quantum Script Extension ProcessInteractive — Documentation

`quantum-script--processinteractive` is the **child process extension of
Quantum Script**. You load it with `Script.requireExtension("ProcessInteractive")`.
It lets a script start a program, write to its standard input and read
its output while it runs. When the program ends, the script can get its exit
code. Windows and Linux use the same API.

The `Shell` extension can run a command (`Shell.system`, `Shell.execute`),
but the child shares the script's console and its output cannot be read
back. `ProcessInteractive` connects the child to the script with **pipes**:

- **Capture the output of a command.** `ProcessInteractive.run(cmd)` returns
  everything the program wrote (standard output and standard error) as a
  string.
- **Handle the output line by line, while it runs.**
  `ProcessInteractive.runLn(cmd, fn)` calls `fn` for every line, for example
  to show the progress of a build or to stop at the first error.
- **Drive a program.** A `ProcessInteractive` object can `execute` a command,
  `write` / `writeLn` input to it, wait for output (`waitToRead`), `read` /
  `readLn` it, and `close`, `join` or `terminate` it. After the end it can
  return the exit code (`getReturnValue`).
- **Binary data.** `readToBuffer` / `writeFromBuffer` move bytes through a
  `Buffer` (from `quantum-script--buffer`, which loads automatically).

```
scripts: fabricare build scripts, quantum-script .js, tools
quantum-script--processinteractive  <-- this extension: ProcessInteractive
quantum-script--buffer               (Buffer, for readToBuffer / writeFromBuffer)
quantum-script                       (Executive, Variable, Context)
xyo-system                           (XYO::System::ProcessInteractive: pipes, CreateProcess / fork + execvp, ConPTY)
xyo-encoding, xyo-multithreading, xyo-data-structures, xyo-managed-memory, xyo-platform
```

## Why it exists

| Need | What `ProcessInteractive` gives |
|------|---------------------------------|
| Read the version / status / JSON printed by a tool (`git`, `uname`, `github-release`, `fabricare`) | `ProcessInteractive.run(cmd)` returns the whole output as a string |
| Show the output of a long command as it comes, react to some lines | `ProcessInteractive.runLn(cmd, function(line) { ...; return true; })` |
| Answer a program that reads commands from its input | `execute`, `writeLn`, `waitToRead`, `read`, `close` |
| Know if the program succeeded | `close()`, then `getReturnValue()` |
| Stop a program that hangs | `terminate(milliseconds)` |
| No console window and no stray console output on Windows | the child runs in a hidden pseudo console (ConPTY) by default |

`fabricare` registers this extension as internal, so build scripts use it
without a DLL. `fabricare` itself uses it, for example in the platform
detection (`uname -s`, `lsb_release`) and the release scripts.

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the extension | `Script.requireExtension("ProcessInteractive");` | also loads `Buffer` |
| All the output of a command | `var out = ProcessInteractive.run("git rev-parse HEAD");` | `undefined` if it cannot start; no exit code |
| Every line, as it comes | `ProcessInteractive.runLn(cmd, function(line) { return true; });` | `fn` **must return true** to keep going |
| New process object | `var p = new ProcessInteractive();` | `ProcessInteractive()` works too; no process yet |
| Start | `p.execute("program arg1 \"arg 2\"")` | **no shell**: `cmd /c ...` or `/bin/sh -c "..."` for shell syntax |
| Send input | `p.write("text")`, `p.writeLn("text")` | `writeLn` adds `"\r\n"` |
| Wait for output | `p.waitToRead(microseconds)` | `1` data, `0` timeout, `-1` no more output |
| Read output | `p.read()`, `p.readLn()` | stdout and stderr are **one** stream |
| End and release | `p.close()` | closes the input, **waits for the end**, discards unread output |
| Exit code | `p.getReturnValue()` | once the process ended: after `close()`, `join()`, or `isRunning()` → `false` |
| Kill | `p.terminate(ms)` then `p.close()` | `ms` is **milliseconds** |
| Windows pseudo console | `p.useConPTY(false)` before `execute` | on by default; no effect on Linux |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, a first script, fabricare scripts, register it in a C++ host, static builds |
| [Process model](process-model.md) | How the child is started on Windows and Linux, the pipes, blocking, end of input, exit codes, ConPTY, lifetime |
| [Script API](script-api.md) | Every function: arguments, exact behavior, edge cases, return values |
| [Recipes](recipes.md) | Capture output, follow a build, talk to an interactive program, timeouts, exit codes, binary data |
| [C++ API](cpp-api.md) | `registerInternalExtension`, `VariableProcessInteractive`, using it from native code |
| [API reference](reference.md) | Every script and C++ symbol on one page |

The underlying C++ class `XYO::System::ProcessInteractive` is documented in
the `xyo-system` repository, `docs/processes.md`. Quantum Script itself (the
language, `Script.requireExtension`, embedding) is documented in the
`quantum-script` repository, `docs/`.

## Source map

```
source/XYO/QuantumScript.Extension/ProcessInteractive.hpp            umbrella header (Library.hpp)
source/XYO/QuantumScript.Extension/ProcessInteractive.Amalgam.cpp    the whole extension in one translation unit
source/XYO/QuantumScript.Extension/ProcessInteractive/
    Dependency.hpp                       <XYO/QuantumScript.hpp>, export macro
    Library[.hpp/.cpp]                   initExecutive, registerInternalExtension, every native function
    Library.js                           ProcessInteractive.runLn, written in Quantum Script
    Library.Source.cpp                   Library.js as a C string (generated by fabricare/make.prepare.js)
    Context.hpp                          ProcessInteractiveContext: the prototype per thread
    VariableProcessInteractive[.hpp/.cpp]  the script value type (wraps XYO::System::ProcessInteractive)
    Copyright / License / Version        library metadata
fabricare/make.prepare.js                file-to-cs: Library.js -> Library.Source.cpp
test/test.01.cpp                         C++ host registering Console, Buffer and ProcessInteractive as internal
test/test.01.js                          runs a command, shows a spinner while it runs, prints its output
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--processinteractive/`](../.claude/skills/quantum-script--processinteractive/SKILL.md).
Claude Code loads it automatically inside this repository. To use it in the
projects that use `ProcessInteractive` (fabricare scripts, Quantum Script
tools, other extensions), copy the folder to `~/.claude/skills/`.
