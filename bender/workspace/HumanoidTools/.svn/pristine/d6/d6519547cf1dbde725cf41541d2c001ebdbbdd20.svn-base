package de.fhwf.humanoid.tools.debugger.tools.gamepad;

import java.util.Timer;
import java.util.TimerTask;
import net.java.games.input.Component;
import net.java.games.input.Controller;
import net.java.games.input.ControllerEnvironment;
import net.java.games.input.Component.Identifier.Axis;

public class GamePad {
	private final static double DEAD_ZONE = 0.3d;
	private final static long ACTIVE_TIMEOUT = 5000;
	private final static boolean USE_KEYBOARD_EMULATION = true;
	private final static int POLL_INTERVAL = 100;

	static int mHighestUsedDevice = -1;

	Controller mDevice;
	GamePadEmulator mEmulator;
	double mXAxis = 0;
	double mYAxis = 0;
	double mZAxis = 0;
	double mZRotAxis = 0;
	boolean mButtons[];
	long mLastChanged = 0;
	Timer mPollTimer;
 int mAxisIDs[];
	int mButtonIDs[];
	float mAxisLastValues[];
	boolean mButtonLastValues[];

	public GamePad(Controller device) {
		mDevice = device; 
		if (device != null) {
			System.out.println("[GamePad] GamePad: Using device '" + getName() + "'...");
			System.out.println("[GamePad] GamePad: * Axis: " + getNumberOfAxes(device));
			System.out.println("[GamePad] GamePad: * Buttons: " + getNumberOfButtons(device));
			mAxisIDs = new int[4]; // X, Y, Z and ZRot
			mAxisLastValues = new float[mAxisIDs.length];
			mButtons = new boolean[getNumberOfButtons(device)];
			mButtonIDs = new int[mButtons.length];
			mButtonLastValues = new boolean[mButtons.length];

			int btnidx = 0;
	  Component[] comps = mDevice.getComponents();
	  for (int i = 0; i < comps.length; i++) {
	   Component c = comps[i];
	   if (c.isAnalog() && !c.isRelative()) { // analog & absolute
	    String className = c.getIdentifier().getClass().getName();
	    if (className.endsWith("Axis")) {
	     if (c.getIdentifier().equals(Axis.X)) {
	      mAxisIDs[0] = i;
	     } else if (c.getIdentifier().equals(Axis.Y)) {
	      mAxisIDs[1] = i;
      } else if (c.getIdentifier().equals(Axis.Z)) {
       mAxisIDs[2] = i;
      } else if (c.getIdentifier().equals(Axis.RZ)) {
       mAxisIDs[3] = i;
	     }
	    }
	   }
	   
	   if (!c.isAnalog() && !c.isRelative()) { // digital & absolute
	    String className = c.getIdentifier().getClass().getName();
	    if (className.endsWith("Button")) {
	     mButtonIDs[btnidx] = i;
	     btnidx++;
	    }
	   }
	  }	
			
			mPollTimer = new Timer(true);
			TimerTask task = new TimerTask() {
    @Override
    public void run() {
     pollDevice(mDevice);
    }
   };
			mPollTimer.schedule(task, 0, POLL_INTERVAL);
		} else if (USE_KEYBOARD_EMULATION && (mEmulator == null)) {
			System.out.println("[GamePad] GamePad: No device given, using keyboard emulation.");
			mEmulator = new GamePadEmulator(this);
			mButtons = new boolean[mEmulator.getButtonCount()];
		} else {
			System.out.println("[GamePad] GamePad: ERROR: No device was given!");
		}
	}
	
	private void pollDevice(Controller ctrl) {
	 if (ctrl.poll()) {
	  Component[] coms = ctrl.getComponents();

	  for (int i = 0; i < mAxisIDs.length; i++) {
	   Component axis = coms[mAxisIDs[i]];
	   float value = axis.getPollData();
	   if (value != mAxisLastValues[i]) {
	    Axis axisType = Axis.X;
	    switch (i) {
	     case 0: axisType = Axis.X; break;
      case 1: axisType = Axis.Y; break;
      case 2: axisType = Axis.Z; break;
      case 3: axisType = Axis.RZ; break;
	    }
	    //System.out.println("Axis " + axisType.toString() + " changed from " + mAxisLastValues[i] + " to " + value);
	    axisChanged(axisType, value);
	    mAxisLastValues[i] = value;
	   }
	  }
	  
   for (int i = 0; i < mButtonIDs.length; i++) {
    Component button = coms[mButtonIDs[i]];
    boolean value = (button.getPollData() > 0.5);
    if (value != mButtonLastValues[i]) {
     //System.out.println("Button " + i + " changed from " + mButtonLastValues[i] + " to " + value);
     buttonChanged(i, value);
     mButtonLastValues[i] = value;
    }
   }
	 }
	}
	
