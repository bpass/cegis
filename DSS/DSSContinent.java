/*
 * DSSContinent.java
 *
 * Created on June 21, 2002, 8:54 AM
 *
 * Modified for relative addresing 6/2004
 * by James Nelson
 * Current version is 100% portable
 */

/**
 *
 * @author  sposch
 */
import java.awt.font.FontRenderContext;
import java.awt.font.TextLayout;
import java.awt.geom.Rectangle2D;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.applet.Applet;
import java.net.*;
import java.applet.AppletContext;

import javax.swing.*;

public class DSSContinent extends javax.swing.JApplet implements ItemListener, MouseListener, MouseMotionListener, Runnable {

  private int HighLight = 2;
  private int HighPath = 1;
  private int Gname = 2;
  private int setMax = 0;
  boolean changed = false;
  boolean nohold = true;
  public String projType;
  public String oldProj;
  public int currentX, currentY;
  public Font font;
  Thread runner;
  public int RID;
  public int oldcX = 0;
  public int oldcY = 0;
  public Rectangle2D.Double MYarea = new Rectangle2D.Double();
  public Rectangle2D.Double textlink;
  public int[] fullPath;
  public int[] vis;
  public boolean EnteredUnChanged = false;
  //public boolean selection = false;

    /** Creates new form DSSContinent */
    public DSSContinent() {
        initComponents();
        
        Container container = getContentPane();
        projType = "Not changing";
        
        //Load the Image
        String fileName = "PlateCarree.gif";  
        try
        {
            URL url = getClass().getResource(fileName);
            image = createImage((ImageProducer)url.getContent());
        }
        catch(IOException e)
        {
            System.out.println("Image not Loaded");
            e.printStackTrace();
            System.exit(1);
        }
        //End Load Image
        
        //Initialize necessary members
        preserve = "Shape";
        dataType = "Raster";
        rasterData = "Continuous";
        selected = 0;
        
        //Initialize the map Rectangles  NOTE: These Rectangles are based
        //entirely on the size of the picture when loaded0
        
        //North America Rectangles
        naRect1 = new Rectangle(0, 30, 240, 120);
        naRect2 = new Rectangle(240, 60, 30, 90);
        MYarea.setFrame(0,0,720,360);

        
        //South America Rectangle
        saRect = new Rectangle(180, 150, 120, 150);
        
        //Europe Rectangle
        euRect = new Rectangle(330, 30, 90, 75);
        
        //Africa Rectangles
        afRect1 = new Rectangle(300, 105, 120, 165);
        afRect2 = new Rectangle(420, 150, 60, 120);
        
        //Asia Rectangles
        asRect1 = new Rectangle(420, 30, 300, 120);
        asRect2 = new Rectangle(480, 150, 240, 30);
        
        //Austrailia Rectangle
        auRect = new Rectangle(540, 180, 180, 90);
        
        //Northern Polar Region Rectangles
        npRect1 = new Rectangle(0, 0, 720, 30);
        npRect2 = new Rectangle(240, 30, 90, 30);
        
        //Antarctica Rectangle
        anRect = new Rectangle(0, 300, 720, 60);
        
        
        //Set Applet Layout
        container.setLayout(new BorderLayout());
        
        //Set cursor to default
        container.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
        
        //Initialize preserve List
        preserveList = new List(2);
        preserveList.add("Shape");
        preserveList.add("Area");
        preserveList.select(0);
        preserveList.addItemListener(this);
        
        //Initialize data type List
        dataTypeList = new List(2);
        dataTypeList.add("Raster");
        dataTypeList.add("Vector");
        dataTypeList.select(0);
        dataTypeList.addItemListener(this);
        
        //Initialize raster Data List
        rasterDataList = new List(2);
        rasterDataList.add("Continous");
        rasterDataList.add("Thematic");
        rasterDataList.select(0);
        rasterDataList.addItemListener(this);
        
        //Initialize Labels
        rasterDataLabel = new JLabel("  Raster Data Type:");
        preserveLabel = new JLabel("Preserve:");
        dataTypeLabel = new JLabel("  Data Type:");
        
        //Add Lists and Labels to new JPanel, then Add panel to border layout
        choicePanel = new JPanel();
        choicePanel.add(preserveLabel);
        choicePanel.add(preserveList);
        choicePanel.add(dataTypeLabel);
        choicePanel.add(dataTypeList);
        choicePanel.add(rasterDataLabel);
        choicePanel.add(rasterDataList);
        choicePanel.setVisible(true);
        container.add(choicePanel, BorderLayout.SOUTH);
    }
    
