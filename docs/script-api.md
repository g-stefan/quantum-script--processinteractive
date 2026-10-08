# Script API

This page lists everything the extension defines after
`Script.requireExtension("ProcessInteractive")`. Arguments are converted the
engine's way: `toString` for text, `toNumber` for numbers (a missing argument
is `undefined`, which converts to `NaN`), and `toBoolean` for flags (a missing
argument converts to `false`).

Calling a method on something that is not a `ProcessInteractive` object, for
example `ProcessInteractive.prototype.read.call({})`, throws
`Error: invalid parameter`.

For blocking, end of input and exit code rules, read
[Process model](process-model.md) first.

## Constructor and type

### `ProcessInteractive()` / `new ProcessInteractive()`

Returns a new object **with no process**. Arguments are ignored.

```javascript
var p = new ProcessInteractive();
typeof(p);                                  // "ProcessInteractive"
p instanceof ProcessInteractive;            // true
"" + p;                                     // "ProcessInteractive"
```

The object is always truthy. It has no readable state other than through
its methods. You can add methods to `ProcessInteractive.prototype`.

### `ProcessInteractive.isProcessInteractive(x)`

Returns `true` if `x` is a `ProcessInteractive` object, `false` otherwise.

## Running a command

### `ProcessInteractive.run(cmd, useConPTY)`

Starts `cmd`, collects **all** its output (standard output and error) until
it ends, closes it, and returns the output as a string. Returns `undefined`
if the program cannot be started (on Windows; on Linux a missing program
"starts" and gives `""`).

- `useConPTY`: Windows pseudo console; **`true` when missing**.
- The exit code is not available. Use an object when you need it.
- The child's input stays open and is never written. A program that reads
  its standard input waits forever.

```javascript
var out = ProcessInteractive.run("git rev-parse --abbrev-ref HEAD");
if (Script.isString(out)) {
	var branch = out.trim();
};
```

### `ProcessInteractive.runLn(cmd, fn, useConPTY, lineMaxLn)`

Starts `cmd` and calls `fn` for each line of its output, as soon as the line
is complete. The function is written in Quantum Script (`Library.js`).

- `fn(line)` is called with **`this` set to the `ProcessInteractive` object**.
  `line` is a string that **keeps its line end** (`"\n"` or `"\r\n"`). The
  last line can have no line end.
- **`fn` must return a truthy value to continue.** A falsy value, including
  a function that returns nothing, stops reading after that line. After
  that, `runLn` closes the process, which **waits for it to end** and
  discards the rest of the output. To stop the process too, call
  `this.terminate(0)` before returning `false`.
- `useConPTY`: Windows pseudo console; `true` when missing, like `run`.
- `lineMaxLn`: the maximum line length. Missing means 32768. A longer line
  arrives in pieces.
- Returns `false` if the process cannot be started, `true` otherwise.
- An exception thrown by `fn` propagates out of `runLn`. The process is not
  closed until the object is garbage collected.

```javascript
var errors = 0;
ProcessInteractive.runLn("fabricare make", function(line) {
	Console.write(line);
	if (line.indexOf("error") >= 0) {
		++errors;
	};
	return true;
}, true);
```

## Starting and controlling a process

### `p.useConPTY(value)`

Windows: start the next `execute` with (`true`) or without (`false`) a
pseudo console. The default for a new object is `true`. `value` is converted
to a boolean, so `useConPTY()` means `false`. This method has no effect on
Linux or after `execute`. Returns `undefined`.

### `p.execute(cmd)`

Closes the previous process of this object, if any (**waiting for it to
end**). Then it starts `cmd` with its input and output connected to the
object. Returns `true` if the process started and `false` if not.

- No shell is used. See
  [Process model](process-model.md#starting-the-program) for how the command
  line is split and how the program is found.
- On Linux, an unknown program still returns `true`, and the child exits at
  once with code `2`.
- The exit code is reset to `0`.

### `p.isRunning()`

`true` while the process runs. `false` once it has ended, and also before
`execute` and after `close`. This call does not wait.

### `p.join()`

Waits for the process to end, reading and **discarding** its output so it
cannot block on a full pipe. The input stays open, so a child that waits for
input never ends. Returns `undefined`. The exit code is available
afterwards.

### `p.close()`

Closes the child's input, waits for the process to end (discarding the
output that was not read), records the exit code and releases the pipes and
handles. Returns `undefined`. It is safe to call on an object with no
process, and to call more than once.

