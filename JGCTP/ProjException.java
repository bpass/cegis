package JGCTP;

public class ProjException extends Exception
{
	private String m_functionName;
	private int m_errorCode;
	
	public ProjException(int errorCode, String functionName)
	{
		m_errorCode = errorCode;
		m_functionName = new String(functionName);
	}
	
	public int errorCode() {return m_errorCode;}
	
	public String function() {return new String(m_functionName);}
}