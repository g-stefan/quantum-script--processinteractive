# Quantum Script Extension ProcessInteractive

```javascript
ProcessInteractive;
ProcessInteractive.isProcessInteractive(x);
ProcessInteractive.prototype.execute(cmd);
ProcessInteractive.prototype.read(size);
ProcessInteractive.prototype.readLn(size);
ProcessInteractive.prototype.write(str);
ProcessInteractive.prototype.writeLn(str);
ProcessInteractive.prototype.close();
ProcessInteractive.prototype.terminate(waitMicroSec);
ProcessInteractive.prototype.waitToRead(microSec);
ProcessInteractive.prototype.readToBuffer(buffer);
ProcessInteractive.prototype.writeFromBuffer(buffer);
ProcessInteractive.prototype.join();
ProcessInteractive.prototype.isRunning();
ProcessInteractive.prototype.getReturnValue();
ProcessInteractive.prototype.useConPTY(value);
ProcessInteractive.run(cmd,useConPTY);
ProcessInteractive.runLn(cmd,fn,useConPTY,lineMaxLn);
```

## License

Copyright (c) 2016-2024 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