    //functions for determining where the cursor is
    public boolean isContinent(int X, int Y)
    {
      boolean isIn =false;
      if(isNASAAF(X,Y) || isNPANSP(X,Y) || isEUAUAS(X,Y))
      {
        isIn = true;
      }
      return isIn;
    }
    /**
     * 
     */
   public boolean isNASAAF(int X, int Y)//is the point within noth America south America or Africa
    {
      boolean isIn =false;
      if((naRect1.contains(X, Y)) ||
         (naRect2.contains(X, Y)) ||
         (saRect.contains(X, Y))  ||
         (afRect1.contains(X, Y)) ||
         (afRect2.contains(X, Y)))
      {
        isIn = true;
      }
      return isIn;
    }
    
    public boolean isNPANSP(int X, int Y)//is the point within noth pole south pole
    {
      boolean isIn =false;
      if((npRect1.contains(X, Y)) ||
         (anRect.contains(X, Y))  ||
         (npRect2.contains(X, Y)))
      {
        isIn = true;
      }
      return isIn;
    }
    
    public boolean isEUAUAS(int X, int Y)//is the point within noth pole south pole
    {
     boolean isIn =false;
     if((euRect.contains(X, Y))  ||
        (auRect.contains(X, Y))  ||
        (asRect1.contains(X, Y)) ||
        (asRect2.contains(X, Y)))
      {
        isIn = true;
      }
      return isIn;
    }
    
    public void justEnterUnChange(int X, int Y)
    {
      if(EnteredUnChanged && MYarea.contains(X,Y))
      {
        EnteredUnChanged = false;
        nohold = true;
        changed = false;        
      }      
    }   
    
     //public void mouseReleased(MouseEvent e) {}
     //public void mousePressed(MouseEvent e) {}
     public void mouseEntered(MouseEvent e) {
          //if entered at y less than? 360
          if(e.getY() < 360)
          {
            nohold = true;
            changed = false; 
            EnteredUnChanged = false;
          }
          else
          {
            EnteredUnChanged = true;
          }
     }
     
     
     public void mouseExited(MouseEvent e) {
          //nohold = true;
          //changed = false;
     }
     public void mouseDragged(MouseEvent e){}
     
     //Functions used to set path  and highlight
     public void getPath(int contX, int contY)
     {
        if(isNASAAF(contX,contY))
           {
            //in north america, south america, africa
            if(preserve == "Area")//Area
            {
              HighPath = 7;//1; 
              vis = new int[] {0,2,7,11,12,13};
            }
            if(preserve == "Shape")//Shape
            {
              HighPath = 8;//2; 
              vis = new int[] {0,2,8,14,15,16};
            }
            if(nohold) changed  = true;//only update if there has been no click
           }
        if(isNPANSP(contX,contY))
           {
            if(preserve == "Area")//Area
            {
              HighPath = 7;//1;  
              vis = new int[] {0,2,7,11,12,13};
            }
            if(preserve == "Shape")//Shape
            {
              HighPath = 8;// 2;  
              vis = new int[] {0,2,8,14,15,16};
            }
            if(nohold) changed  = true;
           }
        if(isEUAUAS(contX,contY))           
           {
            if(preserve == "Area")//Area
            {
              HighPath = 7;//1; 
              vis = new int[] {0,2,7,11,12,13};
            }
            if(preserve == "Shape")//Shape
            {
              HighPath = 8;//2; 
              vis = new int[] {0,2,8,14,15,16};
            }
            if(nohold) changed  = true;
           }
     }
     
