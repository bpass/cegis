import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.applet.Applet;

public class regional extends Applet implements ItemListener
{
    private Image mImage;
    private int ulx,uly,w,h,curx,cury,lrx,lry,wide,extent;
    private Rectangle rect;
    private String projtype;
    private Image offScreenImage; 
    private Dimension offScreenSize;
    private Graphics offScreenGraphics;
    private Font theFont;
    private String preserve,datatype,rasterdata;
    private Panel choicePanel;
    private List preserveList, dataTypeList, rasterDataList;
    private Label rasterDataLabel;
    private int selected;

  
    public void init() 
    {	
        /*  Don't think this will work */
	 /* preserve = getParameter("first");
	  datatype = getParameter("second");
	  rasterdata = getParameter("third");
      */
	  
	  // The image to be displayed to the user.
	  mImage = getImage(getDocumentBase(),"PlateCarree.gif");

	  // Starting projection type
	  projtype = "Mercator";
	  preserve = "Shape";
	  datatype = "Raster";
	  rasterdata = "Continuous";

	  // Bounding box position
	  ulx = 320;          
	  uly = 140;
	  w = 20;
	  h = 20;	  
	  lrx=ulx+w;
	  lry=uly+h;
	  wide = 0;
	  extent = 0;
	  setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
	  theFont = new Font("Times New Roman",Font.BOLD,16);
  	  setLayout(new BorderLayout());

	  preserveList = new List(2);  // was 3 for direction
	  preserveList.add("Shape");
	  preserveList.add("Area");
	  // preserveList.add("Direction");
	  preserveList.select(0);
	  preserveList.addItemListener(this);
	  
	  dataTypeList = new List(2);
	  dataTypeList.add("Raster");
	  dataTypeList.add("Vector");
	  dataTypeList.select(0);
	  dataTypeList.addItemListener(this);

	  rasterDataList = new List(2);
        rasterDataList.add("Continuous");
	  rasterDataList.add("Thematic");
	  rasterDataList.select(0);
	  rasterDataList.addItemListener(this);

	  rasterDataLabel = new Label("  Raster Data Type:");
	  
	  choicePanel = new Panel();
	  choicePanel.add(new Label("Preserve:"));
	  choicePanel.add(preserveList);
	  choicePanel.add(new Label("  Data Type:"));
	  choicePanel.add(dataTypeList);
	  choicePanel.add(rasterDataLabel);
	  choicePanel.add(rasterDataList);
	  add(choicePanel,BorderLayout.SOUTH);
	  
	  addMouseMotionListener ( new MouseMotionAdapter()
	  { 
		// Watch to see if you need to change the cursor type.
	      public void mouseMoved(MouseEvent e) 
		{
		    curx=e.getX();
     		    cury=e.getY();
		    if ( (curx <= ulx+3 ) && (curx >= ulx-3) && (cury <= uly+3) && (cury>=uly-3) )
		    {
		         setCursor(Cursor.getPredefinedCursor(Cursor.NW_RESIZE_CURSOR));
			   repaint(); 
      	    }
		    else if( (curx <= (ulx+w+3)) && (curx>=(ulx+w-3)) && (cury <= (uly+h+3)) && (cury>=(uly+h-3)) )
		    {
		         setCursor(Cursor.getPredefinedCursor(Cursor.SE_RESIZE_CURSOR));
			   repaint();
		    }
		    else if( (curx <= ulx+3) && (curx>=ulx-3) && (cury <= (uly+h+3)) && (cury>=(uly+h-3)))
		    {
			   setCursor(Cursor.getPredefinedCursor(Cursor.SW_RESIZE_CURSOR));
			   repaint();
		    }
		    else if( (curx<=(ulx+w+3)) && (curx>=(ulx+w-3)) && (cury <= uly+3) && cury>=uly-3)
		    {
			   setCursor(Cursor.getPredefinedCursor(Cursor.NE_RESIZE_CURSOR));
			   repaint();
		    }
		    else if( (curx <= ulx+3) && (curx>=ulx-3) && (cury > uly) && (cury < (uly+h)) )
		    {
			   setCursor(Cursor.getPredefinedCursor(Cursor.W_RESIZE_CURSOR));
			   repaint();
		    }	
		    else if( (cury <= uly+3) && (cury>=uly-3) && (curx > ulx) && (curx < (ulx+w)) )
		    {
			   setCursor(Cursor.getPredefinedCursor(Cursor.N_RESIZE_CURSOR));
			   repaint();
		    }
		    else if( (curx <= ulx+w+3) && (curx>=ulx+w-3)&& (cury > uly) && (cury < (uly+h)) )
		    {
			   setCursor(Cursor.getPredefinedCursor(Cursor.E_RESIZE_CURSOR));
			   repaint();
		    }
		    else if( (cury <= uly+h+3) && (cury>=uly+h-3) && (curx > ulx) && (curx < (ulx+w)) )
		    {
			   setCursor(Cursor.getPredefinedCursor(Cursor.S_RESIZE_CURSOR));
			   repaint();
		    }
		    else if( (curx < ulx+w) && (curx > ulx) && (cury > uly) && (cury < uly+h) )
		    {
			   setCursor(Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR));
			   repaint();
		    }
		    else if(getCursor().getType() !=0 )
		    {
			   setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
			   repaint();
		    }
		    
 		}	
		
		// See if user is moving or changing bounding box.
		// Maintains a minimum height/width of 20.
		public void mouseDragged(MouseEvent e)
		{
		    curx = e.getX();
		    cury = e.getY();
		    if(curx >= 720)
			curx = 720;
		    else if(curx <= 0)
			curx = 0;
		    if(cury >= 360)
			cury = 360;
		    else if(cury <=0)
			cury = 0;
		    if( getCursor().getType() == 13)            // Inside box
		    {
			   ulx = curx - (w/2);
			   lrx = curx + w - (w/2);
			   uly = cury - (h/2);
			   lry = cury + h - (h/2);
		    }
		    else if(getCursor().getType() == 4 )		// Bottom left corner
		    {
			   if(lrx - curx >= 20) ulx = curx;			   
			   if(cury - uly >= 20) lry = cury;
		    }
		    else if(getCursor().getType() == 5 )		// Bottom right corner
		    {
			   if(curx - ulx >= 20) lrx = curx;
			   if(cury - uly >= 20) lry = cury;
		    }
		    else if(getCursor().getType() == 6 )		// Top left corner
		    {
			   if(lrx - curx >= 20) ulx = curx;
			   if(lry - cury >= 20) uly = cury;
		    }
		    else if(getCursor().getType() == 7 )		// Top right corner
		    {
			   if(curx - ulx >= 20) lrx = curx;
			   if(lry - cury >= 20) uly = cury;
		    }
		    else if(getCursor().getType() == 8 )		// Top
		    {
			   if(lry - cury >= 20) uly = cury;
		    }
		    else if(getCursor().getType() == 9 )		// Bottom
		    {
			   if(cury - uly >= 20) lry = cury;
		    }
		    else if(getCursor().getType() == 10 )		// Left
		    {
			   if(lrx - curx >= 20) ulx = curx;
		    }
		    else if(getCursor().getType() == 11 )		// Right
		    {
			   if(curx - ulx >= 20) lrx = curx;
		    }
		    
		    w = lrx - ulx;
		    h = lry - uly;		    
		    repaint();
		}	
	 }
       );

