package JGCTP;

public class TransformationException extends Exception
{
	private ProjException m_e;
	
	public TransformationException(ProjException e)
	{
		m_e = e;
	}
	
	public int errorCode() {return m_e.errorCode();}
	
	public String function() {return m_e.function();}
}