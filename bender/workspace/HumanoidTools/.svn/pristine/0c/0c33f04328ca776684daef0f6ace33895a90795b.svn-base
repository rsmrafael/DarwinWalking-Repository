package de.fhwf.humanoid.tools.debugger;

import java.awt.Component;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;
import java.util.Map;
import javax.swing.JButton;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JMenu;
import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockableFactory;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.event.DockingEvent;
import org.flexdock.docking.event.DockingListener;
import org.flexdock.view.View;
import de.fhwf.humanoid.tools.debugger.views.ActiveBotsView;
import de.fhwf.humanoid.tools.debugger.views.BatteryView;
import de.fhwf.humanoid.tools.debugger.views.BlackboardView;
import de.fhwf.humanoid.tools.debugger.views.BodyCalibrationView;
import de.fhwf.humanoid.tools.debugger.views.BotControlView;
import de.fhwf.humanoid.tools.debugger.views.CameraSettings;
import de.fhwf.humanoid.tools.debugger.views.CameraView;
import de.fhwf.humanoid.tools.debugger.views.ColorCalibrationView;
import de.fhwf.humanoid.tools.debugger.views.ColorCalibrationVisualisation;
import de.fhwf.humanoid.tools.debugger.views.ConfigIniView;
import de.fhwf.humanoid.tools.debugger.views.IMUView;
import de.fhwf.humanoid.tools.debugger.views.LocalizationView;
import de.fhwf.humanoid.tools.debugger.views.LogView;
import de.fhwf.humanoid.tools.debugger.views.MovementControlView;
import de.fhwf.humanoid.tools.debugger.views.VisionObjectsView;

public class ViewFactory implements DockableFactory {
	public static final String CAMERA = "CamView";
	public static final String LOG = "LogView";
	public static final String COLORCAL = "ColorCalView";
	public static final String BODYCAL = "BodyCalView";
	public static final String IMU = "ImuView";
	public static final String BATTERY = "BatteryView";
	public static final String VISIONOBJECTS = "VisionObjectsView";
	public static final String BOTCONTROL = "BotControlView";
	public static final String MOVEMENTCONTROL = "MovementsControlView";
	public static final String ACTIVEBOTS = "ActiveBotsView";
	public static final String CAMERASETTINGS = "CameraSettings";
	public static final String COLORCALVIS = "ColorCalibrationVisualisation";
	public static final String CONFIGINI = "ConfigIniView";
	public static final String LOCA = "LocalizationView";
	public static final String BLACKBOARD = "BlackboardView";
	public static final String VIEWS[] = { CAMERA, LOG, COLORCAL, BODYCAL, IMU,
			BATTERY, VISIONOBJECTS, BOTCONTROL, MOVEMENTCONTROL, ACTIVEBOTS,
			CAMERASETTINGS, COLORCALVIS, CONFIGINI, LOCA, BLACKBOARD };
	public static final String VIEWNAMES[] = { "Camera image", "Logging",
			"Color calibration", "Body calibration", "IMU data",
			"Battery status", "Vision objects", "Robot control & settings",
			"Movement control", "Active robots", "Camera Settings",
			"ColorCalibrationVisualisation", "Config.ini editor",
			"LocalizationView", "Blackboard" };

	private static ViewFactory INSTANCE;

	private Map<String, Component> dockComps;
	private Map<String, Dockable> dockables;

	private ViewFactory() {
		dockComps = new HashMap<String, Component>();
		dockables = new HashMap<String, Dockable>();
	}

	public static synchronized ViewFactory getInstance() {
		if (ViewFactory.INSTANCE == null) {
			ViewFactory.INSTANCE = new ViewFactory();
		}
		return ViewFactory.INSTANCE;
	}