     public void getHighlight(int contX, int contY)
     {
        
        if(isNASAAF(contX,contY))
           {
            //in north america, south america, africa
            if(preserve == "Area")//Area
            {
              HighLight = 11;//3;
            }
            if(preserve == "Shape")//Shape
            {
              HighLight = 14;//6;
            }
            if(nohold) changed  = true;//only update if there has been no click
           }
        if(isNPANSP(contX,contY))
           {
            if(preserve == "Area")//Area
            {
              HighLight = 12;//4;                
            }
            if(preserve == "Shape")//Shape
            {
              HighLight = 15;//7;
            }
            if(nohold) changed  = true;
           }
         if(isEUAUAS(contX,contY))           
           {
            if(preserve == "Area")//Area
            {
              HighLight = 13;//5;
            }
            if(preserve == "Shape")//Shape
            {
              HighLight = 16;//8;                      
            }
            if(nohold) changed  = true;
           }       
     }
     
    
     
     public void getFullPath(int contX, int contY)
     {
        if(fullPath == null)//!= 3)
        {
          fullPath = new int[3];
        }
        else{
       if(isNASAAF(contX,contY))
           {
            //in north america, south america, africa
            if(preserve == "Area")//Area
            {              
              //fullPath[1] = HighPath = 1; 
              //fullPath[2] = HighLight = 3;
              //fullPath = new int[]{0,1,3};
              HighPath = 7; 
              HighLight = 11;
              fullPath = new int[]{0,2,7,11};
            }
            if(preserve == "Shape")//Shape
            {
              
              //fullPath[1] = HighPath = 2; 
              //fullPath[2] =HighLight = 6;
              //fullPath = new int[]{0,2,6};
              HighPath = 8; 
              HighLight = 14;
              fullPath = new int[]{0,2,8,14};
            }
            if(nohold) changed  = true;//only update if there has been no click
           }
        if(isNPANSP(contX,contY))
           {
            if(preserve == "Area")//Area
            {
               
              //fullPath[1] = HighPath = 1; 
              //fullPath[2] =HighLight = 4;
              HighPath = 7; 
              HighLight = 12;
              fullPath = new int[]{0,2,7,12};
            }
            if(preserve == "Shape")//Shape
            {
                
              //fullPath[1] = HighPath = 2;
              //fullPath[2] =HighLight = 7;
              HighPath = 8; 
              HighLight = 15;
              fullPath = new int[]{0,2,8,15};
            }
            if(nohold) changed  = true;
           }
        if(isEUAUAS(contX,contY))           
           {
            if(preserve == "Area")//Area
            {               
              //fullPath[1] = HighPath = 1; 
              //fullPath[2] =HighLight = 5;
              HighPath = 7; 
              HighLight = 13;
              fullPath = new int[]{0,2,7,13};
      
            }
            if(preserve == "Shape")//Shape
            {
               
              //fullPath[1] = HighPath = 2; 
              //fullPath[2] = HighLight = 8;
              HighPath = 8; 
              HighLight = 16;
              fullPath = new int[]{0,2,8,16};
            }
            if(nohold) changed  = true;
           }
        }
     }
     
     
        //addMouseMotionListener(new MouseMotionAdapter()
        //{
            //Listen for mouse movement
  public void mouseMoved(MouseEvent e)
  {
    //Get content pane
    Container c = getContentPane();
    
    //Get current x and y value of cursor
    int currentXa = e.getX();
    int currentYb = e.getY();
    String tempProj = projType;
    
    this.justEnterUnChange(currentXa,currentYb);
    
    //If on a continent, set hand cursor
    if(isContinent(currentXa,currentYb) && nohold)
      {
       /***/
        if(isNPANSP(currentXa,currentYb))
        {
          //If preserving shape, goto shape web page 
          if(preserve == "Shape")
          {
          
            String location;
            tempProj = "Stereographic";
            //DSSUpdater.setStuffName("Stereographic");            
          }
          else
          {
            String location;
            tempProj = "Lambert Azimuthal Equal Area";
            //DSSUpdater.setStuffName("Lambert Azimuthal Equal Area");
            
            //Get location from html file
            location = getParameter("location4");
            //repaint();
          }
          //oldcX = currentX;
          //oldcY = currentY;
          //selection = true;
          //nohold = false;
        }
        if(isNASAAF(currentXa, currentYb))
        {
          //If preserving shape, goto shape web page
          if(preserve == "Shape")
          {
            String location;
            //Transverse Mercator 
            tempProj = "Transverse Mercator";
            //DSSUpdater.setStuffName("Lambert Azimuthal Equal Area");
            
            //Get location from html file
            location = getParameter("location3");
            //repaint();
         
          
          }
          else
          {
            String location;
            tempProj ="Cylindrical Equal Area";
          }
          //oldcX = currentX;
          //oldcY = currentY;
          //selection = true;
          //nohold = false;
          
        
        }
        //Check for click in Asia, Europe, or Australia
        if(this.isEUAUAS(currentXa,currentYb))
        {
        //If preserving shape, goto shape web page
        if(preserve == "Shape")
        {
          String location;
          tempProj = "Lambert Conformal Conic";        
        
        //repaint();
        
        }        
        //Goto area web page to preserve area
        else
        {
          String location;
          tempProj ="Albers Equal Area Conic";
        }
        
        }
        
        /****/
        if(nohold && (0 != tempProj.compareTo(projType)))
        {
          projType = tempProj;
          repaint();
        }
        else if(0 != tempProj.compareTo(projType))
        {
          projType = tempProj;
        }
      
      if(nohold)
      {
        changed = true;
        getFullPath(currentXa,currentYb);
      }
    }
    
    if((textlink.contains(currentXa,currentYb) && !nohold)|| isContinent(currentXa, currentYb))
    {
      c.setCursor(Cursor.getPredefinedCursor(
                  Cursor.HAND_CURSOR));
    }
    else 
    {
      c.setCursor(Cursor.getPredefinedCursor(
        Cursor.DEFAULT_CURSOR));
    }
  
  }
  
