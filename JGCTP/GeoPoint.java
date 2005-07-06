package JGCTP;

public class GeoPoint
{
    public double lon;
    public double lat;
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