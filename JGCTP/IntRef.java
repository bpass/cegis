package JGCTP;

public class IntRef
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