	private void axisChanged(Axis axis, float value) {
		if (axis.equals(Axis.X)) {
			mXAxis = value;
			if(Math.abs(mXAxis) < DEAD_ZONE) {
				mXAxis = 0;
			} else {
				mLastChanged = System.currentTimeMillis();
			}
		} else if (axis.equals(Axis.Y)) {
			mYAxis = value;
			if(Math.abs(mYAxis) < DEAD_ZONE) {
				mYAxis = 0;
			} else {
				mLastChanged = System.currentTimeMillis();
			}
		} else if (axis.equals(Axis.RZ)) {
			mZRotAxis = value;
			if(Math.abs(mZRotAxis) < DEAD_ZONE) {
				mZRotAxis = 0;
			} else {
				mLastChanged = System.currentTimeMillis();
			}
		} else if (axis.equals(Axis.Z)) {
			mZAxis = value;
			if(Math.abs(mZAxis) < DEAD_ZONE) {
				mZAxis = 0;
			} else {
				mLastChanged = System.currentTimeMillis();
			}
		}
	}

	public void buttonChanged(int BtnNr, boolean value) {
		if (BtnNr >= 0 && BtnNr < mButtons.length) {
			mButtons[BtnNr] = value;
			mLastChanged = System.currentTimeMillis();
		}
	}

	protected void setPadState(double X, double Y, double Z, boolean[] buttons) {
		mXAxis = X;
		mYAxis = Y;
		mZAxis = Z;
		for (int i = 0; i < mButtons.length; i++) {
			if (i < buttons.length) {
			 mButtons[i] = buttons[i];
			}
		}		
		if(Math.abs(mXAxis) < DEAD_ZONE) {
			mXAxis = 0;
		} else {
			mLastChanged = System.currentTimeMillis();
		}
		if(Math.abs(mYAxis) < DEAD_ZONE) {
			mYAxis = 0;
		} else {
			mLastChanged = System.currentTimeMillis();
		}
		if(Math.abs(mZAxis) < DEAD_ZONE) {
			mZAxis = 0;
		} else {
			mLastChanged = System.currentTimeMillis();
		}
	}
	
	public boolean isActive() {
		if (mDevice != null || mEmulator != null) {
			if (mLastChanged + ACTIVE_TIMEOUT < System.currentTimeMillis()) {
				return false;
			} else {
				return true;
			}
		} else {
			return false;
		}
	}

	public String getName() {
		if (mDevice != null) {
			return mDevice.getName();
		} else {
			return "No Device";
		}
	}

	public int getNumberOfButtons() {
		if (mDevice != null) {
			return getNumberOfButtons(mDevice);
		} else {
			return 0;
		}
	}
	
	private static int getNumberOfButtons(Controller ctrl) {
	 int cnt = 0;
	 Component[] comps = ctrl.getComponents();
	 for (Component c : comps) {
 	 if (!c.isAnalog() && !c.isRelative()) { // digital & absolute
 	  String className = c.getIdentifier().getClass().getName();
 	  if (className.endsWith("Button")) {
 	   cnt++;
 	  }
	  }
	 }
	 return cnt;
	}

	public int getNumberOfAxis(){
		if (mDevice != null) {
			return getNumberOfAxes(mDevice);
		} else {
			return 0;
		}
	}
	
	private static int getNumberOfAxes(Controller ctrl) {
  int cnt = 0;
  Component[] comps = ctrl.getComponents();
  for (Component c : comps) {
   if (c.isAnalog() && !c.isRelative()) { // analog & absolute
    String className = c.getIdentifier().getClass().getName();
    if (className.endsWith("Axis")) {
     cnt++;
    }
   }
  }
  return cnt;	
	}

	public double getXAxis() {
		return mXAxis;
	}

	public double getYAxis() {
		return mYAxis;
	}

	public double getZAxis() {
		return mZAxis;
	}

	public double getZRotAxis() {
		return mZRotAxis;
	}

	public boolean getButton(int buttonNr) {
		if (buttonNr >= 0 && buttonNr < mButtons.length) {
			return mButtons[buttonNr];
		} else {
			return false;
		}
	}

	public static Controller getNextSuitableDevice(int minAxis, int minButtons) {
		Controller device = null;

		ControllerEnvironment ce = ControllerEnvironment.getDefaultEnvironment();
		Controller[] cs = ce.getControllers();
		
		for (int i = mHighestUsedDevice + 1; i < cs.length; i++) {
			device = cs[i];
			if(device != null){
				System.out.println("[GamePad] getNextSuitableDevice: Device '" + device.getName() + "':");
				if (device.getType() == Controller.Type.GAMEPAD || device.getType() == Controller.Type.STICK) {
  			System.out.println("[GamePad] getNextSuitableDevice:  Axis: " + getNumberOfAxes(device));
  			System.out.println("[GamePad] getNextSuitableDevice:  Buttons: " + getNumberOfButtons(device));
 				if (getNumberOfAxes(device) >= minAxis && getNumberOfButtons(device) >= minButtons) {
 					System.out.println("[GamePad] getNextSuitableDevice: Device is suitable!");
 					mHighestUsedDevice = i;
 					break; 
 				} else {
					 System.out.println("[GamePad] getNextSuitableDevice: Device is not suitable...");
					 device = null;
 				}
    } else {
     System.out.println("[GamePad] getNextSuitableDevice: Device is not a gamepad or joystick...");
     device = null;
				}
			}   
		}
		if (device == null) {
			System.out.println("[GamePad] getNextSuitableDevice: ERROR: No more suitable devices found!");
		}
		return device;
	}
}
