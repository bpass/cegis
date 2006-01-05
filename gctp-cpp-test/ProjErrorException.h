#ifndef PROJ_ERR_EX_H
#define PROJ_ERR_EX_H

class ProjErrorException
{
public:

	ProjErrorException(int error, int projNum, std::string func) :
	  m_errNum(error), m_projNum(projNum), m_func(func) {}

	int getErrorNum() {return m_errNum;}
	int getProjNum() {return m_projNum;}
	std::string getFunc() {return m_func;}

private:

	int m_errNum;
	int m_projNum;
	std::string m_func;
};

#endif
