package JGCTP;

public class GeoPoint
{
    public double lon;
    public double lat;
    
    public GeoPoint() 
    {
        lon = 0;
        lat = 0;
    }
    public GeoPoint(double _lon, double _lat)
    {
        lon = _lon;
        lat = _lat;
    }
    
    public GeoPoint(GeoPoint p)
    {
        lon = p.lon;
        lat = p.lat;
    }
}