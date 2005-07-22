package jgctp;

// Projection Codes
public enum ProjCode
{
	NULLPROJ(-1),
	GEO(0), 
	UTM(1), 
	SPCS(2), 
	ALBERS(3), 
	LAMCC(4), 
	MERCAT(5), 
	PS(6), 
	POLYC(7), 
	EQUIDC(8),
	TM(9), 
	STEREO(10), 
	LAMAZ(11), 
	AZMEQD(12),
	GNOMON(13), 
	ORTHO(14), 
	GVNSP(15), 
	SNSOID(16), 
	EQRECT(17), 
	MILLER(18), 
	VGRINT(19), 
	HOM(20), 
	ROBIN(21), 
	SOM(22), 
	ALASKA(23), 
	GOOD(24),
	MOLL(25), 
	IMOLL(26), 
	HAMMER(27), 
	WAGIV(28), 
	WAGVII(29), 
	OBEQA(30);
	
	private final int m_val;

   private ProjCode( int val )
   {
      m_val = val;
   }

   public int val()
   {
      return m_val;
   }

   public ProjCode fromVal( int val )
   {
      switch (val)
      {
      case 0:
         return GEO;
      case 1:
         return UTM;
      case 2:
         return SPCS;
      case 3:
         return ALBERS;
      case 4:
         return LAMCC;
      case 5:
         return MERCAT;
      case 6:
         return PS;
      case 7:
         return POLYC;
      case 8:
         return EQUIDC;
      case 9:
         return TM;
      case 10:
         return STEREO;
      case 11:
         return LAMAZ;
      case 12:
         return AZMEQD;
      case 13:
         return GNOMON;
      case 14:
         return ORTHO;
      case 15:
         return GVNSP;
      case 16:
         return SNSOID;
      case 17:
         return EQRECT;
      case 18:
         return MILLER;
      case 19:
         return VGRINT;
      case 20:
         return HOM;
      case 21:
         return ROBIN;
      case 22:
         return SOM;
      case 23:
         return ALASKA;
      case 24:
         return GOOD;
      case 25:
         return MOLL;
      case 26:
         return IMOLL;
      case 27:
         return HAMMER;
      case 28:
         return WAGIV;
      case 29:
         return WAGVII;
      case 30:
         return OBEQA;
      }

      return NULLPROJ;
   }
}