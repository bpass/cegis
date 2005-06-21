/**
 * This File is a testing platform for rendering the java2d version of the 
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
import java.awt.*;
import java.applet.Applet;
import java.awt.event.ItemListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.font.FontRenderContext;
import java.awt.font.TextLayout;
import java.awt.geom.*;
import java.awt.geom.Ellipse2D;
import java.awt.image.*;
import java.awt.geom.AffineTransform;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.event.WindowAdapter;
import java.lang.reflect.Array;
import javax.swing.JApplet;
import java.lang.String;

public class TextNode 
{   
    public String buffer;
    public static int N_SIZE =512;
    public String parse[];
    public char holder[];
    public static String Delimeters = "/";
    public static char delim[] = {' ','/'};
    int size =0;
    public Point2D.Double cent;
    public Rectangle2D.Double textSpace;
    public Rectangle2D.Double nodeSpace;
    public Color myblue;
    public TextLayout[] ttL;
    public double maxwidth;
    public double stdheight;
    public double lineS = 1.5;

  public TextNode()
  { 
    if(size != 0)
    {      
      breaker();
    }
    nodeSpace = new Rectangle2D.Double();
    textSpace = new Rectangle2D.Double();
    myblue = new Color(204,204,255);
  }
  
  public TextNode(String buf)
  {   
    buffer = new String(buf);        
    breaker();   
    size = Array.getLength(parse);     
    holder = new char[N_SIZE];
    nodeSpace = new Rectangle2D.Double();
    textSpace = new Rectangle2D.Double();
    myblue = new Color(204,204,255);
  }
  
  public void setC(double x, double y)
  {
    cent = new Point2D.Double(x,y);
  }
 
  public void setC(Point2D.Double tcent)
  {
    cent = new Point2D.Double(tcent.x,tcent.y);
  }
  
   public int longestStr()
  {
    int temp=0;
    int id = 0;
    for(int i =0; i < getSize(); i++)
    {
      if(parse[i] != null && temp < parse[i].length())
      {
        temp = parse[i].length();
        id =i;
      }      
    }
    return id;
  }  
  
  public int getSize()
  {
    return size;
  }
 
  public int getLength()
  {
    return buffer.length();
  }
  
  public void breaker()
  {
    int beg,end, cntr, whS;
    beg =0;
    end = 0;
    cntr = 0;
    char del = '/';
    
    beg = buffer.indexOf('/');
    if(beg == -1 && buffer != null) //singal  strings work fine
    {
      parse = new String[1];
      parse[0] = new String(buffer);
    }
    else //here multiple strings create an extra null string
    {
      while(beg != -1)
      {
        cntr++;
        beg = buffer.indexOf('/',beg+1);
      }
      whS =cntr+1;
      if(whS > 0)
      {
        parse = new String[whS];
      }          
      cntr = 0;
      beg = buffer.indexOf(del);
      end = buffer.indexOf(del,beg+1);
      if(beg > 0 && whS >0) //if the first part is string
      {
        if(end == -1)//only one string
        {
          parse[0] =  buffer.substring(0,beg);       
        }
        else
        {
          parse[0] = new String(buffer.substring(0,beg));
          cntr =1;
          while(end != -1 && cntr <whS)
          {
            parse[cntr] =  new String(buffer.substring(beg+1,end));
            beg = end;
            end = buffer.indexOf(del,beg+1); 
          }                           
        }
        int check;
        check = buffer.length();
        String test;
        test = new String(buffer.substring(beg+1,check));
        if( test != null)
        {
          parse[cntr+1] = new String(test);
        }
      }
      for(int al =0; al < Array.getLength(parse); al++)
      {
        if(parse[al] == null)
        {
          parse[al] = new String(String.valueOf(al));
        }
      } 
    }    
  }  
  
  //////////////////////// ADD A LINe Of TEXT
  public void addLinText(String newText)
  {
    //add an extra element to parse and increase size by one
    String temp[];
    temp = new String[size+1];
    for(int i =0; i < size; i++)
    {
      temp[i] = new String(parse[i]);
    }
    temp[size] = new String(newText);
    parse = temp;
    size = size+1;    
  }
  
  /////////////////////////////////////////////
  
  /**
   * This is the build node function it constructs the node and sets its
   * dimmentions based on the text format & layout withingraphics context 
   * */
  public void buildNode(GraphContext GC)
  {
    TextLayout tl;    
    ttL = new TextLayout[size];
    int idL = longestStr();
    double maxwidth;
    double stdheight;
    tl = new TextLayout(parse[idL],GC.F,GC.FRC);
    maxwidth = tl.getBounds().getWidth();
    stdheight = tl.getBounds().getHeight();
    nodeSpace = new Rectangle2D.Double();
    textSpace = new Rectangle2D.Double();
    /**
     * Get the node string data and sizes
     * */    
    for(int i=0; i < size; i++)
    {
      ttL[i] = new TextLayout(parse[i],GC.F,GC.FRC);
      nodeSpace.setRect(ttL[i].getBounds());         
      textSpace.add(nodeSpace);                     
    }
    /**
     * Set the placemnet
     */
    textSpace.setRect(this.cent.x + (maxwidth/2 - textSpace.getWidth())+1,
        this.cent.y-stdheight-3,textSpace.getWidth() ,
          textSpace.getHeight()*this.getSize() +12*0.5*this.getSize());
          
    nodeSpace.setRect(textSpace);
    //Calculations for Hieght and width of enccompasing ellipse
    double tW,tH; 
    //width
    tW = 2*(Math.pow((nodeSpace.getWidth()),2));
    tW = Math.sqrt(tW);
    //height        
    tH = (Math.pow((nodeSpace.getHeight()),2) + (Math.pow((nodeSpace.getHeight()),2)));
    tH = Math.sqrt(tH);
    //set the height widths around a center point          
    nodeSpace.setRect(nodeSpace.getX()-(tW-nodeSpace.getWidth())/2,
        nodeSpace.getY()-(tH-nodeSpace.getHeight())/2,tW, tH);    
  }
  
  /**
   * This is the creategrapic function it draws the node 
   * that was constructed within the buildnode fuction
   * -NOTE-:break into render border, fill, highlight and text
   * */  
  public void creategrphic(GraphContext GC)
  {
    Graphics2D g1 = GC.theG;    
    buildNode(GC);
    renderFill(g1);
    renderBorder(g1);
    renderText(g1,1.25);    
  }
  
  public void renderFillRGP(Graphics2D gc, Paint rpC, float transP)
  { 
    gc.setPaint(rpC);
    gc.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER,transP));    
    gc.fill(getNodeArea());
    gc.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER,1.0f));
  }
  
  public void renderFill(Graphics2D gc)
  { 
    gc.setPaint(myblue);        
    gc.fill(getNodeArea());
  }  
  
  public void renderBorder(Graphics2D gc)
  { 
    gc.setPaint(Color.BLACK);
    gc.setStroke(new BasicStroke(1.0f));//draw boarder
    gc.draw(getNodeArea());
  }
  
  public void renderText(Graphics2D gc, double lineSpace)
  { 
    gc.setPaint(Color.BLACK);
    for(int rh= 0; rh < this.getSize(); rh++)
    {
      ttL[rh].draw(gc,(float)(this.cent.x - (ttL[rh].getBounds().getWidth()/2)), 
      (float)(this.cent.y + (rh*stdheight)+12*lineSpace*rh));
    }/**/ 
  }
  //--------------------- Palete add ins -------------------------------------
  
  public void renderFill(Graphics2D gc, Color F)
  { 
    gc.setPaint(F);        
    gc.fill(getNodeArea());
  }  
  
  public void renderBorder(Graphics2D gc, Color B)
  { 
    gc.setPaint(B);
    gc.setStroke(new BasicStroke(1.0f));//draw boarder
    gc.draw(getNodeArea());
  }
  
  public void renderText(Graphics2D gc, double lineSpace, Color T)
  { 
    gc.setPaint(T);
    for(int rh= 0; rh < this.getSize(); rh++)
    {
      ttL[rh].draw(gc,(float)(this.cent.x - (ttL[rh].getBounds().getWidth()/2)), 
      (float)(this.cent.y + (rh*stdheight)+12*lineSpace*rh));
    }/**/ 
  }
  
  //--------------------------------------------------------------------------
  
  /**
   * getNodeArea function ssts the shape of node space for hit 
   * detection in mouse moved and mose clicked fucntions
   * @return Area
   */
  public Area getNodeArea()
  {
    Area draw;
    Ellipse2D.Double forceShape = new Ellipse2D.Double();
    forceShape.setFrame(nodeSpace);
    draw = new Area(forceShape);
    return draw;
  }
  
  
  /**
   * Inside function returms a slightly larger area of node space for hit 
   * detection in mouse moved and mose clicked fucntions
   * @return Area
   */
  public Area Inside()
  {
    Shape draw;
    Area tU;
    draw= new Area(nodeSpace);
    tU = new Area(draw);
    return tU;
  }
 
}