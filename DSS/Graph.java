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
import java.awt.*;
import java.applet.Applet;
import java.awt.Polygon;
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
import java.util.Arrays;
import javax.swing.JApplet;
import java.lang.String;
import java.util.List;

public class Graph extends TextNode
{
  public TextNode[] nodes;
  public String[] strNode;
  public int numNodes;
  public GraphContext GC;
  public int[] nodeMap; //map for edges
  public Point2D.Double[] nodeCenters;
  public Point2D.Double[] defaultCenters;
  public Point2D.Double offSet; //where to place the graph
  public Popuptext pop;
  public Popuptext pops[];
  public Rectangle2D.Double graphSpace;
  public int visable[];
  public int numVis;
  private int INFINITY = 1000;
  
  public int curHigh;
  public Area HighPath;
  //                                      1:1                    7:4        8:5                        17:9      18:10         19:11         20:12                                  
  //                         //always   //glob  //cL1 //rL1  //cont L2l    L2r       //rL2    rL2     //rL3     //rL3       //rL3           //rL3
  public int show2[][] = {{0,1,2,3},{0,1,2,3,4,5,6},{0,1,2,3,7,8,11,12,13},{0,1,2,3,7,8,14,15,16},{0,1,2,3,9,10,17,18,21,22,23},{0,1,2,3,9,10,17,18,24,25,26},{0,1,2,3,9,10,19,20,27,28,29,30,31},{0,1,2,3,9,10,19,20,32,33,34,35,36}};
  
  public Graph(int num, String[] nodeIds, GraphContext C)
  {
    numNodes = Array.getLength(nodeIds);
    strNode = new String[numNodes];
    System.arraycopy(nodeIds,0,strNode,0,numNodes);  
    GC = new GraphContext(C.FRC,C.F,C.theG);
    nodes = new TextNode[numNodes];
    offSet = new Point2D.Double(0,0);
    numVis = 0;
    visable = new int[numNodes];
    setVisability();
    
  }

  public void setVisability()
  {
    if(visable != null)
    {
      for( int i =0; i < numNodes; i++)
      {
        visable[i] = INFINITY;
        numVis =0;
      }
    }
    else
    {
      visable = new int[numNodes];
      for( int i =0; i < numNodes; i++)
      {
        visable[i] = INFINITY;
        numVis =0;
      }
    }
  }
  
   public void resetVisability()
  {
      for( int i =0; i < numNodes; i++)
      {
        visable[i] = INFINITY;
        numVis =0;
      }
  } 
  
  boolean containsAny(int[] aIndep, int b)
  {    
      boolean cont = false;
      int size = Array.getLength(aIndep);
      int cntr =0;
      while(cntr < size && cont == false && aIndep[cntr] <= b)
      {
        cont = (b == aIndep[cntr]);
        cntr++;
      }
      return cont;
  }
  
//Set Visabillity works
  public void setVisability(int visAr[])
  {
    int size = Array.getLength(visAr);
    
    resetVisability();
    System.arraycopy(visAr,0,visable,0,size);
    numVis =size;
  }

  //This is bad programming but since the values are set and unchanging and 
  //this function is accessed multiple times it is faster and more suitable 
  //solution than a dynamic allocation/re-allocation array search and 
  //display, as that would require searching sorting and a for loopmodification
  public void updateVis(int theH)
  {
    if(theH < 4)
    {
      setVisability(show2[0]);
    }
    else if(theH==4||theH==5||theH==6)
    {
      setVisability(show2[1]);
    }
    else if(theH==11||theH==12||theH==13)
    {
       setVisability(show2[2]);
    }
    else if(theH==14||theH==15||theH==16)
    {
       setVisability(show2[3]);
    }
    else if(theH==21||theH==22||theH==23)
    {
       setVisability(show2[4]);
    }
    else if(theH==24||theH==25||theH==26)
    {
       setVisability(show2[5]);
    }
    else if(theH==27||theH==28||theH==29||theH==30||theH==31)
    {
       setVisability(show2[6]);
    }
   else if(theH==32||theH==33||theH==34||theH==35||theH==36)
    {
       setVisability(show2[7]);
    }
  }
  
  
  public void setGraphSpace(Rectangle2D.Double r)
  {
    graphSpace = new Rectangle2D.Double();
    graphSpace.setRect(r);
  }
  
