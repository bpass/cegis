/**
 * This file is a testing platform for rendering the java2d version of the 
 * DSS decision tree. Since the current version of DSS requires the new JRE, 
 * this element of the DSS tutorial has no additional requirements.
 * @author: James Nelson 8-26-2004
 * version: 1.0 Tutorial Decision Map LM:9-10-2004
 * Additional Notes: Further improvments include, generating different graph
 * displays using vaious graphing architectures, resising to window, enabled 
 * scrolling and zoom in/out functionality, and node shape selection
 * DSS Tutorial is based off the descion tree of the DSS, its serves as a 
 * dynamic outline, or Idea map, to the decisions made by the user as they 
 * work/manuver throught the program. Each element(decison) can be explored 
 * through the decision map by simply holding the cursor over the current 
 * highlighted decision node a veiw the info popup, for additoinal background 
 * information click more hyperlink. 
 */

/**
 * Uses tables for values
/**/
public abstract class DSSUpdater 
{
  /*public DSSUpdater()
  {
  }/**/
  private static int HighLight = 0;
  private static int HighPath = 0;
  private static int Gname = 0;
  private static String stuff = "holy";
  private static int HighPathAr[] = {0};
  private static int VisAr[] = {0};
  public static synchronized void setHighLight(int i)
                {
                HighLight = i;
                }
  public static synchronized void setHighPath(int i)
                {
                HighPath = i;
                }
  public static synchronized void setHighPathAr(int[] i)
                {
                    if(HighPathAr.length != i.length)
                    {
                    HighPathAr = new int[i.length];
                    }
                    System.arraycopy(i,0,HighPathAr,0,i.length);
                }
                
  public static synchronized void setVisAr(int[] i)
                {
                    if(VisAr.length != i.length)
                    {
                    VisAr = new int[i.length];
                    }
                    System.arraycopy(i,0,VisAr,0,i.length);
                }
                
  public static synchronized void setGraphName(int name)
                {
                Gname = name;
                }
  public static synchronized void setStuffName(String name)
  {
    stuff = new String(name);
  }
   public static synchronized int getHighLight()
                {
                return HighLight;
                }
   public static synchronized int getHighPath()
                {
                return HighPath;
                }
  public static synchronized void getHighPathAr(int[] i)
                {
                    //HighPathAr = new int[i.length];
                    if(HighPathAr.length != i.length)
                    {
                    i = new int[HighPathAr.length];
                    }
                    System.arraycopy(HighPathAr,0,i,0,HighPathAr.length);
                }
    public static synchronized int[] getHighPathAr()
                {
                    //HighPathAr = new int[i.length];
                    //i = new int[HighPathAr.length];
                    //System.arraycopy(HighPathAr,0,i,0,HighPathAr.length);
                    return HighPathAr;
                }
                
   public static synchronized int[] getVisAr()
                {
                    //HighPathAr = new int[i.length];
                    //i = new int[HighPathAr.length];
                    //System.arraycopy(HighPathAr,0,i,0,HighPathAr.length);
                    return VisAr;
                }
                
  public static synchronized int getHighPathLength()
  {
    return HighPathAr.length;
  }
   public static synchronized int getGraphName()
                {
                return Gname;
                }
  
   public static synchronized String getStuffName()
   {
     return stuff;
   }
}
