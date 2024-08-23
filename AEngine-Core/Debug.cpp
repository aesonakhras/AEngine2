#include "Debug.h"

//definition of the static variables declard in Debug.h
const char* AECore::Debug::SeverityNames[SEVERITY_COUNT] = {
	ENUM_TO_STRING(AE_ALL),
	ENUM_TO_STRING(AE_WARNING),
	ENUM_TO_STRING(AE_ERROR)
};


AECore::Debug::Severity AECore::Debug::m_severity = AECore::Debug::Severity::AE_WARNING;