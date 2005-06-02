
#include "projexception.h"

std::map<long, std::string> ProjException::m_errorMap;
bool ProjException::m_initError = false;

ProjException::ProjException(long error, std::string function = NULL) {
	
	if(!m_initError) 
		initErrorTable();
	
	std::map<long, std::string>::iterator iter;
	m_errorMessage.clear();
	if((iter = m_errorMap.find(error)) != m_errorMap.end()) 
		m_errorMessage.append(iter->second);
	else
		m_errorMessage.append("Unknown error\n");

	if(!function.empty()) {
		m_errorMessage.append(" in function ");
		m_errorMessage.append(function);
		m_errorMessage.append("\n");
	}
}

//initialize error map 
void ProjException::initErrorTable() {
	typedef std::pair<long, const char*> errorPair;

	m_errorMap.insert(errorPair(2, "Convergence Error"));
	m_errorMap.insert(errorPair(3, "Latitude failed to converge after 15 iterations"));
	m_errorMap.insert(errorPair(4, "Latitude failed to converge"));
	m_errorMap.insert(errorPair(5, "Illegal source or target unit code"));
	m_errorMap.insert(errorPair(6, "Output file name not specified"));
	m_errorMap.insert(errorPair(11, "Illegal zone number"));
	m_errorMap.insert(errorPair(21, "Illegal zone number for spheroid"));
	m_errorMap.insert(errorPair(22, "Error opening state plane parameter file"));
	m_errorMap.insert(errorPair(23, "Illegal spheroid"));
	m_errorMap.insert(errorPair(31, "Equal latitudes for St. parallels on opposite sides of equator"));
	m_errorMap.insert(errorPair(41, "Equal latitudes for St. parallels on opposite sides of equator"));
	m_errorMap.insert(errorPair(44, "Point can not be projected"));
	m_errorMap.insert(errorPair(53, "Transformation cannot be computed at the poles"));
	m_errorMap.insert(errorPair(81, "Standard parallels on opposite sides of equator"));
	m_errorMap.insert(errorPair(93, "Point projects into infinity"));
	m_errorMap.insert(errorPair(95, "Latitude failed to converge"));
	m_errorMap.insert(errorPair(103, "Point projects into infinity"));
	m_errorMap.insert(errorPair(113, "Point projects to a circle"));
	m_errorMap.insert(errorPair(115, "Input data error"));
	m_errorMap.insert(errorPair(123, "Point projects to a circle"));
	m_errorMap.insert(errorPair(125, "Input data error"));
	m_errorMap.insert(errorPair(133, "Point projects into infinity"));
	m_errorMap.insert(errorPair(143, "Point cannot be projected"));
	m_errorMap.insert(errorPair(145, "Input data error"));
	m_errorMap.insert(errorPair(153, "Point cannot be projected"));
	m_errorMap.insert(errorPair(155, "Input data error"));
	m_errorMap.insert(errorPair(164, "Input data error"));
	m_errorMap.insert(errorPair(174, "Input data error"));
	m_errorMap.insert(errorPair(201, "Input data error"));
	m_errorMap.insert(errorPair(202, "Input data error"));
	m_errorMap.insert(errorPair(205, "Point projects into infinity"));
	m_errorMap.insert(errorPair(214, "50 iterations without convergence"));
	m_errorMap.insert(errorPair(234, "Too many iterations in inverse"));
	m_errorMap.insert(errorPair(235, "Too many iterations in inverse"));
	m_errorMap.insert(errorPair(236, "Too many iterations in inverse"));
	m_errorMap.insert(errorPair(241, "Iteration failed to converge"));
	m_errorMap.insert(errorPair(251, "Iteration failed to converge"));
	m_errorMap.insert(errorPair(252, "Input data error"));
	m_errorMap.insert(errorPair(1101, "Incompatible unit codes"));
	m_errorMap.insert(errorPair(1116, "Illegal DMS field"));
	m_errorMap.insert(errorPair(0, "Parameter index out of bounds"));

	m_initError = true;
}

//add a new error code/string pair to the error table
bool ProjException::addError(long errorCode, std::string error) {
	if(m_errorMap.find(errorCode) != m_errorMap.end())
		return false;

	std::pair<long, std::string> newError(errorCode, error);
	m_errorMap.insert(newError);
}