  public void setGraphSpace(Rectangle r)
  {
    graphSpace = new Rectangle2D.Double();
    graphSpace.setFrame(r.getX(),r.getY(),r.getWidth(),r.getHeight());
  }
  
  public void setMap(int[] map)
  {
    int temp;
    temp = Array.getLength(map);
    nodeMap= new int[numNodes-1];
    if(temp == numNodes-1)
    {
      System.arraycopy(map,0,nodeMap,0,numNodes-1); 
    }//should implement a throw exception here
  }
  
  public void setCent(Point2D.Double[] centers)
  {
    int temp;
    temp = Array.getLength(centers);
    if(temp == numNodes)
    {
      nodeCenters = new Point2D.Double[numNodes];

      System.arraycopy(centers,0,nodeCenters,0,numNodes); 
 
    }//should implement a throw exception here
  }
  
  public void setCenters(double[] xes, double[] yes, Point2D.Double offst)
  {
    int temp;
    int temp2;
    offSet.setLocation(offst);
    temp = Array.getLength(xes);
    temp2 = Array.getLength(yes);
    if(temp == temp2 && temp == numNodes)
    {
      nodeCenters = new Point2D.Double[numNodes];
      defaultCenters = new Point2D.Double[numNodes];
      for(int i = 0; i < numNodes; i++)
      {
        nodeCenters[i] = new Point2D.Double(xes[i]+offSet.x,yes[i]+offSet.y);
        defaultCenters[i] = new Point2D.Double(xes[i],yes[i]);
      }
    }//should implement a throw exception here    
  }
  
  public void setCenters(Point2D.Double[] oldcent, Point2D.Double offst)
  {
    int temp;
    offSet.setLocation(offst);
    temp = Array.getLength(oldcent);
    if(temp == numNodes)
    {
      nodeCenters = new Point2D.Double[numNodes];
      for(int i = 0; i < numNodes; i++)
      {
        nodeCenters[i] = new Point2D.Double(oldcent[i].x+offSet.x,oldcent[i].x+offSet.y);
      }
    }//should implement a throw exception here    
  }
  
  public void prepNodeCent()
  {
    for(int i =0; i <numNodes; i++)
    {
      nodes[i].setC(nodeCenters[i].x+offSet.x,nodeCenters[i].y+offSet.y);
    }
  }
  
  //assume centers set and NodeMap set
  public void RenderEdges()
  {
    for(int i=1; i < numNodes; i++)
    {
      GC.theG.setPaint(Color.black);
      GC.theG.setStroke(new BasicStroke(2.0f));
     
      if(isVisable(i))
      {
      GC.theG.drawLine((int)nodes[i].nodeSpace.getCenterX(),
      (int)nodes[i].nodeSpace.getCenterY(), (int)nodes[nodeMap[i-1]].nodeSpace.getCenterX(),
      (int)nodes[nodeMap[i-1]].nodeSpace.getCenterY());
      
      }
    }    
  }
  
  public void ConstructGraph(int []map, double[] xes, double[] yes, Point2D.Double offst)
  {
    setMap(map);
    offSet.setLocation(offst);
    setCenters(xes,yes,offst);
    
    for(int i =0; i < numNodes; i++)
    {
      nodes[i] = new TextNode(strNode[i]); //this will parse and set text 4 node 
      nodes[i].setC(nodeCenters[i]);
      nodes[i].buildNode(GC);
    }
    
  }
  
