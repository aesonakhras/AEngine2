#pragma once
#include <iostream>
#include <string>

#define ENUM_TO_STRING(name) #name

namespace AECore {
	class Debug {
	//Note: private members are first here
	private:
		enum Severity {
			AE_ALL,
			AE_WARNING,
			AE_ERROR,
			SEVERITY_COUNT
		};

		static void _Log(std::string message, Severity severity = AE_ALL) {
			if (severity < SEVERITY_COUNT) {
				if (severity >= m_severity) {
					std::cout << SeverityNames[severity] << ": " << message << std::endl;
				}
			}
		}

		static Severity m_severity;
		static const char* SeverityNames[SEVERITY_COUNT];
	
	public:
			static void SetLogSeverity(Severity newSeverity) {
				m_severity = newSeverity;
			}

			static void LogError(std::string message) {
				_Log(message, AE_ERROR);
			}

			static void LogWarning(std::string message) {
				_Log(message, AE_WARNING);
			}

			static void Log(std::string message) {
				_Log(message, AE_ALL);
			}
	};

	//definition of the static variables declared above
	const char* Debug::SeverityNames[SEVERITY_COUNT] = {
		ENUM_TO_STRING(AE_ALL),
		ENUM_TO_STRING(AE_WARNING),
		ENUM_TO_STRING(AE_ERROR)
	};

	Debug::Severity Debug::m_severity = Debug::Severity::AE_WARNING;
}