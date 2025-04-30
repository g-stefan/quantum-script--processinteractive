// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2023-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

messageAction("make.prepare");

runInPath("source/XYO/QuantumScript.Extension/ProcessInteractive",function(){
	exitIf(fileToCS("--touch=Library.cpp","--file-in=Library.js","--file-out=Library.Source.cpp","--is-string","--name=librarySource"));
});

