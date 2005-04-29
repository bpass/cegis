/*
 * $Id: DSSApplet.java,v 1.2 2005/04/29 21:00:11 jenelson Exp $
 * DSSApplet.java
 *
 * Modified for relative addresing 6/2004
 * by James Nelson
 * Current version is 100% portable
 */

/**
 *
 * @author  sposch
 * Modified 4/19/2004 jtrent  add recommendation explainations
 */
import java.awt.*;
import java.net.*;
import java.lang.*;
import java.util.*;
import java.applet.AppletContext;

import javax.swing.*;

public class DSSApplet extends javax.swing.JApplet implements Runnable {

  //Merger variables
  private int HighLight = 0;
  private int HighPath = 0;
  private int Gname = 0;
  private int setMax = 0;
  public int[] HighPathAr;
  boolean changed = false;
  Thread runner;
  //Merger variables

    private URL Appbase;
    /** Creates new form DSSApplet */
    public DSSApplet() {
        initComponents();
        start();
        
        //Disable all unusable gui items
        preserveWhat.setEnabled(false);
        dataType.setEnabled(false);
        rasterDataType.setEnabled(false);
        submitButton.setEnabled(false);
        
        
        //Set background of applet to white
        getContentPane().setBackground(Color.white);
    
       
        
        //set applet visible
        this.setVisible(true);
    }

