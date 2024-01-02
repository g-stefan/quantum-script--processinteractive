//
// Quantum Script Extension ProcessInteractive
//
// Copyright (c) 2020-2024 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript.Extension/ProcessInteractive/VariableProcessInteractive.hpp>
#include <XYO/QuantumScript.Extension/ProcessInteractive/Context.hpp>

namespace XYO::QuantumScript::Extension::ProcessInteractive {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableProcessInteractive, "{8287D24A-5799-4841-BBE9-300F45815FC0}");
	const char *VariableProcessInteractive::strTypeProcessInteractive = "ProcessInteractive";

	VariableProcessInteractive::VariableProcessInteractive() {
		XYO_DYNAMIC_TYPE_PUSH(VariableProcessInteractive);
	};

	String VariableProcessInteractive::getVariableType() {
		return strTypeProcessInteractive;
	};

	Variable *VariableProcessInteractive::newVariable() {
		return (Variable *)TMemory<VariableProcessInteractive>::newMemory();
	};

	Variable *VariableProcessInteractive::instancePrototype() {
		return (Extension::ProcessInteractive::getContext())->prototypeProcessInteractive->prototype;
	};

	void VariableProcessInteractive::activeDestructor() {
		value.close();
	};

	bool VariableProcessInteractive::toBoolean() {
		return true;
	};

	String VariableProcessInteractive::toString() {
		return strTypeProcessInteractive;
	};

};
