---
name: quantum-script--processinteractive
description: >-
  How to use the Quantum Script ProcessInteractive extension
  (quantum-script--processinteractive), the child process with pipes loaded
  with Script.requireExtension("ProcessInteractive"): ProcessInteractive.run(cmd,
  useConPTY) (whole output as a string, undefined if it cannot start),
  ProcessInteractive.runLn(cmd, fn, useConPTY, lineMaxLn) (fn per line, must
  return true to continue), new ProcessInteractive() with execute, write /
  writeLn, waitToRead (1 / 0 / -1), read / readLn, readToBuffer /
  writeFromBuffer, isRunning, join, close, terminate(ms), getReturnValue,
  useConPTY; no shell (cmd /c, /bin/sh -c), stdout + stderr in one pipe,
  blocking and end of input rules, exit codes, Windows ConPTY;
  the C++ side (VariableProcessInteractive, registerInternalExtension). Use
  when writing or reviewing Quantum Script or fabricare .js code that runs a
  program and reads its output or exit code, C++ code that includes
  <XYO/QuantumScript.Extension/ProcessInteractive.hpp>, a fabricare.json
  depending on "quantum-script--processinteractive", or when working inside
  the quantum-script--processinteractive repository.
---

# quantum-script--processinteractive

This is the child process extension of Quantum Script. The rules of the
`quantum-script` skill (the language and its differences from JavaScript)
and of the `xyo-system` skill (the underlying
`XYO::System::ProcessInteractive`) apply here too. Purpose: **run a program
from a script with pipes to its input and output**. Use it to capture
command output, follow a build line by line, drive an interactive program,
or get an exit code. Windows and Linux use the same API. `Shell.system` /
`Shell.execute` cannot read the output back.

Full documentation lives in `docs/` of the quantum-script--processinteractive
repository (`X:\Storage\XYO\Gitea\CPP\quantum-script--processinteractive\docs`
on this machine):

- README
- getting-started
- **process-model**: no shell, pipes, blocking, end of input, close / join /
  terminate, exit codes, ConPTY
- **script-api**: the exact behavior of every function
- **recipes**: timeout, interactive program, several processes, prompts
- cpp-api
- reference

Read the matching page when you need more than this summary. When in doubt,
read `source/XYO/QuantumScript.Extension/ProcessInteractive/Library.cpp`
(~400 lines) and `Library.js` (`runLn`).

## Script API

```javascript
Script.requireExtension("ProcessInteractive");   // also loads Buffer; fabricare scripts have it already

var out = ProcessInteractive.run("git rev-parse HEAD");   // String (stdout+stderr), undefined if not started
ProcessInteractive.run(cmd, false);                       // Windows: without pseudo console (default true)

ProcessInteractive.runLn(cmd, function(line) {            // this == the process object
	Console.write(line);                                   // line keeps "\n" / "\r\n"
	return true;                                           // MUST return truthy, else stops after this line
}, true, 4096);                                           // useConPTY (missing = true), lineMaxLn (missing = 32768)
                                                          // returns false if not started, true otherwise

var p = new ProcessInteractive();                         // or ProcessInteractive(); no process yet
p.useConPTY(true);                                        // Windows, before execute; default true; no-op on Linux
p.execute("program arg \"two words\"");                   // true / false; NO SHELL
p.write("text");                                          // bytes written; 0 if no process / input closed
p.writeLn("text");                                        // appends "\r\n" on EVERY platform
p.waitToRead(100000);                                     // microseconds -> 1 data, 0 timeout, -1 no more output, NaN bad arg
p.read();                                                 // what is available (<= 32768), "" at end
p.readLn();                                               // one line with its end, undefined at end; waits for the whole line
p.readToBuffer(buf, ln);                                  // waits for ln bytes (<= buf.size) or end; sets buf.length
p.writeFromBuffer(buf);                                   // buf.length bytes
p.isRunning();                                            // no wait
p.terminate(1000);                                        // MILLISECONDS: WM_CLOSE / SIGTERM, wait, then kill
p.close();                                                // close stdin, WAIT for the end, discard unread output, release
p.getReturnValue();                                       // exit code once ended (close / join / isRunning false), 0 before
p.join();                                                 // wait for the end, stdin stays open (prefer close)
ProcessInteractive.isProcessInteractive(p);               // true; typeof(p) == "ProcessInteractive"
```

## Hard rules

1. **No shell.** On Windows `execute` uses `CreateProcessA` with the command
   line as is. On Linux the line is split with `ShellArguments` (`"..."`
   groups words, `\"` is a quote) and run with `execvp`. Built-ins (`echo`,
   `dir`, `cd`), pipes, `>`, `&&`, globs and `.cmd` / `.bat` need
   `cmd /c ...` / `/bin/sh -c "..."`. Keep Windows command lines ASCII
   (ANSI API). The working directory and environment are inherited, so
   `Shell.chdir` / `runInPath` first.
2. **Program not found:** Windows `execute` → `false`, `run` → `undefined`.
   On Linux, `execute` → `true`, `run` → `""` and the exit code is `2`.
   Check both the return value and the exit code.
3. **stdout and stderr are one pipe**, interleaved as written. To separate
   them, redirect in a shell. Bytes are not transcoded.
4. **Exit code once the process ended**: after `close()` or `join()`, or
   once `isRunning()` returned `false`. It is `0` before that, and right after
   `terminate` the process may not have ended yet. Pattern:
   `p.close(); var code = p.getReturnValue();`. `run` / `runLn` give no exit
   code. On Windows this needs xyo-system 8.0.0 build 18 or later; older
   builds set the code only in `close()`.