    /////////////////////////////////////////////////////////////////////
    //The following code was generated by Forte for Java CE 3.0        //
    /////////////////////////////////////////////////////////////////////
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    private void initComponents() {//GEN-BEGIN:initComponents
        mapType = new javax.swing.JComboBox();
        preserveWhat = new javax.swing.JComboBox();
        dataType = new javax.swing.JComboBox();
        rasterDataType = new javax.swing.JComboBox();
        submitButton = new javax.swing.JButton();
        //DSSUpdater.setGraphName(0);                
        //DSSUpdater.setHighLight(0);        
        //DSSUpdater.setHighPath(0);
        HighPathAr = new int[]{0};
        
        getContentPane().setLayout(new java.awt.GridLayout(5, 0, 0, 20));
        
        mapType.setBackground(java.awt.Color.white);
        mapType.setMaximumRowCount(5);
        mapType.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Select Type of Area to Map", "----------", "Global", "Continental", "Regional" }));
        mapType.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                mapTypeActionPerformed(evt);
            }
        });
        
        
        getContentPane().add(mapType);
        
        preserveWhat.setBackground(java.awt.Color.white);
        preserveWhat.setMaximumRowCount(5);
        preserveWhat.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Select What to Preserve", "----------", "Area", "Shape", "Compromise" }));//2,3,4
        preserveWhat.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                preserveWhatActionPerformed(evt);
            }
        });
        
        getContentPane().add(preserveWhat);
        
        dataType.setBackground(java.awt.Color.white);
        dataType.setMaximumRowCount(4);
        dataType.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Select Type of Data", "----------", "Raster", "Vector" }));
        dataType.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                dataTypeActionPerformed(evt);
            }
        });
        
        getContentPane().add(dataType);
        
        rasterDataType.setBackground(java.awt.Color.white);
        rasterDataType.setMaximumRowCount(4);
        rasterDataType.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Select Type of Raster Data", "----------", "Continuous", "Thematic" }));
        rasterDataType.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rasterDataTypeActionPerformed(evt);
            }
        });
        
        getContentPane().add(rasterDataType);
        submitButton.setBackground(java.awt.Color.lightGray);
        submitButton.setText("Submit");
        submitButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                submitButtonActionPerformed(evt);
            }
        });
        
        getContentPane().add(submitButton);
        
        
    }//GEN-END:initComponents
    /////////////////////////////////////////////////////////////////////
    //END OF GENERATED CODE                                            //
    /////////////////////////////////////////////////////////////////////
    
    
    private void submitButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_submitButtonActionPerformed

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
        /////////////////////////////////////////////////
        /*try
        {
        this.Appbase = new URL(this.getCodeBase(),"./"); 
        }
        catch(MalformedURLException urlException)
        {
            urlException.printStackTrace();
        }/**/

        ////////////////////////////////////////////////
        //Get index of selected item in preserve box
        getGlassPane().getGraphics().drawString("clicked (101,101)", 101,101);
        
        int i = preserveWhat.getSelectedIndex();
        
        //If area being preserved, goto area page
        if(i == 2)
        {
            String location;
            
            //Get location from html file
            location = getParameter("location2");
            //INSERTTUT

            //Try to make connection to new web page
            try
            {
                URL url;
                url =new URL(this.getCodeBase(),location);

                AppletContext browser = getAppletContext();

                browser.showDocument(url,"_blank");
            }

            //Handle malformed url exception in connection attempt
            catch(MalformedURLException urlException)
            {
                urlException.printStackTrace();
            }
        }
        
        //If shape being preserved, goto shape page
        if(i == 3)
        {
            String location;
            //INSERTTUT
            
            //Get location from html file
            location = getParameter("location3");

            //Try to make connection to new web page
            try
            {
                URL url;
                url =new URL(this.getCodeBase(),location);
                //"this.Appbase" added 6/2004 for portability

                AppletContext browser = getAppletContext();

                browser.showDocument(url,"_blank");
            }

            //Handle malformed url exception in connection attempt
            catch(MalformedURLException urlException)
            {
                urlException.printStackTrace();
            }
        }

        //If compromise, goto compromise page
        if(i == 4)
        {
            String location;
            //INSERTTUT

            //Get location from html file
            location = getParameter("location4");
            /*String location;
            Gname = 2;//let 3 generate Continental Visualization
             changed  =true;
            //Get location from html file
            location = getParameter("location1");
            DSSUpdater.setGraphName(2);
            
            //Try to make connection to new web page
            try
            {
                URL url;
                //url =new URL(this.Appbase,location);
                url = new URL(this.getCodeBase(),location);
                //"this.Appbase" added 6/2004 for portability
                           
                AppletContext browser = getAppletContext();
            
                browser.showDocument(url);
            }*/

            //Try to make connection to new web page
            try
            {
                URL url;
                url =new URL(this.getCodeBase(),location);
                //"this.Appbase" added 6/2004 for portability

                AppletContext browser = getAppletContext();

                browser.showDocument(url,"_blank");
            }

            //Handle malformed url exception in connection attempt
            catch(MalformedURLException urlException)
            {
                urlException.printStackTrace();
                getGlassPane().getGraphics().drawString("error MFEx(201,201)",124,124);
               /* try
            {
                URL url;
                url =new URL("www.google.com");
                //"this.Appbase" added 6/2004 for portability

                AppletContext browser = getAppletContext();

                browser.showDocument(url);
            }

            //Handle malformed url exception in connection attempt
            catch(MalformedURLException url1Exception)
            {
                url1Exception.printStackTrace();
            }*/
            }
        }
    }//GEN-LAST:event_submitButtonActionPerformed

    private void rasterDataTypeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rasterDataTypeActionPerformed

        //Get index of selected item in raster box
        int i = rasterDataType.getSelectedIndex();
        
        //If invalid selection, reset to default value and set submit disabled
        if(i < 2)
        {
            rasterDataType.setSelectedIndex(0);
            submitButton.setEnabled(false);
            //INSERTTUT
            
            //If data type box is vector, set submit enabled
            if(dataType.getSelectedIndex() == 3)
            {
                submitButton.setEnabled(true);
            }
        }
        
        //If valid selection, set submit enabled
        if(i >= 2)
        {
            //INSERTTUT
            submitButton.setEnabled(true);
        }
    }//GEN-LAST:event_rasterDataTypeActionPerformed

    private void dataTypeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_dataTypeActionPerformed
        
        //Get index of selected item in data type box
        int i = dataType.getSelectedIndex();
        
        //If invalid selection, set to default value and set all following
        //gui components disabled
        if(i < 2)
        { 
            //INSERTTUT
            dataType.setSelectedIndex(0);
            rasterDataType.setEnabled(false);
            submitButton.setEnabled(false);
        }
        
        //If Raster, set box 4 enabled, and set button invisible
        if(i == 2)
        {
            //INSERTTUT
            rasterDataType.setEnabled(true);
            submitButton.setEnabled(false);
        }
        
        //If vector, set raster data type box disabled, and set submit enabled
        if(i == 3)
        {
            //INSERTTUT
            rasterDataType.setEnabled(false);
            submitButton.setEnabled(true);
        }
        
        //Set following gui components to default values
        rasterDataType.setSelectedIndex(0);
    }//GEN-LAST:event_dataTypeActionPerformed

    private void preserveWhatActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_preserveWhatActionPerformed
       
        //Get selected index of preserve box
        int i = preserveWhat.getSelectedIndex();
        
        //If invalid selection, set to default value and set all following
        //gui components to disabled
        if(i < 2)
        {
            //INSERTTUT
            preserveWhat.setSelectedIndex(0);
            dataType.setEnabled(false);
            rasterDataType.setEnabled(false);
            submitButton.setEnabled(false);
        }
        
        //If valid selection, set data type box enabled
        if(i >= 2)
        {   
            //INSERTTUT
            dataType.setEnabled(true);
        }
        //Preserve Area
        if(i == 2)
        {
          HighLight = 4;//1;
          HighPathAr = new int[] {0,1,4};
          changed = true;
        }
        //Preserve Shape
        if(i == 3)
        {
          HighLight = 5;//;2;
           HighPathAr = new int[] {0,1,5};
          changed = true;

        }
        //Preserve Compromise
        if(i == 4)
        {
          HighLight = 6;//3;
          HighPathAr = new int[] {0,1,6};
          changed = true;
        }
        
        //Set following gui components to default values
        dataType.setSelectedIndex(0);
        rasterDataType.setSelectedIndex(0);        
    }//GEN-LAST:event_preserveWhatActionPerformed

    private void mapTypeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_mapTypeActionPerformed

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
       /////////////////////////////////////////////////
        /*try
        {
        //this.Appbase = new URL(this.getCodeBase(),"../"); 
        }
        catch(MalformedURLException urlException)
        {
            urlException.printStackTrace();
        }/**/

        ////////////////////////////////////////////////
        //Get selected index of map type box
        int i = mapType.getSelectedIndex();
        
        //If invalid selection, set to default value and set all following
        //gui components to disabled
        if(i < 2)
        {
            //INSERTTUT
            mapType.setSelectedIndex(0);
            preserveWhat.setEnabled(false);
            dataType.setEnabled(false);
            rasterDataType.setEnabled(false);
            submitButton.setEnabled(false);
        }
        
        //If Global, set preserve box enabled
        if(i == 2)
        {
            //INSERTTUT
            //If "compromise" missing from preserve box, add it back
            Gname = 1;//let 3 generate Global Visualization
            if(preserveWhat.getItemCount() < 5)
            {
                preserveWhat.addItem("Compromise");
            }
            //changed  =true;
            preserveWhat.setEnabled(true);
            DSSUpdater.setHighLight(1);
            HighLight = 1;
            DSSUpdater.setGraphName(1);
            HighPathAr = new int[] {0,1};
            changed  =true;
           
        }
        
        //If Continental, transfer to Continental web page
        if(i == 3)
        {
            String location;
            Gname = 2;//let 3 generate Continental Visualization
             changed  =true;
            //Get location from html file
            location = getParameter("location1");
             DSSUpdater.setHighLight(2);
              HighLight = 2;
            DSSUpdater.setGraphName(2);
             HighPathAr = new int[] {0,2};
            
            //Try to make connection to new web page
            try
            {
                URL url;
                //url =new URL(this.Appbase,location);
                url = new URL(this.getCodeBase(),location);
                //"this.Appbase" added 6/2004 for portability
                           
                AppletContext browser = getAppletContext();
            
                browser.showDocument(url);
            }
            
            //Handle malformed url exception in connection attempt
            catch(MalformedURLException urlException)
            {
                urlException.printStackTrace();
            }
        }
        
        //If regional, transfer to Regional page
        if(i == 4)
        {
            String location;
            Gname = 3;//let 3 generate Regional Visualization
             changed  =true;
            //Get location from html file
            location = getParameter("location0");
             DSSUpdater.setHighLight(3);
              HighLight = 3;
            DSSUpdater.setGraphName(3);
           HighPathAr = new int[] {0,3};
            
            //Try to make connection to new web page
            try
            {
                URL url;
                url =new URL(this.getCodeBase(),location);
                //"this.Appbase" added 6/2004 for portability
                           
                AppletContext browser = getAppletContext();
            
                browser.showDocument(url);
            }
            
            //Handle malformed url exception in connection attempt
            catch(MalformedURLException urlException)
            {
                urlException.printStackTrace();
            }
        }
        
        //Set following gui components to default values
        preserveWhat.setSelectedIndex(0);
        dataType.setSelectedIndex(0);
        rasterDataType.setSelectedIndex(0);
    }//GEN-LAST:event_mapTypeActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JComboBox mapType;
    private javax.swing.JComboBox preserveWhat;
    private javax.swing.JComboBox dataType;
    private javax.swing.JComboBox rasterDataType;
    private javax.swing.JButton submitButton;
    // End of variables declaration//GEN-END:variables
    
    //This is for Threading info to Tutorial Visualizer    
/*  private int HighLight = 0;
  private int HighPath = 0;
  private int Gname = 0;
  private int setMax = 9;
  boolean changed = false;
  Thread runner;/**/

  public void init()
  {
    DSSUpdater.setHighLight(HighLight);//.setTicket(ticket);
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
      //if(changed)
      //{
        //perform appropriate update
        DSSUpdater.setHighLight(HighLight);
        DSSUpdater.setHighPath(HighPath);
        DSSUpdater.setHighPathAr(HighPathAr);
        DSSUpdater.setGraphName(Gname);
        //changed  = false;
      //}
        pause(88);//1000);//pause for DSS is Fine
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
