package de.fhwf.humanoid.tools.debugger;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.prefs.Preferences;

import javax.imageio.ImageIO;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.UIManager;

import org.flexdock.docking.DockingManager;
import org.flexdock.docking.drag.effects.EffectsManager;
import org.flexdock.docking.drag.preview.GhostPreview;
import org.flexdock.docking.state.PersistenceException;
import org.flexdock.perspective.PerspectiveManager;
import org.flexdock.perspective.persist.FilePersistenceHandler;
import org.flexdock.perspective.persist.PersistenceHandler;
import org.flexdock.view.Viewport;

import de.fhwf.humanoid.tools.debugger.connection.ConnectorNetwork;
import de.fhwf.humanoid.tools.debugger.connection.IConnector;
import de.fhwf.humanoid.tools.debugger.events.StatusEvent;
import de.fhwf.humanoid.tools.debugger.events.StatusListener;
import de.fhwf.humanoid.tools.debugger.services.BlackboardService;
import de.fhwf.humanoid.tools.debugger.services.BodyCalibrationService;
import de.fhwf.humanoid.tools.debugger.services.BodyDataService;
import de.fhwf.humanoid.tools.debugger.services.ColorCalibrationService;
import de.fhwf.humanoid.tools.debugger.services.ControlService;
import de.fhwf.humanoid.tools.debugger.services.FileService;
import de.fhwf.humanoid.tools.debugger.services.ImageService;
import de.fhwf.humanoid.tools.debugger.services.LogService;
import de.fhwf.humanoid.tools.debugger.services.ScalingChangedService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;
import de.fhwf.humanoid.tools.debugger.services.VisionDataService;

public class Debugger extends JFrame implements ActionListener, WindowListener, StatusListener {
	private static final long serialVersionUID = 1146948994737690666L;
	private static final String PERSISTANCE_FILE = "WolvesDebuggerLayout.xml";
	private static final String ICON_IMAGE = "teamicon.png";

	private static Debugger INSTANCE = null;

	private IConnector mConnector;
	private JMenu mnuConnection;
	private JLabel lblStatus;
	
