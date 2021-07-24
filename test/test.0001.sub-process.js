// Public domain
// http://unlicense.org/
Script.requireExtension("Console");
Script.requireExtension("Thread");

Console.writeLn("Begin work in sub-process");
CurrentThread.sleep(3000);
Console.writeLn("End work in sub-process");

