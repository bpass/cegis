/**
 * GraphPallet contains the color scheme information of the Graph class for 
 * rendering
 * @author James Nelson
 *  
 */
import java.awt.Color;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
public class GraphPallet 
{
  public Color nodeDF;
  public Color textC;
  public Color nodeFill;
  public Color edgeC;
  public Color borderC;
  public RoundGradientPaint highlightC;
  
  public GraphPallet()
  {
    //setup defaults
    textC = new Color(Color.black.getRGB());
    nodeFill = new Color(204,204,255);
    nodeDF = new Color(204,204,255);
    edgeC = new Color(Color.black.getRGB());
    borderC = new Color(Color.black.getRGB());
    highlightC = new RoundGradientPaint(1
              ,1, Color.yellow,
          new Point2D.Double(0.0, 2/2), Color.green.darker().darker());    
  }
  
  public GraphPallet(Color T,Color N, Color E, Color B, RoundGradientPaint H)
  {
    textC = new Color(T.getRGB());
    nodeFill = new Color(N.getRGB());
    edgeC = new Color(E.getRGB());
    borderC = new Color(B.getRGB());
    highlightC = new RoundGradientPaint(H.mPoint.getX(),H.mPoint.getY(),
                H.mPointColor,H.mRadius,H.mBackgroundColor);
    nodeDF = new Color(204,204,255);
    //this rules
  }
  
  public void setHighLight(Point2D.Double p, Point2D.Double RorW)
  {
    highlightC.setRadius(RorW.getY()-RorW.getX());
    highlightC.setCenter(p.getX(),p.getY());
  }
  
  public void setHighLight(Rectangle2D.Double shape)
  {
    highlightC.setRadius(shape.getWidth());
    highlightC.setCenter(shape.getCenterX(),shape.getCenterY());
  }
  
  public void changeHighC(RoundGradientPaint newP)
  {
    highlightC = new RoundGradientPaint(newP.mPoint.getX(),newP.mPoint.getY(),
                newP.mPointColor,newP.mRadius,newP.mBackgroundColor); 
  }
  
  public void changeHighC(Color p, Color b)
  {
    highlightC.setColorPoint(p);
    highlightC.setColorBackG(b);
  }
  
}