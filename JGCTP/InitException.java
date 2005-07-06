package JGCTP;

public class InitException extends Exception
{
	private ProjException m_e;
	
	public InitException(ProjException e)
	{
		m_e = e;
	}
	
	public int errorCode() {return m_e.errorCode();}
	
	public String function() {return m_e.function();}
}