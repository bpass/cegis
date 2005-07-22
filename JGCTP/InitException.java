package jgctp;

public class InitException extends Exception
{
	private ProjException m_e;
	
	public InitException(ProjException e)
	{
		m_e = e;
	}
	
	public int errorCode() {return m_e.errorCode();}
	
	public String trace() {return m_e.trace();}
}