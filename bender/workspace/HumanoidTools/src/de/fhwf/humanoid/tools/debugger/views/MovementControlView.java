package de.fhwf.humanoid.tools.debugger.views;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.IOException;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTabbedPane;
import javax.swing.Timer;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import net.java.games.input.Controller;
import de.fhwf.humanoid.tools.debugger.events.XYChangedEvent;
import de.fhwf.humanoid.tools.debugger.events.XYChangedListener;
import de.fhwf.humanoid.tools.debugger.services.BodyDataService;
import de.fhwf.humanoid.tools.debugger.services.ServiceManager;
import de.fhwf.humanoid.tools.debugger.tools.XYPositionControl;
import de.fhwf.humanoid.tools.debugger.tools.gamepad.GamePad;

public class MovementControlView extends JPanel implements ActionListener, ChangeListener {
	private static final long serialVersionUID = -7946846304843854568L;

 private static final int BUTTON_KICK_GOAL = 1;
 private static final int BUTTON_GOALIE_LEFT = 4;
 private static final int BUTTON_GOALIE_RIGHT = 5;
 private static final int BUTTON_MOVE_PANTILT = 2;
 private static final double AXIS_X_THRESHOLD = 0.4;
 private static final double AXIS_Y_THRESHOLD = 0.4;
 private static final double AXIS_Z_THRESHOLD = 0.4;
 //private static final double SPEED_FACTOR = (20.0 / 1.0) /* * x */;
	
	JButton mCmdDirection[];
	JCheckBox mChkActive;
	JCheckBox mChkVisionActive;
	JButton mCmdKickLeft;
	JButton mCmdKickRight;
 JButton mCmdGoalieLeft;
 JButton mCmdGoalieRight;
	JSlider mPanPosition;
	JSlider mTiltPosition;
	JLabel mPanLabel;
	JLabel mTiltLabel;
 JLabel mWalkSpeedLabel;
 JButton mCmdGamepad;
	BodyDataService mBodyService;
 GamePad mGamepad;
 Timer mGamepadTimer;
 JTabbedPane mTabMoveMode;
 XYPositionControl mWalkerXY;
 JSlider mWalkerRotation;
 JCheckBox mWalkerTurningAim;
 boolean mMoving;
 int mStopCounter;
 int mLastX;
 int mLastY;
 int mLastR;
	
