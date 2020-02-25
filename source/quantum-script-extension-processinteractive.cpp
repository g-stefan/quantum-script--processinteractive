//
// Quantum Script Extension ProcessInteractive
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-extension-processinteractive-license.hpp"
#include "quantum-script-extension-processinteractive.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_NO_VERSION
#include "quantum-script-extension-processinteractive-version.hpp"
#endif
//
#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"

//
#include "quantum-script-extension-buffer-variablebuffer.hpp"
//
#include "quantum-script-extension-processinteractive-variableprocessinteractive.hpp"

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ProcessInteractive {

				using namespace XYO;
				using namespace Quantum::Script;

				ProcessInteractiveContext::ProcessInteractiveContext() {
					symbolFunctionProcessInteractive = 0;
					prototypeProcessInteractive.pointerLink(this);
				};

				ProcessInteractiveContext *getContext() {
					return TSingleton<ProcessInteractiveContext>::getValue();
				};

				static TPointer<Variable> functionProcessInteractive(VariableFunction *function, Variable *this_, VariableArray *arguments) {
					return VariableProcessInteractive::newVariable();
				};

				static void deleteContext() {
					ProcessInteractiveContext *processInteractiveContext = getContext();
					processInteractiveContext->prototypeProcessInteractive.deleteMemory();
					processInteractiveContext->symbolFunctionProcessInteractive = 0;
				};

				static void newContext(Executive *executive, void *extensionId) {
					VariableFunction *defaultPrototypeFunction;

					ProcessInteractiveContext *processInteractiveContext = getContext();
					executive->setExtensionDeleteContext(extensionId, deleteContext);

					processInteractiveContext->symbolFunctionProcessInteractive = Context::getSymbol("ProcessInteractive");
					processInteractiveContext->prototypeProcessInteractive.newMemory();

					defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionProcessInteractive, nullptr, nullptr);
					((Context::getGlobalObject())->operatorReferenceOwnProperty(processInteractiveContext->symbolFunctionProcessInteractive))=defaultPrototypeFunction;
					processInteractiveContext->prototypeProcessInteractive = defaultPrototypeFunction->prototype;

				};

				static TPointer<Variable> isProcessInteractive(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- script-is-processinteractive\n");
#endif
					return VariableBoolean::newVariable(VariableProcessInteractive::isVariableProcessInteractive(arguments->index(0)));
				};

				static TPointer<Variable> processInteractiveExecute(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-execute\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableProcessInteractive *)( this_ ))->value.execute((arguments->index(0))->toString()));
				};

				static TPointer<Variable> processInteractiveRead(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-read\n");
#endif
					String  retV;
					Number ln;
					size_t readLn;
					size_t readToLn;
					size_t readX;
					size_t readTotal;
					size_t k;
					char buffer[16384];

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					if(VariableUndefined::isVariableUndefined(arguments->index(0))) {
						ln = 16384;
					} else {

						ln = (arguments->index(0))->toNumber();
						if(isnan(ln) || isinf(ln) || signbit(ln)) {
							return Context::getValueUndefined();
						};

					};

					readToLn = (size_t)(ln);
					readTotal = 0;
					readX = 16384;
					if(readToLn < readX) {
						readX = readToLn;
					};
					for(;;) {
						readLn = ((VariableProcessInteractive *) this_)->value.read(buffer, readX);

						if(readLn > 0) {
							retV.concatenate(buffer, readLn);
						};
						//end of file
						if(readLn < readX) {
							break;
						};
						//end of read
						readTotal += readLn;
						if(readTotal >= readToLn) {
							break;
						};
						readX = readToLn - readTotal;
						if(readX > 16384) {
							readX = 16384;
						};
					};

					return VariableString::newVariable(retV);
				};


				static TPointer<Variable> processInteractiveReadLn(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-read-ln\n");
#endif
					String  retV;
					Number ln;

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					if(VariableUndefined::isVariableUndefined(arguments->index(0))) {
						ln = 16384;
					} else {

						ln = (arguments->index(0))->toNumber();
						if(isnan(ln) || isinf(ln) || signbit(ln)) {
							return Context::getValueUndefined();
						};

					};

					if(StreamX::readLn(((VariableProcessInteractive *) this_)->value, retV, ln)) {
						return VariableString::newVariable(retV);
					};

					return Context::getValueUndefined();
				};

				static TPointer<Variable> processInteractiveWrite(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-write\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					String toWrite = (arguments->index(0))->toString();
					return VariableNumber::newVariable((Number)(((VariableProcessInteractive *) this_)->value.write(
									toWrite.value(), toWrite.length()
								)));
				};


				static TPointer<Variable> processInteractiveWriteLn(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-write-ln\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					String toWrite = (arguments->index(0))->toString();
					toWrite << "\r\n";
					return VariableNumber::newVariable((Number)(((VariableProcessInteractive *) this_)->value.write(
									toWrite.value(), toWrite.length()
								)));
				};

				static TPointer<Variable> processInteractiveClose(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-close\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					((VariableProcessInteractive *) this_)->value.close();

					return Context::getValueUndefined();
				};

				static TPointer<Variable> processInteractiveTerminate(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-terminate\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					((VariableProcessInteractive *) this_)->value.terminate((arguments->index(0))->toIndex());

					return Context::getValueUndefined();
				};

				static TPointer<Variable> processInteractiveWaitToRead(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-wait-to-read\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					Number microSeconds = (arguments->index(0))->toNumber();
					if(isnan(microSeconds) || isinf(microSeconds) || signbit(microSeconds)) {
						return VariableNumber::newVariable(NAN);
					};
					return VariableNumber::newVariable(((VariableProcessInteractive *) this_)->value.waitToRead((uint32_t)microSeconds));
				};


				static TPointer<Variable> processInteractiveReadToBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-read-to-buffer\n");
