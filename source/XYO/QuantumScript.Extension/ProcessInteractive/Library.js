// Quantum Script Extension ProcessInteractive
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

ProcessInteractive.runLn = function(cmd, fn, useConPTY, lineMaxLn) {
	var pInteractive = new ProcessInteractive();
	pInteractive.useConPTY(useConPTY);
	if (!pInteractive.execute(cmd)) {
		return false;
	};
	do {
		if (pInteractive.waitToRead(1) > 0) {
			if (!fn.call(pInteractive, pInteractive.readLn(lineMaxLn))) {
				break;
			};
		};
	} while (pInteractive.isRunning());

	pInteractive.close();
};
