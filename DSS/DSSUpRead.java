
public class DSSUpRead implements Runnable
{
  /*public DSSUpRead()
  {
  }/**/
  Thread runner;
  //String ticketFace = "Waiting to get ticket\n";

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
      try
      {
        Thread.sleep(1000);
      }
      catch (InterruptedException e) {}

      /*if (Ticket.getTicket() != 0)
      {
          ticketFace = "The ticket is now " + 
          Ticket.getTicket();
      }*/
      if(DSSUpdater.getHighLight() >= 0)
      {
        DSSUpdater.getHighLight();
      }
        //repaint();
    }
  }

}