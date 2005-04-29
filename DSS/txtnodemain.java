/**
 * This File is a testing platform for rendering the java2d verison of the 
 * DSS decision tree. Since the current verision of DSS requires the new JRE, 
 * this element of the DSS turorial has no additional requirements.
 * @author: James Nelson 8-26-2004
 * version: 1.0 "yet to be named graph maker" LM:9-10-2004
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
//import Popuptext;
import java.applet.AppletContext;
import java.awt.*;
import java.awt.Stroke;
import java.applet.Applet;
//import java.awt.font.FontRenderContext;
import java.awt.font.TextLayout;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.event.*;
import java.lang.reflect.Array;
import java.net.*;
import javax.swing.JApplet;
import java.lang.String;
import java.lang.*;
import javax.swing.JDialog;
import javax.swing.JOptionPane;

public class txtnodemain extends Applet implements MouseListener, MouseMotionListener,Runnable {

    private BufferedImage bi;//still in use with mini overview 
    private BufferedImage bi2;//old variable for laoding a buffered image
    private int oldGN = 0;
    private int highlight = 0;
    private int highE =0;
    private String fileL = new String("North America/South America/Africa");//"C:/Documents and Settings/jenelson/Desktop/DSS tutorial Images");
    public String strsize;
    public double spacing;
    public double offX,offY;
    public Color myblue; 
    public int nodeNum;
    public int fontSiz = 12;
    boolean drawSomething = false;
    boolean popup = false;
    boolean WasInside = false;
    boolean popupD = false;
    public String[] st2;/** holds the node titles for laoding into  the graph*/
    public String[][] graphins; //node names for each node in each graph    
    public double xes[][] = {{127,40,125,215},{110,33,100,191},{333,177,497,61,178,282,380,497,599},{650,280,1020,145,425,795,1250,60,145,230,345,425,500,620,720,790,861,965,1080,1175,1250,1315,1420}};
    public double yes[][] = {{33,110,110,110},{33,110,110,110},{30,60,60,120,120,120,120,120,120},{30,100,100,170,170,170,170,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240}};
    int tempEd[][] = {{0,0,0},{0,0,0},{0,0,1,1,1,2,2,2},{0,0,1,1,2,2,3,3,3,4,4,4,5,5,5,5,5,6,6,6,6,6}};
    /*  xes
     * contains gathered x values for centers of nodes for display, hope to find
     * algoritm to generate dynamically based on either window size, or graph type
     *  yes
     * contains gathered y values for centers of nodes for display, hope to find
     * algoritm to generate dynamically based on either window size, or graph 
     * type
     *   tempEd         
     * contains gathered placement levels for centers of nodes for display, hope
     * to find algoritm to generate dynamically based on either window size, or graph type
     */
    public Graph mygraph; //holds the current graph to be rendered in graph array
    //////////////////////////
    public Graph DSSgraph1;
    public Graph DSSgraph2;
    public Graph DSSgraph3;
    public Graph DSSgraph4;
    /////////////////////////
    public boolean renHighPath =false;
    public DSSUpRead Rup;
    int dsa[] = {0,2};

    public txtnodemain() {
           
            myblue = new Color(204,204,255);
            setBackground(Color.white);
            addMouseListener(this);  
            nodeNum = 9;//  9;
            Image img;/**
                       * old variable used to load a background image, now used
                       * to load overview map
                       * */
            String[] st;
            spacing = 0.5;
            offX = 0;//
            offY = 0;// 
            highlight =0;
            highE = 0;                        
            /**
             * Load node string data
             */
            st2 = new String[] {"Continental","Area","Shape","North America/South America/Africa","Northern or/Southern Poles", "Asia/Europe/Austrailia","North America/South America/Africa","Northern or/Southern Poles", "Asia/Europe/Austrailia" };//{"Global", "Area","Shape", "Compromise"};//{"Continental","Area","Shape","North America/South America/Africa","Northern or/Southern Poles", "Asia/Europe/Austrailia","North America/South America/Africa","Northern or/Southern Poles", "Asia/Europe/Austrailia" };
            graphins = new String[][]{{"DSS/Applet","Global","Continental","Regional"},
                                      {"Global", "Area","Shape", "Compromise"},
                                      {"Continental","Area","Shape","North America/South America/Africa","Northern or/Southern Poles", "Asia/Europe/Austrailia","North America/South America/Africa","Northern or/Southern Poles", "Asia/Europe/Austrailia"},
                                      {"Regional","North South","East West","Area","Shape","Area","Shape","Extreme North","Mid","Extreme South","Extreme North","Mid","Extreme South","Extreme North","Mid North","Mid","Mid South","Extreme South","Extreme North","Mid North","Mid","Mid South","Extreme South"}};

    }

  /**
   *Overloaded paint function from inherited Applet class
   * Used to render graphics elements to the Applet space 
   * @param g
   * graphics object of Applet
   */
    public void paint(Graphics g) { 
        super.paint(g);

        switch(DSSUpdater.getGraphName())
        {
        case 0:
          mygraph = DSSgraph1;
          oldGN = 0;
          break;
        case 1:
          mygraph = DSSgraph2;
          oldGN = 1;
          break;
        case 2:
          mygraph = DSSgraph3;
          oldGN = 2;
          break;    
        case 3:
          mygraph = DSSgraph4;
          oldGN = 3;
          break;
        default:
          mygraph = DSSgraph3;
          oldGN = 4;
          break;
        }
        Graphics2D g3 = (Graphics2D) g;
        RenderingHints qualityHints;
      qualityHints = new RenderingHints(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON); 
      qualityHints.put(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY); 
      g3.setRenderingHints(qualityHints);
        Graphics2D temp = mygraph.GC.theG;
        mygraph.RenderGraph(g3);
        
        mygraph.RenderGraph();
        g3.setPaint(Color.blue);
        g3.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER,0.4f));        
        mygraph.RenderHighlight(highlight);
    
        if(DSSUpdater.getGraphName() >=2)
        {

          mygraph.HighLightPath(DSSUpdater.getHighPathAr());
        }

        if(popup)
        {
          //mygraph.renderPop("This is some node information,/text broken into lines./This information is for node: more>>>",highlight);
        //The DSS Applet is a java applet designed to provide recommendations for small scale data map projections. These recommendations are based on the properties selected by you, the user. The recomedations are based on small scale projection models and USGS research . Additional infomation can be displayed by clicking more>>>
         //mygraph.renderPop("The DSS Applet provides recommendations for small scale data map projections./Results are based on small scale projection models and USGS research ./Additional infomation can be displayed by clicking more>>>",highlight); 
         //mygraph.renderPop("Global data sets of small scale data are projected on every continent and region./The preservation of area and shape with respect to the shape of the earth is the/only concideration. Additional infomation can be displayed by clicking more>>>",highlight); 
         mygraph.renderPop("Regional projections are based on the dimensions of the region being mapped, /and the location of the region with respect to the proximity to the Earth’s poles. /Additional information can be displayed by clicking more>>>",highlight); 

        }
        mygraph.GC.theG = temp;
        // ->END of Default secetion path painting
    }
    
    /**
     * variable used for duoble buffering
     */
    //private Image image;                        //Map Image
    
    private Image offScreenImage;
    private Dimension offScreenSize;
    private Graphics offScreenGraphics;
    
   public final synchronized void update(Graphics g)
    {
      Dimension d = getSize();
      RenderingHints qualityHints;
      Graphics2D g3 = (Graphics2D)g;
      qualityHints = new RenderingHints(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON); 
      qualityHints.put(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY); 
      g3.setRenderingHints(qualityHints);
      
      if((offScreenImage == null) || (d.width != offScreenSize.width) ||
      (d.height != offScreenSize.height))
      {
        offScreenImage = createImage(d.width, d.height);
        offScreenSize = d;
        offScreenGraphics = offScreenImage.getGraphics();        
      }
    
      offScreenGraphics.clearRect(0,0,d.width, d.height);
   
      paint(offScreenGraphics);
      
      g3.drawImage(offScreenImage,0,0,null);
      
    } 
    
        
    /**
     *  Mouse Dragged Event Handler
     *  @param e
     */
      public void mouseDragged(MouseEvent e)
    {
      int currentY = e.getPoint().y;
      int currentX = e.getPoint().x;
    }
    
    
    
    /**
     *  Mouse Moved Event Handler
     *  @param e
     * */   
    public void mouseMoved(MouseEvent e)
    {
      int currentY = e.getPoint().y;
      int currentX = e.getPoint().x;
   
      
      if(mygraph.nodes[highlight].Inside().contains(currentX,currentY) && !popup)
      {
        popup = true;
        repaint();
        //mygraph.renderPop("This is some node information,/text broken into lines./This information is for node: more>>>",highlight);
      }
      else if(mygraph.nodes[highlight].Inside().contains(currentX,currentY) || mygraph.pop.Inside().contains(currentX,currentY))
      {
        //do nothing
      }
      else if(popup && !(mygraph.nodes[highlight].Inside().contains(currentX,currentY) || mygraph.pop.Inside().contains(currentX,currentY)))
      {

        popup = false;
        repaint();
      }
      
    if(mygraph.nodes[highlight].Inside().contains(currentX,currentY) ||(popup ? mygraph.pop.Inside().contains(currentX,currentY) : false) )
   {
      this.setCursor(Cursor.getPredefinedCursor(
                                       Cursor.HAND_CURSOR));
   }
   else
   {
     this.setCursor(Cursor.getPredefinedCursor(
                                       Cursor.DEFAULT_CURSOR));
   }

             
    }
    
    
 
    // CODE SEG: APPLET VERSION -> BEGIN 
    // next 15 lines
    /**
     * Web Applet
     */
     public void init() {
        addMouseMotionListener(this);
        //mygraph = new Graph(nodeNum,st2,new GraphContext(this.getGraphics()));
        DSSgraph1 = new Graph(4,graphins[0],new GraphContext(this.getGraphics()));
        DSSgraph2 = new Graph(4,graphins[1],new GraphContext(this.getGraphics()));
        DSSgraph3 = new Graph(9,graphins[2],new GraphContext(this.getGraphics()));
        DSSgraph4 = new Graph(23,graphins[3],new GraphContext(this.getGraphics()));
          DSSgraph1.setGraphSpace(this.getBounds());
          DSSgraph1.ConstructGraph(tempEd[0],xes[0],yes[0],new Point2D.Double(offX,offY));
          DSSgraph2.setGraphSpace(this.getBounds());
          DSSgraph2.ConstructGraph(tempEd[1],xes[1],yes[1],new Point2D.Double(offX,offY));
          DSSgraph3.setGraphSpace(this.getBounds());
          DSSgraph3.ConstructGraph(tempEd[2],xes[2],yes[2],new Point2D.Double(offX+10,offY));
          DSSgraph4.setGraphSpace(this.getBounds());
          DSSgraph4.ConstructGraph(tempEd[3],xes[3],yes[3],new Point2D.Double(offX,offY));
        switch(DSSUpdater.getGraphName())
        {
        case 0:
          mygraph = DSSgraph1;
          oldGN = 0;
          break;
        case 1:
          mygraph = DSSgraph2;          
          oldGN = 1;
          break;
        case 2:
          mygraph = DSSgraph3;
          oldGN = 2;
          break;    
        case 3:
          mygraph = DSSgraph4;
          
          oldGN = 3;
          break;
        default:
          mygraph = DSSgraph3;
          oldGN = 4;
          break;
        }
        DSSUpdater.setHighPathAr(dsa);
    }
    // APPLET VERSION -> END 

    
    /**
   * mouse Pressed Event Handler
   * @param e
   */
    public void mousePressed(MouseEvent e) {
        
      int currentY = e.getPoint().y;
      int currentX = e.getPoint().x;
      Ellipse2D.Double test;
      test = new Ellipse2D.Double();   
      test.setFrame(mygraph.nodes[highlight].nodeSpace);//0  should be highlight
  
        if(mygraph.pop.wholePop != null){
        if(mygraph.pop.mylink.tTextSpc.contains(currentX,currentY))
        {
          mygraph.pop.mylink.exeLink(this.getAppletContext());
        }}
    }
  /**
   * mouseReleased
   * @param e
   */
    public void mouseReleased(MouseEvent e) {
         
    }
    
 /**
   * mouseEntered
   * @param e
   */
   public void mouseEntered(MouseEvent e) {
   }
  

  /**
   * This is the additional threading code required to communicate between
   * two or more applets, Need DSSUpdater, and threading code in madifying 
   * Applet(s) 
   */
  Thread runner;

