import java.awt.*;
public class DSSUpWrite extends java.applet.Applet implements Runnable
{
    private int ticket = 1;
    private int setMax = 9;
    Thread runner;

  public void init()
  {
    DSSUpdater.setHighLight(ticket);//.setTicket(ticket);
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

      if (++ticket >= setMax)
      {
        ticket = 1;
      }

      DSSUpdater.setHighLight(ticket);
      if(++ticket == 4 ||ticket == 5 ||ticket == 6 ||ticket == 7 ||ticket == 8 ||ticket == 9)
      {
        DSSUpdater.setHighPath(ticket);
      }
      repaint();
      pause(1000);
      }
  }

  public void pause(int time)
  {
    try
    {
      Thread.sleep(time);
    }
    catch (InterruptedException e) {}
  }
  public void paint(Graphics g)
                {
                Font font = new Font("Times Roman", Font.PLAIN, 20);
                FontMetrics fm = getFontMetrics(font);

                g.setFont(font);

                String s = "Node Number is Now " + ticket;
                String s2 = "Path is Now" + ticket;

                int x = (this.size().width - fm.stringWidth(s)) / 2;
                int y = (this.size().height - fm.getHeight()) / 2;

                g.drawString(s, x, y);
                g.drawString(s2,x,y+25);
                }
/*{
  public DSSUpWrite()
  {
  }/**/
}