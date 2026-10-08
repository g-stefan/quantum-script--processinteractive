# API reference

## Script

These symbols are available after `Script.requireExtension("ProcessInteractive")`,
which also loads `Buffer`.

### Global and static functions

| Symbol | Returns | Notes |
|--------|---------|-------|
| `ProcessInteractive()` / `new ProcessInteractive()` | ProcessInteractive | no process yet; arguments ignored |
| `ProcessInteractive.isProcessInteractive(x)` | Boolean | type test |
| `ProcessInteractive.run(cmd, useConPTY)` | String / `undefined` | all output (stdout + stderr) after the end; `undefined` if it cannot start; `useConPTY` missing = `true` |
| `ProcessInteractive.runLn(cmd, fn, useConPTY, lineMaxLn)` | Boolean | `fn.call(process, line)` per line, must return truthy to continue; `useConPTY` missing = `true`; `lineMaxLn` missing = 32768; `false` if it cannot start |

### Methods

| Method | Returns | Behavior |
|--------|---------|----------|
| `p.useConPTY(value)` | `undefined` | Windows, before `execute`: pseudo console on / off (default on); no-op on Linux |
| `p.execute(cmd)` | Boolean | closes the previous process (waits), starts `cmd` without a shell |
| `p.isRunning()` | Boolean | no wait; `false` before `execute` / after the end |
| `p.write(str)` | Number | bytes written to stdin; `0` if no process / input closed |
| `p.writeLn(str)` | Number | `write(str + "\r\n")` |
| `p.writeFromBuffer(buffer)` | Number | writes `buffer.length` bytes; throws if not a Buffer |
| `p.waitToRead(microSeconds)` | Number | `1` data, `0` timeout, `-1` no more output, `NaN` invalid argument |
| `p.read(size)` | String / `undefined` | waits for some output, returns what is available (≤ `size`, default 32768); `""` at the end; `undefined` for invalid `size` |
| `p.readLn(size)` | String / `undefined` | one line with its line end (≤ `size` bytes, default 32768); `undefined` at the end; waits for the whole line |
| `p.readToBuffer(buffer, ln)` | Number | waits for `ln` bytes (default 32768, ≤ `buffer.size`) or the end; sets `buffer.length` |
| `p.join()` | `undefined` | waits for the end, discards output, input stays open |
| `p.close()` | `undefined` | closes input, waits for the end, discards unread output, sets the exit code, releases |
| `p.terminate(waitMilliseconds)` | `undefined` | `WM_CLOSE` / `SIGTERM`, wait up to `ms`, then kill; call `close()` after |
| `p.getReturnValue()` | Number | exit code once the process ended (after `close()` / `join()` / `isRunning()` → `false`), `0` before |

### Platform differences

| | Windows | Linux |
|---|---|---|
| Start | `CreateProcessA`, command line as is | `ShellArguments` + `execvp` |
| Program not found | `execute` → `false`, `run` → `undefined` | `execute` → `true`, exit code `2` |
| Killed by `terminate` | exit code `0` | `128 + signal` |
| `useConPTY` | hidden pseudo console, on by default | no effect |
| Pipe buffer | 4 KB | 64 KB (kernel) |

## C++

Namespace `XYO::QuantumScript::Extension::ProcessInteractive`.

| Symbol | Header | Notes |
|--------|--------|-------|
| `registerInternalExtension(Executive *)` | `ProcessInteractive.hpp` | register as internal extension `"ProcessInteractive"` |
| `initExecutive(Executive *, void *extensionId)` | `ProcessInteractive.hpp` | extension init, run by the engine |
| `quantumScriptExtension(Executive *, void *)` | DLL export | `extern "C"`, DLL build only |
| `VariableProcessInteractive` | `ProcessInteractive/VariableProcessInteractive.hpp` | script value; public `XYO::System::ProcessInteractive value` |
| `VariableProcessInteractive::newVariable()` | | new object, no process |
| `ProcessInteractiveContext`, `getContext()` | `ProcessInteractive/Context.hpp` | per thread prototype and symbol |
| `XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT` | `ProcessInteractive/Dependency.hpp` | export macro |
| `XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_LIBRARY` | define | static build: empty export macro, no DLL entry point |
| `Copyright`, `License`, `Version` | `ProcessInteractive/Copyright.hpp`, ... | library metadata (`Version::versionWithBuild()`) |

## fabricare.json

| Project | Kind | Dependencies |
|---------|------|--------------|
| `quantum-script--processinteractive` | `dll-or-lib` | `quantum-script`, `quantum-script--console`, `quantum-script--buffer` |
| `quantum-script--processinteractive.static` | `lib`, static CRT | the `.static` variants; exports `XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_LIBRARY` |
| `test.01` | `exe`, category `test` | `quantum-script--processinteractive` |
