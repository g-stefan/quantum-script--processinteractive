// Public domain
// http://unlicense.org/
Script.requireExtension("Console");
Script.requireExtension("ProcessInteractive");

Console.writeLn("Starting up ...\n");

var process=new ProcessInteractive();
process.execute("quantum-script test/test.0001.sub-process.js");

index=0;
var map=["-","\\","|","/"];

while(process.isRunning()) {
	if(process.waitToRead(10)) {
		Console.write(process.readLn());
	};
	Console.write(map[index]+"\r");
	++index;
	index%=4;
};

// data in buffer after end
Console.write(process.read());

Console.writeLn("Done.\n");
