/*
 * DSSRegional.java
 *
 * Created on June 20, 2002, 1:38 PM
 * 
 * Last modified by bpassuello on April 24, 2013
 * 
 * This java application provides the functions for the regional projection
 * applet.  It imports the main image than allows the user to use a selection 
 * box, that they can resize, to choose what region they are interested in.  
 * Then, depending on the shape of the region that they selected, and where 
 * that selected space is, it goes uses comparison statements to decide what 
 * type of projection is recommended. The user can then click on the 
 * recommended projection and it will start up another browser with details on
 * what that projection looks like and its uses.
 */

/**
 *
 * @author  sposch
 */
import java.applet.AppletContext; //Enables applet/browser interaction and the 
    //playing of audioclips. In Java 2, class javax.swing.JApplet is used to 
    //define an applet that uses Swing GUI components
import java.awt.font.FontRenderContext; //Allows advanced 2D graphics
import java.awt.font.TextLayout;        //for complex graphical manipulations
import java.awt.geom.Rectangle2D; 
import java.io.*;                     //The input/output package
import java.awt.*;                    //Required to create and manipulate GUIs
import java.awt.event.*;              //Enable event handling
import java.awt.image.*;              
import java.applet.Applet;            //Enables applet interaction
import java.net.*;                    //allows communication via networks

    //Allows for GUI components and and supports portable GUIs
import javax.swing.*; 
import javax.swing.JOptionPane.*;


