# Process model

A `ProcessInteractive` object wraps one `XYO::System::ProcessInteractive`
from `xyo-system`. This page explains what happens below the script API. Most
surprises, such as a script that hangs, lost output or a wrong exit code,
come from the rules on this page.

## The pipes

```
            write / writeLn / writeFromBuffer
script  ─────────────────────────────────────────►  child stdin
        ◄─────────────────────────────────────────  child stdout + stderr (one pipe)
            read / readLn / readToBuffer / waitToRead
```

- The child's **standard output and standard error go to the same pipe**.
  The script sees them interleaved as the child wrote them and cannot tell
  them apart. To separate them, redirect in a shell
  (`/bin/sh -c "tool 2>/dev/null"`, `cmd /c "tool 2>nul"`).
- The pipes carry **bytes**. A Quantum Script string is a byte string, so
  output arrives as written: UTF-8 from most tools, possibly the OEM / ANSI
  code page from older Windows console programs. Nothing is converted.
- **A pipe has a limited size** (64 KB on Linux, 4 KB here on Windows). A
  child that writes more than that while nobody reads **blocks** until the
  script reads. A child that does not read its input blocks the script's
  `write` in the same way. Keep reading while the child runs. `run`,
  `runLn`, `join` and `close` do this for you.

## Starting the program

`execute(cmd)` starts **one program, without a shell**:

| | Windows | Linux |
|---|---|---|
| How | `CreateProcessA(nullptr, cmd, ...)` | `cmd` split by `ShellArguments`, then `fork` + `execvp` |
| Program lookup | CreateProcess rules: the application folder, current folder, system folders, `PATH`; `.exe` added when there is no extension | `execvp`: `PATH` if the name has no `/` |
| Arguments | the rest of the line, passed as is (the program parses it) | split on spaces; `"..."` groups words, `\"` is a literal quote |
| Program not found | `execute` returns **`false`** | `execute` returns **`true`**; the child exits at once with code `2` (`ENOENT`) |
| Built-ins, pipes, `>`, `&&`, `.cmd` / `.bat`, `*` | need `cmd /c ...` | need `/bin/sh -c "..."` |
| Working directory, environment | inherited from the script | inherited from the script |

```javascript
p.execute("git status --short");                       // a program: fine everywhere
p.execute("cmd /c dir /b *.txt");                      // Windows shell syntax
p.execute("/bin/sh -c \"ls *.txt | sort\"");          // Linux shell syntax
```

On Windows, keep the command line ASCII. It is passed to `CreateProcessA`,
which uses the system ANSI code page, not UTF-8. To run in another folder,
change the script's current folder first (`Shell.chdir` or fabricare's
`runInPath`) and restore it afterwards.

