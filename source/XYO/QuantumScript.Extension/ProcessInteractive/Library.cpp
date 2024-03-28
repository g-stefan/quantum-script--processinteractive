// Quantum Script Extension ProcessInteractive
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/ProcessInteractive/Library.hpp>
#include <XYO/QuantumScript.Extension/ProcessInteractive/Copyright.hpp>
#include <XYO/QuantumScript.Extension/ProcessInteractive/License.hpp>
#include <XYO/QuantumScript.Extension/ProcessInteractive/Version.hpp>
#include <XYO/QuantumScript.Extension/ProcessInteractive/VariableProcessInteractive.hpp>
#include <XYO/QuantumScript.Extension/ProcessInteractive/Context.hpp>
#include <XYO/QuantumScript.Extension/ProcessInteractive/Library.Source.cpp>
#include <XYO/QuantumScript.Extension/Buffer/VariableBuffer.hpp>

namespace XYO::QuantumScript::Extension::ProcessInteractive {

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
		(Context::getGlobalObject())->setPropertyBySymbol(processInteractiveContext->symbolFunctionProcessInteractive, defaultPrototypeFunction);
		processInteractiveContext->prototypeProcessInteractive = defaultPrototypeFunction->prototype;
	};

	static TPointer<Variable> isProcessInteractive(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-is-processinteractive\n");
#endif
		return VariableBoolean::newVariable(TIsType<VariableProcessInteractive>(arguments->index(0)));
	};

	static TPointer<Variable> processInteractiveExecute(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-execute\n");
#endif

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableBoolean::newVariable(((VariableProcessInteractive *)(this_))->value.execute((arguments->index(0))->toString()));
	};

	static TPointer<Variable> processInteractiveRead(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-read\n");
#endif
		String retV;
		Number ln;
		size_t readLn;
		size_t readToLn;
		size_t readX;
		size_t readTotal;
		size_t k;
		char buffer[32768];

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (TIsTypeExact<VariableUndefined>(arguments->index(0))) {
			ln = 32768;
		} else {

			ln = (arguments->index(0))->toNumber();
			if (isnan(ln) || isinf(ln) || signbit(ln)) {
				return Context::getValueUndefined();
			};
		};

		readToLn = (size_t)(ln);
		readTotal = 0;
		readX = 32768;
		if (readToLn < readX) {
			readX = readToLn;
		};
		for (;;) {
			readLn = ((VariableProcessInteractive *)this_)->value.read(buffer, readX);

			if (readLn > 0) {
				retV.concatenate(buffer, readLn);
			};
			// end of file
			if (readLn < readX) {
				break;
			};
			// end of read
			readTotal += readLn;
			if (readTotal >= readToLn) {
				break;
			};
			readX = readToLn - readTotal;
			if (readX > 32768) {
				readX = 32768;
			};
		};

		return VariableString::newVariable(retV);
	};

	static TPointer<Variable> processInteractiveReadLn(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-read-ln\n");
#endif
		String retV;
		Number ln;

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (TIsTypeExact<VariableUndefined>(arguments->index(0))) {
			ln = 32768;
		} else {

			ln = (arguments->index(0))->toNumber();
			if (isnan(ln) || isinf(ln) || signbit(ln)) {
				return Context::getValueUndefined();
			};
		};

		if (Stream::readLn(((VariableProcessInteractive *)this_)->value, retV, ln)) {
			return VariableString::newVariable(retV);
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> processInteractiveWrite(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-write\n");
#endif

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		String toWrite = (arguments->index(0))->toString();
		return VariableNumber::newVariable((Number)(((VariableProcessInteractive *)this_)->value.write(toWrite.value(), toWrite.length())));
	};

	static TPointer<Variable> processInteractiveWriteLn(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-write-ln\n");
#endif

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		String toWrite = (arguments->index(0))->toString();
		toWrite << "\r\n";
		return VariableNumber::newVariable((Number)(((VariableProcessInteractive *)this_)->value.write(toWrite.value(), toWrite.length())));
	};

	static TPointer<Variable> processInteractiveClose(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-close\n");
#endif

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		((VariableProcessInteractive *)this_)->value.close();

		return Context::getValueUndefined();
	};

	static TPointer<Variable> processInteractiveTerminate(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-terminate\n");
#endif

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		((VariableProcessInteractive *)this_)->value.terminate((arguments->index(0))->toIndex());

		return Context::getValueUndefined();
	};

	static TPointer<Variable> processInteractiveWaitToRead(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-wait-to-read\n");
#endif

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		Number microSeconds = (arguments->index(0))->toNumber();
		if (isnan(microSeconds) || isinf(microSeconds) || signbit(microSeconds)) {
			return VariableNumber::newVariable(NAN);
		};
		return VariableNumber::newVariable(((VariableProcessInteractive *)this_)->value.waitToRead((uint32_t)microSeconds));
	};

	static TPointer<Variable> processInteractiveReadToBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-read-to-buffer\n");
#endif
		size_t readLn;
		size_t readToLn;
		size_t readX;
		size_t readTotal;
		size_t k;
		Number ln;

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		TPointerX<Variable> &buffer(arguments->index(0));

		if (!TIsType<Extension::Buffer::VariableBuffer>(buffer)) {
			throw(Error("invalid parameter"));
		};

		if (TIsTypeExact<VariableUndefined>(arguments->index(1))) {
			ln = 32768;
		} else {

			ln = (arguments->index(1))->toNumber();
			if (isnan(ln) || signbit(ln) || ln == 0.0) {
				((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.length = 0;
				return VariableNumber::newVariable(0);
			};
			if (isinf(ln)) {
				ln = ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.size;
			};
		};

		if (ln > ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.size) {
			ln = ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.size;
		};

		readToLn = (size_t)ln;
		readTotal = 0;
		readX = readToLn;
		for (;;) {
			readLn = ((VariableProcessInteractive *)this_)->value.read(&(((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.buffer)[readTotal], readX);
			// end of transmision
			if (readLn == 0) {
				break;
			};
			readTotal += readLn;
			if (readTotal >= readToLn) {
				break;
			};
			readX = readToLn - readTotal;
		};
		((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.length = readTotal;
		return VariableNumber::newVariable(readTotal);
	};

	static TPointer<Variable> processInteractiveWriteFromBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-write-from-buffer\n");
#endif

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		TPointerX<Variable> &buffer(arguments->index(0));

		if (!TIsType<Extension::Buffer::VariableBuffer>(buffer)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable((Number)(((VariableProcessInteractive *)this_)->value.write(((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.buffer, ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.length)));
	};

	static TPointer<Variable> processInteractiveJoin(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-join\n");
#endif

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		((VariableProcessInteractive *)this_)->value.join();

		return Context::getValueUndefined();
	};

	static TPointer<Variable> processInteractiveIsRunning(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-is-running\n");
#endif

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableBoolean::newVariable(((VariableProcessInteractive *)(this_))->value.isRunning());
	};

	static TPointer<Variable> processInteractiveGetReturnValue(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-get-return-value\n");
#endif

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableProcessInteractive *)(this_))->value.getReturnValue());
	};

	static TPointer<Variable> processInteractiveUseConPTY(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-use-con-pty\n");
#endif

		if (!TIsType<VariableProcessInteractive>(this_)) {
			throw(Error("invalid parameter"));
		};

		((VariableProcessInteractive *)(this_))->value.useConPTY((arguments->index(0))->toBoolean());

		return Context::getValueUndefined();
	};

	static TPointer<Variable> processRun(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- processinteractive-run\n");
#endif
		String retV;
		if (XYO::System::ProcessInteractive::run(arguments->index(0)->toString(), retV, arguments->index(0)->toBoolean())) {
			return VariableString::newVariable(retV);
		};

		return Context::getValueUndefined();
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("ProcessInteractive", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {

		String info = "ProcessInteractive\r\n";
		info << License::shortLicense().c_str();

		executive->setExtensionName(extensionId, "ProcessInteractive");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::ProcessInteractive::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);

		newContext(executive, extensionId);

		executive->compileStringX("Script.requireExtension(\"Buffer\");");
		executive->setFunction2("ProcessInteractive.isProcessInteractive(x)", isProcessInteractive);
		executive->setFunction2("ProcessInteractive.prototype.execute(cmd)", processInteractiveExecute);
		executive->setFunction2("ProcessInteractive.prototype.read(size)", processInteractiveRead);
		executive->setFunction2("ProcessInteractive.prototype.readLn(size)", processInteractiveReadLn);
		executive->setFunction2("ProcessInteractive.prototype.write(str)", processInteractiveWrite);
		executive->setFunction2("ProcessInteractive.prototype.writeLn(str)", processInteractiveWriteLn);
		executive->setFunction2("ProcessInteractive.prototype.close()", processInteractiveClose);
		executive->setFunction2("ProcessInteractive.prototype.terminate(waitMicroSec)", processInteractiveTerminate);
		executive->setFunction2("ProcessInteractive.prototype.waitToRead(microSec)", processInteractiveWaitToRead);
		executive->setFunction2("ProcessInteractive.prototype.readToBuffer(buffer)", processInteractiveReadToBuffer);
		executive->setFunction2("ProcessInteractive.prototype.writeFromBuffer(buffer)", processInteractiveWriteFromBuffer);
		executive->setFunction2("ProcessInteractive.prototype.join()", processInteractiveJoin);
		executive->setFunction2("ProcessInteractive.prototype.isRunning()", processInteractiveIsRunning);
		executive->setFunction2("ProcessInteractive.prototype.getReturnValue()", processInteractiveGetReturnValue);
		executive->setFunction2("ProcessInteractive.prototype.useConPTY(value)", processInteractiveUseConPTY);
		executive->setFunction2("ProcessInteractive.run(cmd,useConPTY)", processRun);

		//
		executive->compileStringX(librarySource);
	};

};

#ifdef XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_PROCESSINTERACTIVE_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::ProcessInteractive::initExecutive(executive, extensionId);
};
#endif
