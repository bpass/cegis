package JGCTP;

public class ProjException extends Exception
{
	private String m_callTrace;
	private int m_errorCode;
	
	public ProjException(int errorCode, String functionName)
	{
		m_errorCode = errorCode;
		m_callTrace = new String(functionName);
	}
	
	public int errorCode() {return m_errorCode;}
	
	public String trace() {return new String(m_callTrace);}
    
    public void addFunc(String funcName)
    {
        m_callTrace = " -> " + funcName;
    }
}