  public void reConstructGraph()
  {
    //setCenters(defaultCenters,offSet);
    for(int i =0; i < numNodes; i++)
    {
      nodes[i] = new TextNode(strNode[i]); //this will parse and set text 4 node 
      nodes[i].setC(nodeCenters[i]);
      nodes[i].buildNode(GC);
    }    
  }
  
   public Point2D.Double[] cpyCenters(Point2D.Double[] oldcent)
  {
    int temp;
    Point2D.Double[] newcent;
    temp = Array.getLength(oldcent);
    if(temp == numNodes)
    {
      newcent = new Point2D.Double[numNodes];
      for(int i = 0; i < numNodes; i++)
      {
        newcent[i] = new Point2D.Double(oldcent[i].x,oldcent[i].y);
      }
    }//should implement a throw exception here  
    else
    {
      newcent = null;
    }
    return newcent;
  }
  
   /**
   * Translate function shifts the graph by x and y coordinates by the offset 
   * point trans
   * /**/
  public void Translate(Point2D.Double trans)
  {
    setCent(defaultCenters);
    setCenters(nodeCenters,trans);
    Point2D.Double[] temp;
    temp = cpyCenters(nodeCenters);
    for(int i =0; i < numNodes; i++)
    {
      nodes[i] = new TextNode(strNode[i]); //this will parse and set text 4 node 
      nodes[i].setC(nodeCenters[i]);
      nodes[i].buildNode(GC);
    }    
  }
  
  public void RenderGraph()
  {//Alpha shading not causing exsuative runtime 
    for(int j =0; j <numNodes; j++)
    { 
      nodes[j].buildNode(GC);
    }
    RenderEdges();
    for(int i =0; i <numNodes; i++)
    {       
        GC.theG.setPaint(Color.black);
      if(isVisable(i))
      {
        nodes[i].renderFill(GC.theG);
        nodes[i].renderBorder(GC.theG);
        nodes[i].renderText(GC.theG,1.25);   
     
      }
    }
  }

    public void RenderGraph(Graphics2D g)
  {
    GC.theG = g;
  }
  
  public void RenderHighlight(int select)
  {
    RoundGradientPaint rgp = new RoundGradientPaint(nodes[select].nodeSpace.getCenterX()
              ,nodes[select].nodeSpace.getCenterY(), Color.yellow,
          new Point2D.Double(0.0, nodes[select].nodeSpace.getWidth()/2), Color.green.darker().darker());
    curHigh = select;    
    //clear the area before repainting
    myClearRect(nodes[select].nodeSpace); 
    nodes[select].renderFillRGP(GC.theG, rgp,1);
    nodes[select].renderText(GC.theG,1.25);
    nodes[select].renderBorder(GC.theG);
  }
  
