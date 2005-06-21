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

import java.applet.AppletContext;
import java.awt.*;
import java.awt.Stroke;
import java.applet.Applet;
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

public class Tutorial extends Applet implements MouseListener, MouseMotionListener,Runnable,MouseWheelListener {

    /**
     * Non-depriciated variables 
     * */
 
    //very much in use
    private int oldGN = 0;
    private int highlight = 0;
    /**offX and offY  are the ofset for the graph from 
     * top left corner of window*/
    public double offX,offY;
    /**
     * used to detect change that requires rerendering of graph
     */
    boolean drawSomething = false;   
    /**
     * this boolean variable controls limited rerendering of 
     * popup graphics
     */
    boolean popup = false;
    
      
   /** holds the node titles for loading into  the graph*/
    public String[] megaGraph;
    
    int tempEd[][] = {{0,0,0},{0,0,0},{0,0,1,1,1,2,2,2},
                      {0,0,1,1,2,2,3,3,3,4,4,4,5,5,5,5,5,6,6,6,6,6}};
    int megaMap[] = {0,0,0,1,1,1,2,2,3,3,7,7,7,8,8,8,9,9,10,10,17,17,17,18,18,
                    18,19,19,19,19,19,20,20,20,20,20};
    /** holds the X values for the graph, currently this is static as the 
     * type data will not change and of display features weren't defined
     * now that display features have been defined, however a dynamic solution
     * maybe worth investigating (bad runtimes for something that can be static)**/
    public double megaX[] = {673,136,429,1272,33,100,190,270,586,900,1638,160,
                              280,378,475,591,694,764,1048,1413,1866,677,763,850
                              ,965,1044,1130,1244,1342,1414,1481,1586,1698,1796,
                              1868,1936,2041};
    /** megaY holds the Y values for the graph, currently this is static
     * (see megaX for more details)**/
    public double megaY[] = {30,100,100,100,170,170,170,170,170,170,170,260,260,
                            260,260,260,260,260,260,260,260,350,350,350,350,350,
                            350,350,350,350,350,350,350,350,350,350,350};
  
    /** compressX 
     * and compressY are mainipulations of megaX/Y for the purpose of 
     * better display functionality**/
    public double compressX[] = {215,135,215,315,60,125,215,186,250,262,355,70,187,287,133,250,355,232,300,320,385,150,230,320,215,300,380,151,257,320,390,490,215,315,385,455,555};
    
    /** compressY 
     * and compressX are mainipulations of megaX/Y**/
    public double compressY[] = {35,100,100,100,150,150,150,150,150,150,150,220,220,220,220,220,220,200,200,200,200,260,260,260,260,260,260,260,260,260,260,260,260,260,260,260,260};
    /** show 
     * marks displayed nodes**/
                            //always     //glob  //cL1 //rL1  //cont L2           //rL2    rL2     //rL3        //rL3       //rL3           //rL3
   // public int show[][] = {{0,1,2,3},{4,5,6},{7,8},{9,10},{11,12,13,14,15,16},{17,18},{19,20},{21,22,23},{24,25,26},{27,28,29,30,31},{32,33,34,35,36}};
    /** mygraph holds/points to the graph being redered 
     * (possible depriciation)**/
    public Graph mygraph; //holds the current graph to be rendered in graph array
   /** DSSmegagraph holds the graph being redered and is pointed to by
    * mygraph (see mygraph -possible depriciation)**/
    public Graph DSSmegagraph;
   /**
    *  poppers data list of linked pages -BAD
    * should be and exterior data set like HTML params or .cvs
    * this variable is only to insure stable applet
    * **/
    public String[] poppers;
    public String[] urls4pops;
    /////////////////////////
   
   /**retreive the highlight path
        specified outside the class via a abstact tracking class
        DSSUpdater and place it into dsa for rendering*/  
    int dsa[] = {0};
    
   /**
    *depreciated variables
    * *
    //public boolean renHighPath =false;
   /**
    *End-depreciated variables
    * */

