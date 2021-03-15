//
// Quantum Script Extension ProcessInteractive
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEPROCESSINTERACTIVE_HPP
#define QUANTUM_SCRIPT_VARIABLEPROCESSINTERACTIVE_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
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
		template<>
		class TMemory<Quantum::Script::Extension::ProcessInteractive::VariableProcessInteractive>:
			public TMemoryPoolActive<Quantum::Script::Extension::ProcessInteractive::VariableProcessInteractive> {};
	};
};


namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ProcessInteractive {

				using namespace XYO;

				class VariableProcessInteractive :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableProcessInteractive);
					protected:
						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT static const char *strTypeProcessInteractive;
						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT static const char *typeProcessInteractiveKey;
						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT static const void *typeProcessInteractive;
					public:

						XYO::ProcessInteractive value;

						inline VariableProcessInteractive() {
							variableType = registerType(typeProcessInteractive, typeProcessInteractiveKey);
						};

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT void activeDestructor();

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT String getType();

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT Variable &operatorReference(Symbol symbolId);
						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT Variable *clone(SymbolList &inSymbolList);

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT String toString();

						//
						inline static bool isVariableProcessInteractive(const Variable *value) {
							if(typeProcessInteractive == nullptr) {
								typeProcessInteractive = registerType(typeProcessInteractive, typeProcessInteractiveKey);
							};
							return (value->variableType == typeProcessInteractive);
						};

				};


			};
		};
	};
};


#endif

