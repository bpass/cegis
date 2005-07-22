package jgctp;

public class Sphere
{
    public double rMajor;
    public double rMinor;
    public double radius;
    
    
    public Sphere()
    {
        rMajor = 0.0;
        rMinor = 0.0;
        radius = 0.0;
    }
    
    public Sphere(double rMajor, double rMinor, double radius)
    {
        rMajor = 0.0;
        rMinor = 0.0;
        radius = 0.0;
    }
    
    public Sphere(Sphere s)
    {
        rMajor = s.rMajor;
        rMinor = s.rMinor;
        radius = s.radius;
    }

}