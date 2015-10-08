import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;


public class HeadLayout 
{
	private JButton pingAll;
	private RobotControlLayout robotControlLayout;
	private Control control;
	private JList servos2;
	private JScrollPane jScrollPane4;
	private JTextField PGain;
	private JTextField DGain;
	private JButton SetPD;
	private JTextField IGain;
	private JButton SetI;
	private JButton doNewId;
	private JButton newBounds;
	private JTextField newId;
	private JTextField boundMin;
	private JTextField boundMax;
	public HeadLayout(RobotControlLayout robotControlLayout,Control control)
	{
		this.robotControlLayout =robotControlLayout;
		this.control = control;
		initObjeckts();
	}
	public void initObjeckts()
	{
		SetPD = new JButton();
		SetPD.setBounds(400,140,150,30);
	    SetPD.setText("Set PD");
	    SetPD.addActionListener(new ActionListener() {
	         public void actionPerformed(ActionEvent evt) {
	           
	        	 SetPD_ActionPerformed(evt);
	         }
	      });
		pingAll = new JButton();
		pingAll.setBounds(100, 271, 80, 20);
	    pingAll.setText("Ping All");
	    pingAll.addActionListener(new ActionListener() {
	         public void actionPerformed(ActionEvent evt) {
	           
	        	 pingAll_ActionPerformed(evt);
	         }
	      });
		servos2 = new JList();
		servos2.addListSelectionListener(new ListSelectionListener() {
	        public void valueChanged(ListSelectionEvent evt) {
	          servos2_ListValueChanged(evt);
	        }
	      });
		jScrollPane4 = new JScrollPane(servos2);
	    jScrollPane4.setBounds(40, 26, 185, 233);
		PGain = new JTextField();
		PGain.setBounds(300,140,40,30);
	    PGain.setToolTipText("PGAIN");
		DGain = new JTextField();
		IGain = new JTextField();
		SetI = new JButton();
		SetI.setBounds(400,180,150,30);
	    SetI.setText("Set I");
	    SetI.addActionListener(new ActionListener() {
	         public void actionPerformed(ActionEvent evt) {
	           
	        	 SetI_ActionPerformed(evt);
	         }
	      });
		doNewId =new JButton();
		newBounds =new JButton();
		newId = new JTextField();
		newId.setBounds(30,330,40,30);
	    newId.setToolTipText("Neue Id: 0 ~ 30 ");
		boundMin = new JTextField();
		boundMin.setBounds(300, 100, 40, 30);
	    boundMin.setToolTipText("Min Bound Value");
		boundMax = new JTextField();
		boundMax.setBounds(350, 100, 40, 30);
	    boundMax.setToolTipText("Max Bound Value");
	    
	}
	public void addObjeckts()
	{	
	    robotControlLayout.getCp().add(pingAll);
	    robotControlLayout.getCp().add(jScrollPane4);
	    robotControlLayout.getCp().add(SetPD);
	    robotControlLayout.getCp().add(SetI);
	    robotControlLayout.getCp().add(newId);
	    robotControlLayout.getCp().add(boundMin);
	    robotControlLayout.getCp().add(boundMax);
	    robotControlLayout.getCp().add(PGain);
	    DGain.setBounds(350,140,40,30);
	    DGain.setToolTipText("DGAIN");
	    robotControlLayout.getCp().add(DGain);
	    IGain.setBounds(300,180,40,30);
	    IGain.setToolTipText("IGAIN");
	    robotControlLayout.getCp().add(IGain);
	    doNewId.setBounds(80,330,80,30);
	    doNewId.setText("Neue Id");
	    doNewId.addActionListener(new ActionListener() {
	         public void actionPerformed(ActionEvent evt) {	           
	        	 doNewId_ActionPerformed(evt);
	         }
	      });
	    robotControlLayout.getCp().add(doNewId);
	    newBounds.setBounds(400,100,150,30);
	    newBounds.setText("Save Bounds");
	    newBounds.addActionListener(new ActionListener() {
	         public void actionPerformed(ActionEvent evt) {
	           
	        	 newBounds_ActionPerformed(evt);
	         }
	      });
	    robotControlLayout.getCp().add(newBounds);
	}
	public void deleteObjeckt()
	{
		//robotControlLayout.getCp().remove(comp)
	}
	public void pingAll_ActionPerformed(ActionEvent evt)
	{
		try {
			servos2.setListData(control.pingAll());
		} catch (IOException e) {
			//error(""+e);
		} catch (InterruptedException e) {
			//error(""+e);
		}
	}
	public void servos2_ListValueChanged(ListSelectionEvent evt)
	{
		String zwischen = ""+servos2.getSelectedValue();
		String ergebnis = "";
		if(!zwischen.equals(""))
		{
			String[] zwischen2 = zwischen.split(":");
			try {
				ergebnis = control.getBoundarys(Integer.parseInt(zwischen2[1]));
				String erg1 = ergebnis.substring(0,2);
				String erg2 = ergebnis.substring(2, 4);
				boundMin.setText(""+Integer.parseInt(erg1,16));
				boundMax.setText(""+Integer.parseInt(erg2,16));
				ergebnis = control.getPD(Integer.parseInt(zwischen2[1]));
				erg1 = ergebnis.substring(0,2);
				erg2 = ergebnis.substring(2, 4);
				PGain.setText(""+Integer.parseInt(erg1,16));
				DGain.setText(""+Integer.parseInt(erg2,16));
				ergebnis = control.getI(Integer.parseInt(zwischen2[1]));
				erg1 = ergebnis.substring(0,2);
				IGain.setText(""+Integer.parseInt(erg1,16));
			} catch (NumberFormatException e) {
			//	error(""+e);
			} catch (IOException e) {
		//		error(""+e);
			} catch (InterruptedException e) {
	//			error(""+e);
			}
		}
		
	}
	public void SetPD_ActionPerformed(ActionEvent evt)
	{
		String zwischen = ""+servos2.getSelectedValue();
		if(!zwischen.equals(""))
		{
			String[] zwischen2 = zwischen.split(":");
			try {
				if(!control.PDSet(Integer.parseInt(zwischen2[1]), Integer.parseInt(PGain.getText()), Integer.parseInt(DGain.getText())))
				{
				//	error("Fehler beim Umschreiben der PD");
				}
			} catch (NumberFormatException e) {
				//error(""+e);
			} catch (IOException e) {
			//	error(""+e);
			} catch (InterruptedException e) {
		//		error(""+e);
			}
		}
	}
	public void SetI_ActionPerformed(ActionEvent evt)
	{
		String zwischen = ""+servos2.getSelectedValue();
		if(!zwischen.equals(""))
		{
			String[] zwischen2 = zwischen.split(":");
			try {
				if(! control.ISet(Integer.parseInt(zwischen2[1]), Integer.parseInt(IGain.getText())))
				{
					//error("Fehler beim Umschreiben der PD");
				}
			} catch (NumberFormatException e) {
				//error(""+e);
			} catch (IOException e) {
			//	error(""+e);
			} catch (InterruptedException e) {
		//		error(""+e);
			}
		}
	}
	public void newBounds_ActionPerformed(ActionEvent evt)
	{
		String zwischen = ""+servos2.getSelectedValue();
		if(!zwischen.equals(""))
		{
			String[] zwischen2 = zwischen.split(":");
			try {
				if(!control.boundarySet(Integer.parseInt(zwischen2[1]), Integer.parseInt(boundMin.getText()), Integer.parseInt(boundMax.getText())))
				{
				//	error("Fehler beim Umschreiben der Boundarys");
				}
			} catch (NumberFormatException e) {
			//	error(""+e);
			} catch (IOException e) {
		//		error(""+e);
			} catch (InterruptedException e) {
	//			error(""+e);
			}
		}
	}
	public void doNewId_ActionPerformed(ActionEvent evt)
	{
		String zwischen = ""+servos2.getSelectedValue();
		if(!zwischen.equals(""))
		{
			String[] zwischen2 = zwischen.split(":");
			try {
				if(!control.newId(zwischen2[1], newId.getText()))
					{
				//		error("ID wurde nicht richtig verändert");
					}
			} catch (IOException e) {
			//	error(""+e);
			} catch (InterruptedException e) {
			//	error(""+e);
			}
		}
	}
	
	
}