### `p.terminate(waitMilliseconds)`

Stops the process. On Windows it sends `WM_CLOSE` to the process's windows
and, after up to `waitMilliseconds` ms, kills it with `TerminateProcess`. On
Linux it sends `SIGTERM` and, after up to `waitMilliseconds` ms, `SIGKILL`.
The argument is in **milliseconds**. It goes through the index conversion,
so missing / `NaN` / negative values are `0`, which means kill at once.
Returns `undefined`. It does not wait for the kill to complete. Call
`close()` afterwards to release the process and get the exit code.

### `p.getReturnValue()`

The exit code of the last process, as a number. It is valid once the
process has ended (after `close()` or `join()`, or once `isRunning()`
returned `false`) and stays valid until the next `execute`. It is `0`
before that. See [Process model](process-model.md#exit-codes).

## Writing to the process

### `p.write(str)`

Writes `str` (converted to a string, bytes unchanged) to the child's input.
Returns the number of bytes written: `str.length` normally, less (often `0`)
if the child closed its input or exited, and `0` if there is no process.
Blocks while the pipe is full.

### `p.writeLn(str)`

`p.write(str + "\r\n")`. The line end is **`"\r\n"` on every platform**.
Most programs accept it, but Linux programs that compare whole lines see the
`"\r"`. Use `write(str + "\n")` there. The return value counts the 2 extra
bytes.

### `p.writeFromBuffer(buffer)`

Writes the first `buffer.length` bytes of a `Buffer`. Returns the bytes
written. Throws `Error: invalid parameter` if `buffer` is not a `Buffer`.

## Reading from the process

### `p.waitToRead(microSeconds)`

Waits until there is output to read, the output ends, or `microSeconds` pass
(rounded up to whole milliseconds; `0` only checks). Returns:

| Value | Meaning |
|-------|---------|
| `1` | output is available, a `read` will not block |
| `0` | timeout, the process is still running |
| `-1` | no more output: the process ended and the pipe is empty, it closed its output, or there is no process |
| `NaN` | `microSeconds` missing, `NaN`, negative or infinite |

`-1` is truthy, so `while (p.waitToRead(t))` never stops when the process
ends. Compare with `> 0` (data) or `>= 0` (still useful to wait).

### `p.read(size)`

Blocks until **some** output is available (or the output ends), then returns
it as a string of at most `size` bytes. This is usually less than `size`,
because `read` returns what is there. For `size` above 32768 it reads in
32 KB parts and continues only while every part is full.

- `size` missing: 32768.
- `size` `NaN`, negative or infinite: `undefined`.
- `size` `0`: `""` at once.
- `""` at the end of the output, or when there is no process.

### `p.readLn(size)`

Reads one line, byte by byte, so nothing after the line is consumed. Blocks
until the line is complete.

- The line keeps its end: `"\n"` or `"\r\n"`. A lone `"\r"` stays in the line.
- At most `size` bytes without the line end. The rest of a longer line comes
  with the next call. `size` missing: 32768. `NaN`, negative or infinite:
  `undefined`. `0`: `""`.
- Returns the last line even without a line end when the output ends.
  Returns **`undefined`** when the output has ended and nothing was read.
- Blocks forever on a prompt that has no line end while the child waits for
  input. Use `waitToRead` + `read` for those.

### `p.readToBuffer(buffer, ln)`

Reads into a `Buffer` from index 0, **until `ln` bytes have arrived or the
output ends**. Sets `buffer.length` to the count and returns it.

- `ln` missing: 32768. `ln` infinite: `buffer.size`. `ln` is then clipped to
  `buffer.size`.
- `ln` `0`, negative or `NaN`: reads nothing, sets `buffer.length = 0` and
  returns `0`.
- Returns `0` at the end of the output.
- Unlike `read`, it waits for the full count. With a process that writes
  little and keeps running, use a small `ln`, or `read` instead.
- Throws `Error: invalid parameter` if `buffer` is not a `Buffer`.

```javascript
var p = new ProcessInteractive();
p.execute("cmd /c echo 123456789");
var b = Buffer(4);
p.readToBuffer(b);        // 4, b.toString() == "1234"
p.readToBuffer(b, 2);     // 2, "56"
p.readToBuffer(b);        // 4, "789\r"
p.close();
```