/**
 * start is a thread fuction called by run
 */
  public void start()
  {
    if (runner == null)
    {
      runner = new Thread(this);
      runner.start();
    }
  }
  /**
   * stop is a thread fuction called by run
   */
  public void stop()
  {
    if (runner != null)
    {
      runner.stop();
      runner = null;
    }
  }
  
  /**
   * run is a thread fuction called by the Runnable class
   */
  public void run()
  {
    while (true)
    {
      try
      {
        Thread.sleep(10);
      }
      catch (InterruptedException e) {}

      if(DSSUpdater.getHighLight() != highlight)
      {
        highlight = DSSUpdater.getHighLight();
        DSSUpdater.getHighPathAr(dsa);
        drawSomething = true;
      }
          if(DSSUpdater.getGraphName() != oldGN)
        {
          //repaint();
          oldGN = DSSUpdater.getGraphName();
          //repaint();
          drawSomething = true;
        }
      if((DSSUpdater.getHighPath() != dsa[1] || 
          DSSUpdater.getHighPathLength() != dsa.length)
          && DSSUpdater.getGraphName()>=2)
          {
              DSSUpdater.getHighPathAr(dsa);
          }      
      if(drawSomething)
      {
        repaint();
        drawSomething = false;
      }
    }
  }
  /**
   * mouseExited detects if mouse has exited Applet space
   * @param e
   */
    public void mouseExited(MouseEvent e) {       
    }
  /**
   * mouseClicked detects mouse click
   * @param e
   */
    public void mouseClicked(MouseEvent e) {
    }
  

}