	// Not instantiable
	private Debugger(String client) {
		super("WF Wolves Humanoid Terminal");
	  this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	  this.addWindowListener(this);
	  this.setLayout(new BorderLayout());
	  Icon icon = loadIcon(ICON_IMAGE);
	  if (icon instanceof ImageIcon) {
	  	this.setIconImage(((ImageIcon)icon).getImage());
	  }

	  Viewport port = new Viewport();
	  DockingManager.setDockableFactory(ViewFactory.getInstance());
	  DockingManager.setMainDockingPort(this, "MainDockingPort");
		PerspectiveManager.setFactory(new PerspectiveFactoryImpl());
		PerspectiveManager.getInstance().setCurrentPerspective(PerspectiveFactoryImpl.DEFAULT_PERSPECTIVE_ID, true);
		PersistenceHandler persister = FilePersistenceHandler.createDefault(PERSISTANCE_FILE);
		PerspectiveManager.setPersistenceHandler(persister);
		EffectsManager.setPreview(new GhostPreview());
    try {
			DockingManager.loadLayoutModel();
		} catch (IOException e) {
			JOptionPane.showMessageDialog(this, "Could not load layout file: " + e.getMessage());
		} catch (PersistenceException e) {
			JOptionPane.showMessageDialog(this, "Could not restore layout file: " + e.getMessage());
		}

	  // Create connection
		mConnector = new ConnectorNetwork(client);
				
		// Add services
		LogService logs = new LogService();
		mConnector.addPacketListener(logs);
		ServiceManager.getInstance().addService(logs);
		
		FileService files = new FileService(mConnector);
		mConnector.addPacketListener(files);
		ServiceManager.getInstance().addService(files);
		
	  ImageService images = new ImageService(mConnector, files);
	  mConnector.addPacketListener(images);
	  ServiceManager.getInstance().addService(images);
	  
	  ColorCalibrationService colors = new ColorCalibrationService(mConnector, files);
		mConnector.addPacketListener(colors);
	  ServiceManager.getInstance().addService(colors);

	  BodyCalibrationService body = new BodyCalibrationService(mConnector);
		mConnector.addPacketListener(body);
		ServiceManager.getInstance().addService(body);

	  BodyDataService bodyData = new BodyDataService(mConnector);
		mConnector.addPacketListener(bodyData);
		ServiceManager.getInstance().addService(bodyData);

	  VisionDataService visionData = new VisionDataService(mConnector);
		mConnector.addPacketListener(visionData);
		ServiceManager.getInstance().addService(visionData);

	  ControlService control = new ControlService(mConnector);
		mConnector.addPacketListener(control);
		ServiceManager.getInstance().addService(control);
	  
		 //WTF??? Wer war das denn? So war ein Service nun wirklich nicht gedacht!
		 //TODO: Auf weniger perverse Art und Weise implementieren
	  ScalingChangedService scaling = new ScalingChangedService(mConnector);	
	    mConnector.addPacketListener(scaling);
		ServiceManager.getInstance().addService(scaling);

  BlackboardService blackboard = new BlackboardService(mConnector, files);
 mConnector.addPacketListener(blackboard);
 ServiceManager.getInstance().addService(blackboard);

	  try {
	    InetAddress serverAddress = InetAddress.getByName(client);
			logs.setCurrentSourceAddress(serverAddress);
	 	} catch (UnknownHostException e2) {
	 	  System.err.println("[Debugger] Debugger: Error resolving address: " + e2.getMessage());   
	 	}
		
		// Send connection packet to bot
		mConnector.sendPacket("SYS", "HELLO");
			  
		port.setPreferredSize(new Dimension(800, 600));
    this.add(port, BorderLayout.CENTER);
    DockingManager.restoreLayout();

    // Add menu
	  JMenuBar menuBar = new JMenuBar();
	  mnuConnection = new JMenu("Connection (" + mConnector.getCurrentServer() + ")");
	  menuBar.add(mnuConnection);
	  JMenuItem menuItem = new JMenuItem("Connect...");
	  menuItem.setName("connect");
	  menuItem.addActionListener(this);
	  mnuConnection.add(menuItem);
	  JMenu views = ViewFactory.getInstance().createViewMenu();
	  menuBar.add(views);
	  JMenu perspectives = PerspectiveFactoryImpl.createPerspectivesMenu();
	  menuBar.add(perspectives);
	  this.setJMenuBar(menuBar);

	  // Add status bar
	  lblStatus = new JLabel("Status: -"); 
    this.add(lblStatus, BorderLayout.SOUTH);
    control.addStatusListener(this);
	  
	  this.pack();

	  // Restore window state and size
		Preferences prefs = Preferences.userNodeForPackage(Debugger.class);
		int state = Integer.parseInt(prefs.get("Window_State", Integer.toString(this.getExtendedState())));
		this.setExtendedState(state);
		if (this.getExtendedState() == JFrame.NORMAL) {
  		int y = Integer.parseInt(prefs.get("Window_Top", Integer.toString(this.getLocation().y)));
  		int x = Integer.parseInt(prefs.get("Window_Left", Integer.toString(this.getLocation().x)));
  		this.setLocation(x, y);
  		int w = Integer.parseInt(prefs.get("Window_Width", Integer.toString(this.getWidth())));
  		int h = Integer.parseInt(prefs.get("Window_Height", Integer.toString(this.getHeight())));
  		this.setSize(w, h);
		}

	  this.setVisible(true);
	}

	public static synchronized Debugger getInstance() {
		if (Debugger.INSTANCE == null) {
			Preferences prefs = Preferences.userNodeForPackage(Debugger.class);
			String server = prefs.get("ConnectionIP", "192.168.0.151");

			Debugger.INSTANCE = new Debugger(server);
		}
		return Debugger.INSTANCE;
	}

