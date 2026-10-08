// Quantum Script Extension ProcessInteractive
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

ProcessInteractive.runLn = function(cmd, fn, useConPTY, lineMaxLn) {
	// useConPTY is optional, true if missing (like run and the C++ default)
	if (Script.isUndefined(useConPTY)) {
		useConPTY = true;
	};
	var pInteractive = new ProcessInteractive();
	pInteractive.useConPTY(useConPTY);
	if (!pInteractive.execute(cmd)) {
		return false;
	};
	// same as ProcessInteractive::runLn in xyo-system
	var line;
	var isStopped = false;
	do {
		if (pInteractive.waitToRead(1) > 0) {
			line = pInteractive.readLn(lineMaxLn);
			// undefined: no line (end of output)
			if (!Script.isUndefined(line)) {
				if (!fn.call(pInteractive, line)) {
					isStopped = true;
					break;
				};
			};
		};
	} while (pInteractive.isRunning());

	// The process ended, process the lines left in the pipe
	if (!isStopped) {
		while (pInteractive.waitToRead(1) > 0) {
			line = pInteractive.readLn(lineMaxLn);
			if (Script.isUndefined(line)) {
				break;
			};
			if (!fn.call(pInteractive, line)) {
				break;
			};
		};
	};

	pInteractive.close();
	return true;
};
