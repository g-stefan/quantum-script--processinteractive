// Quantum Script Extension ProcessInteractive
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_CONTEXT_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_CONTEXT_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

namespace XYO::QuantumScript::Extension::ProcessInteractive {

	class ProcessInteractiveContext : public Object {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(ProcessInteractiveContext);

		public:
			Symbol symbolFunctionProcessInteractive;
			TPointerX<Prototype> prototypeProcessInteractive;

			XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT ProcessInteractiveContext();
	};

	XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT ProcessInteractiveContext *getContext();
};

#endif
