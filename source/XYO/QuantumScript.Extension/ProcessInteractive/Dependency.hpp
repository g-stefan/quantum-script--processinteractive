// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_DEPENDENCY_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_DEPENDENCY_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

// -- Export

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_INTERNAL
#	ifdef QUANTUM_SCRIPT__PROCESSINTERACTIVE_INTERNAL
#		define XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_INTERNAL
#	endif
#endif

#ifdef XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_INTERNAL
#	define XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT XYO_PLATFORM_LIBRARY_EXPORT
#else
#	define XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT XYO_PLATFORM_LIBRARY_IMPORT
#endif
#ifdef XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_LIBRARY
#	undef XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT
#	define XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT
#endif

#endif