    public void grabbag(int curx,int cury)
    {
      getPath(curx,cury);
      getHighlight(curx,cury);
      getFullPath(curx,cury);
    }

       
        
    public void mouseClicked(MouseEvent e)
    {
      oldcX = currentX;
      oldcY = currentY;
      currentX = e.getX();
      currentY = e.getY();
      int button = e.getButton();
      /*if(button == MouseEvent.BUTTON1)
      {
        
      }*/
      if(button == MouseEvent.BUTTON2 || button == MouseEvent.BUTTON3 )
      {
        nohold = true;
        repaint();
      }
      else{
      
      if(MYarea.contains(currentX,currentY))
      {
      
      //selection = false;
      //if(projType ==

      if(isNPANSP(currentX,currentY))
      {
          //If preserving shape, goto shape web page 
          if(preserve == "Shape")
          {
          
              String location;
              projType = "Stereographic";
              //DSSUpdater.setStuffName("Stereographic");
              RID = 0;
        
          }
          else
          {
            String location;
            projType = "Lambert Azimuthal Equal Area";
            //DSSUpdater.setStuffName("Lambert Azimuthal Equal Area");
            
            //Get location from html file
            location = getParameter("location4");
            RID = 0;
            //repaint();
          }
          oldcX = currentX;
          oldcY = currentY;
          //selection = true;
          nohold = false;
      }
      if(isNASAAF(currentX,currentY))
      {
          //If preserving shape, goto shape web page
          if(preserve == "Shape")
          {
              String location;
              //Transverse Mercator 
              projType = "Transverse Mercator";
              //DSSUpdater.setStuffName("Lambert Azimuthal Equal Area");
  
              //Get location from html file
              location = getParameter("location3");
              RID = 1;
              //repaint();

          }
          else
          {
               String location;
               projType ="Cylindrical Equal Area";
               //DSSUpdater.setStuffName("Lambert Azimuthal Equal Area");
               RID = 1;
          }
          oldcX = currentX;
          oldcY = currentY;
          //selection = true;
          nohold = false;

      }
      //Check for click in Asia, Europe, or Australia
      if(this.isEUAUAS(currentX,currentY))
      {
          //If preserving shape, goto shape web page
          if(preserve == "Shape")
          {
              String location;
              projType = "Lambert Conformal Conic";
              //DSSUpdater.setStuffName("Lambert Azimuthal Equal Area");
              RID = 2;
  
              
              //repaint();
          }
          
          //Goto area web page to preserve area
          else
          {
              String location;
              projType ="Albers Equal Area Conic";
              //DSSUpdater.setStuffName("Lambert Azimuthal Equal Area");
              RID = 2;
              
              //repaint();
          }
          oldcX = currentX;
          oldcY = currentY;
          //selection = true;
          nohold = false;

      }
      /*if(0!=oldProj.compareTo(projType))
      {
        nohold = !nohold;
      }//*/
      repaint();
      }
      else
      {
        if(textlink.contains(currentX,currentY))
        {
          formAndCallURL(RID);
          
        }
        currentX = oldcX;
        currentY = oldcY;
      }
      }
    
      
    }
    
