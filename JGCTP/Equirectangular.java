package JGCTP;

public class Equirectangular extends Projection
{
    public Equirectangular()
    {
        super();
        setNumber(ProjCode.EQRECT);
        setName("Equirectangular");
    }
    
    public Equirectangular(double gctpParams[], ProjUnit units, ProjDatum dat)
    {
        super(gctpParams, units, dat);
        setNumber(ProjCode.EQRECT);
        setName("Equirectangular");
    }
     
    protected void _inverse(CoordPoint _p) throws ProjException
    {
        CoordPoint p = new CoordPoint(_p);
        /* Inverse equations */
        p.x -= m_falseEasting;
        p.y -= m_falseNorthing;

        m_lonLat.lat = p.y / m_sphere.radius;

        if( Math.abs( m_lonLat.lat ) > Constants.HALF_PI ) 
        {
            throw(new ProjException(174, "Equirectangular._inverse()"));
        }

        m_lonLat.lon = Util.adjust_lon( m_center.lon + p.x / ( m_sphere.radius * Math.cos( m_center.lon )));
    }
    protected void _forward(GeoPoint _p) throws ProjException
    {
        double deltaLon;        /* delta longitude value            */
        GeoPoint p = new GeoPoint(_p);
        /* Forward equations */
        deltaLon = Util.adjust_lon( p.lon - m_center.lon );
        m_xy.x = m_falseEasting + m_sphere.radius * deltaLon * Math.cos( m_center.lat );
        m_xy.y = m_falseNorthing + m_sphere.radius * p.lat;
    }
}