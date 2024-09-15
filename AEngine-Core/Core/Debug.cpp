#include "Debug.h"

//definition of the static variables declard in Debug.h
const char* AE::Core::Debug::SeverityNames[SEVERITY_COUNT] = {
	ENUM_TO_STRING(AE_ALL),
	ENUM_TO_STRING(AE_WARNING),
	ENUM_TO_STRING(AE_ERROR)
};


AE::Core::Debug::Severity AE::Core::Debug::m_severity = AE::Core::Debug::Severity::AE_WARNING;