    public void formAndCallURL(int URLid)
    {
      //Get current x and y of cursor

      if(!nohold)
      {
                //Check for click in Northern Polar Region or Antarctica
                if(URLid == 0)
                {
                    //If preserving shape, goto shape web page 
                    if(preserve == "Shape")
                    {
        
                        String location;
            
                        //Get location from html file
                        location = getParameter("location5");

        /*////////////////////////////////////////////////
        Relative Address Changes where made here. The folder
        depth denoted by relative addressing "../", in the
        genaration of a "new URL". These changes were made in
        DSSApplet.java, DSSContinet.java, and 
        DSSRegional.java and coresponding HTML files
        **Important Note: When making changes to these files
        they must be recompiled with javac, and browser cache
        must be cleared to reflect changes!!!!
        ///////////////////////////////////////////////*/
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(getDocumentBase(), location);
                            //url = new URL(url,location);
                                                      
                            AppletContext browser = getAppletContext();
            
                            //browser.showDocument(url);
                            browser.showDocument(url,"_blank");
                        }
            
                        //Handle malformed url exception in connection attempt
                        catch(MalformedURLException urlException)
                        {
                            urlException.printStackTrace();
                        }
                    }
                    
                    //Goto area web page to preserve area
                    else
                    {
                        String location;
            
                        //Get location from html file
                        location = getParameter("location4");

         /*////////////////////////////////////////////////
        Relative Address Changes where made here. The folder
        depth denoted by relative addressing "../", in the
        genaration of a "new URL". These changes were made in
        DSSApplet.java, DSSContinet.java, and 
        DSSRegional.java and coresponding HTML files
        **Important Note: When making changes to these files
        they must be recompiled with javac, and browser cache
        must be cleared to reflect changes!!!!
        ///////////////////////////////////////////////*/
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(getDocumentBase(), location);
                            //url = new URL(getDocumentBase(), "../");
                            //url = new URL(url,location);
                           
                            AppletContext browser = getAppletContext();
            
                            //browser.showDocument(url);
                            browser.showDocument(url,"_blank");
                          
                        }
            
                        //Handle malformed url exception in connection attempt
                        catch(MalformedURLException urlException)
                        {
                            urlException.printStackTrace();
                        }
                    }
                }
                
                //Check for click in North America, South America, or Africa
                if(URLid == 1)
                {
                    //If preserving shape, goto shape web page
                    if(preserve == "Shape")
                    {
                        String location;
            
                        //Get location from html file
                        location = getParameter("location3");

         /*////////////////////////////////////////////////
        Relative Address Changes where made here. The folder
        depth denoted by relative addressing "../", in the
        genaration of a "new URL". These changes were made in
        DSSApplet.java, DSSContinet.java, and 
        DSSRegional.java and coresponding HTML files
        **Important Note: When making changes to these files
        they must be recompiled with javac, and browser cache
        must be cleared to reflect changes!!!!
        ///////////////////////////////////////////////*/
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(getDocumentBase(), location);
                            //url = new URL(getDocumentBase(), "../");
                            //url = new URL(url,location);
                           
                            AppletContext browser = getAppletContext();
            
                            //browser.showDocument(url);
                            browser.showDocument(url,"_blank");
                        }
            
                        //Handle malformed url exception in connection attempt
                        catch(MalformedURLException urlException)
                        {
                            urlException.printStackTrace();
                        }
                    }
                    