	public MovementControlView() {
		mBodyService = (BodyDataService)ServiceManager.getInstance().getService(BodyDataService.SERVICENAME);
		
		this.setLayout(new BorderLayout());

		JPanel p = new JPanel();
		mChkActive = new JCheckBox("Active");
		mChkActive.setActionCommand("active");
		mChkActive.addActionListener(this);
		p.add(mChkActive);
		mChkVisionActive = new JCheckBox("Vision active");
		mChkVisionActive.setActionCommand("visionactive");
		mChkVisionActive.addActionListener(this);
		mChkVisionActive.setEnabled(mChkActive.isSelected());
		mChkVisionActive.setSelected(true);
		p.add(mChkVisionActive);
		JLabel lbl = new JLabel("Gamepad:");
		p.add(lbl);
		mCmdGamepad = new JButton("Enable");
		mCmdGamepad.setActionCommand("gamepad");
		mCmdGamepad.addActionListener(this);
		p.add(mCmdGamepad);
		this.add(p, BorderLayout.NORTH);
  mGamepadTimer = new Timer(50, this);
  mGamepadTimer.setActionCommand("timGamepad");
		
		JPanel centerPanel = new JPanel();
		centerPanel.setLayout(new GridLayout(1, 3));
		
		mTabMoveMode = new JTabbedPane();
		JPanel movementPanelWalker = new JPanel();
		movementPanelWalker.setLayout(new BorderLayout());
  mWalkerXY = new XYPositionControl(-100, -100, 100, 100);
  mWalkerXY.addXYChangedListener(new XYChangedListener() {
   @Override
   public void XYChanged(XYChangedEvent e) {
    int x = -mWalkerXY.getYValue();
    int y = -mWalkerXY.getXValue();
    int r = mWalkerRotation.getValue();
    if (x != mLastX || y != mLastY || r != mLastR) {
     mBodyService.sendWalkerCommand(x, y, r, mWalkerTurningAim.isSelected());
     mLastX = x; mLastY = y; mLastR = r;
    }
   }
  });
  mWalkerXY.setEnabled(mChkActive.isSelected());
  movementPanelWalker.add(mWalkerXY, BorderLayout.CENTER);
  mWalkerRotation = new JSlider(-100, 100, 0);
  mWalkerRotation.setOrientation(JSlider.VERTICAL);
  mWalkerRotation.addMouseListener(new MouseAdapter() {
   @Override
   public void mouseClicked(MouseEvent e) {
    super.mouseClicked(e);
    if (e.getClickCount() == 2) {
     mWalkerRotation.setValue(0);
    }
   }
  });
  mWalkerRotation.addChangeListener(new ChangeListener() {
   @Override
   public void stateChanged(ChangeEvent e) {
    int x = -mWalkerXY.getYValue();
    int y = -mWalkerXY.getXValue();
    int r = mWalkerRotation.getValue();
    if (x != mLastX || y != mLastY || r != mLastR) {
     mBodyService.sendWalkerCommand(x, y, r, mWalkerTurningAim.isSelected());
     mLastX = x; mLastY = y; mLastR = r;
    }
   }
  });
  mWalkerRotation.setEnabled(mChkActive.isSelected());
  movementPanelWalker.add(mWalkerRotation, BorderLayout.EAST);
  mWalkerTurningAim = new JCheckBox("Turning aim");
  movementPanelWalker.add(mWalkerTurningAim, BorderLayout.SOUTH);
  mTabMoveMode.addTab("Walker mode", movementPanelWalker);
		JPanel movementPanelMotion = new JPanel();
		movementPanelMotion.setLayout(new GridLayout(2, 3));
		mCmdDirection = new JButton[4];
		movementPanelMotion.add(new JLabel(""));
		mCmdDirection[0] = new JButton("Forward");
		mCmdDirection[0].setActionCommand("forward");
		movementPanelMotion.add(mCmdDirection[0]);
		movementPanelMotion.add(new JLabel(""));
		mCmdDirection[1] = new JButton("Left");
		mCmdDirection[1].setActionCommand("turn_left");
		movementPanelMotion.add(mCmdDirection[1]);
		mCmdDirection[2] = new JButton("Stop");
		mCmdDirection[2].setActionCommand("stop");
		movementPanelMotion.add(mCmdDirection[2]);
		mCmdDirection[3] = new JButton("Right");
		mCmdDirection[3].setActionCommand("turn_right");
		movementPanelMotion.add(mCmdDirection[3]);
		
		for (int i = 0; i < mCmdDirection.length; i++) {
			mCmdDirection[i].addActionListener(this);
			mCmdDirection[i].setEnabled(mChkActive.isSelected());
		}		
		mTabMoveMode.addTab("Motion mode", movementPanelMotion);
		centerPanel.add(mTabMoveMode);

		JPanel panTiltPanel = new JPanel();
		panTiltPanel.setLayout(new GridLayout(2, 2));
		mPanLabel = new JLabel("Pan: (0)");
		panTiltPanel.add(mPanLabel);
		mPanPosition = new JSlider(-90, 90, 0);
		mPanPosition.addChangeListener(this);
		mPanPosition.setEnabled(mChkActive.isSelected());
		panTiltPanel.add(mPanPosition);
		mTiltLabel = new JLabel("Tilt: (0)");
		panTiltPanel.add(mTiltLabel);
		mTiltPosition = new JSlider(-90, 0, 0);
		mTiltPosition.addChangeListener(this);
		mTiltPosition.setEnabled(mChkActive.isSelected());
		panTiltPanel.add(mTiltPosition);
		centerPanel.add(panTiltPanel);
		
		JPanel kickPanel = new JPanel(new GridLayout(2, 2));
		mCmdKickLeft = new JButton("Kick left");
		mCmdKickLeft.setActionCommand("kick_left");
		mCmdKickLeft.addActionListener(this);
		mCmdKickLeft.setEnabled(mChkActive.isSelected());
		kickPanel.add(mCmdKickLeft);
		mCmdKickRight = new JButton("Kick right");
  mCmdKickRight.setActionCommand("kick_right");
  mCmdKickRight.addActionListener(this);
  mCmdKickRight.setEnabled(mChkActive.isSelected());
  kickPanel.add(mCmdKickRight);
  mCmdGoalieLeft = new JButton("Goalie left");
  mCmdGoalieLeft.setActionCommand("goalie_left");
  mCmdGoalieLeft.addActionListener(this);
  mCmdGoalieLeft.setEnabled(mChkActive.isSelected());
  kickPanel.add(mCmdGoalieLeft);
  mCmdGoalieRight = new JButton("Goalie right");
  mCmdGoalieRight.setActionCommand("goalie_right");
  mCmdGoalieRight.addActionListener(this);
  mCmdGoalieRight.setEnabled(mChkActive.isSelected());
  kickPanel.add(mCmdGoalieRight);
  centerPanel.add(kickPanel);
		
		this.add(centerPanel, BorderLayout.CENTER);
	}