  public void RenderQuestion(int select)
  {
    Color nBlue = new Color(77,69,248);
    //Font ar = new Font("Monospaced",Font.BOLD,48);
    Font ar = new Font("SansSerif",Font.BOLD,28);
    TextLayout ntl = new TextLayout("?",ar,GC.FRC);
    RoundGradientPaint rgp = new RoundGradientPaint(nodes[select].nodeSpace.getCenterX()
              ,nodes[select].nodeSpace.getCenterY(), Color.WHITE,
          new Point2D.Double(0.0, nodes[select].nodeSpace.getWidth()/2), nBlue);
    //curHigh = select;    
    //clear the area before repainting
    myClearRect(nodes[select].nodeSpace); 
    nodes[select].renderFill(GC.theG);
    nodes[select].renderText(GC.theG,1.25);
    //GC.theG.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER,0.01f));
    nodes[select].renderFillRGP(GC.theG, rgp,0.75f);
 
    //GC.theG.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER,1));
    //now render question mark
    //float a = (float)(nodeCenters[select].getX());
     GC.theG.setPaint(nBlue); 
    ntl.draw(GC.theG, (float)((nodes[select].nodeSpace.getX()+nodes[select].nodeSpace.getWidth()/2)-(ntl.getBounds().getWidth()/2)) ,(float)((nodes[select].nodeSpace.getY()+nodes[select].nodeSpace.getHeight()/2)+(ntl.getBounds().getHeight()/2)));
      
    GC.theG.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER,1));
    
    nodes[select].renderBorder(GC.theG);
  }
  
  /**/public void unRenderHighlight(int select)
  {
    myClearRect(nodes[select].nodeSpace);
    nodes[select].renderFill(GC.theG);
    nodes[select].renderBorder(GC.theG);
    nodes[select].renderText(GC.theG,1.25);
  };/**/
  
  //fixes redraw problem for now
  /**/public void myClearRect(Rectangle2D.Double s)
  { 
    Area draw;
    Ellipse2D.Double forceShape = new Ellipse2D.Double();
    //setting up the area to be cleared just slightly larger than 
    //the actual area
    forceShape.setFrame(s.getX()-1,s.getY()-1,s.getWidth()+2,s.getHeight()+2);
    draw = new Area(forceShape);
    myClearRect(draw);
    
  }/**/
  //fixes redraw problem for now
  /**/public void myClearRect(Shape s)
  {
    Color oldColor; //stores old color
    oldColor = new Color(GC.theG.getColor().getRGB());
    GC.theG.setPaint(GC.theG.getBackground());
    GC.theG.fill(s);
    GC.theG.setPaint(oldColor); //return color to old color
  }/**/
  
  public void renderPop(String in, int index)
  {
    renderPop(in,index,true);
  }
  
  public void renderPop(String in,String keyword, String urls, int index)
  {
    pop = new Popuptext(in,keyword,urls);
    renderPop(in,index,false);
  }
  
  public void renderPop(String in, int index, boolean newP)
  {
    if(newP)
    {
      pop = new Popuptext(in);
    }    
    pop.setC(0,0);
    pop.buildNode(GC);
    Rectangle2D.Double bounds = this.graphSpace;
    Rectangle2D.Double nodebounds = nodes[index].nodeSpace;
    Rectangle2D.Double popbounds = pop.nodeSpace;
    Point2D.Double one, two;
    one = new Point2D.Double(0,0);
    two = new Point2D.Double(0,0); 
    /**
     * The following code places the popup of a node above left, above right, 
     * below left and  below right, depending on the position of node and 
     * available space, modification to prevent rendering over paths 
     * would be PRETTY! :)
     */
    //right top corner
    if(bounds.contains(nodebounds.getX() - (popbounds.getWidth()+10),nodebounds.getY() - (popbounds.getHeight()+10), popbounds.getWidth(), popbounds.getHeight()))
    {
      //place in right corner
      pop.setC(nodebounds.getX() - (popbounds.getWidth()+10),nodebounds.getY() - (popbounds.getHeight()+10));
      pop.buildNode(GC);
      one = new Point2D.Double(pop.nodeSpace.getMaxX(),pop.nodeSpace.getMaxY());
      two = new Point2D.Double(pop.nodeSpace.getMaxX()-5,pop.nodeSpace.getMaxY()); 
    }//right lower corner
    else if(bounds.contains(nodebounds.getX() - (popbounds.getWidth()+10),nodebounds.getMaxY() + 10, popbounds.getWidth(), popbounds.getHeight()))
    {
      pop.setC(nodebounds.getX() - (popbounds.getWidth()+10),nodebounds.getMaxY() + 10);
      pop.buildNode(GC);
      one = new Point2D.Double(pop.nodeSpace.getMaxX(),pop.nodeSpace.getY());
      two = new Point2D.Double(pop.nodeSpace.getMaxX(),pop.nodeSpace.getY()+5);
    }//left top corner
    else if(bounds.contains(nodebounds.getMaxX()+10,nodebounds.getY() - (popbounds.getHeight()+10), popbounds.getWidth(), popbounds.getHeight()))
    {
      pop.setC(nodebounds.getMaxX() +10,nodebounds.getY() - (popbounds.getHeight()+10));
      pop.buildNode(GC);
      one = new Point2D.Double(pop.nodeSpace.getX(),pop.nodeSpace.getMaxY());
      two = new Point2D.Double(pop.nodeSpace.getX()+5,pop.nodeSpace.getMaxY());
    }//left bottom corner
    else if(bounds.contains(nodebounds.getMaxX() +10,nodebounds.getMaxY() +10, popbounds.getWidth(), popbounds.getHeight()))
    {
      pop.setC(nodebounds.getMaxX()+10,nodebounds.getMaxY()+10);
      pop.buildNode(GC);
      one = new Point2D.Double(pop.nodeSpace.getX(),pop.nodeSpace.getY());
      two = new Point2D.Double(pop.nodeSpace.getX(),pop.nodeSpace.getY()+5);
    }
    
    pop.buildNode(GC);
    int[] xs =  {(int)nodebounds.getCenterX(),(int)one.x,(int) two.x};
    int[] ys = {(int)nodebounds.getCenterY(),(int) one.y,(int)two.y};
    
    pop.renderFill(GC.theG,new Color(244,240,138));
    pop.renderBorder(GC.theG, Color.black);
    pop.renderText(GC.theG,1.5,Color.black);
    Polygon slice;
    slice = new Polygon(xs,ys,3);  
    Area part1 = new Area(slice);
    pop.getNodeArea();
    pop.wholePop.add(part1);
    GC.theG.fill(slice);
  } 
  
  /**/public void unrenderPop()
  {
    Rectangle2D.Double unrend;
    unrend = new Rectangle2D.Double();
    unrend.setFrame(pop.nodeSpace.getX()-3,pop.nodeSpace.getY()-3,pop.nodeSpace.getWidth()+6,pop.nodeSpace.getHeight()+6);
    Shape draw;
    Area tU;
    Stroke tUS;
    draw= new Area(pop.wholePop);
    tU = new Area(draw);
    tUS = new BasicStroke(2.0f);//Stroke
    tU.add(new Area(tUS.createStrokedShape(draw))); //add an area to an area //Shape from stroke  
    GC.theG.setPaint(GC.theG.getBackground());
    GC.theG.fill(tU);
    tUS = new BasicStroke(4.0f);//Stroke
    tU.add(new Area(tUS.createStrokedShape(draw)));
    GC.theG.setClip(tU); //set render area
    RenderGraph(); //render graph
    RenderHighlight(curHigh); //render highlight 

    GC.theG.setClip(graphSpace);
  }/**/
  
  /**/ public void unrenderPop(int[] path)
  {
    Rectangle2D.Double unrend;
    unrend = new Rectangle2D.Double();
    unrend.setFrame(pop.nodeSpace.getX()-3,pop.nodeSpace.getY()-3,pop.nodeSpace.getWidth()+6,pop.nodeSpace.getHeight()+6);
    Shape draw;
    Area tU;
    Stroke tUS;
    draw= new Area(pop.wholePop);
    tU = new Area(draw);
    tUS = new BasicStroke(2.0f);//Stroke
    tU.add(new Area(tUS.createStrokedShape(draw))); //add an area to an area //Shape from stroke  
    GC.theG.setPaint(GC.theG.getBackground());
    GC.theG.fill(tU);
    tUS = new BasicStroke(4.0f);//Stroke
    tU.add(new Area(tUS.createStrokedShape(draw)));
    GC.theG.setClip(tU); //set render area
    RenderGraph(); //render graph
    RenderHighlight(curHigh); //render highlight 
    HighLightPath(path);
    GC.theG.setClip(graphSpace);
  }/**/
  
  
  
  public boolean HighLightPath(int nodepath[])
  {  
    int numHigh =Array.getLength(nodepath);
    Shape draw;
    Area tU;
    Stroke tUS;    

   
    if(numHigh <= numNodes)
    {
      draw= new Area(nodes[0].getNodeArea());
      tU = new Area(draw);
      int x[];
      int y[];
      x = new int[2];
      y = new int[2];
      
      //int show[][] = {{0,1,2,3},{4,5,6},{7,8},{9,10},{11,12,13,14,15,16},{17,18},{19,20},{21,22,23},{24,25,26},{27,28,29,30,31},{32,33,34,35,36}};

      for(int i=1; i<numHigh; i++)
      {
        //UNFINISHED HERE START DRAW LINE ADD TO HIGHPATH SHAPE
        //                        //always     //glob  //cL1 //rL1  //cont L2           //rL2    rL2     //rL3        //rL3       //rL3           //rL3
       //here is how: show[][] = {{0,1,2,3},{4,5,6},{7,8},{9,10},{11,12,13,14,15,16},{17,18},{19,20},{21,22,23},{24,25,26},{27,28,29,30,31},{32,33,34,35,36}};
        //get info for drawing a line between two nodes
        /*if(containsAny(show[i],nodepath[i-1]))
        {
          addVisibility(show[i]);
        }/**/
        Polygon edge;
        x[0] = (int)nodes[nodepath[i-1]].nodeSpace.getCenterX();
        y[0] = (int)nodes[nodepath[i-1]].nodeSpace.getCenterY();//nodes[nodepath[i-1]].cent.y;
        x[1] = (int)nodes[nodepath[i]].nodeSpace.getCenterX();//nodes[nodepath[i]].cent.x;
        y[1] = (int)nodes[nodepath[i]].nodeSpace.getCenterY();//nodes[nodepath[i]].cent.y;
       
        edge = new Polygon(x,y,2); 
        tUS = new BasicStroke(5.0f);////Stroke
        Area part1 = new Area(tUS.createStrokedShape(edge));  
        Area part2 = new Area(tUS.createStrokedShape(part1));
        tU.add(part2);
        tUS = new BasicStroke(10.0f);//Stroke
        tU.add(new Area(tUS.createStrokedShape(nodes[nodepath[i]].getNodeArea())));
        tU.add(new Area(nodes[nodepath[i]].getNodeArea()));
      }
      
      tUS = new BasicStroke(10.0f);////Stroke
      tU.add(new Area(tUS.createStrokedShape(draw)));
      
      GC.theG.setPaint(Color.RED);
      GC.theG.setStroke(new BasicStroke(5.0f,BasicStroke.CAP_ROUND,BasicStroke.JOIN_BEVEL));
      GC.theG.draw(tU);
      HighPath = new Area(tU);

      return true;
    }
    else
    {
      //throw exception or just don't do anything
      return false;
    }
  } 

  /**/public void unrenderHighPath()
  {

    Shape draw;
    Area tU;
    Stroke tUS;
    draw= new Area(HighPath);
    tU = new Area(draw);
    tUS = new BasicStroke(10.0f);//Stroke
    tU.add(new Area(tUS.createStrokedShape(draw))); //add an area to an area //Shape from stroke  
    GC.theG.setPaint(GC.theG.getBackground());
    GC.theG.fill(tU);
    draw = tU;

    tU.add(new Area(tUS.createStrokedShape(draw)));
    GC.theG.setClip(tU); //set render area
    RenderGraph(); //render graph
    RenderHighlight(curHigh); //render highlight 
    GC.theG.setClip(graphSpace);
  }/**/
  
  //UNFINISHED OR DEPRECIATED FUNCTION of loading all the 
  //popups and having an array of them
  public void loadPopTexts(String texts[])
  {
    int numPops =Array.getLength(texts);
    if(numPops == numNodes)
    {
      
    }
    else
    {
        //throw exception
    }
  }
  
  boolean isVisable(int index)
  {
    boolean result = false;
    //search visavbility for index
    if(visable != null)
    {
      for( int i =0; i < Array.getLength(visable); i++)
      {
        if(visable[i] == index)
        {
         return result = true;
        }
      }
    }
    return result;    
  }
  
  //end of Graph class
}