
class DoubleRef
{
	private double m_number;
	
	public DoubleRef() {m_number = 0;}
	public DoubleRef(double num) {m_number = num;}
	
	public double value() {return(m_number);}
	
	public void setValue(double val) {m_number = val;}
	
	public double multiply(double val) {return(m_number * val);}
	
	public double add(double val) {return (m_number + val);}
	
	public double subtract(double val) {return (m_number - val);}
	
	public double multiply(int val) {return(m_number * val);}
	
	public double add(int val) {return (m_number + val);}
	
	public double subtract(int val) {return (m_number - val);}
}

class IntRef
{
	private int m_number;
	
	public IntRef() {m_number = 0;}
	public IntRef(int num) {m_number = num;}
	
	public int value() {return(m_number);}
	
	public void setValue(int val) {m_number = val;}
	
	public int multiply(int val) {return(m_number * val);}
	
	public int add(int val) {return (m_number + val);}
	
	public int subtract(int val) {return (m_number - val);}
	
	public double multiply(double val) {return(m_number * val);}
	
	public double add(double val) {return (m_number + val);}
	
	public double subtract(double val) {return (m_number - val);}
}