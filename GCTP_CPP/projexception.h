
#ifndef PROJ_EX_H
#define PROJ_EX_H
#include <map>
#include <string>

class ProjException {
public:
	ProjException(long error, std::string function);
	std::string error() {return m_errorMessage;}
	static bool addError(long errorCode, std::string error);
private:
	void initErrorTable();
	static std::map<long, std::string> m_errorMap;
	static bool m_initError;
	std::string m_errorMessage;
};

#endif