       //Was using this, decided not to.  May be handy in future.
       addMouseListener(  new MouseAdapter()
	 {
	     public void mousePressed(MouseEvent e) 
	     {              
    	     } 	     

       }
	 );

        
  } 

  public void itemStateChanged(ItemEvent ie)
  {
	preserve = preserveList.getSelectedItem();
	datatype = dataTypeList.getSelectedItem();
	rasterdata = rasterDataList.getSelectedItem();

	if(ie.getSource() == dataTypeList)
	{
      	if(datatype=="Vector")
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
  
  /*********************************************************/
  // Keeps the page from flickering like mad.
  // *** DO NOT DELETE THIS SECTION *** //
  public final synchronized void update (Graphics g) 
  {
	Dimension d = getSize();
	if((offScreenImage == null) || (d.width != offScreenSize.width) || (d.height != offScreenSize.height))
	{
	    offScreenImage = createImage(d.width, d.height);
	    offScreenSize = d;
	    offScreenGraphics = offScreenImage.getGraphics();
	}
	offScreenGraphics.clearRect(0,0,d.width, d.height);
	paint(offScreenGraphics);
	g.drawImage(offScreenImage,0,0,null);
  }
  /**********************************************************/
  /**********************************************************/

  public void paint(Graphics g)
  {
	  g.drawImage(mImage,0,0,719,369,this);
	  g.setFont(theFont);
	  if(w>=h)
	  	extent=0;
	  else
		extent=1;
	  
	  // This doesn't work.  Leave for now just in case.
	  /* g.drawString("Preserve: ",60,400);
	  if(preserve.equals("1"))
		g.drawString("Shape",130,400);
	  else if( preserve.equals("2"))
		g.drawString("Area",130,400);
	  else if( preserve.equals("3"))
		g.drawString("Direction",130,400); 
	  else if( preserve.equals("4"))
		g.drawString("Distance",130,400);
	  else if( preserve.equals("5"))
		g.drawString("Compromise",130,400);

	  g.drawString("Data Type: ",270,400);
	  if( datatype.equals("1"))
		g.drawString("Raster",350,400);
	  else if( datatype.equals("2"))
		g.drawString("Vector",350,400);

	  g.drawString("Raster Data:",470,400);
	  if( datatype.equals("1"))
		g.drawString("Continuous",560,400);
	  else if( datatype.equals("2"))
		g.drawString("Thematic",560,400);
	  */

	  g.setColor(Color.blue);
	  g.drawRect(ulx,uly,w,h);
  
        /*****************************************************************************************/
        /*															*/
	  /*  Put if/else or switch or for or whatever here to look up projection			*/
 	  /*															*/
	  /*****************************************************************************************/ 
	
	  /*if(preserve == "Direction")
	  {
	      projtype = "Gnomonic";
	  }
	  else */

	  if(extent==0)           // width is >= height  (E-W extent)
	  {
	  	if(preserve == "Shape")
		{
		    if( (uly + h/2) < 39 )
	 	        projtype = "Stereographic";
    		    else if( (uly + h/2) < 145 )
		        projtype = "Lambert Conformal Conic";
	  	    else if( (uly + h/2) < 223 )
		        projtype = "Mercator";
	  	    else if( (uly + h/2) < 329 )
		        projtype = "Lambert Conformal Conic";
		    else 
		        projtype = "Stereographic";
		}
		else if(preserve == "Area")
		{
		    if( (uly + h/2) < 39 )
	 	        projtype = "Lambert Azimuthal";
    		    else if( (uly + h/2) < 145 )
		        projtype = "Albers";
	  	    else if( (uly + h/2) < 223 )
		        projtype = "Any Equal Area or Cylindrical Equal Area";
	  	    else if( (uly + h/2) < 329 )
		        projtype = "Albers";
		    else 
		        projtype = "Lambert Azimuthal";
		}
	  }
	  else                      // height > width  (N-S extent)
	  {
		if(preserve == "Shape")
		{
		    if( (uly + h/2) < 39 )
	 	        projtype = "Stereographic";
		    else if( (uly + h/2) < 329 )
		        projtype = "Transverse Mercator";
		    else
		        projtype = "Stereographic";			    
		}
		else if(preserve == "Area")
		{
		    if( (uly + h/2) < 39 )
	 	        projtype = "Lambert Azimuthal";
		    else if( (uly + h/2) < 329 )
		        projtype = "Cylindrical Equal Area";
		    else
		        projtype = "Lambert Azimuthal";			
		}
	  }
	  g.setColor(Color.black);
	  g.drawString(projtype,360-(projtype.length()*3),390);
   }	   
}