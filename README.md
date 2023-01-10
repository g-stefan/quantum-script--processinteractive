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
ProcessInteractive.prototype.becomeOwner(socket);
ProcessInteractive.prototype.linkOwner(socket);
ProcessInteractive.prototype.unLinkOwner(socket);
ProcessInteractive.prototype.transferOwner(socket);
ProcessInteractive.prototype.join();
ProcessInteractive.prototype.isRunning();
ProcessInteractive.prototype.getReturnValue();
ProcessInteractive.prototype.useConPTY(value);
```

## License

Copyright (c) 2016-2023 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