	public void connectTo(String server) {
		if (server != null) {
			if (!server.equals("")) {
				mConnector.setCurrentServer(server);

			  try {
			    InetAddress serverAddress = InetAddress.getByName(server);
					LogService log = (LogService)ServiceManager.getInstance().getService(LogService.SERVICENAME);
					log.setCurrentSourceAddress(serverAddress);
			 	} catch (UnknownHostException e2) {
			 	  System.err.println("[Debugger] actionPerformed: Error resolving address: " + e2.getMessage());   
			 	}
			
				Preferences prefs = Preferences.userNodeForPackage(Debugger.class);
				prefs.put("ConnectionIP", server);
				
				mnuConnection.setText("Connection (" + mConnector.getCurrentServer() + ")");
				
				mConnector.sendPacket("SYS", "HELLO");
			}
		}		
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if (e.getSource() instanceof JMenuItem) {
			String action = ((JMenuItem)e.getSource()).getName();
			if (action.equals("connect")) {
				String oldServer = mConnector.getCurrentServer();
				String server = JOptionPane.showInputDialog(this, "Enter IP to connect to:", oldServer);
				connectTo(server);
			}
		}
	}

	@Override
	public void windowOpened(WindowEvent e) {
		DockingManager.setAutoPersist(true);
	}

	@Override
	public void windowClosing(WindowEvent e) {
		Preferences prefs = Preferences.userNodeForPackage(Debugger.class);

		prefs.put("Window_State", Integer.toString(this.getExtendedState()));
		if (this.getExtendedState() == JFrame.NORMAL) {
  		prefs.put("Window_Top", Integer.toString(this.getLocation().y));
  		prefs.put("Window_Left", Integer.toString(this.getLocation().x));
  		prefs.put("Window_Width", Integer.toString(this.getWidth()));
  		prefs.put("Window_Height", Integer.toString(this.getHeight()));
		}
	}

	@Override
	public void windowActivated(WindowEvent e) {
	}
	@Override
	public void windowClosed(WindowEvent e) {
	}
	@Override
	public void windowDeactivated(WindowEvent e) {
	}
	@Override
	public void windowDeiconified(WindowEvent e) {
	}
	@Override
	public void windowIconified(WindowEvent e) {
	}
	
	@Override
	public void newStatus(StatusEvent e) {
		if (e.getSender().equals(mConnector.getCurrentServerAddress())) {
			lblStatus.setText("Status: " + e.getStatus() + " (Last update: " + new SimpleDateFormat("HH:mm:ss").format(new Date()) + ")");
		}
	}

	public static Icon loadIcon(String filename) {
		String name = "resources/" + filename;
		java.net.URL imgURL = null;
		BufferedImage img = null;
		try {
		  imgURL = Debugger.class.getResource(name);
		  if (imgURL != null) {
		  	img = ImageIO.read(imgURL);
				return new ImageIcon(img);
		  } else {
		  	File imgFile = new File(name);
		  	img = ImageIO.read(imgFile);
		  	return new ImageIcon(img);
		  }
		} catch (IOException e) {
			if (imgURL != null) {
				System.err.println("[Debugger] loadImage: Image '" + imgURL.toExternalForm() + "' not found!");
			} else {
				System.err.println("[Debugger] loadImage: Image '" + name + "' not found!");    
			}
		}
		return new ImageIcon(new BufferedImage(1, 1, BufferedImage.TYPE_INT_RGB));
	}

	public static void main(String[] args) {
		String laf = UIManager.getSystemLookAndFeelClassName();
		try {
			UIManager.setLookAndFeel(laf);
		} catch (Exception e1) {
			e1.printStackTrace();
		} 

		// Force to initialize
		Debugger.getInstance();
	}
}
