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

public class GraphContext
{
  public FontRenderContext FRC;
  public Font F;
  public Graphics2D theG;
  public int fontSiz = 12;
  
  public GraphContext(Graphics tG)
  {
    theG = (Graphics2D)tG;
    FRC = theG.getFontRenderContext();
    F = new Font("Dialog", Font.PLAIN,fontSiz);
    RenderingHints qualityHints;
    qualityHints = new RenderingHints(RenderingHints.KEY_ANTIALIASING,
                        RenderingHints.VALUE_ANTIALIAS_ON); 
    qualityHints.put(RenderingHints.KEY_RENDERING,
                    RenderingHints.VALUE_RENDER_QUALITY); 
    theG.setRenderingHints(qualityHints);
  }
  
  public GraphContext(FontRenderContext fr,Font font, Graphics2D tG )
  {
    FRC = fr;
    F = font;
    theG = tG;
  }
  public GraphContext(GraphContext cpy)
  {
    FRC = cpy.FRC;
    F = cpy.F;
    theG = cpy.theG;
  }
 /** public Graphics2D getG(){return theG;}
  public FontRenderContext getFRC(){return FRC;}
  public Font getFont(){return F;}**/
}