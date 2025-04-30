//
// Quantum Script Extension ProcessInteractive
//
// Copyright (c) 2020-2025 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_VARIABLEPROCESSINTERACTIVE_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_VARIABLEPROCESSINTERACTIVE_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/ProcessInteractive/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::ProcessInteractive {
	class VariableProcessInteractive;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::ProcessInteractive::VariableProcessInteractive> : public TMemoryPoolActive<XYO::QuantumScript::Extension::ProcessInteractive::VariableProcessInteractive> {};
};

namespace XYO::QuantumScript::Extension::ProcessInteractive {

	class VariableProcessInteractive : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableProcessInteractive);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT, VariableProcessInteractive);

		protected:
			XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT static const char *strTypeProcessInteractive;

		public:
			XYO::System::ProcessInteractive value;

			XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT VariableProcessInteractive();

			XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT void activeDestructor();

			XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT String toString();
	};

};

#endif
