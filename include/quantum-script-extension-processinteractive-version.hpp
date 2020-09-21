//
// Quantum Script Extension ProcessInteractive
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_VERSION_HPP
#define QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_VERSION_HPP

#define QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_VERSION_ABCD                 1,1,0,3
#define QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_VERSION_STR                 "1.1.0"
#define QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_VERSION_STR_BUILD           "3"
#define QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_VERSION_STR_DATETIME        "2020-09-21 17:40:09"

#ifndef XYO_RC

#ifndef QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE__EXPORT_HPP
#include "quantum-script-extension-processinteractive--export.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ProcessInteractive {
				namespace Version {
					QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT const char *version();
					QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT const char *build();
					QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT const char *versionWithBuild();
					QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT const char *datetime();
				};
			};
		};
	};
};

#endif
#endif

