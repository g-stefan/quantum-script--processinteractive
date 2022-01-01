//
// Quantum Script Extension ProcessInteractive
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_HPP
#define QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_HPP

#ifndef QUANTUM_SCRIPT_HPP
#include "quantum-script.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE__EXPORT_HPP
#include "quantum-script-extension-processinteractive--export.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_COPYRIGHT_HPP
#include "quantum-script-extension-processinteractive-copyright.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_LICENSE_HPP
#include "quantum-script-extension-processinteractive-license.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_VERSION_HPP
#include "quantum-script-extension-processinteractive-version.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ProcessInteractive {

				using namespace Quantum::Script;

				class ProcessInteractiveContext:
					public Object {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(ProcessInteractiveContext);
					public:

						Symbol symbolFunctionProcessInteractive;
						TPointerX<Prototype> prototypeProcessInteractive;

						QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT ProcessInteractiveContext();
				};

				QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT ProcessInteractiveContext *getContext();

				QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT void initExecutive(Executive *executive, void *extensionId);
				QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT void registerInternalExtension(Executive *executive);

			};
		};
	};
};

#endif

