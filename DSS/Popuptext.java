/**
 * This File is a testing platform for rendering the java2d verison of the 
 * DSS decision tree. Since the current verision of DSS requires the new JRE, 
 * this element of the DSS turorial has no additional requirements.
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
import java.awt.AlphaComposite;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.Stroke;
import java.awt.font.*;
import java.awt.geom.Area;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.lang.reflect.Array;
public class Popuptext extends TextNode
{
  public TextLayout tl[];
  public TextLayout temptl;
  public int linkAt;
  public int inParse = -1;
  public double edgespace = 5.0;
  public double linspace = 1.5; 
  public Area wholePop;
  public LinkText mylink = new LinkText("http://www.google.com/","more>>>");
  public int arc = 0;
  public GraphContext tGC;
  
  /**
   * This Constructor creates a text popup containing the text "textInPop".
   * */
  public Popuptext(String in)
  {
    //Info = new String(in);
    buffer = new String(in);
    breaker();
    size = Array.getLength(parse);
    holder = new char[N_SIZE];
    nodeSpace = new Rectangle2D.Double();
    textSpace = new Rectangle2D.Double();    
  }
  
  /**
   * This Constructor creates a text popup containing the text "textInPop",
   * the "textToLink" is a string defining some portion in "textInPop" to 
   * highlight and click for the URL defiined in UrlS.
   * This is a dynamic text
   * /**/  
  public Popuptext(String textInPop, String textToLink, String UrlS)
  {
    buffer = new String(textInPop);
    breaker();
    size = Array.getLength(parse);
    holder = new char[N_SIZE];
    nodeSpace = new Rectangle2D.Double();
    textSpace = new Rectangle2D.Double(); 
    mylink = new LinkText(UrlS,textToLink);
  }
  
  /**
   * This is the build node function it constructs the node and sets its
   * dimmentions based on the text format & layout withingraphics context 
   * */
  public void buildNode(GraphContext GC)
  {
    tGC = GC;
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
     //int arc =0;
    for(int i=0; i < size; i++)
    { 
      String temp;
      temp = new String(parse[i]);
      linkAt = parse[i].indexOf(mylink.theText);
      if(linkAt !=  -1)
      {
        inParse = i;
        temptl = new TextLayout(mylink.theText,new Font("Dialog", Font.BOLD,GC.fontSiz),GC.FRC);
        mylink.setTextSpace(temptl.getBounds());
        if(arc == 0)
        {
          arc=1;
        }        
      }
      ttL[i] = new TextLayout(parse[i],GC.F,GC.FRC);
      nodeSpace.setRect(ttL[i].getBounds());         
      textSpace.add(nodeSpace); 
      //parse[i] = temp;
    }
    /**
     * Set the placemnet
     */
    textSpace.setRect(this.cent.x +edgespace,
        this.cent.y-stdheight-edgespace,textSpace.getWidth() +edgespace*2,
          textSpace.getHeight()*this.getSize() +tGC.fontSiz*linspace/2*this.getSize() + edgespace);
          
    nodeSpace.setRect(textSpace);
    
  }
  
  
  public Area getNodeArea()
  {
    //Area draw;
    //draw = new Area(nodeSpace);
    wholePop = new Area(nodeSpace);
    return wholePop;
  }
  
  /**
   * Overlaoded renderText function(from TextNode class) is utilized only when 
   * generating a link text. If no Link has been initialized, old function 
   * should be used 
   * */
  public void renderText(Graphics2D gc, double lineSpace, Color T)
  { 
    gc.setPaint(T);
    for(int rh= 0; rh < this.getSize(); rh++)
    {
      ttL[rh].draw(gc,(float)(nodeSpace.getX()+edgespace), 
      (float)(this.cent.y + (rh*stdheight)+tGC.fontSiz*lineSpace*rh));
      if(inParse != -1 && rh == inParse)
      {
        
        //new Color(244,240,138)
        gc.setPaint(Color.BLUE);
        
        mylink.tTextSpc.setRect(((nodeSpace.getX()+edgespace)+ttL[rh].getBounds().getWidth()-temptl.getBounds().getWidth()),
        ((this.cent.y +(rh*stdheight)+tGC.fontSiz*lineSpace*rh)-temptl.getBounds().getHeight()),
        mylink.tTextSpc.getWidth(),temptl.getBounds().getHeight());
        //Fill the area around the text 
        //begin
        Shape Adraw;
        Area AtU;
        Stroke AtUS;
        Adraw= new Area(mylink.tTextSpc.getBounds2D());
        AtU = new Area(Adraw);
        AtUS = new BasicStroke(2.0f);//Stroke
        AtU.add(new Area(AtUS.createStrokedShape(Adraw)));
        gc.setPaint(new Color(244,240,138));
        gc.fill(AtU);//area around text wih "BasicStroke(" margin ");" from -3lines
        gc.setPaint(Color.BLUE);//pertains to link text
        //end
        
        //get text point ref, lower left corner
        //begin
        float lcBx = (float)(mylink.tTextSpc.getX());
        float lcBy = (float)(mylink.tTextSpc.getY()+temptl.getBounds().getHeight());//+tGC.fontSiz);
        //end
        
        //Draw the text
        //begin
        temptl.draw(gc,lcBx,lcBy);
        //end
        
        //revert to old paint color
        gc.setPaint(T);
        //check position of text space DEBUG
      }/**/
    }
  }
  
  public Area Inside()
  {
    Shape draw;
    Area tU;
    Stroke tUS;
    draw= new Area(wholePop);
    tU = new Area(draw);
    tUS = new BasicStroke(20.0f);//Stroke
    tU.add(new Area(tUS.createStrokedShape(draw)));
    return tU;
  }
  
  public Area InLink()
  {
    Shape draw;
    Area tU;
    Stroke tUS;
    draw= new Area(mylink.tTextSpc);
    tU = new Area(draw);
    tUS = new BasicStroke(20.0f);//Stroke
    tU.add(new Area(tUS.createStrokedShape(draw)));
    return tU;
  }
  
}