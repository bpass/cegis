package jgctp;

public class Transformer
{
   private Projection inv_proj;

   private Projection for_proj;

   public void setInverseProjection( Projection proj )
   {
      inv_proj = proj;
   }

   public void setInverseProjection( ProjCode projCode )
   {
      inv_proj = getProjection( projCode );
   }

   public Projection inverseProjection()
   {
      return inv_proj;
   }

   public void setForwardProjection( Projection proj )
   {
      for_proj = proj;
   }

   public void setForwardProjection( ProjCode projCode )
   {
      for_proj = getProjection( projCode );
   }

   public Projection forwardProjection()
   {
      return for_proj;
   }
   
   public void transform( CoordPoint inPoint, CoordPoint outPoint )
   {
      GeoPoint gPoint = new GeoPoint();
      
      try
      {
         inv_proj.inverse(inPoint, gPoint);
         for_proj.forward(gPoint, outPoint);
      }
      catch( Exception e )
      {
         e.printStackTrace();
      }
   }
   
   public void transformInverse( CoordPoint inPoint, GeoPoint outPoint )
   {
      try
      {
         for_proj.inverse(inPoint, outPoint);
      }
      catch( Exception e )
      {
         e.printStackTrace();
      }
   }
   
   public void transformForward( GeoPoint inPoint, CoordPoint outPoint )
   {
      try
      {
         for_proj.forward(inPoint, outPoint);
      }
      catch( Exception e )
      {
         e.printStackTrace();
      }
   }

   public static Projection getProjection( ProjCode projCode )
   {
      switch (projCode.val())
      {
      case 0:
         return null;
      case 1:
         return new UTM();
      case 2:
         return null;
      case 3:
         return new AlbersConEqArea();
      case 4:
         return new LambertCC();
      case 5:
         return new Mercator();
      case 6:
         return new PolarStereo();
      case 7:
         return new Polyconic();
      case 8:
         return new EquidistantC();
      case 9:
         return new TransverseMercator();
      case 10:
         return new Stereo();
      case 11:
         return new LambertAzimuthal();
      case 12:
         return new AzEquidistant();
      case 13:
         return null;
      case 14:
         return new Orthographic();
      case 15:
         return new GenVertNSP();
      case 16:
         return new Sinusoidal();
      case 17:
         return new Equirectangular();
      case 18:
         return new Miller();
      case 19:
         return new VanDerGrinten();
      case 20:
         return new HotineObMerc();
      case 21:
         return new Robinson();
      case 22:
         return new SpaceObMerc();
      case 23:
         return null;
      case 24:
         return new GoodeH();
      case 25:
         return new Mollweide();
      case 26:
         return new IntMollweide();
      case 27:
         return new Hammer();
      case 28:
         return new WagnerIV();
      case 29:
         return new WagnerVII();
      case 30:
         return new OblatedEqArea();
      }

      return null;
   }
}
