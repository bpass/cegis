package jgctp;

// Unit Codes
public enum ProjUnit
{
   NULLUNIT( -1 ),
   RADIAN( 0 ), // Radians
   FEET( 1 ), // Feet
   METER( 2 ), // Meters
   SECOND( 3 ), // Seconds
   DEGREE( 4 ), // Decimal Degrees
   INT_FEET( 5 ); // International Feet

   private final int m_val;

   private ProjUnit()
   {
      m_val = -1;
   }

   private ProjUnit( int val )
   {
      m_val = val;
   }

   public int val()
   {
      return m_val;
   }

   public static ProjUnit fromVal( int val )
   {
      switch (val)
      {
      case 0:
         return RADIAN;
      case 1:
         return FEET;
      case 2:
         return METER;
      case 3:
         return SECOND;
      case 4:
         return DEGREE;
      case 5:
         return INT_FEET;
      }

      return NULLUNIT;
   }
}