public class DSSRegional extends javax.swing.JApplet 
                         implements Runnable,ItemListener {

          //detects when an the mouse or an item changes
      public boolean changed = false; 
      Thread runner;      
      public int RID;    //Used to tell which URL to call due to the projection
      public String location;//holds the URL address gotten from the DSS applet
     // public boolean textClick = false;  
      
      
    /** Creates new form DSSRegional */
    public DSSRegional() 
    {
        initComponents();
        
        Container container = getContentPane();
        
        //Load the Image
        String fileName = "PlateCarree.gif";  //The map that you want shown
        try
        {
            URL url = getClass().getResource(fileName); //tries to get the map
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
        projType = "Mercator";
        preserve = "Shape";
        dataType = "Raster";
        rasterData = "Continuous";
        selected = 0;
        
        //Set Applet Layout
        container.setLayout(new BorderLayout());
        
        //Initialize Selection Box Vars
        upperLeftX = STARTupperLeftX;
        upperLeftY = STARTupperLeftY;
        width = STARTHW;
        height = STARTHW;
        lowerRightX = upperLeftX + width;
        lowerRightY = upperLeftY + height;
        extent = 0;
        
        //Set cursor to default
        container.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
        
        //Set font to Bold 16-pt Times New Roman
        font = new Font("Times New Roman", Font.BOLD, 16);
        
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
       
          //Handles mouse events  like mouseMoved and mouseDragged
        addMouseMotionListener(new MouseMotionAdapter()
        {
            //Listen for mouse movement
            public void mouseMoved(MouseEvent e) //Detectes when mouse moves
            {
                Container c = getContentPane();
                
                currentX = e.getX();  //Gets current location
                currentY = e.getY();
                
                //If at NW corner, set cursor to NW Resize
                if((currentX <= upperLeftX + DIST) && 
                   (currentX >= upperLeftX - DIST) &&
                   (currentY <= upperLeftY + DIST) &&
                   (currentY >= upperLeftY - DIST))
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                        Cursor.NW_RESIZE_CURSOR));
                    repaint();
                }
                
                //If at SE corner, set cursor to SE Resize
                else if((currentX <= upperLeftX + width + DIST) && 
                        (currentX >= upperLeftX + width - DIST) &&
                        (currentY <= upperLeftY + height + DIST) &&
                        (currentY >= upperLeftY + height - DIST))
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                        Cursor.SE_RESIZE_CURSOR));
                    repaint();
                }
                
                //If at SW corner, set cursor to SW Resize
                else if((currentX <= upperLeftX + DIST) && 
                        (currentX >= upperLeftX - DIST) &&
                        (currentY <= upperLeftY + height + DIST) &&
                        (currentY >= upperLeftY + height - DIST))
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                        Cursor.SW_RESIZE_CURSOR));
                    repaint();
                }
                
                //If at NE corner, set cursor to NE Resize
                else if((currentX <= upperLeftX + width + DIST) && 
                        (currentX >= upperLeftX + width - DIST) &&
                        (currentY <= upperLeftY + DIST) &&
                        (currentY >= upperLeftY - DIST))
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                        Cursor.NE_RESIZE_CURSOR));
                    repaint();
                }
                
                //If at W side, set cursor to W Resize
                else if((currentX <= upperLeftX + DIST) && 
                        (currentX >= upperLeftX - DIST) &&
                        (currentY > upperLeftY) &&
                        (currentY < upperLeftY + height))
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                        Cursor.W_RESIZE_CURSOR));
                    repaint();
                }
                
                //If at N side, set cursor to N Resize
                else if((currentX > upperLeftX) && 
                        (currentX < upperLeftX + width) &&
                        (currentY <= upperLeftY + DIST) &&
                        (currentY >= upperLeftY - DIST))
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                        Cursor.N_RESIZE_CURSOR));
                    repaint();
                }
                
                //If at E side, set cursor to E Resize
                else if((currentX <= upperLeftX + width + DIST) && 
                        (currentX >= upperLeftX + width - DIST) &&
                        (currentY > upperLeftY) &&
                        (currentY < upperLeftY + height))
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                        Cursor.E_RESIZE_CURSOR));
                    repaint();
                }
                
                //If at S side, set cursor to S Resize
                else if((currentX > upperLeftX) && 
                        (currentX < upperLeftX + width) &&
                        (currentY <= upperLeftY + height + DIST) &&
                        (currentY >= upperLeftY + height - DIST))
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                        Cursor.S_RESIZE_CURSOR));
                    repaint();
                }
                
                //If inside box, set cursor to Move
                else if((currentX < upperLeftX + width) && 
                        (currentX > upperLeftX) &&
                        (currentY > upperLeftY) &&
                        (currentY < upperLeftY + height))
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                        Cursor.MOVE_CURSOR));
                    repaint();
                }
                
                //Otherwise, set cursor to Default
                else
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                        Cursor.DEFAULT_CURSOR));
                }
                   //if the box displaying the projection has something in it
                if(textlink != null) 
                { //and the cursor is over the projection name(textlink area)
                  if(textlink.contains(currentX, currentY))
                  {
                    c.setCursor(Cursor.getPredefinedCursor(
                  Cursor.HAND_CURSOR));
                  }
                   /*else 
                   {
                    c.setCursor(Cursor.getPredefinedCursor(
                    Cursor.DEFAULT_CURSOR));
                   }*/
                }
                
            }
            
            //Handles mouse Dragging Events
            public void mouseDragged(MouseEvent e)
            {
                changed = true;   
                Container c = getContentPane();
                
                currentX = e.getX();    //gets the new location
                currentY = e.getY();
                
                //If past max value for x, reset to max x
                if(currentX >= MAXX)
                {
                    currentX = MAXX;
                }
                
                //If past 0 value for x, reset to 0
                else if(currentX <= 0)
                {
                    currentX = 0;
                }
                
                //If past max value for y, reset to max y
                if(currentY >= MAXY)
                {
                    currentY = MAXY;
                }
                
                //If past 0 value for y, reset to 0
                else if(currentY <=0)
                {
                    currentY = 0;
                }
                
                //If inside box, handle movement of box
                if(c.getCursor().getType() == Cursor.MOVE_CURSOR)
                {
                       upperLeftX = currentX - (width/2);
                       lowerRightX = currentX + width - (width/2);
                       upperLeftY = currentY - (height/2);
                       lowerRightY = currentY + height - (height/2);
                }
                
                //If on SW corner, resize SW corner
                else if(c.getCursor().getType() == Cursor.SW_RESIZE_CURSOR)
                {
                       if(lowerRightX - currentX >= STARTHW && 
                          lowerRightX - currentX <= MAXBOXX) 
                       {
                           upperLeftX = currentX;
                       }
                       if((currentY - upperLeftY >= STARTHW) &&
                          (currentY - upperLeftY <= MAXBOXY)) 
                       {
                           lowerRightY = currentY;
                       }
                }
                
                //If on SE corner, resize SE corner
                else if(c.getCursor().getType() == Cursor.SE_RESIZE_CURSOR)
                {
                       if((currentX - upperLeftX >= STARTHW) &&
                          (currentX - upperLeftX <= MAXBOXX)) 
                       {
                           lowerRightX = currentX;
                       }
                       if((currentY - upperLeftY >= STARTHW) &&
                          (currentY - upperLeftY <= MAXBOXY)) 
                       {
                           lowerRightY = currentY;
                       }
                }
                
                //If on NW corner, resize NW corner
                else if(c.getCursor().getType() == Cursor.NW_RESIZE_CURSOR)
                {
                       if((lowerRightX - currentX >= STARTHW) &&
                          (lowerRightX - currentX <= MAXBOXX)) 
                       {
                           upperLeftX = currentX;
                       }
                       if((lowerRightY - currentY >= STARTHW) &&
                          (lowerRightY - currentY <= MAXBOXY)) 
                       {
                           upperLeftY = currentY;
                       }
                }
                
                //If on NE corner, resize NE corner
                else if(c.getCursor().getType() == Cursor.NE_RESIZE_CURSOR)
                {
                       if((currentX - upperLeftX >= STARTHW) &&
                          (currentX - upperLeftX <= MAXBOXX)) 
                       {
                           lowerRightX = currentX;
                       }
                       if((lowerRightY - currentY >= STARTHW) &&
                          (lowerRightY - currentY <= MAXBOXY)) 
                       {
                           upperLeftY = currentY;
                       }
                }
                
                //If on N side, resize N side
                else if(c.getCursor().getType() == Cursor.N_RESIZE_CURSOR)
                {
                       if((lowerRightY - currentY >= STARTHW) &&
                          (lowerRightY - currentY <= MAXBOXY)) 
                       {
                           upperLeftY = currentY;
                       }
                }
                
                //If on S side, resize S side
                else if(c.getCursor().getType() == Cursor.S_RESIZE_CURSOR)
                {
                       if((currentY - upperLeftY >= STARTHW) &&
                          (currentY - upperLeftY <= MAXBOXY)) 
                       {
                           lowerRightY = currentY;
                       }
                }
                
                //If on W side, resize W side
                else if(c.getCursor().getType() == Cursor.W_RESIZE_CURSOR)
                {
                       if((lowerRightX - currentX >= STARTHW) &&
                          (lowerRightX - currentX <= MAXBOXX)) 
                       {
                           upperLeftX = currentX;
                       }
                }
                
                //If on E side, resize E side
                else if(c.getCursor().getType() == Cursor.E_RESIZE_CURSOR)
                {
                       if((currentX - upperLeftX >= STARTHW) &&
                          (currentX - upperLeftX <= MAXBOXX)) 
                       {
                           lowerRightX = currentX;
                       }
                }
                width = lowerRightX - upperLeftX;
                height = lowerRightY - upperLeftY;		    
                repaint();
            }	//end of mouseDragged function
          }    //end of new mouseMotionAdapter
         ); //end of addMouseListener call
         
            //Allows for detecting when the mouse is pressed
         addMouseListener(new MouseAdapter()
         {
            public void mouseClicked(MouseEvent e)
            {
                //for checking if it is in the textlink box
              int currentXa = e.getX();   
              int currentYa = e.getY();
                  //only if the cursor is over the projection name
              if((textlink!=null)&&(textlink.contains(currentXa,currentYa)))
              { 
                    //Gets ands displays the projection type browser
                formAndCallURL(RID);  
              }   //end of if statement
            } //end of mouseClicked function
         } //end of MouseAdapter function
        ); //end of addMouseListener call
     }  //end of DSS constructor

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    private void initComponents() {//GEN-BEGIN:initComponents

    }//GEN-END:initComponents

    public void itemStateChanged(java.awt.event.ItemEvent itemEvent) 
    { //When a change is detected it udpates it variables
        changed = true;
        //Get current list values selected
        preserve = preserveList.getSelectedItem();
        dataType = dataTypeList.getSelectedItem();
        rasterData = rasterDataList.getSelectedItem();
        
        //if data type list is vector, set raster list/label disabled
	if(itemEvent.getSource() == dataTypeList)
	{
            if(dataType=="Vector")
            {
                rasterDataList.select(0);         //You must select each element 
                rasterDataList.setEnabled(false); //and hide them individually
                rasterDataList.select(1);         
                rasterDataList.setEnabled(false);   
                
                selected = rasterDataList.getSelectedIndex();
                rasterDataList.deselect(selected);
                rasterDataLabel.setEnabled(false);
            }
            else 
            {
                rasterDataList.select(0);        //You must select each element
                rasterDataList.setEnabled(true); //and re-enable them 
                rasterDataList.select(1);        //individually
                rasterDataList.setEnabled(true);
                
                rasterDataList.select(0);       //Makes first element selected
                rasterDataLabel.setEnabled(true);
            }
	}
     	repaint(); 
    }
    
    public final synchronized void update(Graphics g)
    {   //Updates the graphics
        Dimension d = getSize();
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

        //the paint function takes care of choosing which projection 
          //and displaying the results
    public void paint(Graphics g)
    {
        //call super's paint method
        font = new Font("Times New Roman", Font.BOLD, 16);
        //FRC = theG.getFontRenderContext();
        Graphics2D tempg = (Graphics2D)g;
        FontRenderContext frc = tempg.getFontRenderContext();
        TextLayout tl; 
    
        super.paint(g);
        
        //draw the image
        g.drawImage(image, 0, 0, MAXX, MAXY, this);
        g.setFont(font);
        ////////From Here
        if(width >= height)
        {
            extent = 0;
        }
        else
        {
            extent = 1;
        }
        ////tohere
        //set color blue and draw box
        g.setColor(Color.BLUE);
        g.drawRect(upperLeftX, upperLeftY, width, height);
        ///againhere
        //If width >= height East West node 2
        if(extent == 0)
        {
            if(preserve == "Shape")
            { 
                DSSUpdater.setVisAr(new int[] {0,3,10,19,20,32,33,34,35,36});
                //If extreme N, set Stereographic
                if((upperLeftY + height/2) < OUTER)
                {
                    //Shape Extreme North
                    projType = "Stereographic"; //0,3,10,20,32
                    RID=0; //Stereographic category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,10,20,32});
                    DSSUpdater.setHighLight(32);
                }
                //If Mid N, set Lambert
                else if((upperLeftY + height/2) < MIDUP)
                {
                    //Shape Mid North
                    projType = "Lambert Conformal Conic";//0,3,10,20,33
                    RID=1; //LambConfCon category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,10,20,33});
                    DSSUpdater.setHighLight(33);
                }
                //If Mid, set Mercator
                else if((upperLeftY + height/2) < MID)
                {
                    //Shape Mid
                    projType = "Mercator";//0,3,10,20,34
                    RID=2; //Mercator category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,10,20,34});
                    DSSUpdater.setHighLight(34);
                }
                //If Mid S, set Lambert
                else if((upperLeftY + height/2) < MIDDN)
                {
                    //Shape Mid South//0,3,10,20,35
                    projType = "Lambert Conformal Conic";
                    RID=1; //LambConfCon category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,10,20,35});
                    DSSUpdater.setHighLight(35);
                }
                //If extreme S, set Stereographic
                else 
                {
                    //Shape Extreme South
                    projType = "Stereographic";//0,3,10,20,36
                    RID=0; //Stereographic category used in formAndCallURL
                    /*DSSUpdater.setHighPathAr(new int[]{0,2,6,22});
                    DSSUpdater.setHighLight(22);/**/
                    DSSUpdater.setHighPathAr(new int[]{0,3,10,20,36});
                    DSSUpdater.setHighLight(36);
                }
            }
            else if(preserve == "Area")
            {
                DSSUpdater.setVisAr(new int[] {0,3,10,19,20,27,28,29,30,31});
                //If extreme N, set Lambert Azimuthal
                if((upperLeftY + height/2) < OUTER)
                {
                    //Area Extreme North
                    projType = "Lambert Azimuthal"; //0,3,10,19,27
                    RID=3; //Stereographic category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,10,19,27});
                    DSSUpdater.setHighLight(27);
                }
                //If Mid N, set Albers
                else if((upperLeftY + height/2) < MIDUP)
                {
                    //Area Mid North
                    projType = "Albers";          //0,3,10,19,28
                    RID=4; //Albers category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,10,19,28});
                    DSSUpdater.setHighLight(28);
                }
                //If Mid, set Any EqArea or CylinEqArea
                else if((upperLeftY + height/2) < MID)
                {
                    //Area Mid                  //0,3,10,19,29
                    projType = "Any Equal Area or Cylindrical Equal Area";
                    RID=5; //AnyEq category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,10,19,29});
                    DSSUpdater.setHighLight(29);
                }
                //If Mid S, set Albers
                else if((upperLeftY + height/2) < MIDDN)
                {
                    //Area Mid South
                    projType = "Albers";          //0,3,10,19,30
                    RID=4; //Albers category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,10,19,30});
                    DSSUpdater.setHighLight(30);
                }
                //If extreme S, set Lambert Azimuthal
                else 
                {
                    //Area Extreme South
                    projType = "Lambert Azimuthal";   //0,3,10,19,31
                    RID=3; //LambAz category used in formAndCallURL
                    /*DSSUpdater.setHighPathAr(new int[]{0,2,5,17});
                    DSSUpdater.setHighLight(17);/**/
                    DSSUpdater.setHighPathAr(new int[]{0,3,10,19,31});
                    DSSUpdater.setHighLight(31);/**/
                }
            }
        }
        
        //If height > width
        //North South node 1
        else
        {
            if(preserve == "Shape")
            {
                //If extreme N, set Stereographic
                DSSUpdater.setVisAr(new int[] {0,3,9,17,18,24,25,26});
                if((upperLeftY + height/2) < OUTER)
                {
                    projType = "Stereographic"; //0,3,9,18,24
                    RID=0; //Stereographic category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,9,18,24});
                    DSSUpdater.setHighLight(24);
                }
                //If Mid N-Mid S, set Transverse Mercator
                else if((upperLeftY + height/2) < MIDDN)
                {
                    projType = "Transverse Mercator";//0,3,9,18,24
                    RID=6; //TransMerc category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,9,18,25});
                    DSSUpdater.setHighLight(25);
                }
                //If extreme S, set Stereographic
                else
                {
                    projType = "Stereographic";	//0,3,9,18,24
                    RID=0; //Stereographic category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,9,18,26});
                    DSSUpdater.setHighLight(26);
                }
            }
            else if(preserve == "Area")
            {
                DSSUpdater.setVisAr(new int[] {0,3,9,17,18,21,22,23});
                //If extreme N, set Lambert Azimuthal
                if((upperLeftY + height/2) < OUTER)
                {
                    projType = "Lambert Azimuthal";//0,3,9,17,21
                    RID=3; //LambAz category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,9,17,21});
                    DSSUpdater.setHighLight(21);
                }
                //If Mid N-Mid S, set CylinEqArea
                else if((upperLeftY + height/2) < MIDDN)
                {
                    projType = "Cylindrical Equal Area";//0,3,9,17,21
                    RID=7; //CylEqArea category used in formAndCallURL
                    DSSUpdater.setHighPathAr(new int[]{0,3,9,17,22});
                    DSSUpdater.setHighLight(22);
                }
                //If extreme S, set Lambert Azimuthal
                else
                {
                    projType = "Lambert Azimuthal";	//0,3,9,17,21
                    RID=3; //LambAz category used in formAndCallURL
                    //int[] a = new int[]{0,1,3,10};
                    /*DSSUpdater.setHighPathAr(new int[]{0,1,3,9});
                    DSSUpdater.setHighLight(9);*/
                    DSSUpdater.setHighPathAr(new int[]{0,3,9,17,23});
                    DSSUpdater.setHighLight(23);/**/
                }
            }
        }
        ////ending here
        
        //set color black and draw projType string
        Color temp;
        temp = g.getColor();
        tl = new TextLayout(projType,font,frc);
        g.setColor(Color.blue);
        g.drawLine((int)(360-(tl.getBounds().getWidth()/2)),393,(int)(360+(tl.getBounds().getWidth()/2)),393);
        tl.draw(tempg,(float)(360-(tl.getBounds().getWidth()/2)),(float)390);
        textlink = new Rectangle2D.Double((double)360-((tl.getBounds().getWidth()+24)/2-5),
          (double)(390-14),(double)(tl.getBounds().getWidth()+10),tl.getBounds().getHeight()+10);
        g.setColor(temp);
    }	   
    
    /**
     * Added for linking to actual page description.  Each projection type got 
     * its own URLid (the RID).
     */
    public void formAndCallURL(int URLid)//location
    {  
      String location=null;
      
      if(URLid == 0) //Stereograhic projection
        {
            //Get location from html file
            location = getParameter("location0");
        }
        
        if(URLid == 1) //Lamber Conformal Conic
        {          
            //Get location from html file
            location = getParameter("location1");
        }
        
        if(URLid == 2) //Mercator
        {
            //Get location from html file
            location = getParameter("location2");
        }
        
        if(URLid == 3) //Lambert Azimuthal Equal Area
        {                        
            //Get location from html file
            location = getParameter("location3");
        }
        
        if(URLid == 4) //Albers Equal Area Conic
        {                         
            //Get location from html file
            location = getParameter("location4");
        }
        
        if(URLid == 5) //Any Equal Area or Cylindrical Equal Area
        {                          
            //Get location from html file
            location = getParameter("location5");
        }
        
        if(URLid == 6) //Transverse Mercator
        {                          
            //Get location from html file
            location = getParameter("location6");
        }
        
        if(URLid == 7) //Any Cylindircal Equal Area
        {                          
            //Get location from html file
            location = getParameter("location7");
        }
        
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
     
     /**
      * end of addition
      */

    // Variables declaration - do not modify//GEN-BEGIN:variables
    // End of variables declaration//GEN-END:variables
    private Image image;                        //Map Image
    
    public Rectangle2D.Double textlink;
    
    private int upperLeftX, upperLeftY;         //Upper Left X and Y of box
    private int lowerRightX, lowerRightY;       //Lower Right X and Y of box
    private int height, width;                  //Height and Width of box
    private int currentX, currentY;             //Current X and Y
    private int extent;                         //Greater E/W or N/S
    
    private String projType;                    //Projection Type
    
    private Image offScreenImage;
    private Dimension offScreenSize;
    private Graphics offScreenGraphics;
    
    public Font font;                          //Font for projType display
    
    private String preserve, dataType, rasterData;  //Selected List Item
    private JPanel choicePanel;                     //Panel for Lists
    private List preserveList, dataTypeList, rasterDataList;    //Lists
    
    private int selected;                       //Currently selected index
    
    private JLabel rasterDataLabel;             //Labels for the Lists
    private JLabel preserveLabel;
    private JLabel dataTypeLabel;

    private final int STARTupperLeftX = 320;    //Start X point of Box
    private final int STARTupperLeftY = 140;    //Start Y point of Box

    private final int STARTHW = 20;             //Start H and W of Box
    
    private final int DIST = 3;                 //Distance from box edge
                                                //allowed for selection
    
    private final int MAXX = 720;               //Max X value
    private final int MAXY = 360;               //Max Y value
						//Therefore one cell = 0.5 degrees lat/long
    
    private final int MAXBOXX = 90;             //Max X value of box
    private final int MAXBOXY = 75;             //Max Y value of box
    
/** 		Old Values

    private final int OUTER = 39;               //High Latitudes
    private final int MIDUP = 145;              //Mid N Latitudes
    private final int MID = 223;                //Mid Latitudes
    private final int MIDDN = 329;              //Mid S Latitudes
*/  

/* Changed April 24, 2013 by bpassuello */
    private final int OUTER = 58;               //High Latitudes 	61 - 90 N
    private final int MIDUP = 128;              //Mid N Latitudes 	25.5 - 60.5 N
    private final int MID = 231;                //Mid Latitudes 	25 N - 25 S
    private final int MIDDN = 301;              //Mid S Latitudes 	25.5 - 60.5 S
						//Else 			61 - 90 S
 /** public void init()
  {
    //DSSUpdater.setHighLight(HighLight);//.setTicket(ticket);
    addMouseMotionListener(this);
    addMouseListener(this);
    //selection = false;
  }**/

  public void start()   //starts a new Thread if there isn't one already
  {
    if (runner == null)
    {
      runner = new Thread(this);
      runner.start();
    }
  }

  public void stop()  //stops a thread if there has been one started
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
  }//End of Threading Code WORKS YEAH!!!!!/**/
}
