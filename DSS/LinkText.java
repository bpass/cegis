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
import java.applet.Applet;
import java.applet.AppletContext;
import java.awt.Component;
import java.awt.geom.Rectangle2D;
import java.net.*;
public class LinkText 
{
  public URL theLink;
  public String tLnk;
  public String theText;
  public Rectangle2D.Double tTextSpc;
  public LinkText(String tURL, String tText)
  {
    tLnk = tURL;
    theText = tText;    
  }
  
  /**
   * Handle link open use folowing code
   */
   /*try
        {
        //URL theUrl;
        theLink = new URL(tLink);//"http://www.google.com/"
        AppletContext browser = getAppletContext();
        browser.showDocument(theLink,"_blank"); //_blank opens new window
        }
       catch(MalformedURLException urlException)
            {
                urlException.printStackTrace();
            }//this Works*/
            
    public void setTextSpace(Rectangle2D tTS)
    {
      tTextSpc = new Rectangle2D.Double();
      tTextSpc.setRect(tTS);
    }
            
    public void exeLink(AppletContext browser)
    {
      try
      {
        //URL theUrl;
        theLink = new URL(tLnk);//"http://www.google.com/"
        //AppletContext browser = t.getAppletContext();
        browser.showDocument(theLink,"_blank"); //_blank opens new window
      }
      catch(MalformedURLException urlException)
      {
        urlException.printStackTrace();
      }      
    }
}