	private Component createComponent(String id) {
		Container comp = null;
		boolean closable = true;
		boolean pinnable = true;
		String name = id;

		if (id.equals(CAMERA)) {
			comp = new CameraView();
			name = VIEWNAMES[0];
		} else if (id.equals(LOG)) {
			comp = new LogView();
			name = VIEWNAMES[1];
		} else if (id.equals(COLORCAL)) {
			comp = new ColorCalibrationView();
			name = VIEWNAMES[2];
		} else if (id.equals(BODYCAL)) {
			comp = new BodyCalibrationView();
			name = VIEWNAMES[3];
		} else if (id.equals(IMU)) {
			comp = new IMUView();
			name = VIEWNAMES[4];
		} else if (id.equals(BATTERY)) {
			comp = new BatteryView();
			name = VIEWNAMES[5];
		} else if (id.equals(VISIONOBJECTS)) {
			comp = new VisionObjectsView();
			name = VIEWNAMES[6];
		} else if (id.equals(BOTCONTROL)) {
			comp = new BotControlView();
			name = VIEWNAMES[7];
		} else if (id.equals(MOVEMENTCONTROL)) {
			comp = new MovementControlView();
			name = VIEWNAMES[8];
		} else if (id.equals(ACTIVEBOTS)) {
			comp = new ActiveBotsView();
			name = VIEWNAMES[9];
		} else if (id.equals(CAMERASETTINGS)) {
			comp = new CameraSettings();
			name = VIEWNAMES[10];
		} else if (id.equals(COLORCALVIS)) {
			comp = new ColorCalibrationVisualisation();
			name = VIEWNAMES[11];
		} else if (id.equals(CONFIGINI)) {
			comp = new ConfigIniView();
			name = VIEWNAMES[12];
		} else if (id.equals(LOCA)) {
			comp = new LocalizationView();
			name = VIEWNAMES[13];
  } else if (id.equals(BLACKBOARD)) {
   comp = new BlackboardView();
   name = VIEWNAMES[14];
		} else {
			comp = new JButton(id);
		}
		View view = new View(id, name);
		// The order of actions matter. Close should be leftmost.
		if (closable) {
			view.addAction(View.CLOSE_ACTION);
		}
		if (pinnable) {
			view.addAction(View.PIN_ACTION);
		}
		view.setContentPane(comp);
		return view;
	}

	@Override
	public Component getDockableComponent(String dockableId) {
		Component comp = (Component) dockComps.get(dockableId);
		if (comp == null) {
			comp = createComponent(dockableId);
			if (comp != null)
				dockComps.put(dockableId, comp);
		}
		return comp;
	}

	@Override
	public Dockable getDockable(String dockableId) {
		Dockable d = (Dockable) dockables.get(dockableId);
		if (d == null) {
			Component comp = getDockableComponent(dockableId);
			if (comp != null) {
				d = DockingManager.getDockable(comp);
				if (d != null) {
					dockables.put(dockableId, d);
				}
			}
		}
		return d;
	}

	public JMenu createViewMenu() {
		JMenu menu = new JMenu("Windows");
		int size = VIEWS.length;
		for (int i = 0; i < size; i++) {
			menu.add(createViewMenuItem(VIEWS[i], VIEWNAMES[i]));
		}
		return menu;
	}

	private JCheckBoxMenuItem createViewMenuItem(String id, String name) {
		Dockable d = getDockable(id);
		boolean shown = DockingManager.isDocked(d);
		JCheckBoxMenuItem mi = new JCheckBoxMenuItem(name, shown);
		mi.addActionListener(new ViewCheckMenuItemListener(d));
		d.addDockingListener(new ViewCheckDockingListener(mi));
		return mi;
	}

	private static class ViewCheckMenuItemListener implements ActionListener {
		private Dockable d;

		public ViewCheckMenuItemListener(Dockable d) {
			this.d = d;
		}

		public void actionPerformed(ActionEvent e) {
			if (DockingManager.isDocked(d)) {
				DockingManager.undock(d);
			} else {
				DockingManager.display(d);
			}
		}
	}

	private static class ViewCheckDockingListener implements DockingListener {
		JCheckBoxMenuItem mi;

		public ViewCheckDockingListener(JCheckBoxMenuItem mi) {
			this.mi = mi;
		}

		public void dockingComplete(DockingEvent evt) {
			System.out.println(mi.getText() + " -> dockingComplete");
			mi.setSelected(true);
		}

		public void dockingCanceled(DockingEvent evt) {
		}

		public void dragStarted(DockingEvent evt) {
		}

		public void dropStarted(DockingEvent evt) {
		}

		public void undockingComplete(DockingEvent evt) {
			System.out.println(mi.getText() + " -> undockingComplete");
			mi.setSelected(false);
		}

		public void undockingStarted(DockingEvent evt) {
		}
	}
}
