package JGCTP;

public class CoordPoint
{
    public double x;
    public double y;
    public CoordPoint(double _x, double _y)
    {
        x = _x;
        y = _y;
    }
    
    public CoordPoint(CoordPoint p)
    {
        x = p.x;
        y = p.y;
    }
    
}