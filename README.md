# Quantum Script Extension ProcessInteractive

Quantum Script extension
- Run a program from a script with pipes to its standard input and output:
the whole output of a command (`ProcessInteractive.run`), or each line as it
comes (`ProcessInteractive.runLn`).
- Drive a program: `execute`, `write` / `writeLn`, `waitToRead`, `read` /
`readLn`, `close`, `terminate`, the exit code (`getReturnValue`).
- Binary data through `Buffer` (`readToBuffer`, `writeFromBuffer`).
- Windows and Linux, the same API; on Windows the child runs in a hidden
pseudo console (ConPTY) by default.

```javascript
Script.requireExtension("ProcessInteractive");

ProcessInteractive;
ProcessInteractive.isProcessInteractive(x);
ProcessInteractive.prototype.execute(cmd);
ProcessInteractive.prototype.read(size);
ProcessInteractive.prototype.readLn(size);
ProcessInteractive.prototype.write(str);
ProcessInteractive.prototype.writeLn(str);
ProcessInteractive.prototype.close();
ProcessInteractive.prototype.terminate(waitMilliseconds);
ProcessInteractive.prototype.waitToRead(microSec);
ProcessInteractive.prototype.readToBuffer(buffer,ln);
ProcessInteractive.prototype.writeFromBuffer(buffer);
ProcessInteractive.prototype.join();
ProcessInteractive.prototype.isRunning();
ProcessInteractive.prototype.getReturnValue();
ProcessInteractive.prototype.useConPTY(value);
ProcessInteractive.run(cmd,useConPTY);
ProcessInteractive.runLn(cmd,fn,useConPTY,lineMaxLn);
```

Built on `quantum-script` and `XYO::System::ProcessInteractive`
(`xyo-system`), part of the XYO C++ SDK.

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, a first script, fabricare scripts, register in a C++ host, static builds
- [Process model](docs/process-model.md) - no shell, pipes, blocking, end of input, exit codes, ConPTY
- [Script API](docs/script-api.md) - every function: exact behavior, edge cases
- [Recipes](docs/recipes.md) - output and exit code, timeout, interactive programs, several processes
- [C++ API](docs/cpp-api.md) - `VariableProcessInteractive`, using it from native code
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--processinteractive](.claude/skills/quantum-script--processinteractive/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