    /**
     * constructor loads standard graph values, moving more and more work 
     * out of this constructor into other classes and functions 
     */
    public Tutorial() {
            setBackground(Color.white);
            addMouseListener(this);  
            start();

            
            offX = 0;//
            offY = 0;// 
            highlight =0;
             
            //there should probably be either an external HTML dos with params 
            //or a .cvs and io parsing implemented for poppers -the container for
            //all link data, it is difined here to insure java class works 
            poppers = new String[]{"The DSS Applet provides recommendations for small scale data map projections./Results are based on small scale projection models and USGS research ./Additional infomation can be displayed by clicking more>>>",
                                     "Global data sets of small scale data are projected on every continent and region./The preservation of area and shape with respect to the shape of the earth is the/only concideration. Additional infomation can be displayed by clicking more>>>",
                                    "Continental projections are based on symetry with respect to various latitudinal and/longitudinal axes and shape of the continent with repect to its coastline. Additional /infomation can be displayed by clicking more>>>",
                                    "Regional projections are based on the dimensions of the region being mapped, /and the location of the region with respect to the proximity to the Earth’s poles. /Additional information can be displayed by clicking more>>>"};
            megaGraph = new String[] {"DSS/Applet","Global","Continental","Regional","Area","Shape",
            "Compromise","Area","Shape","North South","East West","North America/South America/Africa",
            "Northern or/Southern Poles","Asia/Europe/Austrailia","North America/South America/Africa",
            "Northern or/Southern Poles","Asia/Europe/Austrailia","Area","Shape",
            "Area","Shape","Extreme North","Mid","Extreme South","Extreme North",
            "Mid","Extreme South","Extreme North","Mid North","Mid","Mid South",
            "Extreme South","Extreme North","Mid North","Mid","Mid South","Extreme South"};
            
            urls4pops = new String[] {"http://www.colorado.edu/geography/gcraft/notes/mapproj/mapproj.html",
            "http://www.mapthematics.com/Essentials/Essentials.html", "http://www.pathaway.com/mapproj.htm",
            "http://erg.usgs.gov/isb/pubs/MapProjections/projections.html"};
    }

  /**
   *Overloaded paint function from inherited Applet class
   * Used to render graphics elements to the Applet space ,"10"
   * @param g
   * graphics object of Applet
   */
    public void paint(Graphics g) { 
        super.paint(g);

        mygraph = DSSmegagraph;    
        
       DSSmegagraph.updateVis(DSSUpdater.getHighLight());
       highlight = DSSUpdater.getHighLight();
       
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
        
        mygraph.HighLightPath(DSSUpdater.getHighPathAr());//get highlight here

        if(popup)
        {
         mygraph.renderPop(poppers[DSSUpdater.getGraphName()],"more>>>",urls4pops[DSSUpdater.getGraphName()],highlight);//highlight%4],highlight); //select popup by node
         Popuptext p;
        }
        
        mygraph.GC.theG = temp;
        // ->END of Default secetion path painting
    }
    
    /**
     * variable used for double buffering
     */
    
    private Image offScreenImage;
    private Dimension offScreenSize;
    private Graphics offScreenGraphics;
    
  /**
   * update function is used in double buffering to reduce flickering
   * @param g
   */
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
     *  checks to see if a popup needs to be rendered, condition for popup 
     *  rendering is currently based on cursor in/out of highlight
     *  @param e
     * */   
    public void mouseMoved(MouseEvent e)
    {
      int currentY = e.getPoint().y;
      int currentX = e.getPoint().x;
   
      
      if(mygraph.nodes[highlight].Inside().contains(currentX,currentY) && !popup)
      {//if you are inside the highlighted node and there is no pop, render one
        popup = true;
        repaint();
      }
      else if(mygraph.nodes[highlight].Inside().contains(currentX,currentY) || mygraph.pop.Inside().contains(currentX,currentY))
      {//if you are inside the highlight or inside a rendered popup
        //do nothing for now, this conditional stament was necessary whithout double buffering
        //changes may occur when this conditonal maybe necessary again
        //this checks if the cursor is within both the popup or the node space
      
      }
      else if(popup && !(mygraph.nodes[highlight].Inside().contains(currentX,currentY) || mygraph.pop.Inside().contains(currentX,currentY)))
      {//if you have left both the popuparea and the highlighted node get rid of the popup

        popup = false;
        repaint();
      }
     
      
      if(mygraph.nodes[highlight].Inside().contains(currentX,currentY) ||(popup ? mygraph.pop.Inside().contains(currentX,currentY) : false) )
     {//if you are inside the highlight node or the th popup change the cusor
        //change the cursor to signify to the user they are in an active area
        this.setCursor(Cursor.getPredefinedCursor(
                                         Cursor.HAND_CURSOR));
     }
     else
     {
       //change the cursor to signify to the user they are no longer in an active 
       //area
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
        DSSmegagraph = new Graph(37,megaGraph,
        new GraphContext(this.getGraphics()));
        DSSmegagraph.setGraphSpace(this.getBounds());//this.getBounds());
        DSSmegagraph.ConstructGraph(megaMap,compressX,compressY,
        new Point2D.Double(offX,3.0));/**/
        mygraph = DSSmegagraph;//DSSgraph1;
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
    
    public void mouseWheelMoved(MouseWheelEvent e)
    {
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
        Thread.sleep(88);//changing this helped Main but screwed up highlight
      }
      catch (InterruptedException e) {}

      if(DSSUpdater.getHighLight() != highlight)
      {
        highlight = DSSUpdater.getHighLight();
        DSSUpdater.getHighPathAr(dsa);//retreive the highlight path
        //specified outside the class via a abstact tracking class
        //DSSUpdater and place it into dsa for rendering
        drawSomething = true;
      }/**/  
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