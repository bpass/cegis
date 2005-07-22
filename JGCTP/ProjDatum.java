package jgctp;

// Datum/Spheroid codes
public enum ProjDatum
{
	NULLDATUM(-1),
	CLARKE_1866(0),
	CLARKE_1880(1),
	BESSEL(2),
	INTERNATIONAL_1967(3),
	INTERNATIONAL_1909(4),
	WGS_72(5),
	EVEREST(6),
	WGS_66(7),
	GRS_1980(8),
	AIRY(9),
	MODIFIED_EVEREST(10),
	MODIFIED_AIRY(11),
	WGS_84(12),
	SOUTHEAST_ASIA(13),
	AUSTRALIAN_NATIONAL(14),
	KRASSOVSKY(15),
	HOUGH(16),
	MERCURY_1960(17),
	MODIFIED_MERCURY_1968(18),
	EARTH(19),
	BESSEL_1841_NAMIBIA(20),
	EVEREST_SABAH(21),
	EVEREST_INDIA_1956(22),
	EVEREST_MALAYSIA_1969(23),
	EVEREST_MALAY_1948(24),
	EVEREST_PAKISTAN(25),
	HAYFORD(26),
	HELMERT_1906(27),
	INDONESIAN_1974(28),
	SOUTH_AMERICAN_1969(29),
	WGS_60(30);
		
	private final int m_val;

   private ProjDatum( int val )
   {
      m_val = val;
   }

   public int val()
   {
      return m_val;
   }

   public ProjDatum fromVal( int val )
   {
      switch (val)
      {
      case 0:
         return CLARKE_1866;
      case 1:
         return CLARKE_1880;
      case 2:
         return BESSEL;
      case 3:
         return INTERNATIONAL_1967;
      case 4:
         return INTERNATIONAL_1909;
      case 5:
         return WGS_72;
      case 6:
         return EVEREST;
      case 7:
         return WGS_66;
      case 8:
         return GRS_1980;
      case 9:
         return AIRY;
      case 10:
         return MODIFIED_EVEREST;
      case 11:
         return MODIFIED_AIRY;
      case 12:
         return WGS_84;
      case 13:
         return SOUTHEAST_ASIA;
      case 14:
         return AUSTRALIAN_NATIONAL;
      case 15:
         return KRASSOVSKY;
      case 16:
         return HOUGH;
      case 17:
         return MERCURY_1960;
      case 18:
         return MODIFIED_MERCURY_1968;
      case 19:
         return EARTH;
      case 20:
         return BESSEL_1841_NAMIBIA;
      case 21:
         return EVEREST_SABAH;
      case 22:
         return EVEREST_INDIA_1956;
      case 23:
         return EVEREST_MALAYSIA_1969;
      case 24:
         return EVEREST_MALAY_1948;
      case 25:
         return EVEREST_PAKISTAN;
      case 26:
         return HAYFORD;
      case 27:
         return HELMERT_1906;
      case 28:
         return INDONESIAN_1974;
      case 29:
         return SOUTH_AMERICAN_1969;
      case 30:
         return WGS_60;
      }

      return NULLDATUM;
   }
}