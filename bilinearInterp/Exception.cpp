#include "Exception.h"
#include <stdio.h>
GeneralException::GeneralException(const std::string& message):
	m_message(message)
{
}

void GeneralException::printMessage() {
	printf("%s\n", m_message.c_str());
}

std::string GeneralException::getMessage() {
	return(m_message);
}