Calling `execute` on an object that already has a process **closes the old
process first**. That close waits for the old process to end. See
[Ending](#ending-close-join-terminate).

## Waiting and blocking

| Call | Blocks until |
|------|--------------|
| `waitToRead(us)` | output is available, the output ends, or `us` microseconds (rounded up to milliseconds) pass |
| `read(size)` | **some** output is available, or the output ends; then returns what is there (at most `size` bytes) |
| `readLn(size)` | a **whole line** (`"\n"`) or `size` bytes arrived, or the output ends |
| `readToBuffer(buffer, ln)` | **`ln` bytes** arrived (by default the buffer size, at most 32768), or the output ends |
| `write(str)` | all bytes are in the pipe (the child reads them), or the child closed its input |
| `join()` / `close()` | the child ended (the output is read and discarded meanwhile) |
| `run` / `runLn` | the child ended |

The waits are real waits, not polling. `waitToRead` returns as soon as data
arrives or the process ends, on both platforms. To avoid blocking, call
`waitToRead` before reading, and use `read` (which returns what is
available) instead of `readLn` / `readToBuffer` (which wait for more).

**A program that prints a prompt without a newline** (`Password: `,
`> `, `(y/n) `) and then waits for input makes `readLn` block forever. Read
prompts with `waitToRead` + `read`.

**`waitToRead` returns `-1` once there is no more output**: the process
ended and the pipe is empty, or it closed its output, or the object has no
process. `-1` is truthy, so test `> 0`, not `if (p.waitToRead(...))`.

## End of input

There is **no way to close only the input** and keep reading the output.
`close()` closes the input, then waits for the end and **discards whatever
the child writes after that**. As a consequence:

- A filter that only answers at the end of its input (`sort`, `findstr`, `wc`,
  `cat` while buffering) cannot be fed through `write` and read back.
  Instead, give it a file (`sort input.txt`), or use a shell:
  `cmd /c "sort < input.txt"`, `/bin/sh -c "sort input.txt"`.
- An interactive program works when it ends on a **command**, such as
  `exit`, `quit` or `q`. Write that command, then read until `waitToRead`
  returns `-1` (see [Recipes](recipes.md#talk-to-an-interactive-program)).
- `run` and `runLn` never write and never close the input until the end.
  A program that reads its standard input waits forever under them.

## Ending: close, join, terminate

| Call | Input | Waits for the end | Unread output | Releases the process |
|------|-------|-------------------|---------------|----------------------|
| `close()` | closed first | yes | discarded | yes; also sets the exit code |
| `join()` | stays open | yes | discarded | no |
| `terminate(ms)` | unchanged | at most `ms` milliseconds before killing it | stays in the pipe | no; call `close()` after |

- **Always end with `close()`.** It is safe to call it on an object with no
  process, and to call it twice.
- `join()` keeps the input open. A child that waits for input never ends,
  and `join` then never returns. Prefer `close()`.
- An object that is never closed is closed when the garbage collector
  destroys it, and that close **waits for the process**. Do not rely on this.
- `terminate(ms)` on Windows sends `WM_CLOSE` to the child's windows, waits up
  to `ms` milliseconds, then kills it with `TerminateProcess`. On Linux it
  sends `SIGTERM`, waits up to `ms` milliseconds, then sends `SIGKILL`.
  `terminate(0)` kills at once. Only the child itself is stopped, not
  processes it started. The value is in **milliseconds**. `terminate` does
  not wait for the kill to complete, so call `close()` after it.

## Exit codes

`getReturnValue()` returns the exit code of the last process. The code is
valid **once the process has ended**: after `close()` or `join()`, or once
`isRunning()` returned `false`. Before that it is `0`.

| | Windows | Linux |
|---|---|---|
| Killed by `terminate` | `0` (if it did not exit by itself on `WM_CLOSE`) | `128 + signal`: `143` (`SIGTERM`), `137` (`SIGKILL`) |
| Program not found | `execute` returns `false`, no exit code | `2` |

The value stays valid after `close()` until the next `execute`. Right after
`terminate` the process may still be ending, so call `close()` (or wait for
`isRunning()` to return `false`) before reading the code. `run` and `runLn`
do not return the exit code. Use an object when you need it.

On Windows this needs `xyo-system` 8.0.0 build 18 or later. Older builds set
the exit code only in `close()`.

## ConPTY (Windows)

On Windows the child starts by default in a **pseudo console** (ConPTY,
Windows 10 1809 and later). The child gets a hidden console of its own.
Console programs that need a console work, no console window opens, and the
child cannot write around the pipes into the script's console. The child's
input and output are still the plain pipes, so the output contains no
terminal escape sequences.

- `p.useConPTY(false)` before `execute` starts the child without one. The
  child then inherits the script's console, if the script has one.
- The `useConPTY` argument of `ProcessInteractive.run(cmd, useConPTY)` and
  `ProcessInteractive.runLn(cmd, fn, useConPTY)` is `true` when missing.
  Pass `false` to start the child without a pseudo console.
- `useConPTY(value)` converts `value` to a boolean, so `useConPTY()` turns
  it off.
- An `xyo-system` built with `XYO_CONFIG_WINDOWS_DISABLE_CONPTY` (for older
  Windows) never uses a pseudo console. `useConPTY` does nothing on Linux.

## Lifetime and ownership

- `new ProcessInteractive()` creates an object **without a process**. Until
  `execute` succeeds, `isRunning()` is `false`, `waitToRead` returns `-1`,
  `read` returns `""`, `readLn` returns `undefined` and `write` returns `0`.
- One object runs one process at a time. You can reuse the object:
  `execute` again after `close()`.
- Objects are reference values. Assignment shares the same process.
- The object belongs to the thread that created it (see
  [Getting started](getting-started.md#6-threads)).
- A Linux child that ended is reaped by the object, so no zombie process is
  left. Writing to a child that has exited returns `0` and does not raise
  `SIGPIPE` in the script.
