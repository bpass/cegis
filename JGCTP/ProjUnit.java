package JGCTP;

/* Define unit code numbers to their names */
public enum ProjUnit 
{
		  UNDEF(-1),
		  RADIAN(0), //Radians
		  FEET(1), //Feet
		  METER(2), //Meters
		  SECOND(3), //Seconds
		  DEGREE(4), //Decimal Degrees
		  INT_FEET(5); //International Feet
		  
		  private final int m_val;
		  
          private ProjUnit()
          {
              m_val = -1;
          }
		  private ProjUnit(int val)
		  {
		  	m_val = val;
		  }
		  
		  public int val()
		  {
		  	return m_val;
		  }
}