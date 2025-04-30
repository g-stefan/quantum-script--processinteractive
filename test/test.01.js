// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");
Script.requireExtension("ProcessInteractive");

Console.writeLn("wait processing...\n");

var process=new ProcessInteractive();
process.execute("pwsh -command \"Start-Sleep -s 5\"");

index=0;
var map=["-","\\","|","/"];

while(process.isRunning()) {
	if(process.waitToRead(10)) {
		var info=process.readLn();
		if(info){
			Console.write(info);
		};
	};
	Console.write(map[index]+"\r");
	++index;
	index%=4;
};

// data in buffer after end
var line=process.read();
if(line){
	Console.writeLn(line);
};

Console.writeLn("Done.\n");
