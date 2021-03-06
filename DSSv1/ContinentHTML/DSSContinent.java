/*
 * DSSContinent.java
 *
 * Created on June 21, 2002, 8:54 AM
 */

/**
 *
 * @author  sposch
 */
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.applet.Applet;
import java.net.*;
import java.applet.AppletContext;

import javax.swing.*;

public class DSSContinent extends javax.swing.JApplet implements ItemListener {

    /** Creates new form DSSContinent */
    public DSSContinent() {
        initComponents();
        
        Container container = getContentPane();
        
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
        
        addMouseMotionListener(new MouseMotionAdapter()
        {
            //Listen for mouse movement
            public void mouseMoved(MouseEvent e)
            {
                //Get content pane
                Container c = getContentPane();
                
                //Get current x and y value of cursor
                int currentX = e.getX();
                int currentY = e.getY();
                
                //If on a continent, set hand cursor
                if((naRect1.contains(currentX, currentY)) ||
                   (naRect2.contains(currentX, currentY)) ||
                   (saRect.contains(currentX, currentY))  ||
                   (euRect.contains(currentX, currentY))  ||
                   (afRect1.contains(currentX, currentY)) ||
                   (afRect2.contains(currentX, currentY)) ||
                   (asRect1.contains(currentX, currentY)) ||
                   (asRect2.contains(currentX, currentY)) ||
                   (auRect.contains(currentX, currentY))  ||
                   (npRect1.contains(currentX, currentY)) ||
                   (npRect2.contains(currentX, currentY)) ||
                   (anRect.contains(currentX, currentY)))
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                       Cursor.HAND_CURSOR));
                }
                
                //If not on a continent, set default cursor
                else
                {
                    c.setCursor(Cursor.getPredefinedCursor(
                                       Cursor.DEFAULT_CURSOR));
                }
            }
        }
        );
        
        
        ///////////////////////////////NOTICE///////////////////////////////
        //This function will need to be redeveloped when distinguishing   //
        //characteristics between raster and vector data are developed    //
        ////////////////////////////////////////////////////////////////////
        addMouseListener(new MouseAdapter()
        {
            //Listen for mouse click
            public void mouseClicked(MouseEvent e)
            {
                //Get current x and y of cursor
                int currentX = e.getX();
                int currentY = e.getY();
                
                //Check for click in Northern Polar Region or Antarctica
                if((anRect.contains(currentX, currentY))  ||
                   (npRect1.contains(currentX, currentY)) ||
                   (npRect2.contains(currentX, currentY)))
                {
                    //If preserving shape, goto shape web page 
                    if(preserve == "Shape")
                    {
                        String location;
            
                        //Get location from html file
                        location = getParameter("location5");
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(location);
                           
                            AppletContext browser = getAppletContext();
            
                            browser.showDocument(url);
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
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(location);
                           
                            AppletContext browser = getAppletContext();
            
                            browser.showDocument(url);
                        }
            
                        //Handle malformed url exception in connection attempt
                        catch(MalformedURLException urlException)
                        {
                            urlException.printStackTrace();
                        }
                    }
                }
                
                //Check for click in North America, South America, or Africa
                if((naRect1.contains(currentX, currentY)) ||
                   (naRect2.contains(currentX, currentY)) ||
                   (saRect.contains(currentX, currentY))  ||
                   (afRect1.contains(currentX, currentY)) ||
                   (afRect2.contains(currentX, currentY)))
                {
                    //If preserving shape, goto shape web page
                    if(preserve == "Shape")
                    {
                        String location;
            
                        //Get location from html file
                        location = getParameter("location3");
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(location);
                           
                            AppletContext browser = getAppletContext();
            
                            browser.showDocument(url);
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
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(location);
                           
                            AppletContext browser = getAppletContext();
            
                            browser.showDocument(url);
                        }
            
                        //Handle malformed url exception in connection attempt
                        catch(MalformedURLException urlException)
                        {
                            urlException.printStackTrace();
                        }
                    }
                }
                
                //Check for click in Asia, Europe, or Australia
                if((asRect1.contains(currentX, currentY)) ||
                   (asRect2.contains(currentX, currentY)) ||
                   (euRect.contains(currentX, currentY))  ||
                   (auRect.contains(currentX, currentY)))
                {
                    //If preserving shape, goto shape web page
                    if(preserve == "Shape")
                    {
                        String location;
            
                        //Get location from html file
                        location = getParameter("location1");
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(location);
                           
                            AppletContext browser = getAppletContext();
            
                            browser.showDocument(url);
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
            
                        //Try to make connection to new web page
                        try
                        {
                            URL url;
                            url = new URL(location);
                           
                            AppletContext browser = getAppletContext();
            
                            browser.showDocument(url);
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
        );
    }
    
    

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    private void initComponents() {//GEN-BEGIN:initComponents

    }//GEN-END:initComponents

    public void itemStateChanged(java.awt.event.ItemEvent itemEvent) 
    {
        //Get current list values selected
        preserve = preserveList.getSelectedItem();
        dataType = dataTypeList.getSelectedItem();
	rasterData = rasterDataList.getSelectedItem();
        
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
        super.paint(g);
        g.drawImage(image, 0, 0, MAXX, MAXY, this);
    }
    
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
}
