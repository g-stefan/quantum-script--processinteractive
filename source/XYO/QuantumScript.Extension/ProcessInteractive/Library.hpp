// Quantum Script Extension ProcessInteractive
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_LIBRARY_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_LIBRARY_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/ProcessInteractive/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::ProcessInteractive {

	XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT void initExecutive(Executive *executive, void *extensionId);
	XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT void registerInternalExtension(Executive *executive);

};

#endif
