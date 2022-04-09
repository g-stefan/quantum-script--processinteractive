//
// Quantum Script Extension ProcessInteractive
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEPROCESSINTERACTIVE_HPP
#define QUANTUM_SCRIPT_VARIABLEPROCESSINTERACTIVE_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#	include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ProcessInteractive {

				class VariableProcessInteractive;

			};
		};
	};
};

namespace XYO {
	namespace ManagedMemory {
		template <>
		class TMemory<Quantum::Script::Extension::ProcessInteractive::VariableProcessInteractive> : public TMemoryPoolActive<Quantum::Script::Extension::ProcessInteractive::VariableProcessInteractive> {};
	};
};

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ProcessInteractive {

				using namespace XYO;

				class VariableProcessInteractive : public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableProcessInteractive);
						XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT, VariableProcessInteractive);

					protected:
						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT static const char *strTypeProcessInteractive;

					public:
						XYO::ProcessInteractive value;

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT VariableProcessInteractive();

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT void activeDestructor();

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT String getVariableType();

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT Variable *clone(SymbolList &inSymbolList);

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT String toString();
				};

			};
		};
	};
};

#endif
