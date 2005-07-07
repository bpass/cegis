package JGCTP;

public enum ProjCode
{
	NONE(-1),
	GEO(0), 
	_UTM(1), 
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
	
    private ProjCode()
    {
        m_val = -1;
    }
	private ProjCode(int val)
	{
		m_val = val;
	}
	
	public int val()
	{
		return m_val;
	}
}