	@Override
	public void stateChanged(ChangeEvent e) {
		try {
	  mPanLabel.setText("Pan: (" + mPanPosition.getValue() + ")");
		 mTiltLabel.setText("Tilt: (" + mTiltPosition.getValue() + ")");
		 mBodyService.sendPanTiltCommand(mPanPosition.getValue(), mTiltPosition.getValue());
		} catch (IOException e1) {
			JOptionPane.showMessageDialog(null, e1.getMessage(), "Remote control exception", JOptionPane.ERROR_MESSAGE, null);
		} catch (IllegalStateException e1) {
			JOptionPane.showMessageDialog(null, e1.getMessage(), "Remote control exception", JOptionPane.ERROR_MESSAGE, null);
		}
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		String action = e.getActionCommand();
		try {
  		if (action.equals("active")) {
  			mBodyService.setRemoteActive(mChkActive.isSelected());
  			for (int i = 0; i < mCmdDirection.length; i++) {
  				mCmdDirection[i].setEnabled(mChkActive.isSelected());
  			}
  			mWalkerXY.setEnabled(mChkActive.isSelected());
  			mWalkerRotation.setEnabled(mChkActive.isSelected());
  			mPanPosition.setEnabled(mChkActive.isSelected());
  			mTiltPosition.setEnabled(mChkActive.isSelected());
  			mChkVisionActive.setEnabled(mChkActive.isSelected());
  			mCmdKickLeft.setEnabled(mChkActive.isSelected());
  			mCmdKickRight.setEnabled(mChkActive.isSelected());
     mCmdGoalieLeft.setEnabled(mChkActive.isSelected());
     mCmdGoalieRight.setEnabled(mChkActive.isSelected());
  			mBodyService.setVisionActive(mChkVisionActive.isSelected());
  		} else if (action.equals("visionactive")) {
  			mBodyService.setVisionActive(mChkVisionActive.isSelected());
    } else if (action.equals("gamepad")) {
     if (mGamepad == null) {
      Controller ctl = GamePad.getNextSuitableDevice(2, 1);
      if (ctl != null) {
       mGamepad = new GamePad(ctl);
       mCmdGamepad.setText("Disable (" + mGamepad.getName() + ")");
       mCmdGamepad.setEnabled(false);
       mGamepadTimer.start();
      }
     } else {
      mGamepad = null;
      mCmdGamepad.setText("Enable");
      mGamepadTimer.stop();
     }
    } else if (action.equals("timGamepad")) {
     doBotMoves();
  		} else {
  			mBodyService.sendMovementCommand(action);
  		}
		} catch (IOException e1) {
			JOptionPane.showMessageDialog(null, e1.getMessage(), "Remote control exception", JOptionPane.ERROR_MESSAGE, null);
		} catch (IllegalStateException e1) {
			JOptionPane.showMessageDialog(null, e1.getMessage(), "Remote control exception", JOptionPane.ERROR_MESSAGE, null);
		}
	}
	
