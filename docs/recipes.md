# Recipes

Every example below assumes:

```javascript
Script.requireExtension("Console");
Script.requireExtension("ProcessInteractive");
```

## Capture the output of a command

```javascript
var version = ProcessInteractive.run("git --version");
if (Script.isUndefined(version)) {
	throw "git not found";
};
version = version.trim();                       // drop the "\n" / "\r\n" at the end
```

Some tools print status or progress text before the data. Cut the output to
the part you need before parsing it:

```javascript
Script.requireExtension("JSON");
var text = ProcessInteractive.run("tool info --json");
var index = text.indexOf("{");
if (index >= 0) {
	var info = JSON.decode(text.substring(index));
};
```

## Shell syntax: pipes, redirection, built-ins

```javascript
var cmd;
if (Shell.getenv("OS") == "Windows_NT") {          // Script.requireExtension("Shell"); fabricare: OS.isWindows()
	cmd = "cmd /c \"dir /b *.txt\"";
} else {
	cmd = "/bin/sh -c \"ls *.txt | sort\"";
};
var files = ProcessInteractive.run(cmd);
```

## Follow a long command line by line

```javascript
var failed = false;
var started = ProcessInteractive.runLn("fabricare make", function(line) {
	Console.write(line);                            // line has its line end
	if (line.indexOf("* Error") >= 0) {
		failed = true;
	};
	return true;                                    // keep reading
});
if (!started) {
	throw "fabricare not found";
};
```

Stop at the first match and kill the process:

```javascript
var found;
ProcessInteractive.runLn("ping -n 100 example.com", function(line) {
	if (line.indexOf("TTL=") >= 0) {
		found = line;
		this.terminate(0);                          // this == the ProcessInteractive object
		return false;                               // without terminate, runLn waits for the end
	};
	return true;
}, true);
```

## Output and exit code

`run` does not give the exit code. Use an object and read everything before
`close()`, because `close` discards output that was not read:

```javascript
function runWithCode(cmd) {
	var p = new ProcessInteractive();
	if (!p.execute(cmd)) {
		return undefined;
	};
	var output = "";
	var state;
	while ((state = p.waitToRead(1000000)) >= 0) {   // -1: no more output
		if (state > 0) {
			output += p.read();
		};
	};
	p.close();
	return {output: output, code: p.getReturnValue()};
};

var r = runWithCode("git diff --exit-code --stat");
if (r.code != 0) {
	Console.write(r.output);
};
```

## Timeout

```javascript
function runWithTimeout(cmd, timeoutMs) {
	var p = new ProcessInteractive();
	if (!p.execute(cmd)) {
		return undefined;
	};
	var output = "";
	var elapsed = 0;
	var state;
	while ((state = p.waitToRead(100000)) >= 0) {   // 100 ms steps
		if (state > 0) {
			output += p.read();
			continue;
		};
		elapsed += 100;
		if (elapsed >= timeoutMs) {
			p.terminate(1000);                      // ask, then kill after 1 s
			break;
		};
	};
	p.close();
	return {output: output, code: p.getReturnValue(), timedOut: (elapsed >= timeoutMs)};
};
```

`elapsed` counts only the time spent waiting with no output, which is enough
to catch a program that hangs.

## Talk to an interactive program

Write commands, end with the program's own exit command, then read until the
output ends:

```javascript
var p = new ProcessInteractive();
p.execute("cmd /q /k");                          // Windows command interpreter, no echo
p.writeLn("echo one");
p.writeLn("exit 3");                             // the program ends by itself
var output = "";
var state;
while ((state = p.waitToRead(500000)) >= 0) {
	if (state > 0) {
		output += p.read();
	};
};
p.close();
p.getReturnValue();                              // 3
```

For a question / answer dialog, wait for the prompt with `waitToRead` +
`read` (not `readLn`, because a prompt usually has no line end):

```javascript
function waitFor(p, text, timeoutMs) {
	var buffer = "";
	var waited = 0;
	var state;
	while (buffer.indexOf(text) < 0) {
		state = p.waitToRead(50000);
		if (state < 0) {
			return undefined;                    // the process ended
		};
		if (state == 0) {
			waited += 50;
			if (waited >= timeoutMs) {
				return undefined;
			};
			continue;
		};
		buffer += p.read();
	};
	return buffer;
};

var p = new ProcessInteractive();
p.execute("python -i -q");
waitFor(p, ">>> ", 5000);
p.writeLn("print(6 * 7)");
Console.write(waitFor(p, ">>> ", 5000));        // "42\r\n>>> "
p.writeLn("exit()");
p.close();
```

## Feed a filter (sort, wc, findstr)

A filter answers only at the end of its input, and `close()` (the only way
to end the input) discards later output. Give it a file instead:

```javascript
Shell.filePutContents("temp/names.txt", "banana\napple\n");   // Script.requireExtension("Shell")
var sorted = ProcessInteractive.run("sort temp/names.txt");
```

## Several processes from one thread

```javascript
var commands = ["git fetch", "git -C ../other fetch"];
var list = [];
var k;
for (k = 0; k < commands.length; ++k) {
	var p = new ProcessInteractive();
	p.execute(commands[k]);
	list[k] = {process: p, output: "", done: false};
};
var active = list.length;
while (active > 0) {
	for (k = 0; k < list.length; ++k) {
		var item = list[k];
		if (item.done) {
			continue;
		};
		var state = item.process.waitToRead(10000);   // 10 ms per process
		if (state > 0) {
			item.output += item.process.read();
		} else if (state < 0) {
			item.process.close();
			item.done = true;
			--active;
		};
	};
};
```

## Show progress while waiting

This is what `test/test.01.js` does:

```javascript
var p = new ProcessInteractive();
p.execute("pwsh -command \"Start-Sleep -s 5\"");
var spinner = ["-", "\\", "|", "/"];
var k = 0;
while (p.isRunning()) {
	if (p.waitToRead(100000) > 0) {
		var line = p.readLn();
		if (Script.isString(line)) {
			Console.write(line);
		};
	};
	Console.write(spinner[k++ % 4] + "\r");
};
Console.write(p.read());                          // output left after the end
p.close();
```

## Binary output

```javascript
var p = new ProcessInteractive();
p.execute("tool --dump-binary");
var chunk = Buffer(65536);
var data = "";
while (p.readToBuffer(chunk) > 0) {               // waits for up to 32768 bytes each time
	data += chunk.toString();
};
p.close();
```