#endif
					size_t readLn;
					size_t readToLn;
					size_t readX;
					size_t readTotal;
					size_t k;
					Number ln;

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &buffer(arguments->index(0));

					if(!Extension::Buffer::VariableBuffer::isVariableBuffer(buffer)) {
						throw(Error("invalid parameter"));
					};

					if(VariableUndefined::isVariableUndefined(arguments->index(1))) {
						ln = 16384;
					} else {

						ln = (arguments->index(1))->toNumber();
						if(isnan(ln) || signbit(ln) || ln == 0.0) {
							((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.length = 0;
							return VariableNumber::newVariable(0);
						};
						if(isinf(ln)) {
							ln = ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.size;
						};

					};

					if(ln > ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.size) {
						ln = ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.size;
					};

					readToLn = (size_t)ln;
					readTotal = 0;
					readX = readToLn;
					for(;;) {
						readLn = ((VariableProcessInteractive *) this_)->value.read(&(((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.buffer)[readTotal], readX);
						//end of transmision
						if(readLn == 0) {
							break;
						};
						readTotal += readLn;
						if(readTotal >= readToLn) {
							break;
						};
						readX = readToLn - readTotal;
					};
					((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.length = readTotal;
					return VariableNumber::newVariable(readTotal);
				};


				static TPointer<Variable> processInteractiveWriteFromBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-write-from-buffer\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &buffer(arguments->index(0));

					if(!Extension::Buffer::VariableBuffer::isVariableBuffer(buffer)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable((Number)(((VariableProcessInteractive *) this_)->value.write(
									((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.buffer, ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.length
								)));
				};

				static TPointer<Variable> processInteractiveBecomeOwner(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-become-owner\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &value = arguments->index(0);

					if(!VariableProcessInteractive::isVariableProcessInteractive(value)) {
						throw(Error("invalid parameter"));
					};

					((VariableProcessInteractive *) this_)->value.becomeOwner(((VariableProcessInteractive *)(value.value()))->value);

					return Context::getValueUndefined();
				};

				static TPointer<Variable> processInteractiveLinkOwner(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-link-owner\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &value = arguments->index(0);

					if(!VariableProcessInteractive::isVariableProcessInteractive(value)) {
						throw(Error("invalid parameter"));
					};

					((VariableProcessInteractive *) this_)->value.linkOwner(((VariableProcessInteractive *)(value.value()))->value);

					return Context::getValueUndefined();
				};

				static TPointer<Variable> processInteractiveUnLinkOwner(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-unlink-owner\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					((VariableProcessInteractive *) this_)->value.unLinkOwner();

					return Context::getValueUndefined();
				};


				static TPointer<Variable> processInteractiveTransferOwner(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-transfer-owner\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &value = arguments->index(0);

					if(!VariableProcessInteractive::isVariableProcessInteractive(value)) {
						throw(Error("invalid parameter"));
					};

					((VariableProcessInteractive *) this_)->value.transferOwner(((VariableProcessInteractive *)(value.value()))->value);

					return Context::getValueUndefined();
				};


				static TPointer<Variable> processInteractiveJoin(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-join\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					((VariableProcessInteractive *) this_)->value.join();

					return Context::getValueUndefined();
				};

				static TPointer<Variable> processInteractiveIsRunning(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-is-running\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableProcessInteractive *)( this_ ))->value.isRunning());
				};

				static TPointer<Variable> processInteractiveGetReturnValue(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- processinteractive-get-return-value\n");
#endif

					if(!VariableProcessInteractive::isVariableProcessInteractive(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableProcessInteractive *)( this_ ))->value.getReturnValue());
				};


				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("ProcessInteractive", initExecutive);
				};

				void initExecutive(Executive *executive, void *extensionId) {

					String info = "ProcessInteractive\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "ProcessInteractive");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::ProcessInteractive::Version::versionWithBuild());
#endif
					executive->setExtensionPublic(extensionId, true);

					newContext(executive, extensionId);

					executive->compileStringX("Script.requireExtension(\"Buffer\");");
					executive->setFunction2("ProcessInteractive.isProcessInteractive(x)", isProcessInteractive);
					executive->setFunction2("ProcessInteractive.prototype.execute(cmd)",  processInteractiveExecute);
					executive->setFunction2("ProcessInteractive.prototype.read(size)",  processInteractiveRead);
					executive->setFunction2("ProcessInteractive.prototype.readLn(size)",  processInteractiveReadLn);
					executive->setFunction2("ProcessInteractive.prototype.write(str)",  processInteractiveWrite);
					executive->setFunction2("ProcessInteractive.prototype.writeLn(str)",  processInteractiveWriteLn);
					executive->setFunction2("ProcessInteractive.prototype.close()",  processInteractiveClose);
					executive->setFunction2("ProcessInteractive.prototype.terminate(waitMicroSec)",  processInteractiveTerminate);
					executive->setFunction2("ProcessInteractive.prototype.waitToRead(microSec)",  processInteractiveWaitToRead);
					executive->setFunction2("ProcessInteractive.prototype.readToBuffer(buffer)",  processInteractiveReadToBuffer);
					executive->setFunction2("ProcessInteractive.prototype.writeFromBuffer(buffer)",  processInteractiveWriteFromBuffer);
					executive->setFunction2("ProcessInteractive.prototype.becomeOwner(processInteractive)",  processInteractiveBecomeOwner);
					executive->setFunction2("ProcessInteractive.prototype.linkOwner(processInteractive)",  processInteractiveLinkOwner);
					executive->setFunction2("ProcessInteractive.prototype.unLinkOwner(processInteractive)",  processInteractiveUnLinkOwner);
					executive->setFunction2("ProcessInteractive.prototype.transferOwner(processInteractive)",  processInteractiveTransferOwner);
					executive->setFunction2("ProcessInteractive.prototype.join()",  processInteractiveJoin);
					executive->setFunction2("ProcessInteractive.prototype.isRunning()",  processInteractiveIsRunning);
					executive->setFunction2("ProcessInteractive.prototype.getReturnValue()",  processInteractiveGetReturnValue);
				};

			};
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::ProcessInteractive::initExecutive(executive, extensionId);
};
#endif

