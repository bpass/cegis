package JGCTP;

//Spheroid codes
public enum ProjDatum 
{
		NOT_SET(-1),
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
		  
        private ProjDatum(int val)
		{
		  	m_val = val;
		}
		  
		public int val()
		{
		  	return m_val;
		}
}