                    //Goto area web page to preserve area
                    else
                    {
                        String location;
            
                        //Get location from html file
                        location = getParameter("location2");

        /*////////////////////////////////////////////////
        Relative Address Changes where made here. The folder
        depth denoted by relative addressing "../", in the
        genaration of a "new URL". These changes were made in
        DSSApplet.java, DSSContinet.java, and 
        DSSRegional.java and coresponding HTML files
        **Important Note: When making changes to these files
        they must be recompiled with javac, and browser cache
        must be cleared to reflect changes!!!!
        ///////////////////////////////////////////////*/
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(getDocumentBase(), location);
                            //url = new URL(getDocumentBase(), "../");
                            //url = new URL(url,location);
                           
                            AppletContext browser = getAppletContext();
            
                            //browser.showDocument(url);
                            browser.showDocument(url,"_blank");
                        }
            
                        //Handle malformed url exception in connection attempt
                        catch(MalformedURLException urlException)
                        {
                            urlException.printStackTrace();
                        }
                    }
                }
                
                //Check for click in Asia, Europe, or Australia
                if(URLid == 2)
                {
                    //If preserving shape, goto shape web page
                    if(preserve == "Shape")
                    {
                        String location;
            
                        //Get location from html file
                        location = getParameter("location1");
        /*////////////////////////////////////////////////
        Relative Address Changes where made here. The folder
        depth denoted by relative addressing "../", in the
        genaration of a "new URL". These changes were made in
        DSSApplet.java, DSSContinet.java, and 
        DSSRegional.java and coresponding HTML files
        **Important Note: When making changes to these files
        they must be recompiled with javac, and browser cache
        must be cleared to reflect changes!!!!
        ///////////////////////////////////////////////*/
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(getDocumentBase(), location);
                            //url = new URL(getDocumentBase(), "../");
                            //url = new URL(url,location);
                           
                            AppletContext browser = getAppletContext();
            
                            //browser.showDocument(url);
                            browser.showDocument(url,"_blank");
                        }
            
                        //Handle malformed url exception in connection attempt
                        catch(MalformedURLException urlException)
                        {
                            urlException.printStackTrace();
                        }
                    }
                    
                    //Goto area web page to preserve area
                    else
                    {
                        String location;
            
                        //Get location from html file
                        location = getParameter("location0");

        /*////////////////////////////////////////////////
        Relative Address Changes where made here. The folder
        depth denoted by relative addressing "../", in the
        genaration of a "new URL". These changes were made in
        DSSApplet.java, DSSContinet.java, and 
        DSSRegional.java and coresponding HTML files
        **Important Note: When making changes to these files
        they must be recompiled with javac, and browser cache
        must be cleared to reflect changes!!!!
        ///////////////////////////////////////////////*/
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(getDocumentBase(), location);
                            //url = new URL(getDocumentBase(), "../");
                            //url = new URL(url,location);
                           
                            AppletContext browser = getAppletContext();
            
                            //browser.showDocument(url);
                            browser.showDocument(url,"_blank");
                        }
            
                        //Handle malformed url exception in connection attempt
                        catch(MalformedURLException urlException)
                        {
                            urlException.printStackTrace();
                        }
                    }
         } 
      }
    }
    
    
    public void mousePressed(MouseEvent e) {
      
    }
    
    public void mouseReleased(MouseEvent e) {
     
    }
    
    
    

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    private void initComponents() {//GEN-BEGIN:initComponents
    //addMouseMotionListener(this);
    //font = new Font("Times New Roman", Font.BOLD, 16);
    //projType = "InitComp";
    DSSUpdater.setGraphName(2);
    DSSUpdater.setHighLight(0);
    DSSUpdater.setHighPath(2);
    
    fullPath = new int[]{0,2};
    
    DSSUpdater.setHighPathAr(fullPath);
    //DSSUpdater.getHighPathAr(fullPath);

    }//GEN-END:initComponents

    public void itemStateChanged(java.awt.event.ItemEvent itemEvent) 
    {
        //Get current list values selected
        preserve = preserveList.getSelectedItem();
        dataType = dataTypeList.getSelectedItem();
        rasterData = rasterDataList.getSelectedItem();
        if(HighLight >= 11)
        {
              if(preserve == "Area")//Area
              {
                if( HighPath != 7)
                {
                  //HighPath = 1;
                  HighPath = 7;
                  HighLight = HighLight-3; 
                  fullPath = new int[]{0,HighPath,HighLight};
                  fullPath = new int[]{0,2,7,HighLight};//+8};
                  vis = new int[] {0,2,7,11,12,13};
                  //don't show 14,15,16
                  //DSSUpdater.setHighPathAr(fullPath);
                  changed  = true;
                }
              }
              if(preserve == "Shape")//Shape
              {
                if( HighPath != 8)
                {
                  //HighPath = 2;
                  HighPath = 8;
                  HighLight = HighLight+3;
                  fullPath = new int[]{0,HighPath,HighLight};
                  fullPath = new int[]{0,2,8,HighLight};//+8};
                  vis = new int[] {0,2,8,14,15,16};
                  //don't show 11,12,13
                  //DSSUpdater.setHighPathAr(fullPath);
                  changed  = true;
                }
              }
              changed  = true;
        }
        else
        {
          if(preserve == "Area")//Area
              {
                if( HighPath != 7)
                {
                  //HighPath = 1;
                  HighPath = 7;
                  //HighLight = HighLight-3;
                  fullPath = new int[]{0,HighPath};
                  fullPath = new int[]{0,2,7,HighLight};
                  vis = new int[] {0,2,7,11,12,13};
                  //DSSUpdater.setHighPathAr(fullPath);
                  changed  = true;
                }
              }
              if(preserve == "Shape")//Shape
              {
                if( HighPath != 8)
                {
                 // HighPath = 2;
                  HighPath = 8;
                  //HighLight = HighLight+3;
                  fullPath = new int[]{0,HighPath};
                  fullPath = new int[]{0,2,8,HighLight};
                  vis = new int[] {0,2,8,14,15,16};
                  //DSSUpdater.setHighPathAr(fullPath);
                  changed  = true;
                }
              }
              changed  = true;          
        }
        
                    //if data type list is vector, set raster list/label disabled
              if(itemEvent.getSource() == dataTypeList)
              {
                        if(dataType=="Vector")
                        {
                            rasterDataList.setEnabled(false);
                            selected = rasterDataList.getSelectedIndex();
                            rasterDataList.deselect(selected);
                            rasterDataLabel.setEnabled(false);
                        }
                        else 
                        {
                            rasterDataList.setEnabled(true);
                            rasterDataList.select(selected);
                            rasterDataLabel.setEnabled(true);
                        }
              }
                  repaint(); 
    }    

    public final synchronized void update(Graphics g)
    {
      Dimension d = getSize();
      g.setFont(font);
      //projType = DSSUpdater.getStuffName();//("upate");
      
      if((offScreenImage == null) || (d.width != offScreenSize.width) ||
      (d.height != offScreenSize.height))
      {
        offScreenImage = createImage(d.width, d.height);
        offScreenSize = d;
        offScreenGraphics = offScreenImage.getGraphics();        
      }
    
      offScreenGraphics.clearRect(0,0,d.width, d.height);
   
      paint(offScreenGraphics);
      
      g.drawImage(offScreenImage,0,0,null);
      
    }
    
    public void paint(Graphics g)
    {
        font = new Font("Times New Roman", Font.BOLD, 16);
        //FRC = theG.getFontRenderContext();
        Graphics2D tempg = (Graphics2D)g;
        FontRenderContext frc = tempg.getFontRenderContext();
        TextLayout tl; 
        super.paint(g);
        g.drawImage(image, 0, 0, MAXX, MAXY, this);
        g.setFont(font);
        //projType = DSSUpdater.getStuffName();//"This Program Sucks";
        currentX = oldcX;
        currentY = oldcY;
       
        Color temp;
        temp = g.getColor();
        tl = new TextLayout(projType,font,frc);
        if(nohold)//must be modified
        {
         g.setColor(Color.black);//this next line
         //g.drawString(projType,360-(projType.length()*3),390);
        }
        else
        {
          g.setColor(Color.blue);
          g.drawLine((int)(360-(tl.getBounds().getWidth()/2)),393,(int)(360+(tl.getBounds().getWidth()/2)),393);
          //tl.draw(tempg,(float)(360-(tl.getBounds().getWidth()/2)),(float)390);
        }
        Graphics2D g3 = (Graphics2D)g;
        /////////////////////////////
        //g.drawString(projType,360-(projType.length()*3),390);
         textlink = new Rectangle2D.Double((double)360-((tl.getBounds().getWidth()+24)/2-5),
        (double)(390-14),(double)(tl.getBounds().getWidth()+10),tl.getBounds().getHeight()+10);
        //g3.draw(textlink);
        tl.draw(tempg,(float)(360-(tl.getBounds().getWidth()/2)),(float)390);
        /*textlink = new Rectangle2D.Double((double)360-5-(tl.getBounds().getWidth()/2-12),
        (double)390,tl.getBounds().getWidth()+10,tl.getBounds().getWidth()+24);*/
        /////////////////////////////////
     
        g.setColor(temp);
        oldProj = new String(projType);
        //Graphics2D g3 = (Graphics2D)g;
        g3.setStroke(new BasicStroke(2.0f));
        g3.draw(MYarea);
        //g3.draw(textlink);

    }
    
   /* public void update(Graphics g)
    {
      if(projType != null)
        {
          g.drawString(projType,360-(projType.length()*3),390);
        }
    }*/
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    // End of variables declaration//GEN-END:variables
    
    private Image image;                        //Map Image
    
    private Image offScreenImage;
    private Dimension offScreenSize;
    private Graphics offScreenGraphics;
    
    private String preserve, dataType, rasterData;  //Selected List Item
    private JPanel choicePanel;                     //Panel for Lists
    private List preserveList, dataTypeList, rasterDataList;    //Lists
    
    private int selected;                       //Currently selected index
    
    private JLabel rasterDataLabel;             //Labels for the Lists
    private JLabel preserveLabel;
    private JLabel dataTypeLabel;
    
    private final int MAXX = 720;               //Max X value
    private final int MAXY = 360;               //Max Y value

    private Rectangle naRect1, naRect2;         //North America Rectangles
    private Rectangle saRect;                   //South America Rectangle
    private Rectangle euRect;                   //Europe Rectangle
    private Rectangle afRect1, afRect2;         //Africa Rectangles
    private Rectangle asRect1, asRect2;         //Asia Rectangles
    private Rectangle auRect;                   //Australia Rectangle
    private Rectangle npRect1, npRect2;         //Northern Polar Rectangles
    private Rectangle anRect;                   //Antarctica Rectangles
    
    public void init()
  {
    DSSUpdater.setHighLight(HighLight);//.setTicket(ticket);
    addMouseMotionListener(this);
    addMouseListener(this);
    vis = new int[] {0,2,8,14,15,16};
    oldProj = new String(projType);
    //selection = false;
  }

  public void start()
  {
    if (runner == null)
    {
      runner = new Thread(this);
      runner.start();
    }
  }

  public void stop()
  {
    if (runner != null)
    {
      runner.stop();
      runner = null;
    }
  }

  public void run()
  {
    while (true)
    {
      if(changed)
      {
        //perform appropriate update
        DSSUpdater.setHighLight(HighLight);
        DSSUpdater.setHighPath(HighPath);
        DSSUpdater.setGraphName(Gname);
        DSSUpdater.setHighPathAr(fullPath);
        DSSUpdater.setVisAr(vis);
        changed  = false;
      }
        pause(10);//pause for DSS is Fine
      }
  }

  public void pause(int time)
  {
    try
    {
      Thread.sleep(time);
    }
    catch (InterruptedException e) {}
  }//End of Threading Code WORKS YEAH!!!!!
}