5. **`close()` discards output not yet read**, and it is the only way to end
   the child's input. Read until `waitToRead` returns `-1` *before*
   `close()`. A filter that answers at end of input (`sort`, `wc`,
   `findstr`) cannot be fed with `write` and read back. Give it a file or
   use a shell redirection instead.
6. **`run` / `runLn` never write or close stdin until the end**: a program
   that reads stdin hangs them.
7. **`waitToRead` returns `-1` at the end, and `-1` is truthy.** Test `> 0`
   for data and `>= 0` to keep looping. `while (p.waitToRead(t))` never
   ends. Invalid / missing argument → `NaN`.
8. **Blocking:** `read` waits for *some* output and returns it. `readLn`
   waits for a *whole line*, so it blocks forever on a prompt without a
   newline (`"> "`, `"Password: "`); use `waitToRead` + `read`.
   `readToBuffer` waits for the *full count*. Call `waitToRead` first
   whenever the child may be idle.
9. **`runLn` callback must return truthy.** A function with no `return`
   stops after the first line. After a stop, `runLn` still **waits for the
   process to end**: call `this.terminate(0)` before `return false` to kill
   it.
10. **`useConPTY` missing = `true`** for both `run` and `runLn` (Windows
    pseudo console). Pass `false` to start without one.
11. **`terminate(n)` is milliseconds.** `0` / missing kills at once. It does
    not wait for the kill or release the process: call `close()` after. Killed exit code: `0` on Windows, `128 + signal` on
    Linux (`137`, `143`).
12. **Always `close()`.** `execute` on a used object closes (waits for) the
    previous process. An unclosed object is closed by the garbage collector
    with a blocking wait. `close()` is safe twice and on an object with no
    process.
13. **`join()` keeps stdin open**: a child waiting for input never ends.
14. **`writeLn` sends `"\r\n"`** even on Linux. Use `write(s + "\n")` for
    tools that dislike `"\r"`.
15. **Read while it runs.** A child that fills the pipe (4 KB Windows,
    64 KB Linux) blocks until read. `run`, `runLn`, `join` and `close`
    drain it, but a manual loop that only waits on `isRunning()` without
    reading can deadlock.
16. **One thread per object.** Each thread requires the extension itself.
    For parallel processes, poll several objects with short `waitToRead`
    timeouts from one thread, or use one object per thread.

## Patterns

```javascript
// output + exit code
var p = new ProcessInteractive();
if (p.execute(cmd)) {
	var output = "", state;
	while ((state = p.waitToRead(1000000)) >= 0) {
		if (state > 0) { output += p.read(); };
	};
	p.close();
	var code = p.getReturnValue();
};

// timeout: count idle waits, then p.terminate(1000); p.close();
// interactive: writeLn commands, end with the program's own "exit", read until -1, close
// prompt: loop waitToRead(50000) + read() until buffer.indexOf("> ") >= 0
```

`docs/recipes.md` has complete versions: timeout, interactive programs,
prompts, several processes, progress spinner and binary output.

## C++

```cpp
#include <XYO/QuantumScript.Extension/ProcessInteractive.hpp>                              // register / init
#include <XYO/QuantumScript.Extension/ProcessInteractive/VariableProcessInteractive.hpp>   // value type
using namespace XYO::QuantumScript;
using Extension::ProcessInteractive::VariableProcessInteractive;

Extension::Buffer::registerInternalExtension(executive);               // host init callback, Buffer is required
Extension::ProcessInteractive::registerInternalExtension(executive);   // scripts still requireExtension

if (!TIsType<VariableProcessInteractive>(arguments->index(0))) { throw(Error("invalid parameter")); };
XYO::System::ProcessInteractive &process = ((VariableProcessInteractive *)(arguments->index(0)).value())->value;
TPointer<Variable> r(VariableProcessInteractive::newVariable());       // new object, no process
```

- `activeDestructor` calls `value.close()`, which waits for the process.
- Static build: `quantum-script--processinteractive.static` exports
  `XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_LIBRARY`, which gives an
  empty export macro and no `quantumScriptExtension` entry point. Register
  it, and `Buffer`, as internal.
- For C++ only, use `XYO::System::ProcessInteractive` directly (its `runLn`
  takes a C callback, `lineMaxLn` default 4096).

## Working in this repository

- Build with `fabricare make`, `fabricare test` and `fabricare install` (see
  the `fabricare` skill). `fabricare test` runs `test/test.01`, which
  registers Console, Buffer and ProcessInteractive as internal and runs
  `test/test.01.js`, a `pwsh` sleep with a spinner, so it needs `pwsh`.
  Install `quantum-script`, `quantum-script--console` and
  `quantum-script--buffer` first.
- Native methods live in `ProcessInteractive/Library.cpp` as
  `static TPointer<Variable> processInteractiveName(VariableFunction *, Variable *this_, VariableArray *arguments)`.
  They check `TIsType<VariableProcessInteractive>(this_)` first and are
  registered in `initExecutive` with
  `executive->setFunction2("ProcessInteractive.prototype.name(args)", fn)`.
  Static functions use `"ProcessInteractive.name(args)"`.
- `ProcessInteractive.runLn` is written in script in `Library.js`.
  `fabricare/make.prepare.js` (`file-to-cs`) turns it into
  `Library.Source.cpp` (`librarySource`), compiled at the end of
  `initExecutive`. Edit `Library.js`, not the generated file.
- When you add a method, update `README.md`, `docs/script-api.md`,
  `docs/reference.md` and this skill.
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX headers: MIT for `source/` and `docs/`,
  Unlicense for `test/`, `fabricare/` and `.claude/` (see `.reuse/dep5`).