	private void doBotMoves() {
  if (mGamepad.getXAxis() == 0 && mGamepad.getYAxis() == 0 && mGamepad.getZAxis() == 0 && (mMoving || mStopCounter > 0)) {
   if (mMoving) {
    mMoving = false;
    mStopCounter = 4;
   }
   mStopCounter--;
   System.out.println("[GamePadController] doBotMoves: Stop");
   if (mTabMoveMode.getSelectedIndex() == 0) {
    mWalkerRotation.setValue(0);
    mWalkerXY.setXY(0, 0);
   } else {
    actionPerformed(new ActionEvent(this, 0, "stop"));
   }
  } else {
   if (mGamepad.getButton(BUTTON_KICK_GOAL)) {
    System.out.println("[GamePadController] doBotMoves: Kick");
    actionPerformed(new ActionEvent(this, 0, "kick"));
   } else if (mGamepad.getButton(BUTTON_GOALIE_LEFT)) {
     System.out.println("[GamePadController] doBotMoves: Goalie left");
     actionPerformed(new ActionEvent(this, 0, "goalie_left"));
   } else if (mGamepad.getButton(BUTTON_GOALIE_RIGHT)) {
    System.out.println("[GamePadController] doBotMoves: Goalie right");
    actionPerformed(new ActionEvent(this, 0, "goalie_right"));
   } else {
    double x = mGamepad.getXAxis();
    double y = mGamepad.getYAxis();
    double z = mGamepad.getZAxis();
    if (Math.abs(x) < AXIS_X_THRESHOLD) {
     x = 0;
    }
    if (Math.abs(y) < AXIS_Y_THRESHOLD) {
     y = 0;
    }
    if (Math.abs(z) < AXIS_Z_THRESHOLD) {
     z = 0;
    }
    if (mGamepad.getButton(BUTTON_MOVE_PANTILT)) {
     mPanPosition.setValue((int)(x * 90));
     mTiltPosition.setValue((int)(-y * 90));
    } else {
     if (mTabMoveMode.getSelectedIndex() == 0) {
      if (x != 0 || y != 0 || z != 0) {
       mWalkerRotation.setValue(-(int)(z * 100));
       mWalkerXY.setXY((int)(x * 100), (int)(y * 100));
       mMoving = true;
      }
     } else {
      if (x == 0 && y > 0) {
       System.out.println("[GamePadController] doBotMoves: Forward");     
       //botConnection.sendCommand(botID, (int)(y * SPEED_FACTOR), (int)(y * SPEED_FACTOR));
       mMoving = true;
      } else if (x == 0 && y < 0) {
       System.out.println("[GamePadController] doBotMoves: Backward");
       //botConnection.sendCommand(botID, (int)(y * SPEED_FACTOR), (int)(y * SPEED_FACTOR));
       mMoving = true;
      } else if (x > 0 && y == 0) {
       System.out.println("[GamePadController] doBotMoves: Turn right");
       
       if (x < 0.66) {
        //botConnection.sendCommand(botID, (int)(x * SPEED_FACTOR), 0);
       } else {
        //botConnection.sendCommand(botID, (int)(-x * SPEED_FACTOR), (int)(x * SPEED_FACTOR));
       }
       mMoving = true;
      } else if (x < 0 && y == 0) {
       System.out.println("[GamePadController] doBotMoves: Turn left");
       if (x > -0.66) {
        //botConnection.sendCommand(botID, 0, (int)(-x * SPEED_FACTOR));
       } else {
        //botConnection.sendCommand(botID, (int)(-x * SPEED_FACTOR), (int)(x * SPEED_FACTOR));
       }
       mMoving = true;
      } else if (y > 0 && x > 0){
       System.out.println("[GamePadController] doBotMoves: Curve forward right");
       //botConnection.sendCommand(botID, 130, 45);
       mMoving = true;
      } else if (y > 0 && x < 0) {
       System.out.println("[GamePadController] doBotMoves: Curve forward left");
       //botConnection.sendCommand(botID, 45, 130);
       mMoving = true;
      } else if (y < 0 && x < 0) {
       System.out.println("[GamePadController] doBotMoves: Curve backward left");
       //botConnection.sendCommand(botID, -45, -130);
       mMoving = true;
      } else if (y < 0 && x > 0) {
       System.out.println("[GamePadController] doBotMoves: Curve backward right");
       //botConnection.sendCommand(botID, -130, -45);
       mMoving = true;
      }
     }
    }
   }
  }  
	}
}
