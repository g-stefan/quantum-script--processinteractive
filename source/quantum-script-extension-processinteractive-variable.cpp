//
// Quantum Script Extension ProcessInteractive
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-context.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"

#include "quantum-script-extension-processinteractive.hpp"
#include "quantum-script-extension-processinteractive-variable.hpp"

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ProcessInteractive {

				using namespace XYO;

				XYO_DYNAMIC_TYPE_IMPLEMENT(VariableProcessInteractive, "{8287D24A-5799-4841-BBE9-300F45815FC0}");
				const char *VariableProcessInteractive::strTypeProcessInteractive = "ProcessInteractive";

				VariableProcessInteractive::VariableProcessInteractive() {
					XYO_DYNAMIC_TYPE_PUSH(VariableProcessInteractive);
				};

				String VariableProcessInteractive::getVariableType() {
					return strTypeProcessInteractive;
				};

				Variable *VariableProcessInteractive::newVariable() {
					return (Variable *) TMemory<VariableProcessInteractive>::newMemory();
				};

				Variable *VariableProcessInteractive::instancePrototype() {
					return (Extension::ProcessInteractive::getContext())->prototypeProcessInteractive->prototype;
				};

				void VariableProcessInteractive::activeDestructor() {
					value.close();
				};

				Variable *VariableProcessInteractive::clone(SymbolList &inSymbolList) {
					VariableProcessInteractive *out = (VariableProcessInteractive *)newVariable();
					out->value.becomeOwner(value);
					return  out;
				};

				bool VariableProcessInteractive::toBoolean() {
					return true;
				};

				String VariableProcessInteractive::toString() {
					return strTypeProcessInteractive;
				};

			};
		};
	};
};


