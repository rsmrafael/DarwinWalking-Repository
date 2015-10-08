import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.io.IOException;
import java.util.Date;
import java.util.TooManyListenersException;

import javax.comm.PortInUseException;
import javax.comm.UnsupportedCommOperationException;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.WindowConstants;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;


/**
 * 
 * @author Tobias Bolze
 *
 */
public class RobotControlLayout extends JFrame
{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private JMenuBar menuBar = new JMenuBar();
	private JMenu menuMain = new JMenu("Menu");
	private JMenuItem motions = new JMenuItem("Motions", KeyEvent.VK_T);
	private JMenuItem servoId = new JMenuItem("Servo Id´s", KeyEvent.VK_T);
	private JMenuItem servoCommands = new JMenuItem("Servo Commands",KeyEvent.VK_0);
	private JMenu menuConnection = new JMenu("Connection Typ");
	private JMenuItem serialConnection = new JMenuItem("Serial", KeyEvent.VK_T);
	private JMenu menuComPort = new JMenu("Com Port");
	private JMenuItem comPort1 = new JMenuItem("", KeyEvent.VK_T);
	private JMenuItem comPort2 = new JMenuItem("", KeyEvent.VK_T);
	private JMenuItem comPort3 = new JMenuItem("", KeyEvent.VK_T);
	private JMenuItem comPort4 = new JMenuItem("", KeyEvent.VK_T);
	private JMenuItem comPort5 = new JMenuItem("", KeyEvent.VK_T);
	private JMenu baud;
	private JMenuItem baud1 = new JMenuItem("");
	private JMenuItem baud2 = new JMenuItem("");
	private JMenuItem baud3 = new JMenuItem("");
	private JMenuItem baud4 = new JMenuItem("");
	private JMenuItem baud5 = new JMenuItem("");
	JList motions1 = new JList();
	
	JList motionList = new JList();
	JList sceneList = new JList();
	private JButton cmdOpenPort = new JButton();
	
	JButton doMotion = new JButton();
	
	JButton newMotion = new JButton();
	JButton save = new JButton();
	JButton doSzene = new JButton();
	private JTextArea debugText = new JTextArea();
	JScrollPane jScrollPane1 = new JScrollPane(debugText);
	JScrollPane jScrollPane3 = new JScrollPane(motions1);
	JScrollPane jScrollPane5 = new JScrollPane(motionList);
	JScrollPane jScrollPane6 = new JScrollPane(sceneList);
	JTextField id = new JTextField();
	
	JTextField newMotion2 = new JTextField();
	JTextField scenes = new JTextField();
	JTextField servoCount = new JTextField();
	JTextField[] servoPositions ;
	JLabel[] servoNames;
	JLabel capture = new JLabel();
	
	/*
	 * TEST DURCH TEXTFELD
	 */
	//JTextField instant = new JTextField();
	JCheckBox box = new JCheckBox();
	private Container cp;
	private int[] ports;
	private int comPort = -1;
	private boolean isConnected = false;
	Control control = new Control(this);
	Rastergitter raster = new Rastergitter();
	private Date dt = new Date();
	private String sektion;
	public RobotControlLayout(String title)
	{
		super(title);
		baud = new JMenu(""+control.getBaudRate());
		setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
		raster.setfHeight(500);
		raster.setfWidth(748);
	    int frameWidth = raster.getfWidth();
	    int frameHeight = raster.getfHeight();
	    setSize(frameWidth, frameHeight);
	    raster.setBackgroundColor(raster.getBackgroundColor());
	    Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
	    int x = (d.width - getSize().width) / 2;
	    int y = (d.height - getSize().height) / 2;
	    setLocation(x, y);
	    setIconImage(createImage("Iamges\\teamicon.png"));
	    setCp(getContentPane());
	    getCp().setLayout(null);
	    servoPositions = new JTextField[30];
	    servoNames = new JLabel[30];
	    for(int i= 0; i<30;i++)
	    {
	    	servoPositions[i] = new JTextField();
	    	servoNames[i] = new JLabel();
	    	servoNames[i].setText("Servo: "+i);
	    	servoPositions[i].addKeyListener(new KeyAdapter() {
		          public void keyReleased(KeyEvent evt) {
		        	  servo_KeyReleased(evt);
		          }
		          });
	    	cp.add(servoNames[i]);
	    	cp.add(servoPositions[i]);
	    }
	    motions.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              motions_ActionPerformed(evt );
	           }
	        });
	    servoId.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	        	  servoId_ActionPerformed(evt );
	           }
	        });
	    servoCommands.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	        	  servoCommands_ActionPerformed(evt );
	           }
	        });
	    menuMain.add(motions);
	    menuMain.add(servoId);
	    menuMain.add(servoCommands);
	    menuBar.add(menuMain);
	    String[] bauds = control.getBaudrates();
	    baud.add(baud1);
	    baud1.setText(""+bauds[0]);
	    baud1.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              baud1_ActionPerformed(evt );
	           }
	        });
	    baud.add(baud2);
	    baud2.setText(""+bauds[1]);
	    baud2.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              baud2_ActionPerformed(evt );
	           }
	        });
	    baud.add(baud3);
	    baud3.setText(""+bauds[2]);
	    baud3.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              baud3_ActionPerformed(evt );
	           }
	        });
	    baud.add(baud4);
	    baud4.setText(""+bauds[3]);
	    baud4.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              baud4_ActionPerformed(evt );
	           }
	        });
	    baud.add(baud5);
	    baud5.setText(""+bauds[4]);
	    baud5.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              baud5_ActionPerformed(evt );
	           }
	        });
	    
	 
	    serialConnection.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              serialConnection_ActionPerformed(evt );
	           }
	        });
	    menuConnection.add(serialConnection);
	    menuBar.add(menuConnection);
	    comPort1.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              comPort1_ActionPerformed(evt );
	           }
	        });
	    menuComPort.add(comPort1);
	    comPort2.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              comPort2_ActionPerformed(evt );
	           }
	        });
	    menuComPort.add(comPort2);
	    comPort3.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              comPort3_ActionPerformed(evt );
	           }
	        });
	    menuComPort.add(comPort3);
	    comPort4.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              comPort4_ActionPerformed(evt );
	           }
	        });
	    menuComPort.add(comPort4);
	    comPort5.addActionListener(new ActionListener() {
	          public void actionPerformed(ActionEvent evt ) {
	              comPort5_ActionPerformed(evt );
	           }
	        });
	    menuComPort.add(comPort5);
	    menuComPort.setVisible(false);
	    menuBar.add(menuComPort);
	    cmdOpenPort.setText("Open Port");
	    cmdOpenPort.setBackground(Color.RED);
	    cmdOpenPort.addActionListener(new ActionListener() {
	         public void actionPerformed(ActionEvent evt) {
	           
					connect_ActionPerformed(evt);
	         }
	      });
	    cmdOpenPort.setVisible(false);
	    baud.setVisible(false);
	    menuBar.add(baud);
	    menuBar.add(cmdOpenPort);
	    setJMenuBar(menuBar );
	    id.setBounds(30,100,30,25);
	    id.setVisible(false);
	    getCp().add(id);
	    
	    motionList.addListSelectionListener(new ListSelectionListener() {
	        public void valueChanged(ListSelectionEvent evt) {
	        	motionList_ListValueChanged(evt);
	        }
	      });
	    sceneList.addListSelectionListener(new ListSelectionListener() {
	        public void valueChanged(ListSelectionEvent evt) {
	        	szeneList_ListValueChanged(evt);
	        }
	      });
	    debugText.setFocusable(false);
	    jScrollPane1.setBounds(32, 146, 185, 233);
	    jScrollPane1.createVerticalScrollBar();
	    jScrollPane1.setVisible(false);
	    getCp().add(jScrollPane1);
	    jScrollPane3.createVerticalScrollBar();
	    jScrollPane3.setBounds(20, 26, 185, 233);
	    jScrollPane3.setVisible(false);
	    getCp().add(jScrollPane3);
	    
	    jScrollPane5.setBounds(20, 70, 200, 233);
	    jScrollPane5.setVisible(false);
	    getCp().add(jScrollPane5);
	    jScrollPane6.setBounds(220, 70, 200, 233);
	    jScrollPane6.setVisible(false);
	    getCp().add(jScrollPane6);
	    box.setBounds(78, 101, 20, 20);
	    box.addActionListener(new ActionListener() {
	         public void actionPerformed(ActionEvent evt) {
	           
					box_ActionPerformed(evt);
	         }
	      });
	    box.setVisible(false);
	    getCp().add(box);
	    
	    doMotion.setText("DoMotion");
	    doMotion.setBounds(215,26,100,20);
	    doMotion.addActionListener(new ActionListener() {
	         public void actionPerformed(ActionEvent evt) {
	           
					try {
						control.doMotion_ActionPerformed(evt);
					} catch (IOException e) {
						error(""+e);
					}
	         }
	      });
	    doMotion.setVisible(false);
	    getCp().add(doMotion);
	    
	    save.setBounds(120,320,100,30);
	    save.setText("SAVE");
	    save.addActionListener(new ActionListener() {
	         public void actionPerformed(ActionEvent evt) {
	           
	        	 save_ActionPerformed(evt);
	         }
	      });
	    save.setVisible(false);
	    getCp().add(save);
	    
	    newMotion2.setBounds(20,20,100,30);
	    newMotion2.setToolTipText("New Motion Name");
	    newMotion2.setVisible(false);
	    getCp().add(newMotion2); 
	    newMotion.setBounds(220,20,100,30);
	    newMotion.setText("New Motion");
	    newMotion.setVisible(false);
	    newMotion.addActionListener(new ActionListener() {
	         public void actionPerformed(ActionEvent evt) {
	           
	        	 newMotion_ActionPerformed(evt);
	         }
	      });
	    getCp().add(newMotion);
	    scenes.setBounds(120,20,50,30);
	    scenes.setToolTipText("Anzahl der Szenen");
	    scenes.setVisible(false);
	    getCp().add(scenes);
	    doSzene.setBounds(220,320,100,30);
	    doSzene.setText("TESTSZENE");
	    doSzene.addActionListener(new ActionListener() {
	         public void actionPerformed(ActionEvent evt) {
	           
	        	 doSzene_ActionPerformed(evt);
	         }
	      });
	    doSzene.setVisible(false);
	    getCp().add(doSzene);
	    servoCount.setBounds(170,20,50,30);
	    servoCount.setVisible(false);
	    servoCount.setToolTipText("Anzahl der Servos");
	    getCp().add(servoCount);
	    capture.setText("<html>Mit Tastenkürzel K Servo lösen, mit L Capturen.<p/>Mit Tastenkürzel J ServoPosition Spigeln.</html>");
	    capture.setBounds(100,350,500,130);
	    capture.setVisible(false);
	    getCp().add(capture);
	    setResizable(false);
	    setVisible(true);
	    raster.setZellHeight(439);
	    raster.setZellWidth(750);
	    raster.initCounter();
	    raster.drawImaget(createImage("Iamges\\Background.jpg"), 0,0);
	    repaint();
	}
	 public void paint(Graphics g)
     {
		 
        super.paint(g);
     }
	public void debug_ActionPerformed(ActionEvent evt)
	{
		repaint();
		if(isConnected)
		{
			try {
				control.setServoCommands(false);
				control.setMotions(false);
				control.setServoId(false);
			} catch (IOException e) {
				error(e.toString());
			}
			id.setVisible(true);
			jScrollPane1.setVisible(true);
			box.setVisible(true);
			
		}		
	}
	public void error(String error)
	{
		new errorFrame(this,"ERROR",true,error);
	}
	public void drawImagett(String Pfad,int x, int y)
	{
		raster.drawImaget(createImage(Pfad),x,y);
	}
	public Image createImage(String Pfad)
	{
		return Toolkit.getDefaultToolkit().getImage(Pfad);
	}
	public void serialConnection_ActionPerformed(ActionEvent evt)
	{
		repaint();
		if(!control.isSerial())
		{
			control.setSerial(true);
		    ports = control.getAvailablePorts();
			if(ports[0]!=-1)
			{
				comPort1.setText("COM:"+ports[0]);
			}
			if(ports[1]!=-1)
			{
				comPort2.setText("COM:"+ports[1]);
			}
			if(ports[2]!=-1)
			{
				comPort3.setText("COM:"+ports[2]);
			}
			if(ports[3]!=-1)
			{
				comPort4.setText("COM:"+ports[3]);
			}
			if(ports[4]!=-1)
			{
				comPort5.setText("COM:"+ports[4]);
			}
			
			menuComPort.setVisible(true);
		}
		
	}
	public void comPort1_ActionPerformed(ActionEvent evt)
	{
		repaint();
		if(!comPort1.getText().equals(""))
		{
			String[] zwischen = comPort1.getText().split(":");
			comPort = Integer.parseInt(zwischen[1]);
			cmdOpenPort.setVisible(true);
			baud.setVisible(true);
		}
		
	}
	public void comPort2_ActionPerformed(ActionEvent evt)
	{
		repaint();
		if(!comPort2.getText().equals(""))
		{
			String[] zwischen = comPort2.getText().split(":");
			comPort = Integer.parseInt(zwischen[1]);
			cmdOpenPort.setVisible(true);
			baud.setVisible(true);
		}
		
	}
	public void comPort3_ActionPerformed(ActionEvent evt)
	{
		repaint();
		if(!comPort3.getText().equals(""))
		{
			String[] zwischen = comPort3.getText().split(":");
			comPort = Integer.parseInt(zwischen[1]);
			cmdOpenPort.setVisible(true);
			baud.setVisible(true);
		}
		
	}
	public void comPort4_ActionPerformed(ActionEvent evt)
	{
		repaint();
		if(!comPort4.getText().equals(""))
		{
			String[] zwischen = comPort4.getText().split(":");
			comPort = Integer.parseInt(zwischen[1]);
			cmdOpenPort.setVisible(true);
			baud.setVisible(true);
		}
		
	}
	public void comPort5_ActionPerformed(ActionEvent evt)
	{
		repaint();
		if(!comPort5.getText().equals(""))
		{
			String[] zwischen = comPort5.getText().split(":");
			comPort = Integer.parseInt(zwischen[1]);
			cmdOpenPort.setVisible(true);
			baud.setVisible(true);
		}
		
	}
	public void setACK(String grad)
	{
		int waitingMes = Integer.parseInt(grad, 16);
		if(waitingMes < 255)
		{
			String ergebnis = "ACK: "+waitingMes+" \n";
			setListe(ergebnis);
		}
		control.ini2.setStringProperty(sektion, ""+control.getProtokoll(), "ACK: "+waitingMes, null);
		control.ini2.save();
		control.setProtokoll(control.getProtokoll()+1);
	}
	public void setRequest(String request)
	{
		int waitingMes = Integer.parseInt(request, 16);
		if(waitingMes < 255)
		{
			String ergebnis = "Request: "+waitingMes+" \n";
			setListe(ergebnis);
		}
		control.ini2.setStringProperty(sektion, ""+control.getProtokoll(), "Request: "+waitingMes, null);
		control.ini2.save();
		control.setProtokoll(control.getProtokoll()+1);
	}
	public void setListe (String text)
	{
		String ergebnis = debugText.getText()+text;
		debugText.setText(ergebnis);
	}
	public void setIsConnected()
	{
		isConnected = true;
		cmdOpenPort.setText("Close");
		cmdOpenPort.setBackground(Color.GREEN);
	}
	public void motions_ActionPerformed(ActionEvent evt )
	{
		repaint();
		if(isConnected)
		{
			if(control.isSerial())
			{
				try {
					control.setServoCommands(false);
					control.setServoId(false);
					control.setMotions(true);
				} catch (IOException e) {
					error(""+e);
				}
			}
		}
	}
	
	public void connect_ActionPerformed(ActionEvent evt) 
	   {
		repaint();
			if(comPort!=-1 && !isConnected)
			{
				try {
					control.connect(comPort);
				} catch (ClassNotFoundException e) {
					error(e.toString());
				} catch (IllegalAccessException e) {
					error(e.toString());
				} catch (InstantiationException e) {
					error(e.toString());
				} catch (UnsupportedCommOperationException e) {
					error(e.toString());
				} catch (TooManyListenersException e) {
					error(e.toString());
				} catch (IOException e) {
					error(e.toString());
				} catch (InterruptedException e) {
					error(e.toString());
				} catch (PortInUseException e) {
					error(e.toString());
				}
				sektion = "Session:"+dt;
				control.ini2.addSection(sektion,  null);
				control.ini2.save();
			}else if(isConnected)
			{
				try {
					control.setMotions(false);
				} catch (IOException e) {
					error(e.toString());
				}
				try {
					control.close();
				} catch (IOException e) {
					error(e.toString());
				} catch (InstantiationException e) {
					error(e.toString());
				} catch (IllegalAccessException e) {
					error(e.toString());
				} catch (ClassNotFoundException e) {
					error(e.toString());
				} catch (UnsupportedCommOperationException e) {
					error(e.toString());
				}
				cmdOpenPort.setText("Open Port");
			    cmdOpenPort.setBackground(Color.RED);
			    isConnected = false;
			    repaint();
			}
		
	   }
	public void box_ActionPerformed(ActionEvent evt)
	{
		if(!box.isSelected())
		{
			id.setEnabled(true);
		}else
		{
			id.setText("");
			id.setEnabled(false);
		}
	}
	public void servoId_ActionPerformed(ActionEvent evt )
	{
		repaint();
		if(isConnected)
		{
			try {
				control.setServoCommands(false);
				control.setMotions(false);
				control.setServoId(true);
			} catch (IOException e) {
				error(""+e);
			}
		}
	}
	public void servoCommands_ActionPerformed(ActionEvent evt )
	{
		repaint();
		try {
			control.setServoId(false);
			control.setMotions(false);
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		control.setServoCommands(true);
		try {
			control.setDirectControlMode(true);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	
	
	
	
	public void newMotion_ActionPerformed(ActionEvent evt)
	{
		control.newMotion(newMotion2.getText(), Integer.parseInt(servoCount.getText()), Integer.parseInt(scenes.getText()));
		control.writeMotionList();
		
	}
	public void motionList_ListValueChanged(ListSelectionEvent evt)
	{
		control.writeSceneList();
	}
	public void szeneList_ListValueChanged(ListSelectionEvent evt)
	{
		control.writeServoPositions();
	}
	public static void main(String args[])
	{
		new RobotControlLayout("Robot Controller");
	}
	public void setCp(Container cp) {
		this.cp = cp;
	}
	public Container getCp() {
		return cp;
	}
	public void save_ActionPerformed(ActionEvent evt)
	{
		control.setServoPosition();
	}
	public void servo_KeyReleased(KeyEvent evt)
	{
		if(evt.getKeyCode()==KeyEvent.VK_ENTER && control.isServoCommands()&& control.isConnection())
		{
			int serv = -1;
			for(int i=0;i< control.getServoAnzahl((String)motionList.getSelectedValue(),sceneList.getSelectedIndex());i++)
			{
				if(this.servoPositions[i].isFocusOwner())
				{
					serv = i;
				}

			}
			try {
				
				control.setServo(serv, Integer.parseInt(servoPositions[serv].getText()));
			} catch (NumberFormatException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}else if(evt.getKeyCode()==KeyEvent.VK_K && control.isServoCommands()&& control.isConnection())
		{
			try {
				control.setDirectControlMode(true);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			control.run = true;
			
		}else if(evt.getKeyCode()==KeyEvent.VK_L)
		{
			
			control.run = false;
		}else if(evt.getKeyCode()==KeyEvent.VK_J)
		{
			int serv = -1;
			for(int i=0;i< control.getServoAnzahl((String)motionList.getSelectedValue(),sceneList.getSelectedIndex());i++)
			{
				if(this.servoPositions[i].isFocusOwner())
				{
					serv = i;
				}

			}
			try {
				char[] zwischen1 = servoPositions[serv].getText().toCharArray();
				String zwischen2 = "";
				for(int i=0;i< zwischen1.length-1;i++)
				{
					zwischen2 = zwischen2 + zwischen1[i];
				}
				int zwischen = Integer.parseInt(zwischen2);
				zwischen = 127-(zwischen-127);
				servoPositions[serv].setText(""+zwischen);
			} catch (NumberFormatException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	public void doSzene_ActionPerformed(ActionEvent evt)
	{
		if(isConnected)
		{
			try {
				control.setDirectControlMode(true);
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				for(int i=0;i< control.getServoAnzahl((String)motionList.getSelectedValue(),sceneList.getSelectedIndex());i++)
				{
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					control.setServo(i, Integer.parseInt(servoPositions[i].getText()));
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		
	}
	
	public void baud1_ActionPerformed(ActionEvent evt )
	{
		repaint();
		try{

			control.setBaudRate(Integer.parseInt(baud1.getText()));
			baud.setText(baud1.getText());
		}catch(NumberFormatException e)
		{
			error(""+e);
		}
	}
	public void baud2_ActionPerformed(ActionEvent evt )
	{
		repaint();
		try{

			control.setBaudRate(Integer.parseInt(baud2.getText()));
			baud.setText(baud2.getText());
		}catch(NumberFormatException e)
		{
			error(""+e);
		}
	}
	public void baud3_ActionPerformed(ActionEvent evt )
	{
		repaint();
		try{

			control.setBaudRate(Integer.parseInt(baud3.getText()));
			baud.setText(baud3.getText());
		}catch(NumberFormatException e)
		{
			error(""+e);
		}
	}
	public void baud4_ActionPerformed(ActionEvent evt )
	{
		repaint();
		try{

			control.setBaudRate(Integer.parseInt(baud4.getText()));
			baud.setText(baud4.getText());
		}catch(NumberFormatException e)
		{
			error(""+e);
		}
	}
	public void baud5_ActionPerformed(ActionEvent evt )
	{
		repaint();
		try{

			control.setBaudRate(Integer.parseInt(baud5.getText()));
			baud.setText(baud5.getText());
		}catch(NumberFormatException e)
		{
			error(""+e);
		}
	}
	/*
	 * Test instant Message
	 */
	/*public void instant_KeyReleased(KeyEvent evt)
	{
		if(evt.getKeyCode()==KeyEvent.VK_ENTER)
		{
			try {
				control.write(instant.getText());
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}*/
	
}